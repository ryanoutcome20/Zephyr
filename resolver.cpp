#include "includes.h"

Resolver g_resolver{};;


LagRecord* Resolver::FindIdealRecord( AimPlayer* data ) {
	if ( data->m_records.empty( ) )
		return nullptr; // we can't even start.

	// the first valid record.
	LagRecord* first = nullptr;

	// loop through current records.
	for ( const auto& record : data->m_records ) {
		if ( record->immune( ) || !record->valid( ) )
			continue;

		// keep doing this until we find something that isn't an lby update.
		if ( !first || first->m_mode == Modes::RESOLVE_BODY ) {
			first = record.get( );
			continue;
		}

		// check for high prority records.
		if ( record->m_mode == Modes::RESOLVE_NONE || record->m_mode == Modes::RESOLVE_WALK )
			return record.get( );

		// check for onshot.
		if ( record->m_shot )
			return record.get( );
	}

	return first; // return the first valid record found.
}

LagRecord* Resolver::FindLastRecord( AimPlayer* data ) {
	LagRecord* current;

	if ( data->m_records.empty() )
		return nullptr;

	// iterate records in reverse.
	for( auto it = data->m_records.crbegin( ); it != data->m_records.crend( ); ++it ) {
		current = it->get( );

		// if this record is valid.
		// we are done since we iterated in reverse.
		if ( current->valid( ) && !current->immune( ) )
			return current;
	}

	return nullptr;
}

float Resolver::GetAwayAngle( LagRecord* record ) {
	ang_t  away;
	math::VectorAngles( g_cl.m_local->m_vecOrigin( ) - record->m_origin, away );
	return away.y;
}

float Resolver::GetVelocityAngle( LagRecord* record ) {
	ang_t  velocity;
	math::VectorAngles( record->m_velocity, velocity );
	return velocity.y;
}

float Resolver::GetAntiFreestand( LagRecord* record ) {
	// constants.
	constexpr float STEP{ 4.f };
	constexpr float RANGE{ 32.f };

	// get the angles
	vec3_t shoot = record->m_player->GetShootPosition();
	float away = GetAwayAngle(record);

	// construct vector of angles to test.
	std::vector< AdaptiveAngle > angles{ };
	angles.emplace_back(away, 180.f);
	angles.emplace_back(away, 90.f);
	angles.emplace_back(away, -90.f);

	// start the trace at the enemy shoot pos.
	vec3_t start = g_cl.m_local->GetShootPosition();

	// see if we got any valid result.
	// if this is false the path was not obstructed with anything.
	bool valid{ false };

	// iterate vector of angles.
	for ( auto it = angles.begin(); it != angles.end(); ++it ) {
		// compute the 'rough' estimation of where our head will be.
		vec3_t end{ shoot.x + std::cos(math::deg_to_rad(it->m_yaw)) * RANGE,
			shoot.y + std::sin(math::deg_to_rad(it->m_yaw)) * RANGE,
			shoot.z };

		// compute the direction.
		vec3_t dir = end - start;
		float len = dir.normalize();

		// should never happen.
		if ( len <= 0.f )
			continue;

		// step thru the total distance, 4 units per step.
		for ( float i{ 0.f }; i < len; i += STEP ) {
			// get the current step position.
			vec3_t point = start + (dir * i);

			// get the contents at this point.
			int contents = g_csgo.m_engine_trace->GetPointContents(point, MASK_SHOT_HULL);

			// contains nothing that can stop a bullet.
			if ( !(contents & MASK_SHOT_HULL) )
				continue;

			float mult = 1.f;

			// over 50% of the total length, prioritize this shit.
			if ( i > (len * 0.5f) )
				mult = 1.25f;

			// over 90% of the total length, prioritize this shit.
			if ( i > (len * 0.75f) )
				mult = 1.25f;

			// over 90% of the total length, prioritize this shit.
			if ( i > (len * 0.9f) )
				mult = 2.f;

			// append 'penetrated distance'.
			it->m_dist += (STEP * mult);

			// mark that we found anything.
			valid = true;
		}
	}

	if ( !valid ) {
		return away;
	}

	// put the most distance at the front of the container.
	std::sort(angles.begin(), angles.end(),
		[](const AdaptiveAngle& a, const AdaptiveAngle& b) {
			return a.m_dist > b.m_dist;
		});

	// the best angle should be at the front now.
	AdaptiveAngle* best = &angles.front();

	return away + best->m_yaw;
}

void Resolver::MatchShot( AimPlayer* data, LagRecord* record ) {
	// do not attempt to do this in nospread mode.
	if ( g_menu.main.config.mode.get( ) == 1 )
		return;

	record->m_shot = false;

	if ( record->m_lag >= 2 )
		return;

	Weapon* weapon = data->m_player->GetActiveWeapon( );

	if ( !weapon || weapon->IsKnife( ) )
		return;

	int tick = game::TIME_TO_TICKS( weapon->m_fLastShotTime( ) + g_csgo.m_globals->m_interval );

	if ( tick == game::TIME_TO_TICKS( record->m_sim_time ) ) {
		record->m_shot = true;
	}
}

void Resolver::SetMode( LagRecord* record ) {
	if ( record->m_flags & FL_ONGROUND ) {
		if ( record->m_anim_velocity.length_2d() >= 1.f && !record->m_fake_walk )
			record->m_mode = Modes::RESOLVE_WALK;
		else
			record->m_mode = Modes::RESOLVE_STAND;
	}
	else
		record->m_mode = Modes::RESOLVE_AIR;
}

void Resolver::ResolveAngles( Player* player, LagRecord* record ) {
	AimPlayer* data = &g_aimbot.m_players[ player->index( ) - 1 ];

	// mark this record if it contains a shot.
	MatchShot( data, record );

	// next up mark this record with a resolver mode that will be used.
	SetMode( record );

	// don't do this if they aren't using anti-aim.
	if ( player->m_bIsDefusing() || player->m_bIsGrabbingHostage() )
		return;

	// if we are in nospread mode, force all players pitches to down.
	// TODO; we should check thei actual pitch and up too, since those are the other 2 possible angles.
	// this should be somehow combined into some iteration that matches with the air angle iteration.
	if ( g_menu.main.config.mode.get( ) == 1 )
		record->m_eye_angles.x = 90.f;

	// we arrived here we can do the acutal resolve.
	if ( record->m_mode == Modes::RESOLVE_WALK ) 
		ResolveWalk( data, record );

	else if ( record->m_mode == Modes::RESOLVE_STAND )
		ResolveStand( data, record );

	else if ( record->m_mode == Modes::RESOLVE_AIR )
		ResolveAir( data, record );

	// normalize the eye angles, doesn't really matter but its clean.
	math::NormalizeAngle( record->m_eye_angles.y );
}

void Resolver::ResolveWalk( AimPlayer* data, LagRecord* record ) {
	// apply lby to eyeangles.
	record->m_eye_angles.y = record->m_body;

	// delay body update.
	data->m_body_update = record->m_anim_time + 0.22f;

	// reset indexes.
	data->m_brute_index    = 0;
	data->m_body_index	   = 0;
	data->m_lastmove_index = 0;
	data->m_smart_index	   = 0;

	// copy the last record that this player was walking
	// we need it later on because it gives us crucial data.
	std::memcpy( &data->m_walk_record, record, sizeof( LagRecord ) );
}

void Resolver::ResolveStand( AimPlayer* data, LagRecord* record ) {
	// for no-spread call a seperate resolver.
	if( g_menu.main.config.mode.get( ) == 1 ) {
		StandNS( data, record );
		return;
	}

	// reset our movement information.
	data->m_moved = false;

	// pointer for easy access.
	LagRecord* move = &data->m_walk_record;

	// we have a valid moving record.
	if( move->m_sim_time > 0.f ) {
		vec3_t delta = move->m_origin - record->m_origin;

		// check if moving record is close.
		if( delta.length( ) <= 128.f ) {
			// indicate that we are using the moving lby.
			data->m_moved = true;
		}
	}

	// lby is updating.
	if ( record->m_anim_time >= data->m_body_update ) {
		// set angles to current LBY.
		record->m_eye_angles.y = data->m_last_body = record->m_body;

		// predict next body update.
		data->m_body_update = record->m_anim_time + 1.1f;

		// set the resolve mode.
		record->m_mode = Modes::RESOLVE_BODY;

		return;
	}

	// if we can't get a valid previous record we're gonna just bruteforce.
	LagRecord* previous = data->m_records.size( ) >= 2 ? data->m_records[ 1 ].get( ) : nullptr;

	if ( !previous || !data->m_moved || !data->m_last_body ) {
		Bruteforce( data, record );

		return;
	}

	// get our act data.
	bool has_balance = record->m_player->GetSequenceActivity( record->m_layers[ 3 ].m_sequence ) == ACT_CSGO_IDLE_TURN_BALANCEADJUST;
	bool had_balance = record->m_player->GetSequenceActivity( previous->m_layers[ 3 ].m_sequence ) == ACT_CSGO_IDLE_TURN_BALANCEADJUST;

	// they are within the delta range for the last move, shoot for that. 
	float delta = math::NormalizedAngle( record->m_body - move->m_body );

	if ( std::abs( delta ) <= 35.f && data->m_lastmove_index == 0 && !has_balance && !had_balance ) {
		// set angles to current LBY.
		record->m_eye_angles.y = move->m_body;

		// set resolve mode.
		record->m_mode = Modes::RESOLVE_STOPPED_MOVING;

		// exit out of the resolver, thats it.
		return;
	}
	
	// run our smart resolver.
	record->m_mode = Modes::RESOLVE_SMART;

	float freestand = GetAntiFreestand( record );
	float away      = GetAwayAngle( record );

	// check the last flick delta.
	delta = math::NormalizedAngle( record->m_body - data->m_last_body );

	if ( std::abs( delta ) <= 35.f && !has_balance && !had_balance ) {		
		// set angles to the last lby.
		record->m_eye_angles.y = data->m_last_body;

		return;
	}

	// check the freestand delta.
	delta = math::NormalizedAngle( record->m_body - freestand );

	if ( std::abs( delta ) <= 35.f && !has_balance && !had_balance ) {
		// set angles to freestand.
		record->m_eye_angles.y = freestand;

		return;
	}

	// final check.
	delta = math::NormalizedAngle( record->m_eye_angles.y - record->m_body );

	if ( std::abs( delta ) < 120.f && has_balance && data->m_smart_index == 0 ) {
		// set angles to opposite of the current LBY.
		record->m_eye_angles.y = record->m_body;

		return;
	}

	// fallback, bruteforce.
	Bruteforce( data, record );
}

void Resolver::Bruteforce( AimPlayer* data, LagRecord* record ) {
	// set our mode.
	record->m_mode = Modes::RESOLVE_BRUTE;

	// get predicted away angle for the player.
	float away = GetAwayAngle( record );

	// get the freestand angle for the player.
	float freestand = GetAntiFreestand( record );

	switch ( data->m_brute_index % 6 ) {
		case 0:
			record->m_eye_angles.y = record->m_body + 180.f;
			break;

		case 1:
			record->m_eye_angles.y = freestand;
			break;

		case 2:
			record->m_eye_angles.y = away + 180.f;
			break;

		case 3:
			record->m_eye_angles.y = record->m_body;
			break;

		case 4:
			record->m_eye_angles.y = record->m_body + 110.f;
			break;

		case 5:
			record->m_eye_angles.y = record->m_body - 120.f;
			break;

		default:
			break;
	}
}

void Resolver::StandNS( AimPlayer* data, LagRecord* record ) {
	// get away angles.
	float away = GetAwayAngle( record );

	switch( data->m_shots % 8 ) {
	case 0:
		record->m_eye_angles.y = away + 180.f;
		break;

	case 1:
		record->m_eye_angles.y = away + 90.f;
		break;
	case 2:
		record->m_eye_angles.y = away - 90.f;
		break;

	case 3:
		record->m_eye_angles.y = away + 45.f;
		break;
	case 4:
		record->m_eye_angles.y = away - 45.f;
		break;

	case 5:
		record->m_eye_angles.y = away + 135.f;
		break;
	case 6:
		record->m_eye_angles.y = away - 135.f;
		break;

	case 7:
		record->m_eye_angles.y = away + 0.f;
		break;

	default:
		break;
	}

	// force LBY to not fuck any pose and do a true bruteforce.
	record->m_body = record->m_eye_angles.y;
}

void Resolver::ResolveAir( AimPlayer* data, LagRecord* record ) {
	// for no-spread call a seperate resolver.
	if( g_menu.main.config.mode.get( ) == 1 ) {
		AirNS( data, record );
		return;
	}

	// else run our matchmaking air resolver.

	// try to predict the direction of the player based on his velocity direction.
	// this is not as effective on newer cheats with directional autostrafe but would
	// give a perfect idea of the backwards angle of the player if you were on a typical
	// old school cheat (like supremacy for example).
	float velocity = GetVelocityAngle( record );

	// this is basically just trying to roughly guess the away angle of the player relative
	// to our player, at targets.
	float away = GetAwayAngle( record );

	// rough estimate of the player based on what supremacy thinks the freestand angle should
	// be.
	float freestand = GetAntiFreestand( record );

	switch( data->m_shots % 5 ) {
		case 0:
			record->m_eye_angles.y = velocity + 180.f;
			break;

		case 1:
			record->m_eye_angles.y = away + 180.f;
			break;

		case 2:
			record->m_eye_angles.y = freestand;
			break;

		case 3:
			record->m_eye_angles.y = away + 110.f;
			break;

		case 4:
			record->m_eye_angles.y = away - 110.f;
			break;
	}
}

void Resolver::AirNS( AimPlayer* data, LagRecord* record ) {
	// get away angles.
	float away = GetAwayAngle( record );

	switch( data->m_shots % 9 ) {
	case 0:
		record->m_eye_angles.y = away + 180.f;
		break;

	case 1:
		record->m_eye_angles.y = away + 150.f;
		break;
	case 2:
		record->m_eye_angles.y = away - 150.f;
		break;

	case 3:
		record->m_eye_angles.y = away + 165.f;
		break;
	case 4:
		record->m_eye_angles.y = away - 165.f;
		break;

	case 5:
		record->m_eye_angles.y = away + 135.f;
		break;
	case 6:
		record->m_eye_angles.y = away - 135.f;
		break;

	case 7:
		record->m_eye_angles.y = away + 90.f;
		break;
	case 8:
		record->m_eye_angles.y = away - 90.f;
		break;

	default:
		break;
	}
}

void Resolver::ResolvePoses( Player* player, LagRecord* record ) {
	AimPlayer* data = &g_aimbot.m_players[ player->index( ) - 1 ];

	// only do this bs when in air.
	if( record->m_mode == Modes::RESOLVE_AIR ) {
		// ang = pose min + pose val x ( pose range )

		// lean_yaw
		player->m_flPoseParameter( )[ 2 ]  = g_csgo.RandomInt( 0, 4 ) * 0.25f;   

		// body_yaw
		player->m_flPoseParameter( )[ 11 ] = g_csgo.RandomInt( 1, 3 ) * 0.25f;
	}
}