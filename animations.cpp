#include "includes.h"

Animations g_animations{ };;

void Animations::Update( Player* player, LagRecord* record, CCSGOPlayerAnimState* state, AnimationBackup_t& backup ) {
	const float time{ player->m_flOldSimulationTime( ) + g_csgo.m_globals->m_interval };
	const int ticks = game::TIME_TO_TICKS( time );

	// correct time and frametime to match server simulation.
	g_csgo.m_globals->m_curtime			= time;
	g_csgo.m_globals->m_realtime		= time;
	g_csgo.m_globals->m_frametime		= g_csgo.m_globals->m_interval;
	g_csgo.m_globals->m_abs_frametime	= g_csgo.m_globals->m_interval;
	g_csgo.m_globals->m_frame			= ticks;
	g_csgo.m_globals->m_tick_count		= ticks;
	g_csgo.m_globals->m_interp_amt		= 0.f;

	// update animations under controlled variables.
	UpdateClientsideAnimations( player );

	// store new values for next tick prediction.
	record->m_foot_yaw					= state->m_flFootYaw;
	record->m_move_yaw					= state->m_flMoveYaw;
	record->m_move_yaw_current_to_ideal = state->m_flMoveYawCurrentToIdeal;
	record->m_move_yaw_ideal			= state->m_flMoveYawIdeal;
	record->m_move_weight_smoothed		= state->m_flMoveWeightSmoothed;

	player->InvalidatePhysicsRecursive( ANIMATION_CHANGED );

	// set our animation layers.
	player->SetAnimLayers( record->m_layers );

	// store updated rotation.
	player->GetPoseParameters( record->m_poses );

	record->m_abs_ang = ang_t( 0.f, state->m_flFootYaw, 0.f );

	// restore globals.
	g_csgo.m_globals->m_curtime			= backup.m_curtime;
	g_csgo.m_globals->m_realtime		= backup.m_realtime;
	g_csgo.m_globals->m_abs_frametime	= backup.m_abs_frametime;
	g_csgo.m_globals->m_frametime		= backup.m_frametime;
	g_csgo.m_globals->m_frame			= backup.m_frame;
	g_csgo.m_globals->m_tick_count		= backup.m_tick_count;
	g_csgo.m_globals->m_interp_amt		= backup.m_interp_amt;

	// setup bones for this record.
	record->m_setup = g_bones.BuildBonesDirty( player, BONE_USED_BY_ANYTHING, record->m_bones );
}

bool Animations::FixFakewalk( Player* player, LagRecord* record ) {
	record->m_anim_velocity.z = 0.f;

	if ( !( record->m_flags & FL_ONGROUND ) || record->m_anim_velocity.length_2d( ) <= 0.1f || record->m_lag <= 3 )
		return false;

	if ( record->m_velocity.length_sqr( ) > 0.1f
		&& record->m_layers[ ANIMATION_LAYER_MOVEMENT_MOVE ].m_weight == 0.0f
		&& record->m_layers[ ANIMATION_LAYER_LEAN ].m_weight == 0.0f
		&& record->m_layers[ ANIMATION_LAYER_MOVEMENT_MOVE ].m_playback_rate < 0.0001f )
		record->m_fake_walk = true;

	if ( record->m_fake_walk )
		record->m_anim_velocity = record->m_velocity = vec3_t( 0, 0, 0 );

	return record->m_fake_walk;
}

void Animations::FixVelocity( Player* player, LagRecord* record, LagRecord* previous ) {
	// don't do this or predict anything if they're airbourne.
	if ( !( record->m_anim_flags & FL_ONGROUND ) )
		return;

	// fix our fakewalk and z axis on velocity.
	if ( FixFakewalk( player, record ) )
		return;

	// they were in air, no need to fix velocity.
	if ( !( record->m_flags & FL_ONGROUND && previous->m_flags & FL_ONGROUND ) )
		return;

	// check our acceleration.
	float animVelocity = record->m_anim_velocity.length( );

	if ( animVelocity <= previous->m_anim_velocity.length( ) )
		return;

	// clamp velocity.
	float velocityXYC = std::min( animVelocity, 260.f );

	// calculate portion of run top speed.
	float m_flSpeedAsPortionOfRunTopSpeed = ( 1.f - record->m_layers[ ANIMATION_LAYER_ALIVELOOP ].m_weight ) * ( 0.9f - 0.55f );
	if ( m_flSpeedAsPortionOfRunTopSpeed > 0.f ) {
		m_flSpeedAsPortionOfRunTopSpeed += 0.55f;
	}

	// calculate new speed and set our animvelocity.
	float newSpeed = m_flSpeedAsPortionOfRunTopSpeed * velocityXYC;
	if ( m_flSpeedAsPortionOfRunTopSpeed > 0.f ) {
		record->m_anim_velocity = record->m_anim_velocity.normalized( ) * newSpeed;
	}
}

void Animations::FixJump( LagRecord* record, LagRecord* previous ) {
	// set previous flags.
	record->m_anim_flags = previous->m_flags;

	// strip the on ground flag.
	record->m_anim_flags &= ~FL_ONGROUND;

	// been onground for 2 consecutive ticks? fuck yeah.
	if ( record->m_flags & FL_ONGROUND && previous->m_flags & FL_ONGROUND )
		record->m_anim_flags |= FL_ONGROUND;

	// fix jump_fall.
	if ( record->m_layers[ ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL ].m_weight != 1.f && previous->m_layers[ ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL ].m_weight == 1.f && record->m_layers[ ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB ].m_weight != 0.f )
		record->m_anim_flags |= FL_ONGROUND;

	if ( record->m_flags & FL_ONGROUND && !( previous->m_flags & FL_ONGROUND ) )
		record->m_anim_flags &= ~FL_ONGROUND;

	float duck_delta	  = ( record->m_duck - previous->m_duck ) * ( 1.f / game::TICKS_TO_TIME( record->m_lag ) );
	vec3_t velocity_delta = ( record->m_velocity - previous->m_velocity ) * ( 1.f / game::TICKS_TO_TIME( record->m_lag ) );

	// the game animates the first choked packet.
	record->m_anim_duck		= previous->m_duck + ( duck_delta * g_csgo.m_globals->m_interval );
	record->m_anim_velocity = previous->m_velocity + ( velocity_delta * g_csgo.m_globals->m_interval );
}

void Animations::PreUpdate( Player* player, LagRecord* record, CCSGOPlayerAnimState* state, LagRecord* previous ) {
	// fix animating in same frame.
	if ( state->m_flLastUpdateIncrement == g_csgo.m_globals->m_frame )
		state->m_flLastUpdateIncrement -= 1;

	// set origins.
	player->m_vecOrigin( )	 = record->m_origin;
	player->m_vecVelocity( ) = player->m_vecAbsVelocity( ) = record->m_anim_velocity;
	player->SetAbsOrigin( record->m_origin );

	// set animation information.
	player->m_flDuckAmount( )		  = record->m_anim_duck;
	player->m_flLowerBodyYawTarget( ) = record->m_body;
	player->m_fFlags( )				  = record->m_anim_flags;
	player->m_iEFlags( )			  &= ~( EFL_DIRTY_ABSTRANSFORM | EFL_DIRTY_ABSVELOCITY );

	// write potentially resolved angles.
	player->m_angEyeAngles( ) = record->m_eye_angles;
}

// Backup Information
bool Animations::Backup( Player* player, AnimationBackup_t& backup ) {
	backup.m_origin			= player->m_vecOrigin( );
	backup.m_abs_origin		= player->GetAbsOrigin( );
	backup.m_velocity		= player->m_vecVelocity( );
	backup.m_abs_velocity	= player->m_vecAbsVelocity( );
	backup.m_flags			= player->m_fFlags( );
	backup.m_eflags			= player->m_iEFlags( );
	backup.m_duck			= player->m_flDuckAmount( );
	backup.m_body			= player->m_flLowerBodyYawTarget( );

	player->GetAnimLayers( backup.m_layers );
	player->GetPoseParameters( backup.m_poses );

	backup.m_curtime		= g_csgo.m_globals->m_curtime;
	backup.m_realtime		= g_csgo.m_globals->m_realtime;
	backup.m_frametime		= g_csgo.m_globals->m_frametime;
	backup.m_abs_frametime	= g_csgo.m_globals->m_abs_frametime;
	backup.m_frame			= g_csgo.m_globals->m_frame;
	backup.m_tick_count		= g_csgo.m_globals->m_tick_count;
	backup.m_interp_amt		= g_csgo.m_globals->m_interp_amt;

	if ( !backup.m_layers )
		return false;

	return true;
}

// Restore Information
bool Animations::Restore( Player* player, AnimationBackup_t& backup ) {
	player->m_vecOrigin( )			  = backup.m_origin;
	player->m_vecVelocity( )		  = backup.m_velocity;
	player->m_vecAbsVelocity( )		  = backup.m_abs_velocity;
	player->m_fFlags( )			      = backup.m_flags;
	player->m_iEFlags( )			  = backup.m_eflags;
	player->m_flDuckAmount( )		  = backup.m_duck;
	player->m_flLowerBodyYawTarget( ) = backup.m_body;

	player->SetAbsOrigin( backup.m_abs_origin );
	player->SetAnimLayers( backup.m_layers );
	player->SetPoseParameters( backup.m_poses );

	g_csgo.m_globals->m_curtime		  = backup.m_curtime;
	g_csgo.m_globals->m_realtime	  = backup.m_realtime;
	g_csgo.m_globals->m_abs_frametime = backup.m_abs_frametime;
	g_csgo.m_globals->m_frametime     = backup.m_frametime;
	g_csgo.m_globals->m_frame         = backup.m_frame;
	g_csgo.m_globals->m_tick_count    = backup.m_tick_count;
	g_csgo.m_globals->m_interp_amt    = backup.m_interp_amt;

	return true;
}

bool Animations::OnNetUpdate( Player* player, LagRecord* record ) {
	if ( !player || !record )
		return false;

	// backup information.
	AnimationBackup_t backup;

	if ( !Backup( player, backup ) )
		return false;

	// fetch state information.
	CCSGOPlayerAnimState* state = player->m_PlayerAnimState( );

	if ( !state )
		return false;

	// reset resolver information.
	record->m_fake_walk = false;
	record->m_mode		= Resolver::Modes::RESOLVE_NONE;

	// get aimplayer information.
	AimPlayer* data = &g_aimbot.m_players[ player->index( ) - 1 ];

	// player respawned.
	if ( player->m_flSpawnTime( ) != data->m_spawn ) {
		// reset animation state.
		game::ResetAnimationState( state );

		// note new spawn time.
		data->m_spawn = player->m_flSpawnTime( );
	}

	// fetch previous record.
	LagRecord* previous = data->m_records.size( ) >= 2 ? data->m_records[ 1 ].get( ) : nullptr;

	// calculate animation time and lag.
	if ( previous ) {
		record->m_anim_time = previous->m_sim_time + g_csgo.m_globals->m_interval;
		record->m_lag = game::TIME_TO_TICKS( record->m_sim_time - previous->m_sim_time );

		if ( record->m_lag < 1 )
			record->m_lag = 1;

		// proper calculation of velocity.
		record->m_velocity = ( record->m_origin - previous->m_origin ) * ( 1.f / game::TICKS_TO_TIME( record->m_lag ) );
		
		if( !game::IsFakePlayer( player->index( ) ) ) {
			// these only occur when the player is choking packets.
			if ( record->m_lag > 1 ) {
				FixVelocity( player, record, previous );
				FixJump( record, previous );
			}

			if ( g_menu.main.aimbot.correct.get( ) )
				g_resolver.ResolveAngles( player, record );
		}
	}

	// actually update everything.
	PreUpdate( player, record, state, previous );
	Update( player, record, state, backup );
	Restore( player, backup );

	return true;
}

void Animations::UpdateClientsideAnimations( Player* player ) {
	bool backup = player->m_bClientSideAnimation( );

	m_lock_hltv = true;
	player->m_bClientSideAnimation( ) = true;

	g_hooks.m_UpdateClientSideAnimation( player );

	m_lock_hltv = false;
	player->m_bClientSideAnimation( ) = player->m_bIsLocalPlayer( ) ? backup : false;
}

void Animations::UpdateShootPosition( ) {
	// you can do this test yourself by just disabling this function and seeing
	// how bad your shoot position actually becomes. personally when I tested mine
	// was at roughly the origin of the player, that is not accurate at all since
	// bullets should come out of the center of your face.

	// get bone cache.
	CBoneCache* cache = &g_cl.m_local->m_BoneCache( );

	if ( !cache )
		return;

	// get current bone record.
	BoneArray* bones = cache->m_pCachedBones;

	if ( !bones )
		return;

	// make a copy of the bones.
	BoneArray backup[ 128 ];

	std::memcpy( backup, bones, sizeof(BoneArray) * 128 );

	// setup poses.
	float poses[ 24 ];

	g_cl.m_local->GetPoseParameters( poses );

	g_cl.m_local->m_flPoseParameter( )[ PLAYER_POSE_PARAM_AIM_BLEND_CROUCH_IDLE ] = 0.5;

	// generate new shoot position.	
	g_bones.BuildBonesDirty( g_cl.m_local, BONE_USED_BY_ANYTHING, bones );

	g_cl.m_shoot_pos = g_cl.m_local->GetShootPosition( );

	// restore our backups.
	std::memcpy( bones, backup, sizeof( BoneArray ) * 128 );

	g_cl.m_local->SetPoseParameters( poses );
}

void Animations::CacheClientsideAnimations( float* poses, C_AnimationLayer* layers ) {
	// only take the networked on the ones that don't matter.
	g_cl.m_local->GetPoseParameters( m_poses );
	g_cl.m_local->GetAnimLayers( m_layers );

	for ( int i = 0; i < PLAYER_POSE_PARAM_COUNT; i++ ) {
		if ( i == PLAYER_POSE_PARAM_JUMP_FALL ||
			i == PLAYER_POSE_PARAM_BODY_YAW   ||
			i == PLAYER_POSE_PARAM_BODY_PITCH )
			continue;

		m_poses[ i ] = poses[ i ];
	}

	for ( int i = 0; i < ANIMATION_LAYER_COUNT; i++ ) {
		if( i == ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB ||
			i == ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL  ||
			i == ANIMATION_LAYER_MOVEMENT_MOVE			||
			i == ANIMATION_LAYER_MOVEMENT_STRAFECHANGE  )
			continue;

		m_layers[ i ] = layers[ i ];
	}
}

void Animations::UpdateClientsideAnimationState( float* poses, C_AnimationLayer* layers ) {
	// get our animation state.
	CCSGOPlayerAnimState* state = g_cl.m_local->m_PlayerAnimState( );

	if ( !state )
		return;

	// if this is the case then we should just leave it to the engine.
	if ( state->m_nAnimstateModelVersion > 2 )
		return;

	// cache our poses and layers.
	CacheClientsideAnimations( poses, layers );

	// update movement layer and poses.
	UpdateMovement( state );

	// update the render poses and layers with our new ones.
	g_cl.m_local->GetPoseParameters( m_poses );
	g_cl.m_local->GetAnimLayers( m_layers );
}

void Animations::UpdateMovement( CCSGOPlayerAnimState* state ) {
	// this is here as a placeholder because in the future we should be rebuilding alot of these animation layers and poses.
	m_poses[ PLAYER_POSE_PARAM_MOVE_BLEND_WALK ] = ( 1.0f - state->m_flWalkToRunTransition ) * ( 1.0f - state->m_flAnimDuckAmount );
	m_poses[ PLAYER_POSE_PARAM_MOVE_BLEND_RUN ] = ( state->m_flWalkToRunTransition ) * ( 1.0f - state->m_flAnimDuckAmount );
	m_poses[ PLAYER_POSE_PARAM_MOVE_BLEND_CROUCH_WALK ] = state->m_flAnimDuckAmount;
	m_poses[ PLAYER_POSE_PARAM_MOVE_YAW ] = state->m_flMoveYaw;
	m_poses[ PLAYER_POSE_PARAM_JUMP_FALL ] = state->m_bOnGround ? 0.f : 1.f;
}