#include "includes.h"

Chams g_chams{ };;

Chams::model_type_t Chams::GetModelType(const ModelRenderInfo_t& info) {
	std::string mdl{ info.m_model->m_name };

	if (mdl.find(XOR("player")) != std::string::npos && info.m_index >= 1 && info.m_index <= 64)
		return model_type_t::player;

	return model_type_t::invalid;
}

bool Chams::IsInViewPlane(const vec3_t& world) {
	float w;

	const VMatrix& matrix = g_csgo.m_engine->WorldToScreenMatrix();

	w = matrix[3][0] * world.x + matrix[3][1] * world.y + matrix[3][2] * world.z + matrix[3][3];

	return w > 0.001f;
}

void Chams::SetColor(Color col, IMaterial* mat) {
	if (mat)
		mat->ColorModulate(col);

	else
		g_csgo.m_render_view->SetColorModulation(col);
}

void Chams::SetAlpha(float alpha, IMaterial* mat) {
	if (mat)
		mat->AlphaModulate(alpha);

	else
		g_csgo.m_render_view->SetBlend(alpha);
}

void Chams::SetupMaterialData(IMaterial* mat, Color col, bool z_flag) {
	SetColor(col);

	// mat->SetFlag( MATERIAL_VAR_HALFLAMBERT, flags );
	mat->SetFlag(MATERIAL_VAR_ZNEARER, z_flag);
	mat->SetFlag(MATERIAL_VAR_NOFOG, z_flag);
	mat->SetFlag(MATERIAL_VAR_IGNOREZ, z_flag);

	g_csgo.m_studio_render->ForcedMaterialOverride(mat);
}

void Chams::SetupMaterial(int index, Color col, int blend, bool z) {
	// get material from index.
	IMaterial* material = m_basic[ index ];

	if( !material ) {
		init( );
		return;
	}

	// override blend.
	SetAlpha( blend / 100.f );

	// set material and color.
	SetupMaterialData( material, col, z );
}

void Chams::init() {
	// generate our materials.
	// order matters here, see the material handler for how these are generated. this allows us
	// to avoid having a function that litterally just has a big switch statement.
	m_basic.resize( 3 );
	m_overlays.resize( 5 );

	// basic materials.
	m_basic[ 0 ] = g_materials.Create( XOR( "debug/debugambientcube" ) );
	m_basic[ 1 ] = g_materials.Create( XOR( "debug/debugdrawflat" ) );

	m_basic[ 2 ] = g_materials.New(XOR("zephyr_metal"), R"#( "VertexLitGeneric" 
		{
			"$basetexture" "vgui/white_additive"
			"$bumpmap" "effects/flat_normal"
			"$phong" "1"
			"$phongexponent" "5"	
			"$phongboost" "4.0"
			"$phongfresnelranges" "[0 0.5 1]"
			"$rimlight" "1"
			"$rimlightexponent" "2"
			"$rimlightboost" ".2"
			"$selfillum" "1"
			"$halflambert" "1"
			"$znearer" "0"
			"$nocull" "1"
			"$reflectivity" "[1 1 1]"
		})#");

	// overlay materials.
	m_overlays[ 0 ] = g_materials.New( XOR( "zephyr_blink" ), R"#( "VertexLitGeneric" 
		{
			"$additive"  "1"
			"$color"     "[1 1 1]"
	
			"Proxies"
			{
				"Sine"
				{
					"sineperiod"    0.8
					"sinemin"		0
					"sinemax"		5
					"resultvar"     "$color"
				}
			}
		})#");

	m_overlays[ 1 ] = g_materials.New( XOR( "zephyr_glow" ), R"#( "VertexLitGeneric" 
		{
			"$additive" "1"
			"$envmap" "models/effects/cube_white"
			"$envmaptint" "[1 1 1]"
			"$envmapfresnel" "1"
			"$envmapfresnelminmaxexp" "[0 1 2]"
			"$alpha" "0.8"
		})#");

	m_overlays[ 2 ] = g_materials.New( XOR( "zephyr_animated" ), R"#( "VertexLitGeneric" 
		{
			"$basetexture" "models/weapons/customization/paints/anodized_multi/smoke"
			"$nofog" "1"
			"$envmap" "env_cubemap"
			"$envmaptint" "[1 1 1]"
			"$phong" "1"
			"$basemapalphaphongmask" "1"
			"$phongboost" "0"
			"$rimlight" "1"
			"$phongtint" "[1 1 1]"
			"$rimlightexponent" "10"
			"$rimlightboost" "0"
			"$selfillum" "1"
			"$ignorez" "0"
			
			"Proxies"
			{
				"TextureScroll"
				{
					"textureScrollVar" "$BasetextureTransform"
					"textureScrollRate" "0.8"
					"textureScrollAngle" "60.0"
				}
			}
		})#");

	m_overlays[ 3 ] = g_materials.New( XOR( "zephyr_circles" ), R"#( "VertexLitGeneric" 
		{
			"$basetexture" "models/weapons/customization/paints/anodized_multi/gyrate_cz75"
			"$nofog" "1"
			"$envmap" "env_cubemap"
			"$envmaptint" "[1 1 1]"
			"$phong" "1"
			"$basemapalphaphongmask" "1"
			"$phongboost" "0"
			"$rimlight" "1"
			"$phongtint" "[1 1 1]"
			"$rimlightexponent" "10"
			"$rimlightboost" "0"
			"$selfillum" "1"
			"$ignorez" "0"
			
			"Proxies"
			{
				"TextureScroll"
				{
					"textureScrollVar" "$BasetextureTransform"
					"textureScrollRate" "0.5"
					"textureScrollAngle" "45.0"
				}
			}
		})#");

	m_overlays[ 4 ] = g_materials.New( XOR( "zephyr_liner" ), R"#( "VertexLitGeneric" 
		{
			"$basetexture" "models/weapons/customization/paints/hydrographic/liner"
			"$nofog" "1"
			"$envmap" "env_cubemap"
			"$envmaptint" "[1 1 1]"
			"$phong" "1"
			"$basemapalphaphongmask" "1"
			"$phongboost" "0"
			"$rimlight" "1"
			"$phongtint" "[1 1 1]"
			"$rimlightexponent" "10"
			"$rimlightboost" "0"
			"$selfillum" "1"
			"$ignorez" "0"
			
			"Proxies"
			{
				"TextureScroll"
				{
					"textureScrollVar" "$BasetextureTransform"
					"textureScrollRate" "0.5"
					"textureScrollAngle" "0.0"
				}
			}
		})#");
}

bool Chams::OverridePlayer(int index) {
	Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(index);
	if (!player)
		return false;

	// always skip the local player in DrawModelExecute.
	// this is because if we want to make the local player have less alpha
	// the static props are drawn after the players and it looks like aids.
	// therefore always process the local player in scene end.
	if (player->m_bIsLocalPlayer())
		return true;

	// see if this player is an enemy to us.
	bool enemy = g_cl.m_local && player->enemy(g_cl.m_local);

	// we have chams on enemies.
	if (enemy && g_menu.main.players.chams_enemy_visible.get( ))
		return true;

	// we have chams on friendly.
	else if (!enemy && g_menu.main.players.chams_friendly_visible.get( ))
		return true;

	return false;
}

bool Chams::GenerateLerpedMatrix(int index, BoneArray* out) {
	LagRecord* current_record;
	AimPlayer* data;

	Player* ent = g_csgo.m_entlist->GetClientEntity< Player* >(index);
	if (!ent)
		return false;

	if (!g_aimbot.IsValidTarget(ent))
		return false;

	data = &g_aimbot.m_players[index - 1];
	if (!data || data->m_records.empty())
		return false;

	if (data->m_records.size() < 2)
		return false;

	auto* channel_info = g_csgo.m_engine->GetNetChannelInfo();
	if (!channel_info)
		return false;

	static float max_unlag = 0.2f;
	static auto sv_maxunlag = g_csgo.m_cvar->FindVar(HASH("sv_maxunlag"));
	if (sv_maxunlag) {
		max_unlag = sv_maxunlag->GetFloat();
	}

	for (auto it = data->m_records.rbegin(); it != data->m_records.rend(); it++) {
		current_record = it->get();

		bool end = it + 1 == data->m_records.rend();

		if (current_record && current_record->valid() && (!end && ((it + 1)->get()))) {
			if (current_record->m_origin.dist_to(ent->GetAbsOrigin()) < 1.f) {
				return false;
			}

			vec3_t next = end ? ent->GetAbsOrigin() : (it + 1)->get()->m_origin;
			float  time_next = end ? ent->m_flSimulationTime() : (it + 1)->get()->m_sim_time;

			float total_latency = channel_info->GetAvgLatency(0) + channel_info->GetAvgLatency(1);
			
			float correct = std::clamp(total_latency, 0.f, max_unlag) + g_cl.m_lerp;
			float time_delta = time_next - current_record->m_sim_time;
			float add = end ? 1.f : time_delta;
			float deadtime = current_record->m_sim_time + correct + add;

			float curtime = g_csgo.m_globals->m_curtime;
			float delta = deadtime - curtime;

			float mul = 1.f / add;
			vec3_t lerp = math::Interpolate(next, current_record->m_origin, std::clamp(delta * mul, 0.f, 1.f));

			matrix3x4_t ret[128];

			std::memcpy(ret,
				current_record->m_bones,
				sizeof(ret));

			for (size_t i{ }; i < 128; ++i) {
				vec3_t matrix_delta = current_record->m_bones[i].GetOrigin() - current_record->m_origin;
				ret[i].SetOrigin(matrix_delta + lerp);
			}

			std::memcpy(out,
				ret,
				sizeof(ret));

			return true;
		}
	}

	return false;
}

void Chams::RenderHistoryChams(int index) {
	AimPlayer* data;
	LagRecord* record;

	Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(index);
	if (!player)
		return;

	if (!g_aimbot.IsValidTarget(player))
		return;

	bool enemy = g_cl.m_local && player->enemy(g_cl.m_local);
	if (enemy) {
		data = &g_aimbot.m_players[index - 1];
		if (!data || data->m_records.empty())
			return;

		record = g_resolver.FindLastRecord(data);
		if (!record)
			return;

		// load material.
		SetupMaterial( g_menu.main.players.chams_enemy_history_material.get( ), g_menu.main.players.chams_enemy_history_color.get( ), g_menu.main.players.chams_enemy_history_blend.get( ), true );

		// was the matrix properly setup?
		BoneArray arr[128];
		if (Chams::GenerateLerpedMatrix(index, arr)) {
			// backup the bone cache before we fuck with it.
			auto backup_bones = player->m_BoneCache().m_pCachedBones;

			// replace their bone cache with our custom one.
			player->m_BoneCache().m_pCachedBones = arr;

			// manually draw the model.
			player->DrawModel();

			// reset their bone cache to the previous one.
			player->m_BoneCache().m_pCachedBones = backup_bones;
		}
	}
}

void Chams::RenderOverlay( Player* player, Color col, int blend, int index ) {
	// get material.
	IMaterial* material = m_overlays.at( index );

	if ( index == 1 ) {
		material->FindVar( "$envmaptint", nullptr, false )->SetVecValue( col.r( ) / 255.f, col.g( ) / 255.f, col.b( ) / 255.f );
	}

	// override blend.
	SetAlpha( blend / 100.f );

	// set material and color.
	SetupMaterialData( material, col, false );

	// render player.
	player->DrawModel( );
}

void Chams::RenderChams(Player* player) {
	// only draw the localplayer without chams.
	if( g_menu.main.players.chams_local_real_blend_scope.get( )  && player->m_bIsScoped() ) { 
		SetAlpha(0.5f);

		player->DrawModel( );

		return;
	}

	// check if we are enabled.
	if( g_menu.main.players.chams_local_real.get( ) ) {
		// set material and color.
		SetupMaterial( g_menu.main.players.chams_local_real_material.get( ), g_menu.main.players.chams_local_real_color.get( ), g_menu.main.players.chams_local_real_blend.get( ), false );
	}

	// draw our regular model.
	player->DrawModel( );

	// draw our overlay if needed.
	if( g_menu.main.players.chams_local_real_overlay.get( ) )
		RenderOverlay( player, g_menu.main.players.chams_local_real_overlay_color.get( ), g_menu.main.players.chams_local_real_overlay_blend.get( ), g_menu.main.players.chams_local_real_overlay_material.get( ) );

	// draw ghost chams.
	RenderGhostChams(player);
}

void Chams::RenderGhostChams(Player* player) {
	// set our angles. radar is an easy way of tracking our fake.
	g_cl.SetAngles2(ang_t(0.f, g_cl.m_radar.y, 0.f));

	if ( g_menu.main.players.chams_local_fake.get() ) {
		// set material and color.
		SetupMaterial( g_menu.main.players.chams_local_fake_material.get( ), g_menu.main.players.chams_local_fake_color.get( ), g_menu.main.players.chams_local_fake_blend.get( ), false );

		// draw our model.
		player->DrawModel();
	}

	// draw our overlay if needed.
	if ( g_menu.main.players.chams_local_fake_overlay.get() )
		RenderOverlay(player, g_menu.main.players.chams_local_fake_overlay_color.get(), g_menu.main.players.chams_local_fake_overlay_blend.get(), g_menu.main.players.chams_local_fake_overlay_material.get());

	// set our original angles.
	g_cl.SetAngles( );
}

bool Chams::DrawModel(uintptr_t ctx, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* bone) {
	// store and validate model type.
	model_type_t type = GetModelType(info);
	if (type == model_type_t::invalid)
		return true;

	// is a valid player.
	if (type == model_type_t::player) {
		// do not cancel out our own calls from SceneEnd
		// also do not cancel out calls from the glow.
		if (!m_running && !g_csgo.m_studio_render->m_pForcedMaterial && OverridePlayer(info.m_index))
			return false;
	}

	return true;
}

void Chams::SceneEnd() {
	// store and sort ents by distance.
	if (SortPlayers()) {
		// iterate each player and render them.
		for (const auto& p : m_players)
			RenderPlayer(p);
	}

	// restore.
	g_csgo.m_studio_render->ForcedMaterialOverride(nullptr);
	g_csgo.m_render_view->SetColorModulation(colors::white);
	g_csgo.m_render_view->SetBlend(1.f);
}

void Chams::RenderPlayer(Player* player) {
	// prevent recruisive model cancelation.
	m_running = true;

	// restore.
	g_csgo.m_studio_render->ForcedMaterialOverride(nullptr);
	g_csgo.m_render_view->SetColorModulation(colors::white);
	g_csgo.m_render_view->SetBlend(1.f);

	// render local chams.
	if( player->m_bIsLocalPlayer( ) ) {
		RenderChams(player);
		m_running = false;
		return;
	}

	// check if is an enemy.
	bool enemy = g_cl.m_local && player->enemy(g_cl.m_local);

	// enemy chams.
	if (enemy ) {
		if( g_menu.main.players.chams_enemy_history.get( ) )
			RenderHistoryChams(player->index());

		if ( g_menu.main.players.chams_enemy_visible.get( )) {
			if (g_menu.main.players.chams_enemy_invisible.get( )) {
				SetupMaterial(g_menu.main.players.chams_enemy_invisible_material.get(), g_menu.main.players.chams_enemy_invisible_color.get(), g_menu.main.players.chams_enemy_invisible_blend.get(), true);

				player->DrawModel();
			}

			SetupMaterial(g_menu.main.players.chams_enemy_visible_material.get(), g_menu.main.players.chams_enemy_visible_color.get(), g_menu.main.players.chams_enemy_visible_blend.get(), false);

			player->DrawModel();
		}

		// draw our overlay if needed.
		if ( g_menu.main.players.chams_enemy_visible_overlay.get() )
			RenderOverlay(player, g_menu.main.players.chams_enemy_visible_overlay_color.get(), g_menu.main.players.chams_enemy_visible_overlay_blend.get(), g_menu.main.players.chams_enemy_visible_overlay_material.get());
	}
	
	// friendly chams.
	else if ( !enemy ) { // I'm not sure removing this will cause issues with teams that aren't CT or T.
		if( g_menu.main.players.chams_friendly_visible.get() ) {
			if ( g_menu.main.players.chams_friendly_invisible.get( ) ) {
				SetupMaterial(g_menu.main.players.chams_friendly_invisible_material.get(), g_menu.main.players.chams_friendly_invisible_color.get(), g_menu.main.players.chams_friendly_invisible_blend.get(), true);

				player->DrawModel();
			}

			SetupMaterial(g_menu.main.players.chams_friendly_visible_material.get(), g_menu.main.players.chams_friendly_visible_color.get(), g_menu.main.players.chams_friendly_visible_blend.get(), false);

			player->DrawModel();
		}

		// draw our overlay if needed.
		if ( g_menu.main.players.chams_friendly_visible_overlay.get() )
			RenderOverlay(player, g_menu.main.players.chams_friendly_visible_overlay_color.get(), g_menu.main.players.chams_friendly_visible_overlay_blend.get(), g_menu.main.players.chams_friendly_visible_overlay_material.get());
	}

	m_running = false;
}

bool Chams::SortPlayers() {
	// lambda-callback for std::sort.
	// to sort the players based on distance to the local-player.
	static auto distance_predicate = [](Entity* a, Entity* b) {
		vec3_t local = g_cl.m_local->GetAbsOrigin();

		// note - dex; using squared length to save out on sqrt calls, we don't care about it anyway.
		float len1 = (a->GetAbsOrigin() - local).length_sqr();
		float len2 = (b->GetAbsOrigin() - local).length_sqr();

		return len1 < len2;
	};

	// reset player container.
	m_players.clear();

	// find all players that should be rendered.
	for (int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i) {
		// get player ptr by idx.
		Player* player = g_csgo.m_entlist->GetClientEntity< Player* >(i);

		// validate.
		if (!player || !player->IsPlayer() || !player->alive() || player->dormant())
			continue;

		// do not draw players occluded by view plane.
		if (!IsInViewPlane(player->WorldSpaceCenter()))
			continue;

		// this player was not skipped to draw later.
		// so do not add it to our render list.
		if (!OverridePlayer(i))
			continue;

		m_players.push_back(player);
	}

	// any players?
	if (m_players.empty())
		return false;

	// sorting fixes the weird weapon on back flickers.
	// and all the other problems regarding Z-layering in this shit game.
	std::sort(m_players.begin(), m_players.end(), distance_predicate);

	return true;
}