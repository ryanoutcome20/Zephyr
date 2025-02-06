#include "includes.h"

LagCompensation g_lagcomp{};;

LagRecord* LagCompensation::StartPrediction( AimPlayer* data ) {
	// we have no data to work with.
	if (data->m_records.size( ) <= 2)
		return nullptr;

	CCSGOPlayerAnimState* state = data->m_player->m_PlayerAnimState( );

	if (!state)
		return nullptr;

	// get first record.
	LagRecord* current = data->m_records.front( ).get( );
	LagRecord* previous = data->m_records[ 1 ].get( );

	// get tick information.
	int latency_ticks = g_cl.m_latency_ticks * 2;
	int tick = g_cl.m_arrival_tick;

	// sanity checks.
	if (!current || !previous)
		return nullptr;

	if (current->immune( ) || previous->immune( ))
		return nullptr;

	if (!current->m_broke_lc || current->m_shift)
		return nullptr;

	if ((current->m_sim_time + current->m_lag) >= game::TICKS_TO_TIME( tick ))
		return current;

	data->m_records.emplace_front( std::make_shared< LagRecord >( data->m_player ) );

	LagRecord* record = data->m_records.front().get();

	const float curtime = g_csgo.m_globals->m_curtime;
	const float frametime = g_csgo.m_globals->m_frametime;

	record->m_sim_time = game::TICKS_TO_TIME( tick );

	// backup data.
	AnimationBackup_t backup;

	backup.m_origin = data->m_player->m_vecOrigin( );
	backup.m_abs_origin = data->m_player->GetAbsOrigin( );
	backup.m_velocity = data->m_player->m_vecVelocity( );
	backup.m_abs_velocity = data->m_player->m_vecAbsVelocity( );
	backup.m_flags = data->m_player->m_fFlags( );
	backup.m_eflags = data->m_player->m_iEFlags( );
	backup.m_duck = data->m_player->m_flDuckAmount( );
	backup.m_body = data->m_player->m_flLowerBodyYawTarget( );
	data->m_player->GetAnimLayers( backup.m_layers );

	for (int i = 0; i < latency_ticks; ++i) {
		if (!(record->m_flags & FL_ONGROUND))
			current->m_origin -= g_csgo.sv_gravity->GetFloat( ) * g_csgo.m_globals->m_interval;

		PlayerMove( record );

		PredictAnimations( state, record, previous );
	}

	g_csgo.m_globals->m_curtime = record->m_sim_time;

	g_bones.setup( data->m_player, record->m_bones, record );

	data->m_player->m_vecOrigin( ) = backup.m_origin;
	data->m_player->m_vecVelocity( ) = backup.m_velocity;
	data->m_player->m_vecAbsVelocity( ) = backup.m_abs_velocity;
	data->m_player->m_fFlags( ) = backup.m_flags;
	data->m_player->m_iEFlags( ) = backup.m_eflags;
	data->m_player->m_flDuckAmount( ) = backup.m_duck;
	data->m_player->m_flLowerBodyYawTarget( ) = backup.m_body;
	data->m_player->SetAbsOrigin( backup.m_abs_origin );
	data->m_player->SetAnimLayers( backup.m_layers );

	g_csgo.m_globals->m_curtime = curtime;
	g_csgo.m_globals->m_frametime = frametime;

	return record;
}

void LagCompensation::PlayerMove( LagRecord* record ) {
	vec3_t                start, end, normal;
	CGameTrace            trace;
	CTraceFilterWorldOnly filter;

	// define trace start.
	start = record->m_origin;

	// move trace end one tick into the future using predicted velocity.
	end = start + ( record->m_velocity * g_csgo.m_globals->m_interval );

	// trace.
	g_csgo.m_engine_trace->TraceRay( Ray( start, end, record->m_mins, record->m_maxs ), CONTENTS_SOLID, &filter, &trace );

	// we hit shit
	// we need to fix hit.
	if( trace.m_fraction != 1.f ) {

		// fix sliding on planes.
		for( int i{}; i < 2; ++i ) {
			record->m_velocity -= trace.m_plane.m_normal * record->m_velocity.dot( trace.m_plane.m_normal );

			float adjust = record->m_velocity.dot( trace.m_plane.m_normal );
			if( adjust < 0.f )
				record->m_velocity -= ( trace.m_plane.m_normal * adjust );

			start = trace.m_endpos;
			end   = start + ( record->m_velocity * ( g_csgo.m_globals->m_interval * ( 1.f - trace.m_fraction ) ) );

			g_csgo.m_engine_trace->TraceRay( Ray( start, end, record->m_mins, record->m_maxs ), CONTENTS_SOLID, &filter, &trace );
			if( trace.m_fraction == 1.f )
				break;
		}
	}

	// set new final origin.
	start = end = record->m_origin = trace.m_endpos;

	// move endpos 2 units down.
	// this way we can check if we are in/on the ground.
	end.z -= 2.f;

	// trace.
	g_csgo.m_engine_trace->TraceRay( Ray( start, end, record->m_mins, record->m_maxs ), CONTENTS_SOLID, &filter, &trace );

	// strip onground flag.
	record->m_anim_flags &= ~FL_ONGROUND;

	// add back onground flag if we are onground.
	if( trace.m_fraction != 1.f && trace.m_plane.m_normal.z > 0.7f )
		record->m_anim_flags |= FL_ONGROUND;
}

void LagCompensation::PredictAnimations( CCSGOPlayerAnimState* state, LagRecord* record, LagRecord* previous ) {
	struct AnimBackup_t {
		float  curtime;
		float  frametime;
		int    flags;
		int    eflags;
		vec3_t velocity;
	};

	// get player ptr.
	Player* player = record->m_player;

	// set globals appropriately for animation.
	g_csgo.m_globals->m_curtime = record->m_sim_time;
	g_csgo.m_globals->m_frametime = g_csgo.m_globals->m_interval;

	// EFL_DIRTY_ABSVELOCITY
	// skip call to C_BaseEntity::CalcAbsoluteVelocity
	player->m_iEFlags( ) &= ~( EFL_DIRTY_ABSVELOCITY | EFL_DIRTY_ABSTRANSFORM );

	bool bot = game::IsFakePlayer( player->index( ) );

	if (record->m_flags & FL_ONGROUND) {
		record->m_layers[ ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL ].m_cycle = 0.0f;
		record->m_layers[ ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL ].m_weight = 0.0f;
	}
	else if (previous && (!(previous->m_flags & FL_ONGROUND) && (record->m_flags & FL_ONGROUND))) {
		record->m_layers[ ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB ].m_cycle = 1.0f;
		record->m_layers[ ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB ].m_weight = 1.0f;
	}
	
	// set predicted flags and velocity.
	player->m_vecOrigin( ) = record->m_origin;
	player->SetAbsOrigin( record->m_origin );

	player->m_vecVelocity( ) = player->m_vecAbsVelocity( ) = record->m_anim_velocity;

	player->m_flDuckAmount( ) = record->m_anim_duck;
	player->m_flLowerBodyYawTarget( ) = record->m_body;

	player->m_fFlags( ) = record->m_flags;

	// enable re-animation in the same frame if animated already.
	if ( state->m_flLastUpdateIncrement >= g_csgo.m_globals->m_frame )
		state->m_flLastUpdateIncrement = g_csgo.m_globals->m_frame - 1;

	bool fake = !bot && g_menu.main.aimbot.correct.get( );

	// rerun the resolver since we edited the origin.
	if ( fake )
		g_resolver.ResolveAngles( player, record );

	player->m_angEyeAngles( ) = record->m_eye_angles;

	// update animations.
	g_animations.UpdateClientsideAnimations( player );

	record->m_abs_ang = player->GetAbsAngles( );

	player->GetAnimLayers( record->m_layers );
	player->SetAbsAngles( record->m_abs_ang );
	player->GetPoseParameters( record->m_poses );
}