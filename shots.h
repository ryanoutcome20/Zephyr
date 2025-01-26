#pragma once

class ShotRecord {
public:
	__forceinline ShotRecord( ) : m_target{}, m_record{}, m_time{}, m_lat{}, m_damage{}, m_pos{}, m_range{}, m_matched{}, m_landed{}, m_processed{} {}

public:
	Player*    m_target;
	LagRecord* m_record;
	float      m_time, m_lat, m_damage;
	vec3_t     m_pos;
	vec3_t     m_impact_pos;
	float			m_range;

	// matching stuff.
	bool       m_matched; // on_impact processed.
	bool		  m_landed; // on_hurt processed.
	bool       m_processed; // final think processing.
};

struct ShotMatch_t { float delta; ShotRecord* shot; };

class VisualImpactData_t {
public:
    vec3_t m_impact_pos, m_shoot_pos;
    int    m_tickbase;
    bool   m_ignore, m_hit_player;

public:
    __forceinline VisualImpactData_t( const vec3_t &impact_pos, const vec3_t &shoot_pos, int tickbase ) : 
        m_impact_pos{ impact_pos }, m_shoot_pos{ shoot_pos }, m_tickbase{ tickbase }, m_ignore{ false }, m_hit_player{ false } {}
};

class Shots {
private:
    std::array< std::string, 8 > m_groups = {
        XOR( "body" ),
		XOR( "head" ),
		XOR( "chest" ),
		XOR( "stomach" ),
		XOR( "left arm" ),
		XOR( "right arm" ),
		XOR( "left leg" ),
		XOR( "right leg" )
    };

public:
	void OnShotFire( Player* target, float damage, int bullets, LagRecord* record );
	void OnImpact( IGameEvent* evt );
	void OnHurt( IGameEvent* evt );
	void Think( );

public:
	std::deque< ShotRecord >          m_shots;
    std::vector< VisualImpactData_t > m_vis_impacts;
};

extern Shots g_shots;