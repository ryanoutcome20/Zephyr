#include "includes.h"

Precipitation g_precipitation;;

void Precipitation::Paint( ) {
	// get mode.
	int mode = g_menu.main.visuals.weather_modulation.get( );

	// don't run if we aren't in game.
	if( !g_csgo.m_engine->IsInGame( ) )
		return;

	// cleanup our entity if we disabled our weather.
	if( mode == 0 ) {
		Free( );
		return;
	}

	// call our audio handler.
	if ( g_menu.main.visuals.weather_audio.get( ) )
		Audio( );

	// we should setup this object.
	if ( m_setup )
		return;

	// free our entity since we don't need him anymore.
	Free( );

	// call our init function since we need the new entity.
	if( !Init( ) )
		return;

	// thank you 3kliksphillip for the map size measurements.
	// https://www.youtube.com/watch?v=jm6VOaY1G2k
	m_object->PreDataUpdate( DATA_UPDATE_CREATED );
	m_object->OnDataChanged( DATA_UPDATE_CREATED );

	m_object->m_nPrecipType( ) = PRECIPITATION_TYPE_RAIN;
	m_object->m_vecMins( ) = { -32768.f, -32768.f, -32768.f };
	m_object->m_vecMaxs( ) = { 32768.f, 32768.f, 32768.f };

	m_object->OnDataChanged( DATA_UPDATE_CREATED );
	m_object->PostDataUpdate( DATA_UPDATE_CREATED );

	m_setup = true;
}

void Precipitation::Audio( ) {
	// certified windows distruster.
	if( !m_timer )
		m_timer = 0.f;
	
	// compare our timings.
	if( m_timer > g_csgo.m_globals->m_curtime )
		return;

	// run our thunder.
	const char* samples[ ] = {
		XOR( "ambient\\weather\\thunder1.wav" ),
		XOR( "ambient\\weather\\thunder2.wav" ),
		XOR( "ambient\\weather\\thunder3.wav" ),
		XOR( "ambient\\weather\\thunder_distant_03.wav" ),
		XOR( "ambient\\weather\\thunder_distant_04.wav" ),
		XOR( "ambient\\weather\\thunder_distant_05.wav" ),
		XOR( "ambient\\weather\\thunderstorm\\thunder_1.wav" ),
		XOR( "ambient\\weather\\thunderstorm\\thunder_2.wav" ),
		XOR( "ambient\\weather\\thunderstorm\\thunder_3.wav" ),
		XOR( "ambient\\weather\\thunderstorm\\thunder_far_away_1.wav" ),
		XOR( "ambient\\weather\\thunderstorm\\thunder_far_away_2.wav" ),
		XOR( "ambient\\playonce\\weather\\thunder4.wav" ),
		XOR( "ambient\\playonce\\weather\\thunder5.wav" ),
		XOR( "ambient\\playonce\\weather\\thunder6.wav" ),
		XOR( "ambient\\playonce\\weather\\thunder_distant_01.wav" ),
		XOR( "ambient\\playonce\\weather\\thunder_distant_02.wav" ),
		XOR( "ambient\\playonce\\weather\\thunder_distant_06.wav" ),
	};

	g_csgo.m_sound->EmitAmbientSound( samples[ g_csgo.RandomInt( 0, 16 ) ], g_menu.main.visuals.weather_audio_volume.get( ) / 100.f );

	// reset our timer.
	m_timer = g_csgo.m_globals->m_curtime + g_csgo.RandomFloat( 20, 60 );
}

bool Precipitation::Init( ) {
	// already exists.
	if ( m_object )
		return true;

	// get our class object.
	ClientClass* CPrecipitation = GetClass( );

	if ( !CPrecipitation )
		return false;

	// create an ent index and serial number for the new rain entity.
	int index = MAX_EDICTS - 1;
	int serial = 0;

	// create our new rain entity.
	if ( CPrecipitation->m_pCreate && CPrecipitation->m_pCreate(index, serial) ) {
		m_object = g_csgo.m_entlist->GetClientEntity< Entity* >(index);
		return true;
	}

	return false;
}

void Precipitation::Free( ) {
	if ( !m_object )
		return;

	// if the game hasn't freed the object then lets do it ourself.
	if( !IsBadReadPtr( m_object, 4 ) )
		m_object->Release( );

	m_object = nullptr;
}

ClientClass* Precipitation::GetClass( ) {
	// no reason to get this again.
	if( m_class )
		return m_class;

	// get our class list.
	ClientClass* list{ g_csgo.m_client->GetAllClasses( ) };

	// iterate list.
	for ( ; list != nullptr; list = list->m_pNext ) {
		// break if we found precipitation.
		if ( list->m_ClassID == g_netvars.GetClientID(HASH("CPrecipitation")) )
			break;
	}

	m_class = list;

	return list;
}