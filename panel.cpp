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

	// render hack stuff.
	if( panel == tools )
		g_cl.OnPaint( );

	// don't call the original function if we want to remove the scope.
	if( panel == zoom && g_menu.main.visuals.removals.get(7) )
		return;
	
	// recolor console.
	if ( name == HASH("GameConsole") || name == HASH("ConsolePage") || name == HASH("ConsoleEntry") 
		|| name == HASH("ConsoleSubmit") || name == HASH("ConsoleHistory")) {
		if( g_csgo.m_engine->IsConsoleVisible( ) ) { 
			g_visuals.ModulateConsole( false );
		}
	}
	else if( g_visuals.m_reset_console ) {
		g_visuals.ModulateConsole( true );
	}

	g_hooks.m_panel.GetOldMethod< PaintTraverse_t >( IPanel::PAINTTRAVERSE )( this, panel, repaint, force );
}