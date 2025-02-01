#pragma once

#define MAX_EDICTS 2048;

class Precipitation {
public:
	Entity* m_object;
	ClientClass* m_class;
	bool m_setup;
	float m_timer;
public:
	void Paint( );
	void Audio( );
	bool Init( );
	void Free( );
	ClientClass* GetClass( );
};

extern Precipitation g_precipitation;