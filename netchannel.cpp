#include "includes.h"

#define NET_FRAMES_BACKUP 64 // must be power of 2. 
#define NET_FRAMES_MASK ( NET_FRAMES_BACKUP - 1 )

int Hooks::SendDatagram( void* data ) {
	// get our net channel.
	INetChannel* channel = (INetChannel*)this;

	// sanity checks.
	if (!channel || !g_csgo.m_engine->IsInGame() || !g_csgo.m_net || !g_cl.m_processing || channel != g_csgo.m_cl->m_net_channel)
		return g_hooks.m_net_channel.GetOldMethod< SendDatagram_t >(INetChannel::SENDDATAGRAM)(this, data);

	// backup our networking variables.
	int backup_in_seq	 = channel->m_in_seq;
	int backup_rel_state = channel->m_in_rel_state;

	// fake latency.
	if (g_aimbot.m_fake_latency || g_menu.main.misc.fake_latency_always.get()) {
		float ping = g_menu.main.misc.fake_latency_amt.get() / 1000.f;

		if ( g_cl.m_latency < ping ) {
			int target_in_seq = channel->m_in_seq - game::TIME_TO_TICKS(ping - g_cl.m_latency);

			channel->m_in_seq = target_in_seq;

			for ( auto& seq : g_cl.m_sequences ) {
				if ( seq.m_seq != target_in_seq )
					continue;

				channel->m_in_rel_state = seq.m_state;
			}
		}
	}
	
	// call exploit handler.
	g_exploits.Loss( channel );

	// call our old method and reset our backup data.
	int ret = g_hooks.m_net_channel.GetOldMethod< SendDatagram_t >(INetChannel::SENDDATAGRAM)(this, data);

	channel->m_in_seq		= backup_in_seq;
	channel->m_in_rel_state = backup_rel_state;

	return ret;
}

void Hooks::SendNetMsg( INetMessage* msg, bool reliable, bool voice ) { 
	g_hooks.m_net_channel.GetOldMethod< SendNetMsg_t >(INetChannel::SENDNETMSG)(this, msg, reliable, voice);

	if( g_menu.main.misc.active_exploit.get( ) == 3 ) {
		for( int i = 0; i <= g_menu.main.misc.active_strength.get( ); i++ )
			g_hooks.m_net_channel.GetOldMethod< SendNetMsg_t >(INetChannel::SENDNETMSG)(this, msg, reliable, voice);
	}
}

void Hooks::ProcessPacket( void* packet, bool header ) {
	g_hooks.m_net_channel.GetOldMethod< ProcessPacket_t >( INetChannel::PROCESSPACKET )( this, packet, header );

	g_cl.UpdateIncomingSequences( );

	// get this from CL_FireEvents string "Failed to execute event for classId" in engine.dll
	for( CEventInfo* it{ g_csgo.m_cl->m_events }; it != nullptr; it = it->m_next ) {
		if( !it->m_class_id )
			continue;

		// set all delays to instant.
		it->m_fire_delay = 0.f;
	}

	// game events are actually fired in OnRenderStart which is WAY later after they are received
	// effective delay by lerp time, now we call them right after theyre received (all receive proxies are invoked without delay).
	g_csgo.m_engine->FireEvents( );
}