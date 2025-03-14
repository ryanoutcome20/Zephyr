#pragma once

class Visuals {
public:
	std::array< bool, 64 >                  m_draw;
    std::array< float, 64 >                 m_scaled_health;
    std::array< float, 64 >                 m_scaled_ammo;
	std::array< float, 2048 >               m_opacities;
	vec2_t                                  m_crosshair;
	bool                                    m_thirdperson;
	float					                m_hit_start, m_hit_end, m_hit_duration;

    // info about planted c4.
    bool        m_c4_planted;
    Entity      *m_planted_c4;
    float       m_planted_c4_explode_time;
    vec3_t      m_planted_c4_explosion_origin;
    float       m_planted_c4_damage;
    float       m_planted_c4_radius;
    float       m_planted_c4_radius_scaled;
    std::string m_last_bombsite;


    // info about cascade light handler.
    Entity      *m_cascade_light;
    vec3_t      m_cascade_shadow_direction;

	IMaterial* smoke1;
	IMaterial* smoke2;
	IMaterial* smoke3;
	IMaterial* smoke4;

    int bullet_counter;

    std::unordered_map< int, char > m_weapon_icons = {
    	{ DEAGLE, 'F' },
    	{ ELITE, 'S' },
    	{ FIVESEVEN, 'U' },
    	{ GLOCK, 'C' },
    	{ AK47, 'B' },
    	{ AUG, 'E' },
    	{ AWP, 'R' },
    	{ FAMAS, 'T' },
    	{ G3SG1, 'I' },
    	{ GALIL, 'V' },
    	{ M249, 'Z' },
    	{ M4A4, 'W' },
    	{ MAC10, 'L' },
    	{ P90, 'M' },
    	{ UMP45, 'Q' },
    	{ XM1014, ']' },
    	{ BIZON, 'D' },
    	{ MAG7, 'K' },
    	{ NEGEV, 'Z' },
    	{ SAWEDOFF, 'K' },
    	{ TEC9, 'C' },
    	{ ZEUS, 'Y' },
    	{ P2000, 'Y' },
    	{ MP7, 'X' },
    	{ MP9, 'D' },
    	{ NOVA, 'K' },
    	{ P250, 'Y' },
    	{ SCAR20, 'I' },
    	{ SG553, '[' },
    	{ SSG08, 'N' },
    	{ KNIFE_CT, 'J' },
    	{ FLASHBANG, 'G' },
    	{ HEGRENADE, 'H' },
    	{ SMOKE, 'P' },
    	{ MOLOTOV, 'H' },
    	{ DECOY, 'G' },
    	{ FIREBOMB, 'H' },
    	{ C4, '\\' },
    	{ KNIFE_T, 'J' },
    	{ M4A1S, 'W' },
    	{ USPS, 'Y' },
    	{ CZ75A, 'Y' },
    	{ REVOLVER, 'F' },
    	{ KNIFE_BAYONET, 'J' },
    	{ KNIFE_FLIP, 'J' },
    	{ KNIFE_GUT, 'J' },
    	{ KNIFE_KARAMBIT, 'J' },
    	{ KNIFE_M9_BAYONET, 'J' },
    	{ KNIFE_HUNTSMAN, 'J' },
    	{ KNIFE_FALCHION, 'J' },
    	{ KNIFE_BOWIE, 'J' },
    	{ KNIFE_BUTTERFLY, 'J' },
    	{ KNIFE_SHADOW_DAGGERS, 'J' },
    };

public:
	static void ModulateWorld( );
    void ModulateConsole( bool reset );
	void ThirdpersonThink( );
	void Hitmarker( );
	void NoSmoke( );
	void think( );
	void Spectators( );
	void StatusIndicators( );
    void AutopeekIndicator( );
	void SpreadCrosshair( );
    void PenetrationCrosshair( );
    void DrawPlantedC4();
    void UpdateShadows( );
	void draw( Entity* ent );
	void DrawProjectile( Weapon* ent );
	void DrawItem( Weapon* item );
	void OffScreen( Player* player, int alpha );
	void DrawPlayer( Player* player );
	bool GetPlayerBoxRect( Player* player, Rect& box );
	void DrawHistorySkeleton( Player* player, int opacity );
	void DrawSkeleton( Player* player, int opacity );
	void RenderGlow( );
	void DrawHitboxMatrix( LagRecord* record, Color col, float time );
    void DrawBulletImpacts();
    void DrawBeams( );
	void DebugAimbotPoints( Player* player );
};

extern Visuals g_visuals;