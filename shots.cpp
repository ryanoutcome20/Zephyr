#include "includes.h"

Shots g_shots{ };

void Shots::OnShotFire( Player *target, float damage, int bullets, LagRecord *record ) {
	if( target && record ) {
		// iterate all bullets in this shot.
		for ( int i{ }; i < bullets; ++i ) {
			// setup new shot data.
			ShotRecord shot;
			shot.m_target = target;
			shot.m_record = record;
			shot.m_time = g_csgo.m_globals->m_realtime;
			shot.m_lat = g_cl.m_latency;
			shot.m_damage = damage;
			shot.m_landed = false;
			shot.m_matched = false;
			shot.m_processed = false;

			// set our shoot positions.
			shot.m_pos = shot.m_impact_pos = g_cl.m_local->GetShootPosition( );

			// this is the first bullet, only do this once.
			if ( i == 0 ) {
				// increment total shots on this player.
				AimPlayer *data = &g_aimbot.m_players[ target->index( ) - 1 ];
				if ( data )
					++data->m_shots;
			}

			// get our weapon range.
			shot.m_range = g_cl.m_weapon_info ? g_cl.m_weapon_info->m_range :  0;

			// add to tracks.
			m_shots.push_front( shot );
		}
	}

	// no need to keep an insane amount of shots.
	while ( m_shots.size( ) > 128 )
		m_shots.pop_back( );
}

void Shots::OnImpact( IGameEvent *evt ) {
	// define constants.
	int attacker;
	float time;
	vec3_t pos;
	ShotMatch_t match;

	// sanity checks.
	if ( !evt || !g_cl.m_local )
		return;

	// get attacker, if its not us, we don't need it.
	attacker = g_csgo.m_engine->GetPlayerForUserID(evt->m_keys->FindKey(HASH("userid"))->GetInt());
	
	if ( attacker != g_csgo.m_engine->GetLocalPlayer() )
		return;

	// decode impact coordinates and convert to vec3.
	pos = {
		evt->m_keys->FindKey(HASH("x"))->GetFloat(),
		evt->m_keys->FindKey(HASH("y"))->GetFloat(),
		evt->m_keys->FindKey(HASH("z"))->GetFloat()
	};

	// get real time.
	time = g_csgo.m_globals->m_realtime;

	// add to visual impacts if we have features that rely on it enabled.
	if ( g_menu.main.visuals.impact_beams.get() )
		m_vis_impacts.push_back({ pos, g_cl.m_local->GetShootPosition(), g_cl.m_local->m_nTickBase() });

	// the aimbot didn't shoot yet.
	if ( m_shots.empty() )
		return;

	// try to match our shot.
	match.delta = std::numeric_limits< float >::max();
	match.shot = nullptr;

	// iterate all shots.
	for ( auto& s : m_shots ) {
		// this shot already was matched with our bullet impact event.
		if ( s.m_matched )
			continue;

		// get the delta between the current time.
		float delta = std::abs(time - s.m_time);

		// ignore this.
		if ( delta > 1.f )
			continue;

		// store this shot as being the best for now.
		if ( delta <= match.delta ) {
			match.delta = delta;
			match.shot = &s;
		}
	}
	
	// get our new shot record.
	ShotRecord* shot = match.shot;
	if ( !shot )
		return;

	// this shot was matched.
	shot->m_matched = true;
	shot->m_impact_pos = pos;
}

void Shots::OnHurt( IGameEvent *evt ) {
	// define constants.
	int attacker, victim, hitgroup, hp;
	float damage, time;
	std::string name;
	ShotMatch_t match;

	// sanity checks.
	if ( !evt || !g_cl.m_local )
		return;

	// get our attacker and victim.
	attacker = g_csgo.m_engine->GetPlayerForUserID(evt->m_keys->FindKey(HASH("attacker"))->GetInt());
	victim = g_csgo.m_engine->GetPlayerForUserID(evt->m_keys->FindKey(HASH("userid"))->GetInt());

	// make sure this isn't damage from the world or a grenade.
	if ( attacker < 1 || attacker > 64 || victim < 1 || victim > 64 )
		return;

	// if we aren't the attacker this event really doesn't matter.
	if ( attacker != g_csgo.m_engine->GetLocalPlayer() || victim == g_csgo.m_engine->GetLocalPlayer() )
		return;

	// get where we landed.
	hitgroup = evt->m_keys->FindKey(HASH("hitgroup"))->GetInt( );

	// based on the hitgroup standards I don't think this is valid.
	// "doesn't do damage or bleed"
	if( hitgroup == HITGROUP_GEAR )
		return;

	// get the player that was hurt.
	Player* target = g_csgo.m_entlist->GetClientEntity< Player* >(victim);
	if ( !target )
		return;

	// get player info.
	player_info_t info;
	if ( !g_csgo.m_engine->GetPlayerInfo(victim, &info) )
		return;

	// get player name.
	name = std::string(info.m_name).substr(0, 24);

	// get damage reported by the server.
	damage = (float)evt->m_keys->FindKey(HASH("dmg_health"))->GetInt();

	// get remaining hp.
	hp = evt->m_keys->FindKey(HASH("health"))->GetInt();

	// hitmarker.
	if ( g_menu.main.misc.hitmarker.get() ) {
		g_visuals.m_hit_duration = 1.f;
		g_visuals.m_hit_start = g_csgo.m_globals->m_curtime;
		g_visuals.m_hit_end = g_visuals.m_hit_start + g_visuals.m_hit_duration;

		g_csgo.m_sound->EmitAmbientSound(XOR("buttons/arena_switch_press_02.wav"), 1.f);
	}

	// print this shit.
	if ( g_menu.main.misc.notifications.get(0) )
		g_notify.add(tfm::format(XOR("Hit %s in the %s for %i (%i remaining)\n"), name, m_groups[ hitgroup ], (int)damage, hp));

	// get our real time.
	time = g_csgo.m_globals->m_realtime;

	// try to match our shot.
	match.delta = std::numeric_limits< float >::max();
	match.shot = nullptr;

	// iterate all shots.
	for ( auto& s : m_shots ) {
		// this shot already was matched with our onhurt event.
		if( s.m_landed )
			continue;

		// get the delta between the current time.
		float delta = std::abs(time - s.m_time);

		// ignore this.
		if ( delta > 1.f )
			continue;

		// store this shot as being the best for now.
		if ( delta <= match.delta ) {
			match.delta = delta;
			match.shot = &s;
		}
	}

	// get our new shot record.
	ShotRecord* shot = match.shot;
	if ( !shot )
		return;

	// this shot landed.
	shot->m_landed = true;
}

void Shots::Think() {
	// nospread mode.
	// shots don't need to be processed.
	bool nospread = g_menu.main.config.mode.get( ) == 1;

	// store our constants.
	vec3_t     pos, dir, start, end;
	CGameTrace trace;

	// iterate all shots.
	for ( auto& shot : m_shots ) {
		// make sure we aren't waiting for it to be matched.
		if ( !shot.m_matched )
			continue;

		// make sure we haven't already done this.
		if( shot.m_processed )
			continue;

		// we might miss processing a few times where there is a bug or something with bones but one shot won't kill us (probably).
		shot.m_processed = true;

		// if this shot landed we can stop here.
		if( shot.m_landed || nospread )
			continue;

		// get the shot target.
		Player* target = shot.m_target;
		if ( !target )
			continue;

		// sanity checks
		if ( !target->alive( ) || !g_cl.m_processing )
			continue;

		// get our data.
		AimPlayer* data = &g_aimbot.m_players[ target->index() - 1 ];
		if ( !data )
			continue;

		// this record was deleted already.
		if ( !shot.m_record->m_bones )
			continue;

		// backup this players data to be restored later (we have to mess with his bones for the trace).
		BackupRecord backup;
		backup.store( target );

		// write historical matrix of the time that we shot
		// into the games bone cache, so we can trace against it.
		shot.m_record->cache( );

		// start position of trace is where we took the shot.
		start = shot.m_pos;

		// the impact pos contains the spread from the server
		// which is generated with the server seed, so this is where the bullet
		// actually went, compute the direction of this from where the shot landed
		// and from where we actually took the shot.
		dir = (shot.m_impact_pos - start).normalized();

		// get end pos by extending direction forward.
		end = start + (dir * shot.m_range);

		// intersect our historical matrix with the path the shot took.
		g_csgo.m_engine_trace->ClipRayToEntity(Ray(start, end), MASK_SHOT, target, &trace);

		// we did not hit this player.
		if ( !trace.m_entity || !trace.m_entity->IsPlayer() || trace.m_entity != target ) {
			if( g_menu.main.misc.notifications.get( 4 ) )
				g_notify.add(XOR("Missed shot due to spread\n"));
		}

		// we should have 100% hit this player..
		// this is a miss due to wrong angles.
		else if ( trace.m_entity == target ) {
			size_t mode = shot.m_record->m_mode;

			// increase indexes of our shot counters.
			if ( mode == Resolver::Modes::RESOLVE_BODY )
				++data->m_body_index;

			else if ( mode == Resolver::Modes::RESOLVE_STAND )
				++data->m_stand_index;

			else if ( mode == Resolver::Modes::RESOLVE_STAND2 )
				++data->m_stand_index2;

			++data->m_missed_shots;
		}

		// print out our debug information.
		if ( g_menu.main.misc.extend_logs.get() ) {
			// get player info.
			player_info_t info;

			if( g_csgo.m_engine->GetPlayerInfo(target->index(), &info) ) {
				// get player name.
				std::string name = std::string(info.m_name).substr(0, 24);
			
				// output data.
				g_notify.add(tfm::format("[dbg] target: %s; mode: %s; target damage: %s; ping: %s\n", name, shot.m_record->m_mode, shot.m_damage, std::max(5.f, g_cl.m_latency * 1000.f)));
			}
		}

		// restore player to his original state.
		backup.restore(target);
	}
}
