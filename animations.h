#pragma once

struct AnimationBackup_t {
	vec3_t           m_origin, m_abs_origin;
	vec3_t           m_velocity, m_abs_velocity;
	int              m_flags, m_eflags;
	int				 m_tick_count, m_frame;
	float            m_duck, m_body;
	float			 m_curtime, m_frametime, m_realtime, m_abs_frametime, m_interp_amt;
	C_AnimationLayer m_layers[ 13 ];
	float		     m_poses[ 24 ];
};

class Animations {
public:
	// updating clientside animations.
	bool m_lock_hltv;

	// localplayer animation variables.
	C_AnimationLayer	 m_layers[ 13 ];
	float				 m_poses[ 24 ];

public:
	// enemies.
	bool FixFakewalk( Player* player, LagRecord* record );
	void FixVelocity( Player* player, LagRecord* record, LagRecord* previous );
	void FixJump( LagRecord* record, LagRecord* previous );
	void Update( Player* player, LagRecord* record, CCSGOPlayerAnimState* state, AnimationBackup_t& backup );
	void PreUpdate( Player* player, LagRecord* record, CCSGOPlayerAnimState* state, LagRecord* previous );
	bool Backup( Player* player, AnimationBackup_t& backup );
	bool Restore( Player* player, AnimationBackup_t& backup );
	bool OnNetUpdate( Player* player, LagRecord* record );

	// shared.
	void UpdateClientsideAnimations( Player* player );

	// clientside animations.
	void UpdateShootPosition( );
	void CacheClientsideAnimations( float* poses, C_AnimationLayer* layers );
	void UpdateClientsideAnimationState( float* poses, C_AnimationLayer* layers );

	void UpdateMovement(CCSGOPlayerAnimState* state);
};

extern Animations g_animations;