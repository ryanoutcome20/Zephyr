#include "includes.h"

Client g_cl{ };


// init routine.
ulong_t __stdcall Client::init(void* arg) {
	// set our username and config file location.
	g_cl.m_user = XOR("user");

	// stop here if we failed to acquire all the data needed from csgo.
	if (!g_csgo.init())
		return 0;

	// welcome the user.
	g_notify.add(tfm::format(XOR("Welcome %s\n"), g_cl.m_user));

	return 1;
}

void Client::DrawHUD() {
	if ( !g_csgo.m_engine->IsInGame( ) )
		return;

	// get time.
	time_t t = std::time( nullptr );
	std::ostringstream time;
	time << std::put_time( std::localtime( &t ), ( "%H:%M:%S" ) );

	// get round trip time in milliseconds.
	int ms = std::max( 0, ( int )std::round( g_cl.m_latency * 1000.f ) );

	// get tickrate.
	int rate = ( int )std::round( 1.f / g_csgo.m_globals->m_interval );

	std::string text = tfm::format( XOR( "Zephyr | rtt: %ims | rate: %i | %s" ), ms, rate, time.str( ).data( ) );
	render::FontSize_t size = render::hud.size( text );

	// background.
	render::rect_filled( m_width - size.m_width - 20, 10, size.m_width + 10, size.m_height + 2, { 0, 0, 0, 120 } );

	// outline.
	Color outline = g_gui.m_color;

	outline.a( ) = 120;

	render::rect( m_width - size.m_width - 20, 10, size.m_width + 10, size.m_height + 2, outline );

	// text.
	render::hud.string( m_width - 15, 10, { 255, 255, 255, 179 }, text, render::ALIGN_RIGHT );
}

void Client::KillFeed() {
	if (!g_menu.main.misc.killfeed.get())
		return;

	if (!g_csgo.m_engine->IsInGame())
		return;

	// get the addr of the killfeed.
	KillFeed_t* feed = (KillFeed_t*)g_csgo.m_hud->FindElement(HASH("SFHudDeathNoticeAndBotStatus"));
	if (!feed)
		return;

	int size = feed->notices.Count();
	if (!size)
		return;

	for (int i{ }; i < size; ++i) {
		NoticeText_t* notice = &feed->notices[i];

		// this is a local player kill, delay it.
		if (notice->spawn_time == 1.5f)
			notice->spawn_time = FLT_MAX;
	}
}

void Client::OnPaint() {
	// update screen size.
	g_csgo.m_engine->GetScreenSize(m_width, m_height);

	// render stuff.
	g_visuals.think();
	g_grenades.paint();
	g_notify.think();

	// run some rendering functions.
	DrawHUD();
	KillFeed();

	// menu goes last.
	g_gui.think();
}

void Client::OnMapload() {
	// store class ids.
	g_netvars.SetupClassData();

	// createmove will not have been invoked yet.
	// but at this stage entites have been created.
	// so now we can retrive the pointer to the local player.
	m_local = g_csgo.m_entlist->GetClientEntity< Player* >(g_csgo.m_engine->GetLocalPlayer());

	// world materials.
	g_visuals.ModulateWorld();

	// reset animation / bone data.
	g_bones.m_running = false;
	g_animations.m_lock_hltv = false;
	
	// reset our shadows.
	g_visuals.m_cascade_light = nullptr;
	g_visuals.m_cascade_shadow_direction = vec3_t{ };

	// get our new shadow entity (if available)
	for ( int i{ 1 }; i <= g_csgo.m_entlist->GetHighestEntityIndex(); ++i ) {
		Entity* ent = g_csgo.m_entlist->GetClientEntity(i);
	
		if ( !ent || !ent->is( HASH( "CCascadeLight" ) ) )
			continue;

		g_visuals.m_cascade_light = ent;
		g_visuals.m_cascade_shadow_direction = ent->m_envLightShadowDirection( );
	}

	// init knife shit.
	g_skins.load();

	// clear networking sequences.
	m_sequences.clear();

	// if the INetChannelInfo pointer has changed, store it for later.
	g_csgo.m_net = g_csgo.m_engine->GetNetChannelInfo();

	if (g_csgo.m_net) {
		g_hooks.m_net_channel.reset();
		g_hooks.m_net_channel.init(g_csgo.m_net);
		g_hooks.m_net_channel.add(INetChannel::PROCESSPACKET, util::force_cast(&Hooks::ProcessPacket));
		g_hooks.m_net_channel.add(INetChannel::SENDDATAGRAM, util::force_cast(&Hooks::SendDatagram));
		g_hooks.m_net_channel.add(INetChannel::SENDNETMSG, util::force_cast(&Hooks::SendNetMsg));
	}
}

void Client::StartMove(CUserCmd* cmd) {
	// save some usercmd stuff.
	m_cmd = cmd;
	m_tick = cmd->m_tick;
	m_view_angles = cmd->m_view_angles;
	m_buttons = cmd->m_buttons;

	// get local ptr.
	m_local = g_csgo.m_entlist->GetClientEntity< Player* >(g_csgo.m_engine->GetLocalPlayer());
	if (!m_local)
		return;

	// store max choke
	m_max_lag = (m_local->m_fFlags() & FL_ONGROUND) ? 16 : 14;
	m_lag = g_csgo.m_cl->m_choked_commands;
	m_lerp = game::GetClientInterpAmount();
	m_unlag = .2f;
	m_latency = g_csgo.m_net->GetLatency(INetChannel::FLOW_OUTGOING);
	math::clamp(m_latency, 0.f, 1.f);
	m_latency_ticks = game::TIME_TO_TICKS(m_latency);
	m_server_tick = g_csgo.m_cl->m_server_tick;
	m_arrival_tick = m_server_tick + m_latency_ticks;

	// adjust our unlag if needed.
	if ( g_aimbot.m_fake_latency || g_menu.main.misc.fake_latency_always.get() ) {
		m_unlag += g_menu.main.misc.fake_latency_amt.get( ) / 800.f;
	} 

	else if ( g_menu.main.aimbot.modify_unlag.get() ) {
		m_unlag = g_menu.main.aimbot.modify_unlag_maximum.get( ) / 1000.f;
	}
 
	// run our exploit handler
	g_exploits.Command( );

	// processing indicates that the localplayer is valid and alive.
	m_processing = m_local && m_local->alive();
	if (!m_processing)
		return;

	// make sure prediction has ran on all usercommands.
	// because prediction runs on frames, when we have low fps it might not predict all usercommands.
	// also fix the tick being inaccurate.
	g_inputpred.update();

	// store some stuff about the local player.
	m_flags = m_local->m_fFlags( );
	m_movetype = m_local->m_MoveType( );

	// ...
	m_shot = false;

	// run our shot handler.
	g_shots.Think( );
}

void Client::BackupPlayers(bool restore) {
	if (restore) {
		// restore stuff.
		for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);

			if (!g_aimbot.IsValidTarget(player))
				continue;

			g_aimbot.m_backup[i - 1].restore(player);
		}
	}

	else {
		// backup stuff.
		for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);

			if (!g_aimbot.IsValidTarget(player))
				continue;

			g_aimbot.m_backup[i - 1].store(player);
		}
	}
}

void Client::DoMove() {
	penetration::PenetrationOutput_t tmp_pen_data{ };

	// run movement code before input prediction.
	if ( m_movetype != MOVETYPE_NOCLIP && m_movetype != MOVETYPE_LADDER ) {
		g_movement.JumpRelated();
		g_movement.Strafe();
		g_movement.FakeWalk();
		g_movement.DuckDelay();
	}

	// backup strafe angles (we need them for input prediction)
	m_strafe_angles = m_cmd->m_view_angles;

	// run any strafe angle required movement code.
	if ( m_movetype != MOVETYPE_NOCLIP && m_movetype != MOVETYPE_LADDER ) {
		g_movement.AutoPeek( );

		if( g_menu.main.movement.autostop.get( ) && g_aimbot.m_stop )
			g_movement.QuickStop( );
	}

	// predict input.
	g_inputpred.run();

	// restore original angles after input prediction
	m_cmd->m_view_angles = m_view_angles;

	// convert viewangles to directional forward vector.
	math::AngleVectors(m_view_angles, &m_forward_dir);

	// store stuff after input pred.
	m_ground = (m_flags & FL_ONGROUND);
	m_speed  = g_cl.m_local->m_vecVelocity().length();

	// get shoot position.
	g_animations.UpdateShootPosition( );

	// reset shit.
	m_weapon = nullptr;
	m_weapon_info = nullptr;
	m_weapon_id = -1;
	m_weapon_type = WEAPONTYPE_UNKNOWN;
	m_player_fire = m_weapon_fire = false;

	// store weapon stuff.
	m_weapon = m_local->GetActiveWeapon();

	if (m_weapon) {
		m_weapon_info = m_weapon->GetWpnData();
		m_weapon_id = m_weapon->m_iItemDefinitionIndex();
		m_weapon_type = m_weapon_info->m_weapon_type;

		// ensure weapon spread values / etc are up to date.
		if (m_weapon_type != WEAPONTYPE_GRENADE)
			m_weapon->UpdateAccuracyPenalty();

		// run autowall once for penetration crosshair if we have an appropriate weapon.
		if (m_weapon_type != WEAPONTYPE_KNIFE && m_weapon_type != WEAPONTYPE_C4 && m_weapon_type != WEAPONTYPE_GRENADE) {
			penetration::PenetrationInput_t in;
			in.m_from = m_local;
			in.m_target = nullptr;
			in.m_pos = m_shoot_pos + (m_forward_dir * m_weapon_info->m_range);
			in.m_damage = 1.f;
			in.m_damage_pen = 1.f;
			in.m_can_pen = true;

			// run autowall.
			penetration::run(&in, &tmp_pen_data);
		}

		// set pen data for penetration crosshair.
		m_pen_data = tmp_pen_data;

		// can the player fire.
		m_player_fire = g_csgo.m_globals->m_curtime >= m_local->m_flNextAttack() && !g_csgo.m_gamerules->m_bFreezePeriod() && !(g_cl.m_flags & FL_FROZEN);

		UpdateRevolverCock();
		m_weapon_fire = CanFireWeapon();
	}

	// last tick defuse.
	// todo - dex;  figure out the range for CPlantedC4::Use?
	//              add indicator if valid (on ground, still have time, not being defused already, etc).
	//              move this? not sure where we should put it.
	if (g_input.GetKeyState(g_menu.main.misc.last_tick_defuse.get()) && g_visuals.m_c4_planted) {
		float defuse = (m_local->m_bHasDefuser()) ? 5.f : 10.f;
		float remaining = g_visuals.m_planted_c4_explode_time - g_csgo.m_globals->m_curtime;
		float dt = remaining - defuse - (g_cl.m_latency / 2.f);

		m_cmd->m_buttons &= ~IN_USE;
		if (dt <= game::TICKS_TO_TIME(2))
			m_cmd->m_buttons |= IN_USE;
	}

	// grenade prediction.
	g_grenades.think();

	// run fakelag.
	g_hvh.SendPacket();

	// run aimbot.
	g_aimbot.think();

	// run antiaims.
	g_hvh.AntiAim();
}

void Client::EndMove(CUserCmd* cmd) {
	// update client-side animations.
	UpdateInformation();

	// if matchmaking mode, anti untrust clamp.
	if (g_menu.main.config.mode.get() == 0)
		m_cmd->m_view_angles.SanitizeAngle();

	// fix our movement.
	g_movement.FixMove(cmd, m_strafe_angles);

	// this packet will be sent.
	if (*m_packet) {
		g_hvh.m_step_switch = (bool)g_csgo.RandomInt(0, 1);

		// we are sending a packet, so this will be reset soon.
		// store the old value.
		m_old_lag = m_lag;

		// get radar angles.
		m_radar = cmd->m_view_angles;
		m_radar.normalize();

		// get current origin.
		vec3_t cur = m_local->m_vecOrigin();

		// get prevoius origin.
		vec3_t prev = m_net_pos.empty() ? cur : m_net_pos.front().m_pos;

		// check if we broke lagcomp.
		m_lagcomp = (cur - prev).length_sqr() > 4096.f;

		// save sent origin and time.
		m_net_pos.emplace_front(g_csgo.m_globals->m_curtime, cur);
	}

	// store some values for next tick.
	m_old_packet = *m_packet;
	m_old_shot = m_shot;

	// run our dropping if needed.
	if ( g_hvh.m_dropping == m_local->m_nTickBase() ) {
		g_csgo.m_engine->ExecuteClientCmd(XOR("drop"));
	}
}

void Client::OnTick(CUserCmd* cmd) {
	if (g_menu.main.misc.clantag.get())
		g_cl.SetClantag();

	// store some data and update prediction.
	StartMove(cmd);

	// not much more to do here.
	if (!m_processing)
		return;

	// save the original state of players.
	BackupPlayers(false);

	// run all movement related code.
	DoMove();

	// store stome additonal stuff for next tick
	// sanetize our usercommand if needed and fix our movement.
	EndMove(cmd);

	// restore the players.
	BackupPlayers(true);

	// restore curtime/frametime
	// and prediction seed/player.
	g_inputpred.restore();
}

void Client::UpdateAnimations() {
	if (!g_cl.m_local || !g_cl.m_processing)
		return;

	CCSGOPlayerAnimState* state = g_cl.m_local->m_PlayerAnimState();
	if (!state)
		return;

	// set the required flags.
	m_local->AddEffect(EF_NOINTERP);

	m_local->AddEntityFlag(EFL_DIRTY_ABSVELOCITY);
	m_local->AddEntityFlag(EFL_DIRTY_ABSANGVELOCITY);
	m_local->AddEntityFlag(EFL_DIRTY_ABSTRANSFORM);

	// apply the rotation.
	m_local->SetAbsAngles(m_rotation);
	m_local->m_angRotation() = m_rotation;
	m_local->m_angNetworkAngles() = m_rotation;

	// rebuild our animations.UpdateClientsideAnimationState
	g_animations.UpdateClientsideAnimationState( m_poses, m_layers );

	// update abs yaw with last networked abs yaw.
	m_local->SetAbsAngles(ang_t(0.f, m_abs_yaw, 0.f));
}

void Client::UpdateInformation() {
	if (g_cl.m_lag > 0)
		return;

	CCSGOPlayerAnimState* state = g_cl.m_local->m_PlayerAnimState();
	if (!state)
		return;

	// update time.
	m_anim_frame = g_csgo.m_globals->m_curtime - m_anim_time;
	m_anim_time = g_csgo.m_globals->m_curtime;

	// current angle will be animated.
	m_angle = g_cl.m_cmd->m_view_angles;

	math::clamp(m_angle.x, -90.f, 90.f);
	m_angle.normalize();

	// write angles to model.
	g_csgo.m_prediction->SetLocalViewAngles(m_angle);

	// set lby to predicted value.
	g_cl.m_local->m_flLowerBodyYawTarget() = m_body;

	// CCSGOPlayerAnimState::Update, bypass already animated checks.
	if ( state->m_flLastUpdateIncrement == g_csgo.m_globals->m_frame )
		state->m_flLastUpdateIncrement -= 1;

	// call original, bypass hook.
	g_hooks.m_UpdateClientSideAnimation(g_cl.m_local);

	// store updated abs yaw.
	g_cl.m_abs_yaw = state->m_flFootYaw;

	// we landed.
	if (!m_ground && state->m_bOnGround) {
		m_body = m_angle.y;
		m_body_pred = m_anim_time;
	}

	// walking, delay lby update by .22.
	else if (state->m_flVelocityLengthXY > 0.1f) {
		if (state->m_bOnGround)
			m_body = m_angle.y;

		m_body_pred = m_anim_time + 0.22f;
	}

	// standing update every 1.1s
	else if (m_anim_time > m_body_pred) {
		m_body = m_angle.y;
		m_body_pred = m_anim_time + 1.1f;
	}

	// get last networked poses.
	g_cl.m_local->GetPoseParameters(m_poses);
	g_cl.m_local->GetAnimLayers(m_layers);

	// save updated data.
	m_rotation = g_cl.m_local->m_angAbsRotation();
	m_speed = g_cl.m_local->m_vecVelocity( ).length( );
	m_ground = state->m_bOnGround;
}

void Client::print(const std::string text, ...) {
	va_list     list;
	int         size;
	std::string buf;

	if (text.empty())
		return;

	va_start(list, text);

	// count needed size.
	size = std::vsnprintf(0, 0, text.c_str(), list);

	// allocate.
	buf.resize(size);

	// print to buffer.
	std::vsnprintf(buf.data(), size + 1, text.c_str(), list);

	va_end(list);

	// print to console.
	g_csgo.m_cvar->ConsoleColorPrintf(g_gui.m_color, XOR("[Zephyr] "));
	g_csgo.m_cvar->ConsoleColorPrintf(colors::white, buf.c_str());
}

bool Client::CanFireWeapon() {
	// the player cant fire.
	if (!m_player_fire)
		return false;

	if (m_weapon_type == WEAPONTYPE_GRENADE)
		return false;

	// if we have no bullets, we cant shoot.
	if (m_weapon_type != WEAPONTYPE_KNIFE && m_weapon->m_iClip1() < 1)
		return false;

	// do we have any burst shots to handle?
	if ((m_weapon_id == GLOCK || m_weapon_id == FAMAS) && m_weapon->m_iBurstShotsRemaining() > 0) {
		// new burst shot is coming out.
		if (g_csgo.m_globals->m_curtime >= m_weapon->m_fNextBurstShot())
			return true;
	}

	// r8 revolver.
	if (m_weapon_id == REVOLVER) {
		int act = m_weapon->m_Activity();

		// mouse1.
		if (!m_revolver_fire) {
			if ((act == 185 || act == 193) && m_revolver_cock == 0)
				return g_csgo.m_globals->m_curtime >= m_weapon->m_flNextPrimaryAttack();

			return false;
		}
	}

	// yeez we have a normal gun.
	if (g_csgo.m_globals->m_curtime >= m_weapon->m_flNextPrimaryAttack())
		return true;

	return false;
}

void Client::UpdateRevolverCock() {
	// default to false.
	m_revolver_fire = false;

	// reset properly.
	if (m_revolver_cock == -1)
		m_revolver_cock = 0;

	// we dont have a revolver.
	// we have no ammo.
	// player cant fire
	// we are waiting for we can shoot again.
	if (m_weapon_id != REVOLVER || m_weapon->m_iClip1() < 1 || !m_player_fire || g_csgo.m_globals->m_curtime < m_weapon->m_flNextPrimaryAttack()) {
		// reset.
		m_revolver_cock = 0;
		m_revolver_query = 0;
		return;
	}

	// calculate max number of cocked ticks.
	// round to 6th decimal place for custom tickrates..
	int shoot = (int)(0.25f / (std::round(g_csgo.m_globals->m_interval * 1000000.f) / 1000000.f));

	// amount of ticks that we have to query.
	m_revolver_query = shoot - 1;

	// we held all the ticks we needed to hold.
	if (m_revolver_query == m_revolver_cock) {
		// reset cocked ticks.
		m_revolver_cock = -1;

		// we are allowed to fire, yay.
		m_revolver_fire = true;
	}

	else {
		// we still have ticks to query.
		// apply inattack.
		if (g_menu.main.config.mode.get() == 0 && m_revolver_query > m_revolver_cock)
			m_cmd->m_buttons |= IN_ATTACK;

		// count cock ticks.
		// do this so we can also count 'legit' ticks
		// that didnt originate from the hack.
		if (m_cmd->m_buttons & IN_ATTACK)
			m_revolver_cock++;

		// inattack was not held, reset.
		else m_revolver_cock = 0;
	}

	// remove inattack2 if cocking.
	if (m_revolver_cock > 0)
		m_cmd->m_buttons &= ~IN_ATTACK2;
}

void Client::UpdateIncomingSequences() {
	if (!g_csgo.m_net)
		return;

	if (m_sequences.empty() || g_csgo.m_net->m_in_seq > m_sequences.front().m_seq) {
		// store new stuff.
		m_sequences.emplace_front(g_csgo.m_globals->m_realtime, g_csgo.m_net->m_in_rel_state, g_csgo.m_net->m_in_seq);
	}

	// do not save too many of these.
	while (m_sequences.size() > 2048)
		m_sequences.pop_back();
}

void Client::SetClantag() {
	static int(__fastcall * clantag)(const char*, const char*);

	if (!clantag) {
		clantag = pattern::find(g_csgo.m_engine_dll, XOR("53 56 57 8B DA 8B F9 FF 15")).as< int(__fastcall*)(const char*, const char*) >();
	}

	// get our text.
	std::string text = g_menu.main.misc.clantag_text.gets( );

	// run clantag lambda.
	clantag(text.c_str(), text.c_str());
}

