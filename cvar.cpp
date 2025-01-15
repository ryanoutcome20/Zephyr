#include "includes.h"

bool Hooks::DebugSpreadGetBool( ) {
	// strangely it decides not to work unless I return the original method? odd.
	if(g_menu.main.visuals.force_xhair.get())
		return true;

	return g_hooks.m_debug_spread.GetOldMethod< GetBool_t >(ConVar::GETBOOL)(this);
}