#include "includes.h"

void Hooks::LevelInitPreEntity( const char* map ) {
	float rate{ 1.f / g_csgo.m_globals->m_interval };

	// set rates when joining a server.
	g_csgo.cl_updaterate->SetValue( rate );
	g_csgo.cl_cmdrate->SetValue( rate );

	g_aimbot.reset( );
	g_visuals.m_hit_start = g_visuals.m_hit_end = g_visuals.m_hit_duration = 0.f;

	// invoke original method.
	g_hooks.m_client.GetOldMethod< LevelInitPreEntity_t >( CHLClient::LEVELINITPREENTITY )( this, map );
}

void Hooks::LevelInitPostEntity( ) {
	g_cl.OnMapload( );

	// invoke original method.
	g_hooks.m_client.GetOldMethod< LevelInitPostEntity_t >( CHLClient::LEVELINITPOSTENTITY )( this );
}

void Hooks::LevelShutdown( ) {
	g_aimbot.reset( );

	g_cl.m_local       = nullptr;
	g_cl.m_weapon      = nullptr;
	g_cl.m_processing  = false;
	g_cl.m_weapon_info = nullptr;
	g_cl.m_round_end   = false;

	g_cl.m_sequences.clear( );

	// invoke original method.
	g_hooks.m_client.GetOldMethod< LevelShutdown_t >( CHLClient::LEVELSHUTDOWN )( this );
}

int Hooks::IN_KeyEvent( int evt, int key, const char* bind ) {
	// see if this key event was fired for the drop bind.
	if( bind && evt && FNV1a::get( bind ) == HASH( "drop" ) ) {
		// delay our drop by one tick, we'll manually drop later on in EndMove.
		if( g_cl.m_local ) {
			g_hvh.m_dropping = g_cl.m_local->m_nTickBase( ) + 1;
			return 0;
		}
	}

	return g_hooks.m_client.GetOldMethod< IN_KeyEvent_t >( CHLClient::INKEYEVENT )( this, evt, key, bind );
}

void Hooks::FrameStageNotify( Stage_t stage ) {
	// save stage.
	if( stage != FRAME_START )
		g_cl.m_stage = stage;

	// damn son.
	g_cl.m_local = g_csgo.m_entlist->GetClientEntity< Player* >( g_csgo.m_engine->GetLocalPlayer( ) );

	if( stage == FRAME_RENDER_START ) {	
        // draw our custom beams.
        g_visuals.DrawBeams( );

		// draw our bullet impacts.
		g_visuals.DrawBulletImpacts( );

		// draw our precipitation.
		g_precipitation.Paint();

		// update model interpolation.
		g_animations.UpdateInterpolation( );
	}

	// call original.
	g_hooks.m_client.GetOldMethod< FrameStageNotify_t >( CHLClient::FRAMESTAGENOTIFY )( this, stage );

	if ( stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START ) {
		// run our skins.
		g_skins.think();
	}

	else if( stage == FRAME_NET_UPDATE_END ) {
		// this is the last stage called, prefer it over anything else
		// when messing with clientside data (animations, bones, etc).

		// update animations.
		g_cl.UpdateAnimations( );

		// run our no smoke.
		g_visuals.NoSmoke( );

        // restore non-compressed netvars.
		g_netdata.apply( );

		// update all players.
		// call this here so that all of the bone processing and stuff has already been 
		// done by the client and we can override it with our own instead.
		for( int i{ 1 }; i <= g_csgo.m_globals->m_max_clients; ++i ) {
			Player* player = g_csgo.m_entlist->GetClientEntity< Player* >( i );
			if( !player || player->m_bIsLocalPlayer( ) )
				continue;

			AimPlayer* data = &g_aimbot.m_players[ i - 1 ];
			if( !data )
				continue;

			data->OnNetUpdate( player );
		}
	}
}