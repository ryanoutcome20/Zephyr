#include "includes.h"

void Hooks::LockCursor( ) {
	if( g_gui.m_open ) {

		// un-lock the cursor.
		g_csgo.m_surface->UnlockCursor( );

		// disable input.
		g_csgo.m_input_system->EnableInput( false );
	}

	else {
		// call the original.
		g_hooks.m_surface.GetOldMethod< LockCursor_t >( ISurface::LOCKCURSOR )( this );

		// enable input.
		g_csgo.m_input_system->EnableInput( true );
	}
}

void Hooks::PlaySound( const char* name ) {
	g_hooks.m_surface.GetOldMethod< PlaySound_t >( ISurface::PLAYSOUND )( this, name );
}

void Hooks::OnScreenSizeChanged( int oldwidth, int oldheight ) {
	g_hooks.m_surface.GetOldMethod< OnScreenSizeChanged_t >( ISurface::ONSCREENSIZECHANGED )( this, oldwidth, oldheight );

	render::init( );

	Visuals::ModulateWorld( );
}