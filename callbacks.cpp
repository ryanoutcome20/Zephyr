#include "includes.h"

// execution callbacks..
void callbacks::SkinUpdate( ) {
	g_skins.m_update = true;
}

void callbacks::ForceFullUpdate( ) {	
	if( g_csgo.m_net && g_csgo.m_net->IsLoopback( ) )
		return;

	g_csgo.m_cl->m_delta_tick = -1;
}

void callbacks::ToggleMinimalDamageOverride( ) {
	g_aimbot.m_override = !g_aimbot.m_override;
}

void callbacks::ToggleExtendedLag( ) {
	g_hvh.m_extended = !g_hvh.m_extended;
}

void callbacks::ToggleThirdPerson( ) {
	g_visuals.m_thirdperson = !g_visuals.m_thirdperson;
}

void callbacks::ToggleWeather( ) {
	g_precipitation.m_setup = false;
}

void callbacks::ToggleFakeLatency( ) {
	g_aimbot.m_fake_latency = !g_aimbot.m_fake_latency;
}

void callbacks::ToggleKillfeed( ) {
    KillFeed_t* feed = ( KillFeed_t* )g_csgo.m_hud->FindElement( HASH( "SFHudDeathNoticeAndBotStatus" ) );
    if( feed )
        g_csgo.ClearNotices( feed );
}

void callbacks::ConfigLoad( ) {
	std::string config = g_menu.main.config.config.GetActiveItem( );

	g_config.load( &g_menu.main, config.data( ) );
	g_notify.add( tfm::format( XOR( "Loaded config %s\n" ), config.data( ) ) );

	g_visuals.ModulateWorld( );
	g_precipitation.m_setup = false;
}

void callbacks::ConfigSave( ) {
	std::string config = g_menu.main.config.config.GetActiveItem( );

	g_config.save( &g_menu.main, config.data( ) );
	g_notify.add( tfm::format( XOR( "Saved config %s\n" ), config.data( ) ) );
}

void callbacks::ConfigCreate( ) {
	std::string config = g_menu.main.config.config_edit.gets( );

	g_config.save( &g_menu.main, config );
	g_notify.add( tfm::format(XOR("Created config %s\n"), config ) );

	g_config.update( &g_menu.main );
}

void callbacks::ConfigDelete() {
	std::string config = g_menu.main.config.config.GetActiveItem( );

	g_config.remove( &g_menu.main, config.data( ) );
	g_notify.add( tfm::format(XOR("Removed config %s\n"), config.data( ) ) );

	g_config.update( &g_menu.main );
}

void callbacks::ConfigRefresh( ) {
	g_config.update( &g_menu.main );
}

bool callbacks::IsBaimHealth( ) {
	return g_menu.main.aimbot.baim2.get( 1 );
}

bool callbacks::IsFovOn( ) {
	return g_menu.main.aimbot.fov.get( );
}

bool callbacks::IsHitchanceOn( ) {
	return g_menu.main.aimbot.hitchance.get( );
}

bool callbacks::IsPenetrationOn( ) {
	return g_menu.main.aimbot.penetrate.get( );
}

bool callbacks::IsMultipointOn( ) {
	return !g_menu.main.aimbot.multipoint.GetActiveIndices( ).empty( );
}

bool callbacks::IsMultipointBodyOn( ) {
	return g_menu.main.aimbot.multipoint.get( 2 );
}

bool callbacks::IsModifyUnlagOn( ) {
	return g_menu.main.aimbot.modify_unlag.get( );
}

bool callbacks::IsAntiAimModeStand( ) {
	return g_menu.main.antiaim.mode.get( ) == 0;
}

bool callbacks::HasStandYaw( ) {
	return g_menu.main.antiaim.yaw_stand.get( ) > 0;
}

bool callbacks::IsStandYawJitter( ) {
	return g_menu.main.antiaim.yaw_stand.get( ) == 2;
}

bool callbacks::IsStandYawRotate( ) {
	return g_menu.main.antiaim.yaw_stand.get( ) == 3;
}

bool callbacks::IsStandYawRnadom( ) {
	return g_menu.main.antiaim.yaw_stand.get( ) == 4;
}

bool callbacks::IsStandDirAuto( ) {
	return g_menu.main.antiaim.dir_stand.get( ) == 0;
}

bool callbacks::IsStandDirCustom( ) {
	return g_menu.main.antiaim.dir_stand.get( ) == 4;
}

bool callbacks::IsStandBodyCustom() {
	return g_menu.main.antiaim.body_fake_stand.get( ) == 5;
}

bool callbacks::IsStandBodyDouble() {
	return g_menu.main.antiaim.body_fake_stand_custom_double.get( );
}

bool callbacks::IsAntiAimModeWalk( ) {
	return g_menu.main.antiaim.mode.get( ) == 1;
}

bool callbacks::WalkHasYaw( ) {
	return g_menu.main.antiaim.yaw_walk.get( ) > 0;
}

bool callbacks::IsWalkYawJitter( ) {
	return g_menu.main.antiaim.yaw_walk.get( ) == 2;
}

bool callbacks::IsWalkYawRotate( ) {
	return g_menu.main.antiaim.yaw_walk.get( ) == 3;
}

bool callbacks::IsWalkYawRandom( ) {
	return g_menu.main.antiaim.yaw_walk.get( ) == 4;
}

bool callbacks::IsWalkDirAuto( ) {
	return g_menu.main.antiaim.dir_walk.get( ) == 0;
}

bool callbacks::IsWalkDirCustom( ) {
	return g_menu.main.antiaim.dir_walk.get( ) == 4;
}

bool callbacks::IsAntiAimModeAir( ) {
	return g_menu.main.antiaim.mode.get( ) == 2;
}

bool callbacks::AirHasYaw( ) {
	return g_menu.main.antiaim.yaw_air.get( ) > 0;
}

bool callbacks::IsAirYawJitter( ) {
	return g_menu.main.antiaim.yaw_air.get( ) == 2;
}

bool callbacks::IsAirYawRotate( ) {
	return g_menu.main.antiaim.yaw_air.get( ) == 3;
}

bool callbacks::IsAirYawRandom( ) {
	return g_menu.main.antiaim.yaw_air.get( ) == 4;
}

bool callbacks::IsAirDirAuto( ) {
	return g_menu.main.antiaim.dir_air.get( ) == 0;
}

bool callbacks::IsAirDirCustom( ) {
	return g_menu.main.antiaim.dir_air.get( ) == 4;
}

bool callbacks::IsFakeAntiAimRelative( ) {
	return g_menu.main.antiaim.fake_yaw.get( ) == 2;
}

bool callbacks::IsFakeAntiAimJitter( ) {
	return g_menu.main.antiaim.fake_yaw.get( ) == 3;
}

bool callbacks::IsFakeLagOn() {
	return g_menu.main.antiaim.lag_enable.get( );
}

bool callbacks::IsChamsEnemy( ) {
	return g_menu.main.players.chams_selection.get( ) == 0;
}

bool callbacks::IsChamsEnemyVisible( ) {
	return g_menu.main.players.chams_enemy_visible.get( );
}

bool callbacks::IsChamsEnemyOverlay() {
	return g_menu.main.players.chams_enemy_visible_overlay.get();
}

bool callbacks::IsChamsEnemyInvisible() {
	return g_menu.main.players.chams_enemy_invisible.get();
}

bool callbacks::IsChamsEnemyHistory() {
	return g_menu.main.players.chams_enemy_history.get();
}

bool callbacks::IsChamsFriendly( ) {
	return g_menu.main.players.chams_selection.get() == 1;
}

bool callbacks::IsChamsFriendlyVisible( ) {
	return g_menu.main.players.chams_friendly_visible.get();
}

bool callbacks::IsChamsFriendlyOverlay( ) {
	return g_menu.main.players.chams_friendly_visible_overlay.get();
}

bool callbacks::IsChamsFriendlyInvisible( ) {
	return g_menu.main.players.chams_friendly_invisible.get();
}

bool callbacks::IsChamsLocal( ) {
	return g_menu.main.players.chams_selection.get() == 2;
}

bool callbacks::IsChamsLocalReal( ) {
	return g_menu.main.players.chams_local_real.get();
}

bool callbacks::IsChamsLocalRealOverlay( ) {
	return g_menu.main.players.chams_local_real_overlay.get();
}

bool callbacks::IsChamsLocalBlendScopedOn( ) {
	return g_menu.main.players.chams_local_real_blend_scope.get( );
}

bool callbacks::IsChamsLocalFake( ) {
	return g_menu.main.players.chams_local_fake.get();
}

bool callbacks::IsChamsLocalFakeOverlay( ) {
	return g_menu.main.players.chams_local_fake_overlay.get();
}

bool callbacks::IsBoxEnemy( ) {
	return g_menu.main.players.box.get( 0 );
}

bool callbacks::IsBoxFriendly( ) {
	return g_menu.main.players.box.get( 1 );
}

bool callbacks::IsBoxLocal( ) {
	return g_menu.main.players.box.get( 2 );
}

bool callbacks::IsNameEnemy( ) {
	return g_menu.main.players.name.get( 0 );
}

bool callbacks::IsNameFriendly( ) {
	return g_menu.main.players.name.get( 1 );
}

bool callbacks::IsNameLocal( ) {
	return g_menu.main.players.name.get( 2 );
}

bool callbacks::IsWeaponOn() {
	return g_menu.main.players.weapon.get( 0 ) || g_menu.main.players.weapon.get( 1 ) || g_menu.main.players.weapon.get( 2 );
}

bool callbacks::IsWeaponEnemy( ) {
	return g_menu.main.players.weapon.get( 0 );
}

bool callbacks::IsWeaponFriendly( ) {
	return g_menu.main.players.weapon.get( 1 );
}

bool callbacks::IsWeaponLocal( ) {
	return g_menu.main.players.weapon.get( 2 );
}

bool callbacks::IsGlowOn( ) {
	return !g_menu.main.players.glow.GetActiveIndices( ).empty( );
}

bool callbacks::IsGlowStyleOutline( ) {
	return g_menu.main.players.glow_style.get( ) == 0;
}

bool callbacks::IsGlowEnemy() {
	return g_menu.main.players.glow.get( 0 );
}

bool callbacks::IsGlowFriendly( ) {
	return g_menu.main.players.glow.get( 1 );
}

bool callbacks::IsGlowLocal( ) {
	return g_menu.main.players.glow.get( 2 );
}

bool callbacks::IsSkeletonEnemy( ) {
	return g_menu.main.players.skeleton.get( 0 );
}

bool callbacks::IsSkeletonFriendly( ) {
	return g_menu.main.players.skeleton.get( 1 );
}

bool callbacks::IsSkeletonLocal( ) {
	return g_menu.main.players.skeleton.get( 2 );
}

bool callbacks::IsOffscreenOn( ) {
	return g_menu.main.players.offscreen.get( );
}

bool callbacks::IsOffscreenPulsateOn( ) {
	return g_menu.main.players.offscreen_pulsate.get( );
}

bool callbacks::IsHealthOn( ) {
	return g_menu.main.players.health.get( 0 ) || g_menu.main.players.health.get( 1 );
}

bool callbacks::IsHealthOverrideOn( ) {
	return g_menu.main.players.health_override_mode.get( ) != 0;
}

bool callbacks::IsHealthOverrideGradient( ) {
	return g_menu.main.players.health_override_mode.get() == 2;
}

bool callbacks::IsHealthOverrideFadeOn( ) {
	return g_menu.main.players.health_color_fade_off.get();
}

bool callbacks::IsNotHealthOverrideGradient( ) {
	return g_menu.main.players.health_override_mode.get() != 2;
}

bool callbacks::IsLBYUpdateOn( ) {
	return g_menu.main.players.lby_update.get( );
}

bool callbacks::IsShotMatrixOn( ) {
	return g_menu.main.players.shot_matrix.get( );
}

bool callbacks::IsItemsOn( ) {
	return g_menu.main.visuals.items.get( );
}

bool callbacks::IsAmmoOn( ) {
	return g_menu.main.visuals.ammo.get();
}

bool callbacks::IsAmmoBarOn( ) {
	return g_menu.main.visuals.ammo_bar.get();
}

bool callbacks::IsDroppedC4On( ) {
	return g_menu.main.visuals.dropped_c4.get();
}

bool callbacks::IsProjectileOn( ) {
	return g_menu.main.visuals.proj.get( );
}

bool callbacks::IsProjectileSphereOn( ) {
	return !g_menu.main.visuals.proj_sphere.GetActiveIndices( ).empty( );
}

bool callbacks::IsProjectileGroundOn( ) {
	return !g_menu.main.visuals.proj_ground.GetActiveIndices().empty();
}

bool callbacks::IsWorldModulationOn( ) {
	return g_menu.main.visuals.world_modulation.get( );
}

bool callbacks::IsAmbientModulationOn( ) {
	return g_menu.main.visuals.ambient_modulation.get( );
}

bool callbacks::IsWeatherModulationOn( ) {
	return g_menu.main.visuals.weather_modulation.get( );
}

bool callbacks::IsWeatherModulationAudioOn() {
	return g_menu.main.visuals.weather_audio.get( );
}

bool callbacks::IsSkyboxModulationOn( ) {
	return g_menu.main.visuals.skybox_modulation.get();
}

bool callbacks::IsPropModulationOn( ) {
	return g_menu.main.visuals.prop_modulation.get();
}

bool callbacks::IsShadowModulationOn( ) {
	return g_menu.main.visuals.shadow_modulation.get( );
}

bool callbacks::IsConsoleModulationOn( ) {
	return g_menu.main.visuals.console_modulation.get( );
}

bool callbacks::IsVisualizeSpreadOn( ) {
	return g_menu.main.visuals.spread_xhair.get( );
}

bool callbacks::IsPenetrationCrosshairOn( ) {
	return g_menu.main.visuals.pen_crosshair.get( );
}

bool callbacks::IsPenetrationCrosshairTextOn() {
	return g_menu.main.visuals.pen_crosshair_text.get();
}

bool callbacks::IsAutopeekVisualizeOn( ) {
	return g_menu.main.visuals.autopeek_indicator.get( );
}

bool callbacks::IsGrenadeTracerOn( ) {
	return g_menu.main.visuals.tracers.get( );
}

bool callbacks::IsImpactBeamsOn( ) {
	return g_menu.main.visuals.impact_beams.get( );
}

bool callbacks::IsImpactBoxesServer( ) {
	return g_menu.main.visuals.impact_boxes.get( 0 );
}

bool callbacks::IsImpactBoxesClient( ) {
	return g_menu.main.visuals.impact_boxes.get( 1 );
}

bool callbacks::IsImpactBoxesOn( ) {
	return g_menu.main.visuals.impact_boxes.get( 0 ) || g_menu.main.visuals.impact_boxes.get( 1 );
}

bool callbacks::IsAutoBuyOn( ) {
	return g_menu.main.misc.buy.get( );
}

bool callbacks::IsNotificationsOn() {
	return g_menu.main.misc.notifications.get( 4 );
}

bool callbacks::IsHitmarkerOn() {
	return g_menu.main.misc.hitmarker.get( );
}

bool callbacks::IsSpawnExploitOn() {
	return g_menu.main.misc.spawn_exploit.get( );
}

bool callbacks::IsCrashExploitOn( ) {
	return g_menu.main.misc.active_exploit.get( ) == 3;
}

bool callbacks::IsAutomaticStopOn( ) {
	return g_menu.main.movement.autostop.get( );
}

bool callbacks::IsConfigMM( ) {
	return g_menu.main.config.mode.get( ) == 0;
}

bool callbacks::IsConfigNS( ) {
	return g_menu.main.config.mode.get( ) == 1;
}

// weaponcfgs callbacks.
bool callbacks::DEAGLE( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::DEAGLE;
}

bool callbacks::ELITE( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::ELITE;
}

bool callbacks::FIVESEVEN( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::FIVESEVEN;
}

bool callbacks::GLOCK( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::GLOCK;
}

bool callbacks::AK47( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::AK47;
}

bool callbacks::AUG( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::AUG;
}

bool callbacks::AWP( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::AWP;
}

bool callbacks::FAMAS( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::FAMAS;
}

bool callbacks::G3SG1( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::G3SG1;
}

bool callbacks::GALIL( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::GALIL;
}

bool callbacks::M249( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::M249;
}

bool callbacks::M4A4( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::M4A4;
}

bool callbacks::MAC10( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::MAC10;
}

bool callbacks::P90( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::P90;
}

bool callbacks::UMP45( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::UMP45;
}

bool callbacks::XM1014( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::XM1014;
}

bool callbacks::BIZON( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::BIZON;
}

bool callbacks::MAG7( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::MAG7;
}

bool callbacks::NEGEV( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::NEGEV;
}

bool callbacks::SAWEDOFF( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::SAWEDOFF;
}

bool callbacks::TEC9( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::TEC9;
}

bool callbacks::P2000( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::P2000;
}

bool callbacks::MP7( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::MP7;
}

bool callbacks::MP9( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::MP9;
}

bool callbacks::NOVA( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::NOVA;
}

bool callbacks::P250( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::P250;
}

bool callbacks::SCAR20( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::SCAR20;
}

bool callbacks::SG553( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::SG553;
}

bool callbacks::SSG08( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::SSG08;
}

bool callbacks::M4A1S( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::M4A1S;
}

bool callbacks::USPS( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::USPS;
}

bool callbacks::CZ75A( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::CZ75A;
}

bool callbacks::REVOLVER( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::REVOLVER;
}

bool callbacks::KNIFE_BAYONET( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_BAYONET;
}

bool callbacks::KNIFE_FLIP( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_FLIP;
}

bool callbacks::KNIFE_GUT( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_GUT;
}

bool callbacks::KNIFE_KARAMBIT( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_KARAMBIT;
}

bool callbacks::KNIFE_M9_BAYONET( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_M9_BAYONET;
}

bool callbacks::KNIFE_HUNTSMAN( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_HUNTSMAN;
}

bool callbacks::KNIFE_FALCHION( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_FALCHION;
}

bool callbacks::KNIFE_BOWIE( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_BOWIE;
}

bool callbacks::KNIFE_BUTTERFLY( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_BUTTERFLY;
}

bool callbacks::KNIFE_SHADOW_DAGGERS( ) {
	if( !g_csgo.m_engine->IsInGame( ) || !g_cl.m_processing )
		return false;

	return g_cl.m_weapon_id == Weapons_t::KNIFE_SHADOW_DAGGERS;
}