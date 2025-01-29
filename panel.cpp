#include "includes.h"

void Hooks::PaintTraverse( VPANEL panel, bool repaint, bool force ) {
	static VPANEL tools{}, zoom{};

	// get panel name.
	hash32_t name = FNV1a::get( g_csgo.m_panel->GetName(panel) );

	// cache CHudZoom panel once.
	if( !zoom && name == HASH( "HudZoom" ) )
		zoom = panel;

	// cache tools panel once.
	if( !tools && panel == g_csgo.m_engine_vgui->GetPanel( PANEL_TOOLS ) )
		tools = panel;

	// disable our console override.
	if ( panel != tools )
		g_visuals.ModulateConsole( true );

	// render hack stuff.
	if( panel == tools )
		g_cl.OnPaint( );

	// don't call the original function if we want to remove the scope.
	if( panel == zoom && g_menu.main.visuals.removals.get(7) )
		return;

	// ensure our console override is running.
	if( panel != tools )
		g_visuals.ModulateConsole( false );

	g_hooks.m_panel.GetOldMethod< PaintTraverse_t >( IPanel::PAINTTRAVERSE )( this, panel, repaint, force );
}