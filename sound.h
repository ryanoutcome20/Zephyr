#pragma once

class IEngineSound {
public:
	// indexes for virtuals and hooks.
	enum indices : size_t {
		GETSOUNDDURATION = 5,
		EMITAMBIENTSOUND = 12,
	};

	enum {
		PITCH_NORM = 100,	// non-pitch shifted
		PITCH_LOW = 95,		// other values are possible - 0-255, where 255 is very high
		PITCH_HIGH = 120
	};

	__forceinline float GetSoundDuration( const char* sample ) {
		return util::get_method< float( __thiscall* )( void*, const char* ) >( this, GETSOUNDDURATION )( this, sample );
	}

	__forceinline void EmitAmbientSound( const char* sample, float volume, int pitch = PITCH_NORM, int flags = 0, float time = 0.f ) {
		return util::get_method< void( __thiscall* )( void*, const char*, float, int, int, float ) >( this, EMITAMBIENTSOUND )( this, sample, volume, pitch, flags, time );
	}
};