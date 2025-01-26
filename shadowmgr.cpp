#include "includes.h"

void Hooks::ComputeShadowDepthTextures( const CViewSetup &view, bool setup ) {
	//  you can actually fix enemy feet shadows being glitchy on the ground by doing this.
	g_csgo.cl_foot_contact_shadows->SetValue( 0 );

	// run our remove shadows.
	if ( g_menu.main.visuals.removals.get(8) ) {
		g_csgo.cl_csm_shadows->SetValue( 0 );
	} 
	else { 
		g_csgo.cl_csm_shadows->SetValue( 1 );
	}

	g_hooks.m_shadow_mgr.GetOldMethod< ComputeShadowDepthTextures_t >( IClientShadowMgr::COMPUTESHADOWDEPTHTEXTURES )( this, view, setup);
}