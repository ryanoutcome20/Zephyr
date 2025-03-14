#pragma once

class ShotRecord;

class Resolver {
public:
	enum Modes : size_t {
		RESOLVE_NONE = 0,
		RESOLVE_WALK,
		RESOLVE_STAND,
		RESOLVE_BRUTE,
		RESOLVE_SMART,
		RESOLVE_AIR,
		RESOLVE_BODY,
		RESOLVE_STOPPED_MOVING,
	};

public:
	LagRecord* FindIdealRecord( AimPlayer* data );
	LagRecord* FindLastRecord( AimPlayer* data );

	float GetAwayAngle( LagRecord* record );
	float GetVelocityAngle( LagRecord* record );
	float GetAntiFreestand( LagRecord* record );

	void MatchShot( AimPlayer* data, LagRecord* record );
	void SetMode( LagRecord* record );

	void ResolveAngles( Player* player, LagRecord* record );
	void ResolveWalk( AimPlayer* data, LagRecord* record );
	void ResolveStand( AimPlayer* data, LagRecord* record );
	void Bruteforce( AimPlayer* data, LagRecord* record );
	void StandNS( AimPlayer* data, LagRecord* record );
	void ResolveAir( AimPlayer* data, LagRecord* record );

	void AirNS( AimPlayer* data, LagRecord* record );
	void ResolvePoses( Player* player, LagRecord* record );

public:
	std::array< vec3_t, 64 > m_impacts;
};

extern Resolver g_resolver;