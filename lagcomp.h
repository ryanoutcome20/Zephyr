#pragma once

class AimPlayer;

class LagCompensation {
public:
	enum LagType : size_t {
		INVALID = 0,
		CONSTANT,
		ADAPTIVE,
		RANDOM,
	};

public:
	LagRecord* StartPrediction( AimPlayer* data );
	void	   PlayerMove( LagRecord* record );
	void	   PredictAnimations( CCSGOPlayerAnimState* state, LagRecord* record, LagRecord* previous );
};

extern LagCompensation g_lagcomp;