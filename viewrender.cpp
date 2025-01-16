#include "includes.h"


void Hooks::OnRenderStart() {
	// call og.
	g_hooks.m_view_render.GetOldMethod< OnRenderStart_t >( CViewRender::ONRENDERSTART )( this );

	// get our config variables.
	float fov_amount = g_menu.main.visuals.fov_amt.get();
	bool fov_override = g_menu.main.visuals.fov.get();

	// run our scoped calculations.
	if ( g_cl.m_local && g_cl.m_local->m_bIsScoped() ) {
		Weapon* weapon = g_cl.m_local->GetActiveWeapon();

		if ( g_menu.main.visuals.fov_scoped.get() && weapon ) {
			if ( weapon->m_zoomLevel() != 2 ) {
				if ( fov_override && 90.f >= fov_amount )
					g_csgo.m_view_render->m_view.m_fov = fov_amount;
				else
					g_csgo.m_view_render->m_view.m_fov = 90.f;
			}
			else {
				g_csgo.m_view_render->m_view.m_fov += 45.f;
			}
		}
	}

	// run baseline fov adjustment.
	if ( fov_override && g_cl.m_local && !g_cl.m_local->m_bIsScoped() ) {
		g_csgo.m_view_render->m_view.m_fov = fov_amount;
	}

	// run viewmodel fov adjustment.
	if ( g_menu.main.visuals.viewmodel_fov.get() )
		g_csgo.m_view_render->m_view.m_viewmodel_fov = g_menu.main.visuals.viewmodel_fov_amt.get();
}

void Hooks::RenderView( const CViewSetup &view, const CViewSetup &hud_view, int clear_flags, int what_to_draw ) {
	// ...

	g_hooks.m_view_render.GetOldMethod< RenderView_t >( CViewRender::RENDERVIEW )( this, view, hud_view, clear_flags, what_to_draw );
}

void Hooks::Render2DEffectsPostHUD( const CViewSetup &setup ) {
	// do note render flashbang overlay.
	if( !g_menu.main.visuals.removals.get(6) )
		g_hooks.m_view_render.GetOldMethod< Render2DEffectsPostHUD_t >( CViewRender::RENDER2DEFFECTSPOSTHUD )( this, setup );
}

void Hooks::RenderSmokeOverlay( bool unk ) {
	// do not render smoke overlay.
	
	if( !g_menu.main.visuals.removals.get( 2 ) )
		g_hooks.m_view_render.GetOldMethod< RenderSmokeOverlay_t >( CViewRender::RENDERSMOKEOVERLAY )( this, unk );
}
