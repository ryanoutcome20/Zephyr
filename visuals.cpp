#include "includes.h"

Visuals g_visuals{ };;

void Visuals::ModulateWorld( ) {
	std::vector< IMaterial* > world, skybox, props;

	// iterate material handles.
	for( uint16_t h{ g_csgo.m_material_system->FirstMaterial( ) }; h != g_csgo.m_material_system->InvalidMaterial( ); h = g_csgo.m_material_system->NextMaterial( h ) ) {
		// get material from handle.
		IMaterial* mat = g_csgo.m_material_system->GetMaterial( h );
		if( !mat )
			continue;

		// store world materials.
		if( FNV1a::get( mat->GetTextureGroupName( ) ) == HASH( "World textures" ) )
			world.push_back( mat );

		// store the skybox itself.
		else if ( FNV1a::get(mat->GetTextureGroupName()) == HASH("SkyBox textures") )
			skybox.push_back(mat);

		// store props.
		else if( FNV1a::get( mat->GetTextureGroupName( ) ) == HASH( "StaticProp textures" ) )
			props.push_back( mat );
	}

	// props.
	bool prop_modulate = g_menu.main.visuals.prop_modulation.get();

	g_csgo.r_DrawSpecificStaticProp->SetValue( prop_modulate );

	Color prop_color = g_menu.main.visuals.prop_modulation_color.get( );
	prop_color.a( ) = g_menu.main.visuals.prop_modulation_blend.get( ) * 2.55f;

	g_materials.Modulate( props, prop_color, !prop_modulate );

	// world.
	Color world_color = g_menu.main.visuals.world_modulation_color.get();
	world_color.a() = g_menu.main.visuals.world_modulation_blend.get() * 2.55f;

	g_materials.Modulate(world, world_color, !g_menu.main.visuals.world_modulation.get());

	// skybox.
	g_csgo.sv_skyname->SetValue( g_menu.main.visuals.skybox_sky.GetActiveItem( ).c_str( ) );

	g_materials.Modulate( skybox, g_menu.main.visuals.skybox_modulation_color.get(), !g_menu.main.visuals.skybox_modulation.get( ) );

	// ambient.
	// this is, in fact, super stupid if you know how slow convars are to get the value of, 
	// but Valve does it anyway, and no, there is no overriding this in another way; trust me, 
	// I looked. reguardless it isn't that slow to set convars so this should be fine, albeit ugly.
	Color ambient_color = g_menu.main.visuals.ambient_modulation.get( ) ? g_menu.main.visuals.ambient_modulation_color.get( ) : colors::black;

	g_csgo.mat_ambient_light_r->SetValue( ambient_color.r( ) / 255.f );
	g_csgo.mat_ambient_light_g->SetValue( ambient_color.g( ) / 255.f );
	g_csgo.mat_ambient_light_b->SetValue( ambient_color.b( ) / 255.f );
}

void Visuals::ModulateConsole( bool reset ) {
	if( !g_menu.main.visuals.console_modulation.get( ) )
		return;

	// get our materials.
	std::vector< IMaterial* > console = g_materials.Get({ "vgui_white", "800corner1", "800corner2", "800corner3", "800corner4" });

	// reset if needed.
	if( reset ) {
		g_materials.Modulate( console );
		return;
	}

	// get color.
	Color color = g_menu.main.visuals.console_color.get( );
	color.a( ) = g_menu.main.visuals.console_blend.get( ) * 2.55f;

	// modulate materials.
	g_materials.Modulate( console, color, false );
}

void Visuals::ThirdpersonThink( ) {
	ang_t                          offset;
	vec3_t                         origin, forward;
	static CTraceFilterSimple_game filter{ };
	CGameTrace                     tr;

	// for whatever reason overrideview also gets called from the main menu.
	if( !g_csgo.m_engine->IsInGame( ) )
		return;

	// check if we have a local player and he is alive.
	bool alive = g_cl.m_local && g_cl.m_local->alive( );

	// camera should be in thirdperson.
	if( m_thirdperson ) {

		// if alive and not in thirdperson already switch to thirdperson.
		if( alive && !g_csgo.m_input->CAM_IsThirdPerson( ) )
			g_csgo.m_input->CAM_ToThirdPerson( );

		// if dead and spectating in firstperson switch to thirdperson.
		else if( g_cl.m_local->m_iObserverMode( ) == 4 ) {

			// if in thirdperson, switch to firstperson.
			// we need to disable thirdperson to spectate properly.
			if( g_csgo.m_input->CAM_IsThirdPerson( ) ) {
				g_csgo.m_input->CAM_ToFirstPerson( );
				g_csgo.m_input->m_camera_offset.z = 0.f;
			}

			g_cl.m_local->m_iObserverMode( ) = 5;
		}
	}

	// camera should be in firstperson.
	else if( g_csgo.m_input->CAM_IsThirdPerson( ) ) {
		g_csgo.m_input->CAM_ToFirstPerson( );
		g_csgo.m_input->m_camera_offset.z = 0.f;
	}

	// if after all of this we are still in thirdperson.
	if( g_csgo.m_input->CAM_IsThirdPerson( ) ) {
		// get camera angles.
		g_csgo.m_engine->GetViewAngles( offset );

		// get our viewangle's forward directional vector.
		math::AngleVectors( offset, &forward );

		// cam_idealdist convar.
		offset.z = g_menu.main.visuals.thirdperson_distance.get();

		// start pos.
		origin = g_cl.m_shoot_pos;

		// setup trace filter and trace.
		filter.SetPassEntity( g_cl.m_local );

		g_csgo.m_engine_trace->TraceRay(
			Ray( origin, origin - ( forward * offset.z ), { -16.f, -16.f, -16.f }, { 16.f, 16.f, 16.f } ),
			MASK_NPCWORLDSTATIC,
			( ITraceFilter* ) &filter,
			&tr
		);

		// adapt distance to travel time.
		math::clamp( tr.m_fraction, 0.f, 1.f );
		offset.z *= tr.m_fraction;

		// override camera angles.
		g_csgo.m_input->m_camera_offset = { offset.x, offset.y, offset.z };
	}
}

void Visuals::Hitmarker( ) {
	if( !g_menu.main.misc.hitmarker.get( ) )
		return;

	if( g_csgo.m_globals->m_curtime > m_hit_end )
		return;

	if( m_hit_duration <= 0.f )
		return;

	float complete = ( g_csgo.m_globals->m_curtime - m_hit_start ) / m_hit_duration;
	int x = g_cl.m_width / 2,
		y = g_cl.m_height / 2,
		alpha = ( 1.f - complete ) * 240;

	constexpr int line{ 6 };

	render::line( x - line, y - line, x - ( line / 4 ), y - ( line / 4 ), { 200, 200, 200, alpha } );
	render::line( x - line, y + line, x - ( line / 4 ), y + ( line / 4 ), { 200, 200, 200, alpha } );
	render::line( x + line, y + line, x + ( line / 4 ), y + ( line / 4 ), { 200, 200, 200, alpha } );
	render::line( x + line, y - line, x + ( line / 4 ), y - ( line / 4 ), { 200, 200, 200, alpha } );
}

void Visuals::NoSmoke( ) {
	if( !smoke1 )
		smoke1 = g_csgo.m_material_system->FindMaterial( XOR( "particle/vistasmokev1/vistasmokev1_fire" ), XOR( "Other textures" ) );

	if( !smoke2 )
		smoke2 = g_csgo.m_material_system->FindMaterial( XOR( "particle/vistasmokev1/vistasmokev1_smokegrenade" ), XOR( "Other textures" ) );

	if( !smoke3 )
		smoke3 = g_csgo.m_material_system->FindMaterial( XOR( "particle/vistasmokev1/vistasmokev1_emods" ), XOR( "Other textures" ) );

	if( !smoke4 )
		smoke4 = g_csgo.m_material_system->FindMaterial( XOR( "particle/vistasmokev1/vistasmokev1_emods_impactdust" ), XOR( "Other textures" ) );

	if( g_menu.main.visuals.removals.get( 2 ) ) {
		if( !smoke1->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke1->SetFlag( MATERIAL_VAR_NO_DRAW, true );

		if( !smoke2->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke2->SetFlag( MATERIAL_VAR_NO_DRAW, true );

		if( !smoke3->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke3->SetFlag( MATERIAL_VAR_NO_DRAW, true );

		if( !smoke4->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke4->SetFlag( MATERIAL_VAR_NO_DRAW, true );
	}

	else {
		if( smoke1->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke1->SetFlag( MATERIAL_VAR_NO_DRAW, false );

		if( smoke2->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke2->SetFlag( MATERIAL_VAR_NO_DRAW, false );

		if( smoke3->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke3->SetFlag( MATERIAL_VAR_NO_DRAW, false );

		if( smoke4->GetFlag( MATERIAL_VAR_NO_DRAW ) )
			smoke4->SetFlag( MATERIAL_VAR_NO_DRAW, false );
	}
}

void Visuals::think( ) {
	// don't run anything if our local player isn't valid.
	if( !g_cl.m_local )
		return;

	if( !g_csgo.m_engine->IsInGame( ) )
		return;

	if( g_menu.main.visuals.removals.get(7)
		&& g_cl.m_local->alive( )
		&& g_cl.m_local->GetActiveWeapon( )
		&& g_cl.m_local->GetActiveWeapon( )->GetWpnData( )->m_weapon_type == CSWeaponType::WEAPONTYPE_SNIPER_RIFLE
		&& g_cl.m_local->m_bIsScoped( ) ) {

		// rebuild the original scope lines.
		int w = g_cl.m_width,
			h = g_cl.m_height,
			x = w / 2,
			y = h / 2,
			size = g_csgo.cl_crosshair_sniper_width->GetInt( );

		// Here We Use The Euclidean distance To Get The Polar-Rectangular Conversion Formula.
		if( size > 1 ) {
			x -= ( size / 2 );
			y -= ( size / 2 );
		}

		// draw our lines.
		render::rect_filled( 0, y, w, size, colors::black );
		render::rect_filled( x, 0, size, h, colors::black );
	}

	// draw esp on ents.
	for( int i{ 1 }; i <= g_csgo.m_entlist->GetHighestEntityIndex( ); ++i ) {
		Entity* ent = g_csgo.m_entlist->GetClientEntity( i );
		if( !ent )
			continue;

		draw( ent );
	}

	// draw everything else.
	SpreadCrosshair( );
	StatusIndicators( );
	AutopeekIndicator( );
	Spectators( );
	PenetrationCrosshair( );
	Hitmarker( );
	DrawPlantedC4( );
	UpdateShadows( );
}

void Visuals::Spectators( ) {
	if( !g_menu.main.visuals.spectators.get( ) )
		return;

	std::vector< std::string > spectators{ XOR( "Spectators" ) };
	int h = render::notifications.m_size.m_height;

	for( int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i ) {
		Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( i );
		if( !player )
			continue;

		if( player->m_bIsLocalPlayer( ) )
			continue;

		if( player->dormant( ) )
			continue;

		if( player->m_lifeState( ) == LIFE_ALIVE )
			continue;

		if( player->GetObserverTarget( ) != g_cl.m_local )
			continue;

		player_info_t info;
		if( !g_csgo.m_engine->GetPlayerInfo( i, &info ) )
			continue;

		spectators.push_back( std::string( info.m_name ).substr( 0, 24 ) );
	}

	size_t total_size = spectators.size( ) * ( h - 1 );

	for( size_t i{ }; i < spectators.size( ); ++i ) {
		const std::string& name = spectators[ i ];

		render::notifications.string( g_cl.m_width - 20, ( g_cl.m_height / 2 ) - ( total_size / 2 ) + ( i * ( h - 1 ) ),
			{ 255, 255, 255, 179 }, name, render::ALIGN_RIGHT );
	}
}

void Visuals::StatusIndicators( ) {
	// dont do if dead.
	if ( !g_cl.m_processing )
		return;

	// compute hud size.
	// int size = ( int )std::round( ( g_cl.m_height / 17.5f ) * g_csgo.hud_scaling->GetFloat( ) );

	struct Indicator_t { Color color; std::string text; };
	std::vector< Indicator_t > indicators{ };

	// LC
	if ( g_menu.main.visuals.indicators.get( 1 ) ) {
		if( g_cl.m_local->m_vecVelocity( ).length_2d( ) > 270.f || g_cl.m_lagcomp ) {
			Indicator_t ind{ };
			ind.color = g_cl.m_lagcomp ? colors::green: colors::red;
			ind.text = XOR( "LC" );

			indicators.push_back( ind );
		}
	}

	// LBY
	if ( g_menu.main.visuals.indicators.get( 0 ) ) {
		// get the absolute change between current lby and animated angle.
		float change = std::abs( math::NormalizedAngle( g_cl.m_body - g_cl.m_angle.y ) );

		Indicator_t ind{ };
		ind.color = change > 35.f ? colors::green : colors::red;
		ind.text = XOR( "LBY" );
		indicators.push_back( ind );
	}

	// PING
	if ( g_menu.main.visuals.indicators.get( 2 ) ) {
		Indicator_t ind{ };
		ind.color = (g_aimbot.m_fake_latency || g_menu.main.misc.fake_latency_always.get()) ? colors::green : colors::red;
		ind.text = XOR( "PING" );

		indicators.push_back( ind );
	}

	if( g_hvh.m_extended && g_menu.main.visuals.indicators.get( 3 ) ) {
		Indicator_t ind{ };
		ind.color = colors::white;
		ind.text = XOR( "EXTENDED" );

		indicators.push_back( ind );
	}

	if ( g_aimbot.m_override && g_menu.main.visuals.indicators.get( 4 ) ) {
		Indicator_t ind{ };
		ind.color = colors::white;
		ind.text = XOR("MINIMUM");

		indicators.push_back(ind);
	}

	if ( indicators.empty( ) )
		return;

	// iterate and draw indicators.
	for ( size_t i{ }; i < indicators.size( ); ++i ) {
		auto& indicator = indicators[ i ];

		render::indicator.string( 20, g_cl.m_height - 90 - ( 30 * i ), indicator.color, indicator.text );
	}
}

void Visuals::AutopeekIndicator( ) {
	if ( !g_cl.m_processing )
		return;

	if ( !g_menu.main.visuals.autopeek_indicator.get( ) )
		return;

	if ( g_input.GetKeyState( g_menu.main.movement.autopeek.get( ) ) ) {
		render::worldcircle( g_movement.m_autopeek_origin, 15.f, 5.f, 1.f, g_menu.main.visuals.autopeek_color.get( ) );
	}
}

void Visuals::SpreadCrosshair( ) {
	// dont do if dead.
	if ( !g_cl.m_processing )
		return;

	if ( !g_menu.main.visuals.spread_xhair.get( ) )
		return;

	// get active weapon.
	Weapon* weapon = g_cl.m_local->GetActiveWeapon( );
	if ( !weapon )
		return;

	WeaponInfo* data = weapon->GetWpnData( );
	if ( !data )
		return;

	// do not do this on: bomb, knife and nades.
	CSWeaponType type = data->m_weapon_type;
	if ( type == WEAPONTYPE_KNIFE || type == WEAPONTYPE_C4 || type == WEAPONTYPE_GRENADE )
		return;

	// calc radius.
	float radius = ( ( weapon->GetInaccuracy( ) + weapon->GetSpread( ) ) * 320.f ) / ( std::tan( math::deg_to_rad( g_cl.m_local->GetFOV( ) ) * 0.5f ) + FLT_EPSILON );

	// scale by screen size.
	radius *= g_cl.m_height * ( 1.f / 480.f );

	// get color.
	Color col = g_menu.main.visuals.spread_xhair_col.get( );

	// modify alpha channel.
	col.a( ) = 200 * ( g_menu.main.visuals.spread_xhair_blend.get( ) / 100.f );

	int segements = std::max( 16, ( int ) std::round( radius * 0.75f ) );
	render::circle( g_cl.m_width / 2, g_cl.m_height / 2, radius, segements, col );
}

void Visuals::PenetrationCrosshair( ) {
	int   x, y;
	bool  valid_player_hit;
	Color final_color;

	if ( !g_menu.main.visuals.pen_crosshair.get( ) || !g_cl.m_processing )
		return;

	x = g_cl.m_width / 2;
	y = g_cl.m_height / 2;


	valid_player_hit = ( g_cl.m_pen_data.m_target && g_cl.m_pen_data.m_target->enemy( g_cl.m_local ) );
	if ( valid_player_hit )
		final_color = g_menu.main.visuals.pen_crosshair_hit.get( );

	else if ( g_cl.m_pen_data.m_pen )
		final_color = g_menu.main.visuals.pen_crosshair_valid.get();

	else
		final_color = g_menu.main.visuals.pen_crosshair_solid.get();

	final_color.a( ) = 200;

	// draw small square in center of screen.
	render::rect_filled( x - 1, y - 1, 3, 3, final_color );

	// draw our text.
	if  ( ( valid_player_hit || g_cl.m_pen_data.m_pen ) && g_menu.main.visuals.pen_crosshair_text.get( ) ) {
		// get color.
		Color color = g_menu.main.visuals.pen_crosshair_text_color.get();
		
		color.a( ) = 200;

		// convert text to int.
		int dmg = (int)g_cl.m_pen_data.m_damage;

		render::esp_small.string( x + 10, y - 10, color, tfm::format(XOR("%i"), dmg), render::ALIGN_CENTER);
	}
}

void Visuals::draw( Entity* ent ) {
	if ( ent->IsPlayer( ) ) {
		Player* player = ent->as< Player* >( );

		// dont draw dead players.
		if( !player->alive( ) )
			return;

		// draw player esp.
		DrawPlayer( player );
	}

	else if ( ent->IsBaseCombatWeapon( ) && !ent->dormant( ) )
		DrawItem( ent->as< Weapon* >( ) );

	else if ( g_menu.main.visuals.proj.get( ) )
		DrawProjectile( ent->as< Weapon* >( ) );
}

void Visuals::DrawProjectile( Weapon* ent ) {
	vec2_t screen;
	vec3_t origin = ent->GetAbsOrigin( );
	if ( !render::WorldToScreen( origin, screen ) )
		return;

	Color col = g_menu.main.visuals.proj_color.get( );
	col.a( ) = 180;

	// draw decoy.
	if ( ent->is( HASH( "CDecoyProjectile" ) ) )
		render::esp_small.string( screen.x, screen.y, col, XOR( "DECOY" ), render::ALIGN_CENTER );

	// draw molotov.
	else if ( ent->is( HASH( "CMolotovProjectile" ) ) )
		render::esp_small.string( screen.x, screen.y, col, XOR( "MOLLY" ), render::ALIGN_CENTER );

	else if ( ent->is( HASH( "CBaseCSGrenadeProjectile" ) ) ) {
		const model_t* model = ent->GetModel( );

		if( model ) {
			// grab modelname.
			std::string name{ ent->GetModel( )->m_name };

			if( name.find( XOR( "flashbang" ) ) != std::string::npos )
				render::esp_small.string( screen.x, screen.y, col, XOR( "FLASH" ), render::ALIGN_CENTER );

			else if( name.find( XOR( "fraggrenade" ) ) != std::string::npos ) {
				// grenade range.
				if( g_menu.main.visuals.proj_sphere.get( 0 ) )
					render::sphere( origin, 350.f, 5.f, 1.f, g_menu.main.visuals.proj_sphere_color.get( ) );

				render::esp_small.string( screen.x, screen.y, col, XOR( "FRAG" ), render::ALIGN_CENTER );
			}
		}
	}

	// find classes.
	else if ( ent->is( HASH( "CInferno" ) ) ) {
		// fire range.
		if( g_menu.main.visuals.proj_sphere.get( 1 ) )
			render::sphere( origin, 150.f, 5.f, 1.f, g_menu.main.visuals.proj_sphere_color.get( ) );

		// fire ground.
		if( g_menu.main.visuals.proj_ground.get( 1 ) )
			render::worldcircle( origin, 150.f, 5.f, 1.f, g_menu.main.visuals.proj_ground_color.get( ) );

		render::esp_small.string( screen.x, screen.y, col, XOR( "FIRE" ), render::ALIGN_CENTER );
	}

	else if ( ent->is( HASH( "CSmokeGrenadeProjectile" ) ) ) { 
		// smoke ground.
		if ( ent->m_nSmokeEffectTickBegin( ) && g_menu.main.visuals.proj_ground.get( 0 ) )
			render::worldcircle( origin, 175.f, 5.f, 1.f, g_menu.main.visuals.proj_ground_color.get( ) );

		render::esp_small.string( screen.x, screen.y, col, XOR( "SMOKE" ), render::ALIGN_CENTER );
	}
}

void Visuals::DrawItem( Weapon* item ) {
	// we only want to draw shit without owner.
	Entity* owner = g_csgo.m_entlist->GetClientEntityFromHandle( item->m_hOwnerEntity( ) );
	if ( owner )
		return;

	// is the fucker even on the screen?
	vec2_t screen;
	vec3_t origin = item->GetAbsOrigin( );
	if ( !render::WorldToScreen( origin, screen ) )
		return;

	WeaponInfo* data = item->GetWpnData( );
	if ( !data )
		return;

	// render bomb esp.
	if ( item->is( HASH( "CC4" ) ) ) {
		if( g_menu.main.visuals.dropped_c4.get( ) )
			render::esp_small.string( screen.x, screen.y, g_menu.main.visuals.dropped_c4_color.get( ), XOR( "BOMB" ), render::ALIGN_CENTER );

		// exit out, thats it.
		return;
	}

	// if not bomb
	if ( !g_menu.main.visuals.items.get( ) )
		return;

	// normal item, get its name.
	std::string name{ item->GetLocalizedName( ) };

	// smallfonts needs uppercase.
	std::transform( name.begin( ), name.end( ), name.begin( ), ::toupper );

	// render our text.
	render::esp_small.string( screen.x, screen.y, g_menu.main.visuals.item_color.get(), name, render::ALIGN_CENTER );

	// nades do not have ammo.
	if ( data->m_weapon_type == WEAPONTYPE_GRENADE || data->m_weapon_type == WEAPONTYPE_KNIFE )
		return;

	if ( item->m_iItemDefinitionIndex( ) == NULL || item->m_iItemDefinitionIndex( ) == C4 )
		return;

	// render ammo text.
	if ( g_menu.main.visuals.ammo.get() ) {
		std::string ammo = tfm::format( XOR( "(%i/%i)" ), item->m_iClip1( ), item->m_iPrimaryReserveAmmoCount( ) );

		render::esp_small.string( screen.x, screen.y - render::esp_small.m_size.m_height - 1, g_menu.main.visuals.ammo_color.get( ), ammo, render::ALIGN_CENTER );
	}

	// render ammo bar.
	if ( g_menu.main.visuals.ammo_bar.get() ) {
		// get weapon data for scaling calculation.
		WeaponInfo* data = item->GetWpnData();

		// calculate size.
		render::FontSize_t size = render::esp_small.size(name);

		// get some constants.
		int x = screen.x - (size.m_width / 2.f) + 2; 
		int y = screen.y + size.m_height;

		// draw our outline rect.
		render::rect_filled(x, y, size.m_width, 4, { 10, 10, 10, 179 });

		// get the length of the bar inside of the outline.
		int fullness = (int)std::round((size.m_width - 1) * ((float)item->m_iClip1() / data->m_max_clip1));

		// draw our overlay rect.
		render::rect(x, y + 1, fullness, 2, g_menu.main.visuals.ammo_bar_color.get());
	}
}

void Visuals::OffScreen( Player* player, int alpha ) {
	vec3_t view_origin, target_pos, delta;
	vec2_t screen_pos, offscreen_pos;
	float  leeway_x, leeway_y, radius, offscreen_rotation;
	bool   is_on_screen;
	Vertex verts[ 3 ];
	Color  color;

	// todo - dex; move this?
	static auto get_offscreen_data = [ ] ( const vec3_t& delta, float radius, vec2_t& out_offscreen_pos, float& out_rotation ) {
		ang_t  view_angles( g_csgo.m_view_render->m_view.m_angles );
		vec3_t fwd, right, up( 0.f, 0.f, 1.f );
		float  front, side, yaw_rad, sa, ca;

		// get viewport angles forward directional vector.
		math::AngleVectors( view_angles, &fwd );

		// convert viewangles forward directional vector to a unit vector.
		fwd.z = 0.f;
		fwd.normalize( );

		// calculate front / side positions.
		right = up.cross( fwd );
		front = delta.dot( fwd );
		side = delta.dot( right );

		// setup offscreen position.
		out_offscreen_pos.x = radius * -side;
		out_offscreen_pos.y = radius * -front;

		// get the rotation ( yaw, 0 - 360 ).
		out_rotation = math::rad_to_deg( std::atan2( out_offscreen_pos.x, out_offscreen_pos.y ) + math::pi );

		// get needed sine / cosine values.
		yaw_rad = math::deg_to_rad( -out_rotation );
		sa = std::sin( yaw_rad );
		ca = std::cos( yaw_rad );

		// rotate offscreen position around.
		out_offscreen_pos.x = ( int ) ( ( g_cl.m_width / 2.f ) + ( radius * sa ) );
		out_offscreen_pos.y = ( int ) ( ( g_cl.m_height / 2.f ) - ( radius * ca ) );
	};

	if( !g_menu.main.players.offscreen.get( ) )
		return;

	if( !g_cl.m_processing || !g_cl.m_local->enemy( player ) )
		return;

	// get the player's center screen position.
	target_pos = player->WorldSpaceCenter( );
	is_on_screen = render::WorldToScreen( target_pos, screen_pos );

	// give some extra room for screen position to be off screen.
	leeway_x = g_cl.m_width / 18.f;
	leeway_y = g_cl.m_height / 18.f;

	// origin is not on the screen at all, get offscreen position data and start rendering.
	if( !is_on_screen
		|| screen_pos.x < -leeway_x
		|| screen_pos.x >( g_cl.m_width + leeway_x )
		|| screen_pos.y < -leeway_y
		|| screen_pos.y >( g_cl.m_height + leeway_y ) ) {

		// get viewport origin.
		view_origin = g_csgo.m_view_render->m_view.m_origin;

		// get direction to target.
		delta = ( target_pos - view_origin ).normalized( );

		// note - dex; this is the 'YRES' macro from the source sdk.
		radius = g_menu.main.players.offscreen_dist.get( ) * ( g_cl.m_height / 480.f );

		// get the data we need for rendering.
		get_offscreen_data( delta, radius, offscreen_pos, offscreen_rotation );

		// bring rotation back into range... before rotating verts, sine and cosine needs this value inverted.
		// note - dex; reference: 
		// https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/src_main/game/client/tf/tf_hud_damageindicator.cpp#L182
		offscreen_rotation = -offscreen_rotation;
		
		// get our color.
		color = g_menu.main.players.offscreen_color.get();

		// get alpha channel.
		if ( g_menu.main.players.offscreen_pulsate.get( ) && alpha == 255 ) {
			float pulsate = std::sin( g_csgo.m_globals->m_realtime * ( g_menu.main.players.offscreen_pulsate_speed.get( ) / 10.f ) );

			// normalize.
			pulsate = ( pulsate + 1.f ) * 0.5f;

			// load, 50.f is the minimum.
			color.a( ) = 50.f + 200.f * pulsate;
		}

		else if( alpha != 255 )
			color.a( ) = alpha / 2;

		// render!
		render::triangle( offscreen_pos, g_menu.main.players.offscreen_size.get( ), offscreen_rotation, color );
	}
}

void Visuals::DrawPlayer( Player* player ) {
	constexpr float MAX_DORMANT_TIME = 10.f;
	constexpr float DORMANT_FADE_TIME = MAX_DORMANT_TIME / 2.f;

	Rect		  box;
	player_info_t info;
	Color		  color;

	// get player index.
	int index = player->index( );

	// get reference to array variable.
	float& opacity = m_opacities[ index - 1 ];
	bool& draw = m_draw[ index - 1 ];

	// opacity should reach 1 in 300 milliseconds.
	constexpr int frequency = 1.f / 0.3f;

	// the increment / decrement per frame.
	float step = frequency * g_csgo.m_globals->m_frametime;

	// is player local.
	bool local = player->m_bIsLocalPlayer( );

	// is player enemy.
	bool enemy = player->enemy( g_cl.m_local );
	bool dormant = player->dormant( );

	if( g_menu.main.visuals.enemy_radar.get( ) && enemy && !dormant )
		player->m_bSpotted( ) = true;

	// we can draw this player again.
	if( !dormant )
		draw = true;

	if( !draw )
		return;

	// if non-dormant	-> increment
	// if dormant		-> decrement
	dormant ? opacity -= step : opacity += step;

	// is dormant esp enabled for this player.
	bool dormant_esp = enemy && g_menu.main.players.dormant.get( );

	// clamp the opacity.
	math::clamp( opacity, 0.f, 1.f );
	if( !opacity && !dormant_esp )
		return;

	// stay for x seconds max.
	float dt = g_csgo.m_globals->m_curtime - player->m_flSimulationTime( );
	if( dormant && dt > MAX_DORMANT_TIME )
		return;

	// calculate alpha channels.
	int alpha = ( int ) ( 255.f * opacity );
	int low_alpha = ( int ) ( 179.f * opacity );
	int very_low_alpha = (int)(75.f * opacity);

	// get color based on enemy or not.
	color = enemy ? g_menu.main.players.box_enemy.get( ) : g_menu.main.players.box_friendly.get( );

	if( local )
		color = g_menu.main.players.box_local.get( );

	if( dormant && dormant_esp ) {
		alpha = 112;
		low_alpha = 80;
		very_low_alpha = 30;

		// fade.
		if( dt > DORMANT_FADE_TIME ) {
			// for how long have we been fading?
			float faded = ( dt - DORMANT_FADE_TIME );
			float scale = 1.f - ( faded / DORMANT_FADE_TIME );

			alpha *= scale;
			low_alpha *= scale;
		}

		// override color.
		color = { 112, 112, 112 };
	}

	// override alpha.
	color.a( ) = alpha;

	// get player info.
	if( !g_csgo.m_engine->GetPlayerInfo( index, &info ) )
		return;

	// run offscreen ESP.
	if( !local )
		OffScreen( player, alpha );

	// attempt to get player box.
	if( !GetPlayerBoxRect( player, box ) )
		return;

	// DebugAimbotPoints( player );

	bool bone_esp = ( local && g_menu.main.players.skeleton.get( 2 ) ) || ( enemy && g_menu.main.players.skeleton.get( 0 ) ) || ( !enemy && !local && g_menu.main.players.skeleton.get( 1 ) );
	if( bone_esp )
		DrawSkeleton( player, alpha );

	// is box esp enabled for this player.
	bool box_esp = ( local && g_menu.main.players.box.get( 2 ) ) || ( enemy && g_menu.main.players.box.get( 0 ) ) || ( !enemy && !local && g_menu.main.players.box.get( 1 ) );

	// render box if specified.
	if( box_esp )
		render::rect_outlined( box.x, box.y, box.w, box.h, color, { 10, 10, 10, low_alpha } );

	// is name esp enabled for this player.
	bool name_esp = ( local && g_menu.main.players.name.get( 2 ) ) ||  ( enemy && g_menu.main.players.name.get( 0 ) ) || ( !enemy && !local && g_menu.main.players.name.get( 1 ) );

	// draw name.
	if( name_esp ) {
		// fix retards with their namechange meme 
		// the point of this is overflowing unicode compares with hardcoded buffers, good hvh strat
		std::string name{ std::string( info.m_name ).substr( 0, 24 ) };

		Color clr = player->enemy(g_cl.m_local) ? g_menu.main.players.name_enemy.get() : g_menu.main.players.name_friendly.get();

		if( local )
			clr = g_menu.main.players.name_local.get( );

		// override alpha.
		clr.a( ) = low_alpha;

		render::esp.string( box.x + box.w / 2, box.y - render::esp.m_size.m_height, clr, name, render::ALIGN_CENTER );
	}

	// is health esp enabled for this player.
	bool health_esp = ( local && g_menu.main.players.health.get( 2 ) ) || ( enemy && g_menu.main.players.health.get( 0 ) ) || ( !enemy && !local && g_menu.main.players.health.get( 1 ) );

	if( health_esp ) {
		// get x and y.
		int y = box.y + 1;
		int h = box.h - 2;

		// should override.
		int health_override = g_menu.main.players.health_override_mode.get( );

		// save this for checking our scaled factor later.
		int hp = std::min( 100, player->m_iHealth( ) );

		// get our scaled health.
		float& scaled = m_scaled_health[ index - 1 ];

		// reset if needed. note that this will reset the health injector but since
		// the health injector already does some scaling it'll look fine.
		if ( !scaled || scaled < hp )
			scaled = hp;

		// converted health.
		int health = (int)scaled;

		// calculate hp bar color.
		Color bar = { std::min((510 * (100 - health)) / 100, 255), std::min((510 * health) / 100, 255), 0 };
		
		if ( health_override != 0 )
			bar = g_menu.main.players.health_color.get( );

		bar.a( ) = alpha;

		// get hp bar height.
		int fill = ( int ) std::round( health * h / 100.f );

		// render background.
		render::rect_filled( box.x - 6, y - 1, 4, h + 2, { 10, 10, 10, very_low_alpha } );

		// if in gradient mode, render the bars with a gradient to allow for color scaling.
		if ( health_override == 2 ) {
			// get our secondary color.
			Color secondary = g_menu.main.players.health_color_gradient.get();

			secondary.a() = alpha;

			// run our rendering.
			render::rect_filled_fade(box.x - 5, y + h - fill, 2, fill, bar, 255, 0);

			render::rect_filled_fade(box.x - 5, y + h - ( fill / 2 ), 2, ( fill / 2), secondary, 0, 255);
		}

		// otherwise render as normal.
		else {
			if( g_menu.main.players.health_color_fade_off.get( ) ) {
				if( g_menu.main.players.health_color_fade_off_direction.get( ) == 0 )
					render::rect_filled_fade(box.x - 5, y + h - fill, 2, fill, bar, 255, 0);
				else 
					render::rect_filled_fade(box.x - 5, y + h - fill, 2, fill, bar, 0, 255);
			}
			else {
				render::rect(box.x - 5, y + h - fill, 2, fill, bar);
			}
		}

		// if hp is below max, draw a string.
		if( health < 100 )
			render::esp_small.string( box.x - 5, y + ( h - fill ) - 5, { 255, 255, 255, alpha }, std::to_string( health ), render::ALIGN_CENTER );

		// adjust our scaled speed for later.
		if ( scaled > hp )
			scaled -= 255 * g_csgo.m_globals->m_frametime;
	}

	// draw flags.
	if ( !local ) {
		std::vector< std::pair< std::string, Color > > flags;

		auto items = enemy ? g_menu.main.players.flags_enemy.GetActiveIndices( ) : g_menu.main.players.flags_friendly.GetActiveIndices( );

		// NOTE FROM NITRO TO DEX -> stop removing my iterator loops, i do it so i dont have to check the size of the vector
		// with range loops u do that to do that.
		for( auto it = items.begin( ); it != items.end( ); ++it ) {

			// money.
			if( *it == 0 )
				flags.push_back( { tfm::format( XOR( "$%i" ), player->m_iAccount( ) ), { 0, 255, 0, low_alpha } } );

			// armor.
			if( *it == 1 ) {
				// helmet.
				if( player->m_bHasHelmet( ) )
					flags.push_back( { XOR( "HK" ), { 255, 255, 255, low_alpha } } );

				// only kevlar.
				else if( player->m_ArmorValue( ) > 0 )
					flags.push_back( { XOR( "K" ), { 255, 255, 255, low_alpha } } );
			}

			// scoped.
			if( *it == 2 && player->m_bIsScoped( ) )
				flags.push_back( { XOR( "ZOOM" ), { 60, 180, 225, low_alpha } } );

			// flashed.
			if( *it == 3 && player->m_flFlashBangTime( ) > 0.f )
				flags.push_back( { XOR( "FLASHED" ), { 255, 255, 0, low_alpha } } );

			// reload.
			if( *it == 4 ) {
				// get ptr to layer 1.
				C_AnimationLayer* layer1 = &player->m_AnimOverlay( )[ 1 ];

				// check if reload animation is going on.
				if( layer1->m_weight != 0.f && player->GetSequenceActivity( layer1->m_sequence ) == 967 /* ACT_CSGO_RELOAD */ )
					flags.push_back( { XOR( "RELOAD" ), { 60, 180, 225, low_alpha } } );
			}

			// bomb.
			if( *it == 5 && player->HasC4( ) )
				flags.push_back( { XOR( "BOMB" ), { 255, 0, 0, low_alpha } } );

			if ( *it == 6 && enemy ) {
				// get our aimplayer.
				AimPlayer* data = &g_aimbot.m_players[ index - 1 ];

				if ( !data || data->m_records.empty() )
					continue;

				// get our record.
				LagRecord* lag = data->m_records.front( ).get( );
				
				if( lag ) {
					if( lag->m_broke_lc )
						flags.push_back({ XOR("LC"), { 255, 0, 0, low_alpha } });
					else 
						flags.push_back({ XOR("LC"), { 0, 255, 0, low_alpha } });
				}
			}
		}

		// iterate flags.
		for( size_t i{ }; i < flags.size( ); ++i ) {
			// get flag job (pair).
			const auto& f = flags[ i ];

			int offset = i * ( render::esp_small.m_size.m_height - 1 );

			// draw flag.
			render::esp_small.string( box.x + box.w + 2, box.y + offset, f.second, f.first );
		}
	}

	// draw bottom bars.
	{
		int  offset{ 0 };

		// draw lby update bar.
		if( enemy && g_menu.main.players.lby_update.get( ) ) {
			AimPlayer* data = &g_aimbot.m_players[ player->index( ) - 1 ];

			// make sure everything is valid.
			if( data && data->m_moved &&data->m_records.size( ) ) {
				// grab lag record.
				LagRecord* current = data->m_records.front( ).get( );

				if( current ) {
					if( !( current->m_velocity.length_2d( ) > 0.1 && !current->m_fake_walk ) && data->m_body_index <= 3 ) {
						// calculate box width
						float cycle = std::clamp<float>( data->m_body_update - current->m_anim_time, 0.f, 1.0f );
						float width = ( box.w * cycle ) / 1.1f;

						if( width > 0.f ) {
							// draw.
							render::rect_filled( box.x, box.y + box.h + 2, box.w, 4, { 10, 10, 10, low_alpha } );

							Color clr = g_menu.main.players.lby_update_color.get( );
							clr.a( ) = alpha;
							render::rect( box.x + 1, box.y + box.h + 3, width, 2, clr );

							// move down the offset to make room for the next bar.
							offset += 5;
						}
					}
				}
			}
		}

		// draw weapon.
		if( ( local && g_menu.main.players.weapon.get( 2 ) ) || ( enemy && g_menu.main.players.weapon.get( 0 ) ) || ( !enemy && !local && g_menu.main.players.weapon.get( 1 ) ) ) {
			Weapon* weapon = player->GetActiveWeapon( );
			if( weapon ) {
				WeaponInfo* data = weapon->GetWpnData( );
				if( data ) {
					int bar;
					float scale;

					// the maxclip1 in the weaponinfo
					int max = data->m_max_clip1;
					int current = weapon->m_iClip1( );

					C_AnimationLayer* layer1 = &player->m_AnimOverlay( )[ 1 ];

					// set reload state.
					bool reload = ( layer1->m_weight != 0.f ) && ( player->GetSequenceActivity( layer1->m_sequence ) == 967 );

					// ammo bar.
					if( weapon->m_iItemDefinitionIndex( ) != ZEUS && max != -1 && g_menu.main.players.ammo.get( ) ) {
						// setup current animation.
						float& scaled = m_scaled_ammo[ index - 1 ];
					
						if ( !scaled || scaled < current || scaled < max || scaled > max )
							scaled = current;

						// check for reload.
						if( reload )
							scale = layer1->m_cycle;

						// not reloading.
						else
							scale = scaled / (float) max;

						// relative to bar.
						bar = ( int ) std::round( ( box.w - 2 ) * scale );

						// draw.
						render::rect_filled( box.x, box.y + box.h + 2 + offset, box.w, 4, { 10, 10, 10, very_low_alpha } );

						Color clr = player->enemy(g_cl.m_local) ? g_menu.main.players.ammo_enemy.get() : g_menu.main.players.ammo_friendly.get();

						if ( local )
							clr = g_menu.main.players.ammo_local.get();

						clr.a( ) = alpha;
						render::rect( box.x + 1, box.y + box.h + 3 + offset, bar, 2, clr );

						// less then a 5th of the bullets left.
						if( current <= ( int ) std::round( max / 5 ) && !reload )
							render::esp_small.string( box.x + bar, box.y + box.h + offset, { 255, 255, 255, alpha }, std::to_string( current ), render::ALIGN_CENTER );

						offset += 8;

						// adjust our scaled for next frame.
						if( scaled > current )
							scaled -= 125 * g_csgo.m_globals->m_frametime;
					}

					// text.
					if( g_menu.main.players.weapon_mode.get( ) == 0 ) {
						// construct std::string instance of localized weapon name.
						std::string name{ weapon->GetLocalizedName( ) };

						// smallfonts needs upper case.
						std::transform( name.begin( ), name.end( ), name.begin( ), ::toupper );

						render::esp_small.string( box.x + box.w / 2, box.y + box.h + offset, { 255, 255, 255, alpha }, name, render::ALIGN_CENTER );
					}

					// icons.
					else if( g_menu.main.players.weapon_mode.get( ) == 1 ) {
						// icons are super fat..
						// move them back up.
						offset -= 5;

						std::string icon = tfm::format( XOR( "%c" ), m_weapon_icons[ weapon->m_iItemDefinitionIndex( ) ] );
						render::cs.string( box.x + box.w / 2, box.y + box.h + offset, { 255, 255, 255, alpha }, icon, render::ALIGN_CENTER );
					}
				}
			}
		}
	}
}

void Visuals::DrawPlantedC4( ) {
	bool        is_visible;
	float       explode_time_diff, dist, range_damage;
	vec3_t      dst, to_target;
	int         final_damage;
	std::string time_str, damage_str;
	Color       damage_color;
	vec2_t      screen_pos;

	static auto scale_damage = [ ] ( float damage, int armor_value ) {
		float new_damage, armor;

		if( armor_value > 0 ) {
			new_damage = damage * 0.5f;
			armor = ( damage - new_damage ) * 0.5f;

			if( armor > ( float ) armor_value ) {
				armor = ( float ) armor_value * 2.f;
				new_damage = damage - armor;
			}

			damage = new_damage;
		}

		return std::max( 0, ( int ) std::floor( damage ) );
	};

	// bomb not currently active, do nothing.
	if( !m_c4_planted )
		return;

	// calculate bomb damage.
	// references:
	//     https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/shared/cstrike/weapon_c4.cpp#L271
	//     https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/game/shared/cstrike/weapon_c4.cpp#L437
	//     https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/sdk/sdk_gamerules.cpp#L173
	{
		// get our distance to the bomb.
		// todo - dex; is dst right? might need to reverse CBasePlayer::BodyTarget...
		dst = g_cl.m_local->WorldSpaceCenter( );
		to_target = m_planted_c4_explosion_origin - dst;
		dist = to_target.length( );

		// calculate the bomb damage based on our distance to the C4's explosion.
		range_damage = m_planted_c4_damage * std::exp( ( dist * dist ) / ( ( m_planted_c4_radius_scaled * -2.f ) * m_planted_c4_radius_scaled ) );

		// now finally, scale the damage based on our armor (if we have any).
		final_damage = scale_damage( range_damage, g_cl.m_local->m_ArmorValue( ) );
	}

	// m_flC4Blow is set to gpGlobals->curtime + m_flTimerLength inside CPlantedC4.
	explode_time_diff = m_planted_c4_explode_time - g_csgo.m_globals->m_curtime;

	// get formatted strings for bomb.
	time_str = tfm::format( XOR( "%s - %.2f" ), m_last_bombsite, explode_time_diff );
	damage_str = tfm::format( XOR( "%i" ), final_damage );

	// get damage color.
	damage_color = ( final_damage < g_cl.m_local->m_iHealth( ) ) ? colors::white : colors::red;

	// finally do all of our rendering.
	is_visible = render::WorldToScreen( m_planted_c4_explosion_origin, screen_pos );

	// render 2d operations.
	if( g_menu.main.visuals.planted_c4.get(0) && explode_time_diff > 0.f )
		render::indicator.string( 4, 85, colors::white, time_str, render::ALIGN_LEFT );

	if( g_menu.main.visuals.planted_c4.get(2) && g_cl.m_local->alive( ) )
		render::indicator.string( 4, 85 + render::indicator.m_size.m_height, damage_color, damage_str, render::ALIGN_LEFT );

	// 'on bomb (3D)'.
	if( is_visible ) {
		if( g_menu.main.visuals.planted_c4.get(1) && explode_time_diff > 0.f )
			render::esp_small.string( screen_pos.x, screen_pos.y, colors::white, time_str, render::ALIGN_CENTER );

		// only render damage string if we're alive.
		if( g_menu.main.visuals.planted_c4.get(3) && g_cl.m_local->alive( ) )
			render::esp_small.string( screen_pos.x, ( int ) screen_pos.y + render::esp_small.m_size.m_height, damage_color, damage_str, render::ALIGN_CENTER );
	}
}

void Visuals::UpdateShadows( ) {
	if ( !m_cascade_light )
		return;

	m_cascade_light->m_envLightShadowDirection( ) = g_menu.main.visuals.shadow_modulation.get() ? vec3_t{ 
		g_menu.main.visuals.shadow_modulation_x.get( ) / 100.f,
		g_menu.main.visuals.shadow_modulation_y.get( ) / 100.f,
		g_menu.main.visuals.shadow_modulation_z.get( ) / 100.f
	} : m_cascade_shadow_direction;
}

bool Visuals::GetPlayerBoxRect( Player* player, Rect& box ) {
	vec3_t origin, mins, maxs;
	vec2_t bottom, top;

	// get interpolated origin.
	origin = player->GetAbsOrigin( );

	// get hitbox bounds.
	player->ComputeHitboxSurroundingBox( &mins, &maxs );

	// correct x and y coordinates.
	mins = { origin.x, origin.y, mins.z };
	maxs = { origin.x, origin.y, maxs.z + 8.f };

	if( !render::WorldToScreen( mins, bottom ) || !render::WorldToScreen( maxs, top ) )
		return false;

	box.h = bottom.y - top.y;
	box.w = box.h / 2.f;
	box.x = bottom.x - ( box.w / 2.f );
	box.y = bottom.y - box.h;

	return true;
}

void Visuals::DrawHistorySkeleton( Player* player, int opacity ) {
	const model_t* model;
	studiohdr_t* hdr;
	mstudiobone_t* bone;
	AimPlayer* data;
	LagRecord* record;
	int           parent;
	vec3_t        bone_pos, parent_pos;
	vec2_t        bone_pos_screen, parent_pos_screen;

	if( !g_menu.main.misc.fake_latency.get( ) )
		return;

	// get player's model.
	model = player->GetModel( );
	if( !model )
		return;

	// get studio model.
	hdr = g_csgo.m_model_info->GetStudioModel( model );
	if( !hdr )
		return;

	data = &g_aimbot.m_players[ player->index( ) - 1 ];
	if( !data )
		return;

	record = g_resolver.FindLastRecord( data );
	if( !record )
		return;

	for( int i{ }; i < hdr->m_num_bones; ++i ) {
		// get bone.
		bone = hdr->GetBone( i );
		if( !bone || !( bone->m_flags & BONE_USED_BY_HITBOX ) )
			continue;

		// get parent bone.
		parent = bone->m_parent;
		if( parent == -1 )
			continue;

		// resolve main bone and parent bone positions.
		record->m_bones->get_bone( bone_pos, i );
		record->m_bones->get_bone( parent_pos, parent );

		Color clr = player->enemy( g_cl.m_local ) ? g_menu.main.players.skeleton_enemy.get( ) : g_menu.main.players.skeleton_friendly.get( );
		clr.a( ) = opacity;

		// world to screen both the bone parent bone then draw.
		if( render::WorldToScreen( bone_pos, bone_pos_screen ) && render::WorldToScreen( parent_pos, parent_pos_screen ) )
			render::line( bone_pos_screen.x, bone_pos_screen.y, parent_pos_screen.x, parent_pos_screen.y, clr );
	}
}

void Visuals::DrawSkeleton( Player* player, int opacity ) {
	const model_t* model;
	studiohdr_t* hdr;
	mstudiobone_t* bone;
	int           parent;
	BoneArray     matrix[ 128 ];
	vec3_t        bone_pos, parent_pos;
	vec2_t        bone_pos_screen, parent_pos_screen;

	// get player's model.
	model = player->GetModel( );
	if( !model )
		return;

	// get studio model.
	hdr = g_csgo.m_model_info->GetStudioModel( model );
	if( !hdr )
		return;

	// get bone matrix.
	if( !player->SetupBones( matrix, 128, BONE_USED_BY_ANYTHING, g_csgo.m_globals->m_curtime ) )
		return;

	// get our color
	Color clr = player->enemy(g_cl.m_local) ? g_menu.main.players.skeleton_enemy.get() : g_menu.main.players.skeleton_friendly.get();

	if ( player->m_bIsLocalPlayer() )
		clr = g_menu.main.players.skeleton_local.get();

	clr.a() = opacity;

	for( int i{ }; i < hdr->m_num_bones; ++i ) {
		// get bone.
		bone = hdr->GetBone( i );
		if( !bone || !( bone->m_flags & BONE_USED_BY_HITBOX ) )
			continue;

		// get parent bone.
		parent = bone->m_parent;
		if( parent == -1 )
			continue;

		// resolve main bone and parent bone positions.
		matrix->get_bone( bone_pos, i );
		matrix->get_bone( parent_pos, parent );

		// world to screen both the bone parent bone then draw.
		if( render::WorldToScreen( bone_pos, bone_pos_screen ) && render::WorldToScreen( parent_pos, parent_pos_screen ) )
			render::line( bone_pos_screen.x, bone_pos_screen.y, parent_pos_screen.x, parent_pos_screen.y, clr );
	}
}

void Visuals::RenderGlow( ) {
	Color   color;
	Player* player;

	if( !g_cl.m_local )
		return;

	if( !g_csgo.m_glow->m_object_definitions.Count( ) )
		return;

	float blend = g_menu.main.players.glow_blend.get( ) / 100.f;
	int style = g_menu.main.players.glow_style.get( );
	bool bloom = g_menu.main.players.glow_bloom.get( );

	for( int i{ }; i < g_csgo.m_glow->m_object_definitions.Count( ); ++i ) {
		GlowObjectDefinition_t* obj = &g_csgo.m_glow->m_object_definitions[ i ];

		// skip non-players.
		if( !obj->m_entity || !obj->m_entity->IsPlayer( ) )
			continue;

		// get player ptr.
		player = obj->m_entity->as< Player* >( );

		// get local.
		bool local = player->m_bIsLocalPlayer( );

		// get reference to array variable.
		float& opacity = m_opacities[ player->index( ) - 1 ];

		bool enemy = player->enemy( g_cl.m_local );

		if( enemy && !g_menu.main.players.glow.get( 0 ) )
			continue;

		if( !enemy && !local && !g_menu.main.players.glow.get( 1 ) )
			continue;

		if( local && !g_menu.main.players.glow.get( 2 )  )
			continue;

		// enemy color.
		if( enemy )
			color = g_menu.main.players.glow_enemy.get( );

		// local color.
		else if( local )
			color = g_menu.main.players.glow_local.get( );

		// friendly color.
		else
			color = g_menu.main.players.glow_friendly.get( );

		obj->m_render_style = style;
		obj->m_render_occluded = true;
		obj->m_render_unoccluded = false;
		obj->m_render_full_bloom = bloom;
		obj->m_color = { ( float ) color.r( ) / 255.f, ( float ) color.g( ) / 255.f, ( float ) color.b( ) / 255.f };
		obj->m_alpha = opacity * blend;
	}
}

void Visuals::DrawHitboxMatrix( LagRecord* record, Color col, float time ) {
	const model_t* model;
	studiohdr_t* hdr;
	mstudiohitboxset_t* set;
	mstudiobbox_t* bbox;
	vec3_t             mins, maxs, origin;
	ang_t			   angle;

	model = record->m_player->GetModel( );
	if( !model )
		return;

	hdr = g_csgo.m_model_info->GetStudioModel( model );
	if( !hdr )
		return;

	set = hdr->GetHitboxSet( record->m_player->m_nHitboxSet( ) );
	if( !set )
		return;

	for( int i{ }; i < set->m_hitboxes; ++i ) {
		bbox = set->GetHitbox( i );
		if( !bbox )
			continue;

		// bbox.
		if( bbox->m_radius <= 0.f ) {
			// https://developer.valvesoftware.com/wiki/Rotation_Tutorial

			// convert rotation angle to a matrix.
			matrix3x4_t rot_matrix;
			g_csgo.AngleMatrix( bbox->m_angle, rot_matrix );

			// apply the rotation to the entity input space (local).
			matrix3x4_t matrix;
			math::ConcatTransforms( record->m_bones[ bbox->m_bone ], rot_matrix, matrix );

			// extract the compound rotation as an angle.
			ang_t bbox_angle;
			math::MatrixAngles( matrix, bbox_angle );

			// extract hitbox origin.
			vec3_t origin = matrix.GetOrigin( );

			// draw box.
			g_csgo.m_debug_overlay->AddBoxOverlay( origin, bbox->m_mins, bbox->m_maxs, bbox_angle, col.r( ), col.g( ), col.b( ), 0, time );
		}

		// capsule.
		else {
			// NOTE; the angle for capsules is always 0.f, 0.f, 0.f.

			// create a rotation matrix.
			matrix3x4_t matrix;
			g_csgo.AngleMatrix( bbox->m_angle, matrix );

			// apply the rotation matrix to the entity output space (world).
			math::ConcatTransforms( record->m_bones[ bbox->m_bone ], matrix, matrix );

			// get world positions from new matrix.
			math::VectorTransform( bbox->m_mins, matrix, mins );
			math::VectorTransform( bbox->m_maxs, matrix, maxs );

			g_csgo.m_debug_overlay->AddCapsuleOverlay( mins, maxs, bbox->m_radius, col.r( ), col.g( ), col.b( ), col.a( ), time, 0, 0 );
		}
	}
}

void Visuals::DrawBulletImpacts( ) 	{
	// handles drawing the clientside bullet impacts.
	if( !g_cl.m_processing )
		return;

	auto impacts = g_cl.m_local->m_vecBulletVerifyListClient( );

	if( g_menu.main.visuals.impact_boxes.get( 1 ) ) { 
		Color col = g_menu.main.visuals.impact_boxes_client.get( );
		float time = g_menu.main.visuals.impact_boxes_time.get( );

		for ( int i = impacts.Count(); i > bullet_counter; i-- )
		{
			g_csgo.m_debug_overlay->AddBoxOverlay( impacts[ i - 1 ].vecPosition, vec3_t(-2, -2, -2), vec3_t(2, 2, 2), ang_t(0, 0, 0), col.r( ), col.g( ), col.b( ), 127, time );
		}
	}

	if ( impacts.Count() != bullet_counter )
		bullet_counter = impacts.Count();
}

void Visuals::DrawBeams( ) {
	size_t     impact_count;
	float      curtime, dist;
	bool       is_final_impact;
	vec3_t     va_fwd, start, dir, end;
	BeamInfo_t beam_info;
	Beam_t* beam;

	if( !g_cl.m_local )
		return;

	if( !g_menu.main.visuals.impact_beams.get( ) )
		return;

	auto vis_impacts = &g_shots.m_vis_impacts;

	// the local player is dead, clear impacts.
	if( !g_cl.m_processing ) {
		if( !vis_impacts->empty( ) )
			vis_impacts->clear( );
	}

	else {
		impact_count = vis_impacts->size( );
		if( !impact_count )
			return;

		curtime = game::TICKS_TO_TIME( g_cl.m_local->m_nTickBase( ) );

		for( size_t i{ impact_count }; i-- > 0; ) {
			auto impact = &vis_impacts->operator[ ]( i );
			if( !impact )
				continue;

			// impact is too old, erase it.
			if( std::abs( curtime - game::TICKS_TO_TIME( impact->m_tickbase ) ) > g_menu.main.visuals.impact_beams_time.get( ) ) {
				vis_impacts->erase( vis_impacts->begin( ) + i );

				continue;
			}

			// already rendering this impact, skip over it.
			if( impact->m_ignore )
				continue;

			// is this the final impact?
			// last impact in the vector, it's the final impact.
			if( i == ( impact_count - 1 ) )
				is_final_impact = true;

			// the current impact's tickbase is different than the next, it's the final impact.
			else if( ( i + 1 ) < impact_count && impact->m_tickbase != vis_impacts->operator[ ]( i + 1 ).m_tickbase )
				is_final_impact = true;

			else
				is_final_impact = false;

			// is this the final impact?
			// is_final_impact = ( ( i == ( impact_count - 1 ) ) || ( impact->m_tickbase != vis_impacts->at( i + 1 ).m_tickbase ) );

			if( is_final_impact ) {
				// calculate start and end position for beam.
				start = impact->m_shoot_pos;

				dir = ( impact->m_impact_pos - start ).normalized( );
				dist = ( impact->m_impact_pos - start ).length( );

				end = start + ( dir * dist );

				// setup beam info.
				// note - dex; possible beam models: sprites/physbeam.vmt | sprites/white.vmt
				beam_info.m_vecStart = start;
				beam_info.m_vecEnd = end;
				beam_info.m_nModelIndex = g_csgo.m_model_info->GetModelIndex( XOR( "sprites/purplelaser1.vmt" ) );
				beam_info.m_pszModelName = XOR( "sprites/purplelaser1.vmt" );
				beam_info.m_flHaloScale = 0.f;
				beam_info.m_flLife = g_menu.main.visuals.impact_beams_time.get( );
				beam_info.m_flWidth = 2.f;
				beam_info.m_flEndWidth = 2.f;
				beam_info.m_flFadeLength = 0.f;
				beam_info.m_flAmplitude = 0.f;   // beam 'jitter'.
				beam_info.m_flBrightness = 255.f;
				beam_info.m_flSpeed = 0.5f;  // seems to control how fast the 'scrolling' of beam is... once fully spawned.
				beam_info.m_nStartFrame = 0;
				beam_info.m_flFrameRate = 0.f;
				beam_info.m_nSegments = 2;     // controls how much of the beam is 'split up', usually makes m_flAmplitude and m_flSpeed much more noticeable.
				beam_info.m_bRenderable = true;  // must be true or you won't see the beam.
				beam_info.m_nFlags = 0;

				if( !impact->m_hit_player ) {
					beam_info.m_flRed = g_menu.main.visuals.impact_beams_color.get( ).r( );
					beam_info.m_flGreen = g_menu.main.visuals.impact_beams_color.get( ).g( );
					beam_info.m_flBlue = g_menu.main.visuals.impact_beams_color.get( ).b( );
				}

				else {
					beam_info.m_flRed = g_menu.main.visuals.impact_beams_hurt_color.get( ).r( );
					beam_info.m_flGreen = g_menu.main.visuals.impact_beams_hurt_color.get( ).g( );
					beam_info.m_flBlue = g_menu.main.visuals.impact_beams_hurt_color.get( ).b( );
				}

				// attempt to render the beam.
				beam = game::CreateGenericBeam( beam_info );
				if( beam ) {
					g_csgo.m_beams->DrawBeam( beam );

					// we only want to render a beam for this impact once.
					impact->m_ignore = true;
				}
			}
		}
	}
}

void Visuals::DebugAimbotPoints( Player* player ) {
	std::vector< vec3_t > p2{ };

	AimPlayer* data = &g_aimbot.m_players.at( player->index( ) - 1 );
	if( !data || data->m_records.empty( ) )
		return;

	LagRecord* front = data->m_records.front( ).get( );
	if( !front || front->dormant( ) )
		return;

	// get bone matrix.
	BoneArray matrix[ 128 ];
	if( !g_bones.setup( player, matrix, front ) )
		return;

	data->SetupHitboxes( front, false );
	if( data->m_hitboxes.empty( ) )
		return;

	for( const auto& it : data->m_hitboxes ) {
		std::vector< vec3_t > p1{ };

		if( !data->SetupHitboxPoints( front, matrix, it.m_index, p1 ) )
			continue;

		for( auto& p : p1 )
			p2.push_back( p );
	}

	if( p2.empty( ) )
		return;

	for( auto& p : p2 ) {
		vec2_t screen;

		if( render::WorldToScreen( p, screen ) )
			render::rect_filled( screen.x, screen.y, 2, 2, { 0, 255, 255, 255 } );
	}
}