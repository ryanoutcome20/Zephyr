#include "includes.h"

void Hooks::ComputeShadowDepthTextures( const CViewSetup &view, bool setup ) {
	g_hooks.m_shadow_mgr.GetOldMethod< ComputeShadowDepthTextures_t >( IClientShadowMgr::COMPUTESHADOWDEPTHTEXTURES )( this, view, setup);
}