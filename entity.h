#pragma once

#define MAX_WEAPONS	48

enum Hitboxes_t : int {
	HITBOX_HEAD = 0,
	HITBOX_NECK,
	HITBOX_LOWER_NECK,
	HITBOX_PELVIS,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_R_THIGH,
	HITBOX_L_THIGH,
	HITBOX_R_CALF,
	HITBOX_L_CALF,
	HITBOX_R_FOOT,
	HITBOX_L_FOOT,
	HITBOX_R_HAND,
	HITBOX_L_HAND,
	HITBOX_R_UPPER_ARM,
	HITBOX_R_FOREARM,
	HITBOX_L_UPPER_ARM,
	HITBOX_L_FOREARM,
	HITBOX_MAX
};

enum RenderFlags_t : uint32_t {
	STUDIO_NONE = 0x00000000,
	STUDIO_RENDER = 0x00000001,
	STUDIO_VIEWXFORMATTACHMENTS = 0x00000002,
	STUDIO_DRAWTRANSLUCENTSUBMODELS = 0x00000004,
	STUDIO_TWOPASS = 0x00000008,
	STUDIO_STATIC_LIGHTING = 0x00000010,
	STUDIO_WIREFRAME = 0x00000020,
	STUDIO_ITEM_BLINK = 0x00000040,
	STUDIO_NOSHADOWS = 0x00000080,
	STUDIO_WIREFRAME_VCOLLIDE = 0x00000100,
	STUDIO_NOLIGHTING_OR_CUBEMAP = 0x00000200,
	STUDIO_SKIP_FLEXES = 0x00000400,
	STUDIO_DONOTMODIFYSTENCILSTATE = 0x00000800,
	STUDIO_TRANSPARENCY = 0x80000000,
	STUDIO_SHADOWDEPTHTEXTURE = 0x40000000,
	STUDIO_SHADOWTEXTURE = 0x20000000,
	STUDIO_SKIP_DECALS = 0x10000000
};

enum BoneFlags_t : int {
	BONE_USED_BY_ANYTHING = 0x0007FF00,
	BONE_USED_BY_HITBOX = 0x00000100, // bone (or child) is used by a hit box
	BONE_USED_BY_ATTACHMENT = 0x00000200, // bone (or child) is used by an attachment point
	BONE_USED_BY_VERTEX_MASK = 0x0003FC00,
	BONE_USED_BY_VERTEX_LOD0 = 0x00000400, // bone (or child) is used by the toplevel model via skinned vertex
	BONE_USED_BY_VERTEX_LOD1 = 0x00000800,
	BONE_USED_BY_VERTEX_LOD2 = 0x00001000,
	BONE_USED_BY_VERTEX_LOD3 = 0x00002000,
	BONE_USED_BY_VERTEX_LOD4 = 0x00004000,
	BONE_USED_BY_VERTEX_LOD5 = 0x00008000,
	BONE_USED_BY_VERTEX_LOD6 = 0x00010000,
	BONE_USED_BY_VERTEX_LOD7 = 0x00020000,
	BONE_USED_BY_BONE_MERGE = 0x00040000
};

enum CSWeaponType : int {
	WEAPONTYPE_UNKNOWN = -1,
	WEAPONTYPE_KNIFE,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_TASER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_HEALTHSHOT = 11
};

enum teams_t : int {
	TEAM_NOTEAM = 0,
	TEAM_SPECTATOR,
	TEAM_TERRORISTS,
	TEAM_COUNTERTERRORISTS
};

enum effects_t : int {
	EF_BONEMERGE = 0x001,	// Performs bone merge on client side
	EF_BRIGHTLIGHT = 0x002,	// DLIGHT centered at entity origin
	EF_DIMLIGHT = 0x004,	// player flashlight
	EF_NOINTERP = 0x008,	// don't interpolate the next frame
	EF_NOSHADOW = 0x010,	// Don't cast no shadow
	EF_NODRAW = 0x020,	// don't draw entity
	EF_NORECEIVESHADOW = 0x040,	// Don't receive no shadow
	EF_BONEMERGE_FASTCULL = 0x080,	// For use with EF_BONEMERGE. If this is set, then it places this ent's origin at its
	EF_ITEM_BLINK = 0x100,	// blink an item so that the user notices it.
	EF_PARENT_ANIMATES = 0x200,	// always assume that the parent entity is animating
	EF_MAX_BITS = 10
};

enum flags_t : int {
	EFL_KILLME = (1 << 0),	// This entity is marked for death -- This allows the game to actually delete ents at a safe time
	EFL_DORMANT = (1 << 1),	// Entity is dormant, no updates to client
	EFL_NOCLIP_ACTIVE = (1 << 2),	// Lets us know when the noclip command is active.
	EFL_SETTING_UP_BONES = (1 << 3),	// Set while a model is setting up its bones.
	EFL_KEEP_ON_RECREATE_ENTITIES = (1 << 4), // This is a special entity that should not be deleted when we restart entities only
	EFL_DIRTY_SHADOWUPDATE = (1 << 5),	// Client only- need shadow manager to update the shadow...
	EFL_NOTIFY = (1 << 6),	// Another entity is watching events on this entity (used by teleport).
	EFL_FORCE_CHECK_TRANSMIT = (1 << 7),
	EFL_BOT_FROZEN = (1 << 8),	// This is set on bots that are frozen.
	EFL_SERVER_ONLY = (1 << 9),	// Non-networked entity.
	EFL_NO_AUTO_EDICT_ATTACH = (1 << 10), 
	EFL_DIRTY_ABSTRANSFORM = (1 << 11),
	EFL_DIRTY_ABSVELOCITY = (1 << 12),
	EFL_DIRTY_ABSANGVELOCITY = (1 << 13),
	EFL_DIRTY_SURROUNDING_COLLISION_BOUNDS = (1 << 14),
	EFL_DIRTY_SPATIAL_PARTITION = (1 << 15),
	EFL_HAS_PLAYER_CHILD = (1 << 16),	// One of the child entities is a player.
	EFL_IN_SKYBOX = (1 << 17),
	EFL_USE_PARTITION_WHEN_NOT_SOLID = (1 << 18),	// Entities with this flag set show up in the partition even when not solid
	EFL_TOUCHING_FLUID = (1 << 19),	// Used to determine if an entity is floating
	EFL_IS_BEING_LIFTED_BY_BARNACLE = (1 << 20),
	EFL_NO_ROTORWASH_PUSH = (1 << 21),		// I shouldn't be pushed by the rotorwash
	EFL_NO_THINK_FUNCTION = (1 << 22),
	EFL_NO_GAME_PHYSICS_SIMULATION = (1 << 23),
	EFL_CHECK_UNTOUCH = (1 << 24),
	EFL_DONTBLOCKLOS = (1 << 25),		// I shouldn't block NPC line-of-sight
	EFL_DONTWALKON = (1 << 26),		// NPC;s should not walk on this entity
	EFL_NO_DISSOLVE = (1 << 27),		// These guys shouldn't dissolve
	EFL_NO_MEGAPHYSCANNON_RAGDOLL = (1 << 28),	// Mega physcannon can't ragdoll these guys.
	EFL_NO_WATER_VELOCITY_CHANGE = (1 << 29),	// Don't adjust this entity's velocity when transitioning into water
	EFL_NO_PHYSCANNON_INTERACTION = (1 << 30),	// Physcannon can't pick these up or punt them
	EFL_NO_DAMAGE_FORCES = (1 << 31),	// Doesn't accept forces from physics damage
};

enum InvalidatePhysicsBits_t : int {
	POSITION_CHANGED = 0x1,
	ANGLES_CHANGED = 0x2,
	VELOCITY_CHANGED = 0x4,
	ANIMATION_CHANGED = 0x8,
};

enum DataUpdateType_t : int {
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED,
};

enum PrecipitationType_t : int {
	PRECIPITATION_TYPE_RAIN = 0,
	PRECIPITATION_TYPE_SNOW,
	PRECIPITATION_TYPE_ASH,
	PRECIPITATION_TYPE_SNOWFALL,
	PRECIPITATION_TYPE_PARTICLERAIN,
	PRECIPITATION_TYPE_PARTICLEASH,
	PRECIPITATION_TYPE_PARTICLERAINSTORM,
	PRECIPITATION_TYPE_PARTICLESNOW,
	NUM_PRECIPITATION_TYPES,
};

enum LifeStates_t : int {
	LIFE_ALIVE = 0,
	LIFE_DYING,
	LIFE_DEAD,
	LIFE_RESPAWNABLE,
	LIFE_DISCARDBODY,
};

enum PlayerFlags_t : int {
	FL_ONGROUND = (1 << 0),
	FL_DUCKING = (1 << 1),
	FL_WATERJUMP = (1 << 3),
	FL_ONTRAIN = (1 << 4),
	FL_INRAIN = (1 << 5),
	FL_FROZEN = (1 << 6),
	FL_ATCONTROLS = (1 << 7),
	FL_CLIENT = (1 << 8),
	FL_FAKECLIENT = (1 << 9),
	FL_INWATER = (1 << 10),
};

enum MoveType_t : int {
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4,
};

enum Weapons_t : int {
	DEAGLE = 1,
	ELITE = 2,
	FIVESEVEN = 3,
	GLOCK = 4,
	AK47 = 7,
	AUG = 8,
	AWP = 9,
	FAMAS = 10,
	G3SG1 = 11,
	GALIL = 13,
	M249 = 14,
	M4A4 = 16,
	MAC10 = 17,
	P90 = 19,
	UMP45 = 24,
	XM1014 = 25,
	BIZON = 26,
	MAG7 = 27,
	NEGEV = 28,
	SAWEDOFF = 29,
	TEC9 = 30,
	ZEUS = 31,
	P2000 = 32,
	MP7 = 33,
	MP9 = 34,
	NOVA = 35,
	P250 = 36,
	SCAR20 = 38,
	SG553 = 39,
	SSG08 = 40,
	KNIFE_T = 42,
	FLASHBANG = 43,
	HEGRENADE = 44,
	SMOKE = 45,
	MOLOTOV = 46,
	DECOY = 47,
	FIREBOMB = 48,
	C4 = 49,
	MUSICKIT = 58,
	KNIFE_CT = 59,
	M4A1S = 60,
	USPS = 61,
	TRADEUPCONTRACT = 62,
	CZ75A = 63,
	REVOLVER = 64,
	KNIFE_BAYONET = 500,
	KNIFE_FLIP = 505,
	KNIFE_GUT = 506,
	KNIFE_KARAMBIT = 507,
	KNIFE_M9_BAYONET = 508,
	KNIFE_HUNTSMAN = 509,
	KNIFE_FALCHION = 512,
	KNIFE_BOWIE = 514,
	KNIFE_BUTTERFLY = 515,
	KNIFE_SHADOW_DAGGERS = 516,
};

class VarMapEntry_t {
public:
	unsigned short m_type;
	unsigned short m_bNeedsToInterpolate;
	void* m_data;
	void* m_watcher;
};

struct VarMapping_t {
	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};

struct RenderableInstance_t {
	uint8_t m_alpha;
	__forceinline RenderableInstance_t() : m_alpha{ 255ui8 } {}
};

class Entity {
public:
	// helper methods.
	template< typename t >
	__forceinline t& get(size_t offset) {
		return *(t*)((uintptr_t)this + offset);
	}

	template< typename t >
	__forceinline void set(size_t offset, const t& val) {
		*(t*)((uintptr_t)this + offset) = val;
	}

	template< typename t >
	__forceinline t as() {
		return (t)this;
	}

public:
	// netvars / etc.
	__forceinline vec3_t& m_vecOrigin() {
		return get< vec3_t >(g_entoffsets.m_vecOrigin);
	}

	__forceinline vec3_t& m_vecOldOrigin() {
		return get< vec3_t >(g_entoffsets.m_vecOldOrigin);
	}

	__forceinline vec3_t& m_vecVelocity() {
		return get< vec3_t >(g_entoffsets.m_vecVelocity);
	}

	__forceinline vec3_t& m_vecMins() {
		return get< vec3_t >(g_entoffsets.m_vecMins);
	}

	__forceinline vec3_t& m_vecMaxs() {
		return get< vec3_t >(g_entoffsets.m_vecMaxs);
	}

	__forceinline int& m_iTeamNum() {
		return get< int >(g_entoffsets.m_iTeamNum);
	}

	__forceinline int& m_nSequence() {
		return get< int >(g_entoffsets.m_nSequence);
	}

	__forceinline float& m_flCycle() {
		return get< float >(g_entoffsets.m_flCycle);
	}

	__forceinline float& m_flC4Blow() {
		return get< float >(g_entoffsets.m_flC4Blow);
	}

	__forceinline bool& m_bBombTicking() {
		return get< bool >(g_entoffsets.m_bBombTicking);
	}

	__forceinline int& m_fEffects() {
		return get< int >(g_entoffsets.m_fEffects);
	}

	__forceinline int& m_nModelIndex() {
		return get< int >(g_entoffsets.m_nModelIndex);
	}

	__forceinline bool& m_bReadyToDraw() {
		return get< bool >(g_entoffsets.m_bReadyToDraw);
	}

	__forceinline bool& m_bFireIsBurning() {
		return get< bool >(g_entoffsets.m_bFireIsBurning);
	}

	__forceinline int& m_nSmokeEffectTickBegin() {
		return get< int >(g_entoffsets.m_nSmokeEffectTickBegin);
	}

	__forceinline int& m_iEFlags() {
		return get< int >(g_entoffsets.m_iEFlags);
	}

	__forceinline int& m_nPrecipType() {
		return get< int >(g_entoffsets.m_nPrecipType);
	}

	__forceinline vec3_t& m_envLightShadowDirection() {
		return get< vec3_t >( g_entoffsets.m_envLightShadowDirection );
	}

public:
	// virtual indices
	enum indices : size_t {
		R_GETRENDERORIGIN = 1,
		R_GETRENDERANGLES = 2,
		R_GETMODEL = 8,
		R_DRAWMODEL = 9,
		R_SETUPBONES = 13,

		N_RELEASE = 1,
		N_GETCLIENTCLASS = 2,
		N_ONDATACHANGED = 5,
		N_PREDATAUPDATE = 6,
		N_POSTDATAUPDATE = 7,
		N_DORMANT = 9,
		N_INDEX = 10,
		N_SETDDESTROYEDONRECREATEENTITIES = 13,

		GETABSORIGIN = 10,
		GETABSANGLES = 11,
		WORLDSPACECENTER = 78,
		GETMAXHEALTH = 122,
		ISPLAYER = 152,
		ISBASECOMBATWEAPON = 160,
	};

public:
	// virtuals.
	// renderable table.
	__forceinline void* renderable() {
		return (void*)((uintptr_t)this + 0x4);
	}

	__forceinline vec3_t& GetRenderOrigin() {
		return util::get_method< vec3_t& (__thiscall*)(void*) >(renderable(), R_GETRENDERORIGIN)(renderable());
	}

	__forceinline ang_t& GetRenderAngles() {
		return util::get_method< ang_t& (__thiscall*)(void*) >(renderable(), R_GETRENDERANGLES)(renderable());
	}

	__forceinline const model_t* GetModel() {
		return util::get_method< const model_t* (__thiscall*)(void*) >(renderable(), R_GETMODEL)(renderable());
	}

	__forceinline void DrawModel(int flags = STUDIO_RENDER, const RenderableInstance_t& instance = {}) {
		return util::get_method< void(__thiscall*)(void*, int, const RenderableInstance_t&)>(renderable(), R_DRAWMODEL)(renderable(), flags, instance);
	}

	__forceinline bool SetupBones(matrix3x4_t* out, int max, int mask, float time) {	
		return util::get_method< bool(__thiscall*)(void*, matrix3x4_t*, int, int, float)>(renderable(), R_SETUPBONES)(renderable(), out, max, mask, time);
	}

	// networkable table.
	__forceinline void* networkable() {
		return (void*)((uintptr_t)this + 0x8);
	}

	__forceinline void Release() {
		return util::get_method< void(__thiscall*)(void*) >(networkable(), N_RELEASE)(networkable());
	}

	__forceinline ClientClass* GetClientClass() {
		return util::get_method< ClientClass* (__thiscall*)(void*) >(networkable(), N_GETCLIENTCLASS)(networkable());
	}

	__forceinline void OnDataChanged(DataUpdateType_t type) {
		return util::get_method< void(__thiscall*)(void*, DataUpdateType_t) >(networkable(), N_ONDATACHANGED)(networkable(), type);
	}

	__forceinline void PreDataUpdate(DataUpdateType_t type) {
		return util::get_method< void(__thiscall*)(void*, DataUpdateType_t) >(networkable(), N_PREDATAUPDATE)(networkable(), type);
	}

	__forceinline void PostDataUpdate(DataUpdateType_t type) {
		return util::get_method< void(__thiscall*)(void*, DataUpdateType_t) >(networkable(), N_POSTDATAUPDATE)(networkable(), type);
	}

	__forceinline bool dormant() {
		return util::get_method< bool(__thiscall*)(void*) >(networkable(), N_DORMANT)(networkable());
	}

	__forceinline int index() {
		return util::get_method< int(__thiscall*)(void*) >(networkable(), N_INDEX)(networkable());
	}

	__forceinline void SetDestroyedOnRecreateEntities() {
		return util::get_method< void(__thiscall*)(void*) >(networkable(), N_SETDDESTROYEDONRECREATEENTITIES)(networkable());
	}

	// normal table.
	__forceinline const vec3_t& GetAbsOrigin() {
		return util::get_method< const vec3_t& (__thiscall*)(void*) >(this, GETABSORIGIN)(this);
	}

	__forceinline const ang_t& GetAbsAngles() {
		return util::get_method< const ang_t& (__thiscall*)(void*) >(this, GETABSANGLES)(this);
	}

	__forceinline bool IsPlayer() {
		return util::get_method< bool(__thiscall*)(void*) >(this, ISPLAYER)(this);
	}

	__forceinline bool IsBaseCombatWeapon() {
		return util::get_method< bool(__thiscall*)(void*) >(this, ISBASECOMBATWEAPON)(this);
	}

	__forceinline std::string GetBombsiteName() {
		std::string out;

		// note - dex; bomb_target + 0x150 has a char array for site name... not sure how much memory gets allocated for it.
		out.resize(32u);

		std::memcpy(&out[0], (const void*)((uintptr_t)this + 0x150), 32u);

		return out;
	}

	__forceinline void InvalidatePhysicsRecursive(InvalidatePhysicsBits_t bits) {
		using InvalidatePhysicsRecursive_t = void(__thiscall*)(decltype(this), InvalidatePhysicsBits_t);
		g_csgo.InvalidatePhysicsRecursive.as< InvalidatePhysicsRecursive_t >()(this, bits);
	}

	__forceinline void SetAbsAngles(const ang_t& angles) {
		using SetAbsAngles_t = void(__thiscall*)(decltype(this), const ang_t&);
		g_csgo.SetAbsAngles.as< SetAbsAngles_t >()(this, angles);
	}

	__forceinline void SetAbsOrigin(const vec3_t& origin) {
		using SetAbsOrigin_t = void(__thiscall*)(decltype(this), const vec3_t&);
		g_csgo.SetAbsOrigin.as< SetAbsOrigin_t >()(this, origin);
	}

	__forceinline void SetAbsVelocity(const vec3_t& velocity) {
		using SetAbsVelocity_t = void(__thiscall*)(decltype(this), const vec3_t&);
		g_csgo.SetAbsVelocity.as< SetAbsVelocity_t >()(this, velocity);
	}

	__forceinline void AddEffect(int effects) {
		m_fEffects() |= effects;
	}

	__forceinline void AddEntityFlag(int flag) {
		m_iEFlags() |= flag;
	}

	__forceinline int get_class_id() {
		ClientClass* cc{ GetClientClass() };

		return (cc) ? cc->m_ClassID : -1;
	}

	__forceinline bool is(hash32_t hash) {
		return g_netvars.GetClientID(hash) == get_class_id();
	}
};

struct procedural_foot_t {
	vec3_t m_vecPosAnim;
	vec3_t m_vecPosAnimLast;
	vec3_t m_vecPosPlant;
	vec3_t m_vecPlantVel;
	float  m_flLockAmount;
	float  m_flLastPlantTime;
};

struct aimmatrix_transition_t {
	float	m_flDurationStateHasBeenValid;
	float	m_flDurationStateHasBeenInValid;
	float	m_flHowLongToWaitUntilTransitionCanBlendIn;
	float	m_flHowLongToWaitUntilTransitionCanBlendOut;
	float	m_flBlendValue;
};

struct animstate_pose_param_cache_t {
	bool		m_bInitialized;
	int			m_nIndex;
	const char* m_szName;
};

enum act {
	ACT_CSGO_NULL = 957,
	ACT_CSGO_DEFUSE,
	ACT_CSGO_DEFUSE_WITH_KIT,
	ACT_CSGO_FLASHBANG_REACTION,
	ACT_CSGO_FIRE_PRIMARY,
	ACT_CSGO_FIRE_PRIMARY_OPT_1,
	ACT_CSGO_FIRE_PRIMARY_OPT_2,
	ACT_CSGO_FIRE_SECONDARY,
	ACT_CSGO_FIRE_SECONDARY_OPT_1,
	ACT_CSGO_FIRE_SECONDARY_OPT_2,
	ACT_CSGO_RELOAD,
	ACT_CSGO_RELOAD_START,
	ACT_CSGO_RELOAD_LOOP,
	ACT_CSGO_RELOAD_END,
	ACT_CSGO_OPERATE,
	ACT_CSGO_DEPLOY,
	ACT_CSGO_CATCH,
	ACT_CSGO_SILENCER_DETACH,
	ACT_CSGO_SILENCER_ATTACH,
	ACT_CSGO_TWITCH,
	ACT_CSGO_TWITCH_BUYZONE,
	ACT_CSGO_PLANT_BOMB,
	ACT_CSGO_IDLE_TURN_BALANCEADJUST,
	ACT_CSGO_IDLE_ADJUST_STOPPEDMOVING,
	ACT_CSGO_ALIVE_LOOP,
	ACT_CSGO_FLINCH,
	ACT_CSGO_FLINCH_HEAD,
	ACT_CSGO_FLINCH_MOLOTOV,
	ACT_CSGO_JUMP,
	ACT_CSGO_FALL,
	ACT_CSGO_CLIMB_LADDER,
	ACT_CSGO_LAND_LIGHT,
	ACT_CSGO_LAND_HEAVY,
	ACT_CSGO_EXIT_LADDER_TOP,
	ACT_CSGO_EXIT_LADDER_BOTTOM
};

enum animstate_pose_param_idx_t {
	PLAYER_POSE_PARAM_FIRST = 0,
	PLAYER_POSE_PARAM_LEAN_YAW = PLAYER_POSE_PARAM_FIRST,
	PLAYER_POSE_PARAM_SPEED,
	PLAYER_POSE_PARAM_LADDER_SPEED,
	PLAYER_POSE_PARAM_LADDER_YAW,
	PLAYER_POSE_PARAM_MOVE_YAW,
	PLAYER_POSE_PARAM_RUN,
	PLAYER_POSE_PARAM_BODY_YAW,
	PLAYER_POSE_PARAM_BODY_PITCH,
	PLAYER_POSE_PARAM_DEATH_YAW,
	PLAYER_POSE_PARAM_STAND,
	PLAYER_POSE_PARAM_JUMP_FALL,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_IDLE,
	PLAYER_POSE_PARAM_AIM_BLEND_CROUCH_IDLE,
	PLAYER_POSE_PARAM_STRAFE_DIR,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_WALK,
	PLAYER_POSE_PARAM_AIM_BLEND_STAND_RUN,
	PLAYER_POSE_PARAM_AIM_BLEND_CROUCH_WALK,
	PLAYER_POSE_PARAM_MOVE_BLEND_WALK,
	PLAYER_POSE_PARAM_MOVE_BLEND_RUN,
	PLAYER_POSE_PARAM_MOVE_BLEND_CROUCH_WALK,
	PLAYER_POSE_PARAM_COUNT
};

enum animstate_layer_t {
	ANIMATION_LAYER_AIMMATRIX = 0,
	ANIMATION_LAYER_WEAPON_ACTION,
	ANIMATION_LAYER_WEAPON_ACTION_RECROUCH,
	ANIMATION_LAYER_ADJUST,
	ANIMATION_LAYER_MOVEMENT_JUMP_OR_FALL,
	ANIMATION_LAYER_MOVEMENT_LAND_OR_CLIMB,
	ANIMATION_LAYER_MOVEMENT_MOVE,
	ANIMATION_LAYER_MOVEMENT_STRAFECHANGE,
	ANIMATION_LAYER_WHOLE_BODY,
	ANIMATION_LAYER_FLASHED,
	ANIMATION_LAYER_FLINCH,
	ANIMATION_LAYER_ALIVELOOP,
	ANIMATION_LAYER_LEAN,
	ANIMATION_LAYER_COUNT
};

class CCSGOPlayerAnimState {
public:
	int* m_pLayerOrderPreset;
	bool						 m_bFirstRunSinceInit;
	bool						 m_bFirstFootPlantSinceInit;
	int							 m_iLastUpdateFrame;
	float						 m_flEyePositionSmoothLerp;
	float						 m_flStrafeChangeWeightSmoothFalloff;
	float						 m_flFlashedAmountEaseOutStart;
	float						 m_flFlashedAmountEaseOutEnd;
	aimmatrix_transition_t		 m_tStandWalkAim;
	aimmatrix_transition_t		 m_tStandRunAim;
	aimmatrix_transition_t		 m_tCrouchWalkAim;
	int	    					 m_cachedModelIndex;
	float						 m_flStepHeightLeft;
	float						 m_flStepHeightRight;
	Weapon* m_pWeaponLastBoneSetup;
	Player* m_pPlayer;
	Weapon* m_pWeapon;
	Weapon* m_pWeaponLast;
	float   					 m_flLastUpdateTime;
	int     					 m_nLastUpdateFrame;
	float   					 m_flLastUpdateIncrement;
	float   					 m_flEyeYaw;
	float   					 m_flEyePitch;
	float   					 m_flFootYaw;
	float   					 m_flFootYawLast;
	float						 m_flMoveYaw;
	float						 m_flMoveYawIdeal;
	float						 m_flMoveYawCurrentToIdeal;
	float						 m_flTimeToAlignLowerBody;
	float						 m_flPrimaryCycle;
	float						 m_flMoveWeight;
	float						 m_flMoveWeightSmoothed;
	float						 m_flAnimDuckAmount;
	float						 m_flDuckAdditional;
	float						 m_flRecrouchWeight;
	vec3_t						 m_vecPositionCurrent;
	vec3_t						 m_vecPositionLast;
	vec3_t						 m_vecVelocity;
	vec3_t						 m_vecVelocityNormalized;
	vec3_t						 m_vecVelocityNormalizedNonZero;
	float						 m_flVelocityLengthXY;
	float						 m_flVelocityLengthZ;
	float						 m_flSpeedAsPortionOfRunTopSpeed;
	float						 m_flSpeedAsPortionOfWalkTopSpeed;
	float						 m_flSpeedAsPortionOfCrouchTopSpeed;
	float						 m_flDurationMoving;
	float						 m_flDurationStill;
	bool						 m_bOnGround;
	bool						 m_bJumping;
	float						 m_flLowerBodyRealignTimer;
	bool						 m_bLanding;
	float						 m_flJumpToFall;
	float						 m_flDurationInAir;
	float						 m_flLeftGroundHeight;
	float						 m_flLandAnimMultiplier;
	float						 m_flWalkToRunTransition;
	bool						 m_bLandedOnGroundThisFrame;
	bool						 m_bLeftTheGroundThisFrame;
	float						 m_flInAirSmoothValue;
	bool						 m_bOnLadder;
	float						 m_flLadderWeight;
	float						 m_flLadderSpeed;
	bool						 m_bWalkToRunTransitionState;
	bool						 m_bDefuseStarted;
	bool						 m_bPlantAnimStarted;
	bool						 m_bTwitchAnimStarted;
	bool						 m_bAdjustStarted;
	char						 m_ActivityModifiers[ 20 ];
	float						 m_flNextTwitchTime;
	float						 m_flTimeOfLastKnownInjury;
	float						 m_flLastVelocityTestTime;
	vec3_t						 m_vecVelocityLast;
	vec3_t						 m_vecTargetAcceleration;
	vec3_t						 m_vecAcceleration;
	float						 m_flAccelerationWeight;
	float						 m_flAimMatrixTransition;
	float						 m_flAimMatrixTransitionDelay;
	bool						 m_bFlashed;
	float						 m_flStrafeChangeWeight;
	float						 m_flStrafeChangeTargetWeight;
	float						 m_flStrafeChangeCycle;
	int							 m_nStrafeSequence;
	bool						 m_bStrafeChanging;
	float						 m_flDurationStrafing;
	float						 m_flFootLerp;
	bool						 m_bFeetCrossed;
	bool						 m_bPlayerIsAccelerating;
	animstate_pose_param_cache_t m_tPoseParamMappings[ PLAYER_POSE_PARAM_COUNT ];
	bool						 m_bDeployRateLimiting;
	float						 m_flDurationMoveWeightIsTooHigh;
	float						 m_flStaticApproachSpeed;
	int							 m_nPreviousMoveState;
	float						 m_flStutterStep;
	float						 m_flActionWeightBiasRemainder;
	procedural_foot_t			 m_footLeft;
	procedural_foot_t			 m_footRight;
	float 						 m_flCameraSmoothHeight;
	bool  						 m_bSmoothHeightValid;
	float 						 m_flLastTimeVelocityOverTen;
	float 						 m_flAimYawMin;
	float 						 m_flAimYawMax;
	float 						 m_flAimPitchMin;
	float 						 m_flAimPitchMax;
	int   						 m_nAnimstateModelVersion;
}; // size: 0x344

class CStudioHdr {
public:
	class mstudioposeparamdesc_t {
	public:
		int					sznameindex;
		__forceinline char* const name(void) const { return ((char*)this) + sznameindex; }
		int					flags;	// ????
		float				start;	// starting value
		float				end;	// ending value
		float				loop;	// looping range, 0 for no looping, 360 for rotations, etc.
	};

	studiohdr_t* m_pStudioHdr;
	void* m_pVModel;
};

class C_AnimationLayer {
public:
	float   m_anim_time;			// 0x0000
	float   m_fade_out_time;		// 0x0004
	int     m_flags;				// 0x0008
	int     m_activty;				// 0x000C
	int     m_priority;				// 0x0010
	int     m_order;				// 0x0014
	int     m_sequence;				// 0x0018
	float   m_prev_cycle;			// 0x001C
	float   m_weight;				// 0x0020
	float   m_weight_delta_rate;	// 0x0024
	float   m_playback_rate;		// 0x0028
	float   m_cycle;				// 0x002C
	Entity* m_owner;				// 0x0030
	int     m_bits;					// 0x0034
}; // size: 0x0038

class CBoneAccessor {
public:
	void* m_pAnimating;
	BoneArray* m_pBones;
	int        m_ReadableBones;
	int        m_WritableBones;
};

class CBoneCache {
public:
	BoneArray* m_pCachedBones;
	PAD(0x8);
	int        m_CachedBoneCount;
};

struct clientHitVerify_t {
	vec3_t	vecPosition;
	float	flTimestamp;
	float	flExpireTime;
};

class Ragdoll : public Entity {
public:
	__forceinline Player* GetPlayer() {
		return g_csgo.m_entlist->GetClientEntityFromHandle< Player* >(m_hPlayer());
	}

	__forceinline EHANDLE& m_hPlayer() {
		return get< EHANDLE >(g_entoffsets.m_hPlayer);
	}

	__forceinline float& m_flDeathYaw() {
		return get< float >(g_entoffsets.m_flDeathYaw);
	}

	__forceinline float& m_flAbsYaw() {
		return get< float >(g_entoffsets.m_flAbsYaw);
	}
};

class Player : public Entity {
public:
	// netvars / etc.
	__forceinline vec3_t& m_vecAbsVelocity() {
		return get< vec3_t >(g_entoffsets.m_vecAbsVelocity);
	}

	__forceinline int& m_lifeState() {
		return get< int >(g_entoffsets.m_lifeState);
	}

	__forceinline int& m_fFlags() {
		return get< int >(g_entoffsets.m_fFlags);
	}

	__forceinline int& m_MoveType() {
		return get< int >(g_entoffsets.m_MoveType);
	}

	__forceinline int& m_iHealth() {
		return get< int >(g_entoffsets.m_iHealth);
	}

	__forceinline vec3_t& GetBonePosition(int i)
	{
		matrix3x4a_t boneMatrix[128];
		if (this->SetupBones(boneMatrix, 128, 0x00000100, GetTickCount64()))
		{
			return vec3_t(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]);
		}
		return vec3_t(0, 0, 0);
	}

	__forceinline int& m_iAccount() {
		return get< int >(g_entoffsets.m_iAccount);
	}

	__forceinline bool& m_bIsDefusing() {
		return get< bool >(g_entoffsets.m_bIsDefusing);
	}

	__forceinline bool& m_bIsGrabbingHostage() {
		return get< bool >(g_entoffsets.m_bIsGrabbingHostage);
	}

	__forceinline bool& m_bHasDefuser() {
		return get< bool >(g_entoffsets.m_bHasDefuser);
	}

	__forceinline bool& m_bIsBuyMenuOpen() {
		return get< bool >(g_entoffsets.m_bIsBuyMenuOpen);
	}

	__forceinline int& m_nHitboxSet() {
		return get< int >(g_entoffsets.m_nHitboxSet);
	}

	__forceinline ang_t& m_angAbsRotation() {
		return get< ang_t >(g_entoffsets.m_angAbsRotation);
	}

	__forceinline ang_t& m_angRotation() {
		return get< ang_t >(g_entoffsets.m_angRotation);
	}

	__forceinline ang_t& m_angNetworkAngles() {
		return get< ang_t >(g_entoffsets.m_angNetworkAngles);
	}

	__forceinline bool m_bIsLocalPlayer() {
		// .text:101E0078 674     84 C0				   test    al, al          ; Logical Compare
		// .text:101E007A 674     74 17				   jz      short loc_101E0093; Jump if Zero( ZF = 1 )
		// .text:101E007C 674     8A 83 F8 35 00 00	   mov     al, [ ebx + 35F8h ]
		return get< bool >(g_csgo.IsLocalPlayer);
	}

	__forceinline CUtlVector< clientHitVerify_t > m_vecBulletVerifyListClient() {
        //1036b9e8 f3 0f 10        MOVSS      XMM1,dword ptr [DAT_109ed0b8]                    = 3F800000h
        //         0d b8 d0 
		//         9e 10
		//1036b9f0 03 f8           ADD        EDI,EAX
		//1036b9f2 8b 86 84        MOV        EAX,dword ptr [ESI + 0xba84]
        //         ba 00 00

		return get< CUtlVector< clientHitVerify_t > >(g_csgo.BulletVerifyListClient);
	}

	__forceinline CCSGOPlayerAnimState* m_PlayerAnimState() {
		// .text:1037A5B8 00C     E8 E3 40 E6 FF         call    C_BasePlayer__Spawn ; Call Procedure
		// .text:1037A5BD 00C     80 BE E1 39 00 00 00   cmp     byte ptr[ esi + 39E1h ], 0; Compare Two Operands
		// .text:1037A5C4 00C     74 48                  jz      short loc_1037A60E; Jump if Zero( ZF = 1 )
		// .text:1037A5C6 00C     8B 8E 74 38 00 00      mov     ecx, [ esi + 3874h ]; this
		// .text:1037A5CC 00C     85 C9                  test    ecx, ecx; Logical Compare
		// .text:1037A5CE 00C     74 3E                  jz      short loc_1037A60E; Jump if Zero( ZF = 1 )
		return get< CCSGOPlayerAnimState* >(g_csgo.PlayerAnimState);
	}

	__forceinline CStudioHdr* m_studioHdr() {
		// .text:1017E902 08C    8B 86 3C 29 00 00    mov     eax, [ esi + 293Ch ]
		// .text:1017E908 08C    89 44 24 10          mov[ esp + 88h + var_78 ], eax
		return get< CStudioHdr* >(g_csgo.studioHdr);
	}

	__forceinline ulong_t& m_iMostRecentModelBoneCounter() {
		// .text:101AC9A9 000    89 81 80 26 00 00    mov[ ecx + 2680h ], eax
		return get< ulong_t >(g_csgo.MostRecentModelBoneCounter);
	}

	__forceinline float& m_flLastBoneSetupTime() {
		// .text:101AC99F 000    C7 81 14 29 00 00 FF FF+    mov     dword ptr [ecx+2914h], 0FF7FFFFFh;
		return get< float >(g_csgo.LastBoneSetupTime);
	}

	__forceinline bool& m_bControlledBot() {
		return get< bool >(g_entoffsets.m_bControlledBot);
	}

	__forceinline int& m_nTickBase() {
		return get< int >(g_entoffsets.m_nTickBase);
	}

	__forceinline float& m_flNextAttack() {
		return get< float >(g_entoffsets.m_flNextAttack);
	}

	__forceinline float& m_flDuckAmount() {
		return get< float >(g_entoffsets.m_flDuckAmount);
	}

	__forceinline float& m_flSimulationTime() {
		return get< float >(g_entoffsets.m_flSimulationTime);
	}

	__forceinline float& m_flOldSimulationTime() {
		return get< float >(g_entoffsets.m_flOldSimulationTime);
	}

	__forceinline float& m_flLowerBodyYawTarget() {
		return get< float >(g_entoffsets.m_flLowerBodyYawTarget);
	}

	__forceinline float& m_fImmuneToGunGameDamageTime() {
		return get< float >(g_entoffsets.m_fImmuneToGunGameDamageTime);
	}

	__forceinline bool& m_bHasHelmet() {
		return get< bool >(g_entoffsets.m_bHasHelmet);
	}

	__forceinline bool& m_bClientSideAnimation() {
		return get< bool >(g_entoffsets.m_bClientSideAnimation);
	}

	__forceinline bool& m_bHasHeavyArmor() {
		return get< bool >(g_entoffsets.m_bHasHeavyArmor);
	}

	__forceinline bool& m_bIsScoped() {
		return get< bool >(g_entoffsets.m_bIsScoped);
	}

	__forceinline bool& m_bDucking() {
		return get< bool >(g_entoffsets.m_bDucking);
	}

	__forceinline bool& m_bSpotted() {
		return get< bool >(g_entoffsets.m_bSpotted);
	}

	__forceinline int& m_iObserverMode() {
		return get< int >(g_entoffsets.m_iObserverMode);
	}

	__forceinline int& m_ArmorValue() {
		return get< int >(g_entoffsets.m_ArmorValue);
	}

	__forceinline float& m_flMaxspeed() {
		return get< float >(g_entoffsets.m_flMaxspeed);
	}

	__forceinline float& m_surfaceFriction() {
		return get< float >(g_entoffsets.m_surfaceFriction);
	}

	__forceinline float& m_flFlashBangTime() {
		return get< float >(g_entoffsets.m_flFlashBangTime);
	}

	__forceinline ang_t& m_angEyeAngles() {
		return get< ang_t >(g_entoffsets.m_angEyeAngles);
	}

	__forceinline ang_t& m_aimPunchAngle() {
		return get< ang_t >(g_entoffsets.m_aimPunchAngle);
	}

	__forceinline ang_t& m_viewPunchAngle() {
		return get< ang_t >(g_entoffsets.m_viewPunchAngle);
	}

	__forceinline ang_t& m_aimPunchAngleVel() {
		return get< ang_t >(g_entoffsets.m_aimPunchAngleVel);
	}

	__forceinline vec3_t& m_vecViewOffset() {
		return get< vec3_t >(g_entoffsets.m_vecViewOffset);
	}

	__forceinline CUserCmd& m_PlayerCommand() {
		return get< CUserCmd >(g_entoffsets.m_PlayerCommand);
	}

	__forceinline CUserCmd*& m_pCurrentCommand() {
		return get< CUserCmd* >(g_entoffsets.m_pCurrentCommand);
	}

	__forceinline int& m_nLastNonSkippedFrame() {
		return get< int >(g_entoffsets.m_nLastNonSkippedFrame);
	}

	__forceinline float* m_flPoseParameter() {
		return (float*)((uintptr_t)this + g_entoffsets.m_flPoseParameter);
	}

	__forceinline CBaseHandle* m_hMyWearables() {
		return (CBaseHandle*)((uintptr_t)this + g_entoffsets.m_hMyWearables);
	}

	__forceinline VarMapping_t* m_VarMap() {
		return (VarMapping_t*)((uintptr_t)this + g_entoffsets.m_VarMap);
	}

	__forceinline CBoneCache& m_BoneCache() {
		// TODO; sig
		return get< CBoneCache >(g_entoffsets.m_BoneCache);
	}

	__forceinline EHANDLE& m_hObserverTarget() {
		return get< EHANDLE >(g_entoffsets.m_hObserverTarget);
	}

	__forceinline EHANDLE& m_hActiveWeapon() {
		return get< EHANDLE >(g_entoffsets.m_hActiveWeapon);
	}

	__forceinline EHANDLE& m_hGroundEntity() {
		return get< EHANDLE >(g_entoffsets.m_hGroundEntity);
	}

	__forceinline CBaseHandle* m_hMyWeapons() {
		return (CBaseHandle*)((uintptr_t)this + g_entoffsets.m_hMyWeapons);
	}

	__forceinline C_AnimationLayer* m_AnimOverlay() {
		// .text:1017EAB1 08C    8B 47 1C                mov     eax, [edi+1Ch]
		// .text:1017EAB4 08C    8D 0C D5 00 00 00 00    lea     ecx, ds:0[ edx * 8 ]; Load Effective Address
		// .text:1017EABB 08C    2B CA                   sub     ecx, edx; Integer Subtraction
		// .text:1017EABD 08C    8B 80 70 29 00 00       mov     eax, [ eax + 2970h ]
		// .text:1017EAC3 08C    8D 34 C8                lea     esi, [ eax + ecx * 8 ]; Load Effective Address
		// .text:1017EAC6
		return get< C_AnimationLayer* >(g_csgo.AnimOverlay);
	}

	__forceinline float& m_flSpawnTime() {
		// .text:10381AB3 00C    F3 0F 10 49 10             movss   xmm1, dword ptr [ecx+10h] ; Move Scalar Single-FP
		// .text:10381AB8 00C    F3 0F 5C 88 90 A2 00 00    subss   xmm1, dword ptr[ eax + 0A290h ]; Scalar Single - FP Subtract
		return get< float >(g_csgo.SpawnTime);
	}

	__forceinline CBoneAccessor& m_BoneAccessor() {
		// .text:101A9253 1C4    C7 81 A0 26 00 00 00 FF 0F 00    mov     dword ptr[ ecx + 26A0h ], 0FFF00h
		// .text:101A925D 1C4    C7 81 9C 26 00 00 00 FF 0F 00    mov     dword ptr[ ecx + 269Ch ], 0FFF00h
		// .text:101A9267 1C4    8B 10                            mov     edx, [ eax ]
		// .text:101A9269 1C4    8D 81 94 26 00 00                lea     eax, [ ecx + 2694h ]; Load Effective Address
		// .text:101A926F 1C4    50                               push    eax
		return get< CBoneAccessor >(g_csgo.BoneAccessor);
	}

public:
	enum indices : size_t {
		GETREFEHANDLE = 2,
		TESTHITBOXES = 52,
		GETEYEPOS = 163,
		BUILDTRANSFORMATIONS = 184,
		SETUPBONES						= 187,
		DOEXTRABONEPROCESSING = 192,
		STANDARDBLENDINGRULES = 200,
		ACCUMULATELAYERS			   = 201,
		UPDATECLIENTSIDEANIMATION = 218, // 55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36
		GETACTIVEWEAPON = 262,
		GETFOV = 321,
		UPDATECOLLISIONBOUNDS = 329 // 56 57 8B F9 8B 0D ? ? ? ? F6 87 ? ? ? ? ?
	};

public:
	// virtuals.
	__forceinline ulong_t GetRefEHandle() {
		using GetRefEHandle_t = ulong_t(__thiscall*)(decltype(this));
		return util::get_method< GetRefEHandle_t >(this, GETREFEHANDLE)(this);
	}

	__forceinline void BuildTransformations(CStudioHdr* hdr, vec3_t* pos, quaternion_t* q, const matrix3x4_t& transform, int mask, uint8_t* computed) {
		using BuildTransformations_t = void(__thiscall*)(decltype(this), CStudioHdr*, vec3_t*, quaternion_t*, matrix3x4_t const&, int, uint8_t*);
		return util::get_method< BuildTransformations_t >(this, BUILDTRANSFORMATIONS)(this, hdr, pos, q, transform, mask, computed);
	}

	__forceinline void StandardBlendingRules(CStudioHdr* hdr, vec3_t* pos, quaternion_t* q, float time, int mask) {
		using StandardBlendingRules_t = void(__thiscall*)(decltype(this), CStudioHdr*, vec3_t*, quaternion_t*, float, int);
		return util::get_method< StandardBlendingRules_t >(this, STANDARDBLENDINGRULES)(this, hdr, pos, q, time, mask);
	}

	__forceinline float GetFOV() {
		return util::get_method< float(__thiscall*)(decltype(this)) >(this, GETFOV)(this);
	}

	__forceinline const vec3_t& WorldSpaceCenter() {
		return util::get_method< const vec3_t& (__thiscall*)(void*) >(this, WORLDSPACECENTER)(this);
	}

	__forceinline void GetEyePos(vec3_t* pos) {
		util::get_method< void(__thiscall*)(decltype(this), vec3_t*) >(this, GETEYEPOS)(this, pos);
	}


	#define FIRSTPERSON_TO_THIRDPERSON_VERTICAL_TOLERANCE_MIN 4.0f
	#define FIRSTPERSON_TO_THIRDPERSON_VERTICAL_TOLERANCE_MAX 10.0f
	__forceinline void ModifyEyePosition(CCSGOPlayerAnimState* state, vec3_t* pos) {
		if ( !state )
			return;
		if ( !state->m_pPlayer )
			return;
		if ( !state->m_bLanding && state->m_pPlayer->m_flDuckAmount() == 0 )
			return;

		auto v5 = 8;

		if ( v5 != -1 && state->m_pPlayer->m_BoneCache().m_pCachedBones ) {
			vec3_t head_pos(
				state->m_pPlayer->m_BoneCache().m_pCachedBones[ 8 ][ 0 ][ 3 ],
				state->m_pPlayer->m_BoneCache().m_pCachedBones[ 8 ][ 1 ][ 3 ],
				state->m_pPlayer->m_BoneCache().m_pCachedBones[ 8 ][ 2 ][ 3 ]);

			auto v12 = head_pos;
			auto v7 = v12.z + 1.7;

			auto v8 = pos->z;
			if ( v8 > v7 ) // if (v8 > (v12 + 1.7))
			{
				float v13 = 0.f;
				float v3 = (*pos).z - v7;

				float v4 = (v3 - 4.f) * 0.16666667;
				if ( v4 >= 0.f )
					v13 = std::fminf(v4, 1.f);

				(*pos).z = (((v7 - (*pos).z)) * (((v13 * v13) * 3.0) - (((v13 * v13) * 2.0) * v13))) + (*pos).z;
			}
		}
	}

	__forceinline vec3_t GetShootPosition() {
		vec3_t pos;

		GetEyePos(&pos);

		if ( *reinterpret_cast <int32_t*> (uintptr_t(this) + 0x39E1) ) {
			auto v3 = m_PlayerAnimState();
			if ( v3 ) {
				ModifyEyePosition(v3, &pos);
			}
		}

		return pos;
	}

	__forceinline void UpdateClientSideAnimation() {
		return util::get_method< void(__thiscall*)(decltype(this)) >(this, UPDATECLIENTSIDEANIMATION)(this);
	}

	__forceinline void UpdateCollisionBounds() {
		return util::get_method< void(__thiscall*)(decltype(this)) >(this, UPDATECOLLISIONBOUNDS)(this);
	}

	// misc funcs.
	__forceinline CStudioHdr* GetModelPtr() {
		using LockStudioHdr_t = void(__thiscall*)(decltype(this));

		if (!m_studioHdr())
			g_csgo.LockStudioHdr.as< LockStudioHdr_t >()(this);

		return m_studioHdr();
	}

	__forceinline Weapon* GetActiveWeapon() {
		return g_csgo.m_entlist->GetClientEntityFromHandle< Weapon* >(m_hActiveWeapon());
	}

	__forceinline Entity* GetObserverTarget() {
		return g_csgo.m_entlist->GetClientEntityFromHandle(m_hObserverTarget());
	}

	__forceinline Entity* GetGroundEntity() {
		return g_csgo.m_entlist->GetClientEntityFromHandle(m_hGroundEntity());
	}

	__forceinline void SetAnimLayers(C_AnimationLayer* layers) {
		std::memcpy(m_AnimOverlay(), layers, sizeof(C_AnimationLayer) * 13);
	}

	__forceinline void GetAnimLayers(C_AnimationLayer* layers) {
		std::memcpy(layers, m_AnimOverlay(), sizeof(C_AnimationLayer) * 13);
	}

	__forceinline void SetPoseParameters(float* poses) {
		std::memcpy(m_flPoseParameter(), poses, sizeof(float) * 24);
	}

	__forceinline void GetPoseParameters(float* poses) {
		std::memcpy(poses, m_flPoseParameter(), sizeof(float) * 24);
	}

	__forceinline bool ComputeHitboxSurroundingBox(vec3_t* mins, vec3_t* maxs) {
		using ComputeHitboxSurroundingBox_t = bool(__thiscall*)(void*, vec3_t*, vec3_t*);

		return g_csgo.ComputeHitboxSurroundingBox.as< ComputeHitboxSurroundingBox_t >()(this, mins, maxs);
	}

	__forceinline int GetSequenceActivity(int sequence) {
		using GetSequenceActivity_t = int(__fastcall*)(CStudioHdr*, int);

		return g_csgo.GetSequenceActivity.as< GetSequenceActivity_t >()(GetModelPtr(), sequence);
	}

	__forceinline bool HasC4() {
		using HasC4_t = bool(__thiscall*)(decltype(this));
		return g_csgo.HasC4.as< HasC4_t >()(this);
	}

	__forceinline void InvalidateBoneCache() {
		CBoneAccessor* accessor = &m_BoneAccessor();
		if (!accessor)
			return;

		accessor->m_WritableBones = 0;
		accessor->m_ReadableBones = 0;

		m_iMostRecentModelBoneCounter() = 0;
		m_flLastBoneSetupTime() = std::numeric_limits< float >::lowest();
	}

	__forceinline bool alive() {
		return m_lifeState() == LIFE_ALIVE;
	}

	__forceinline bool enemy(Player* from) {
		if (m_iTeamNum() != from->m_iTeamNum())
			return true;

		else if (g_csgo.mp_teammates_are_enemies->GetInt())
			return true;

		return false;
	}
};

class WeaponInfo {
private:
	PAD(0x4);											// 0x0000

public:
	const char* m_weapon_name;						// 0x0004 -- actual weapon name, even for usp-s and revolver. ex: "weapon_revolver"
	PAD(0xC);												// 0x0008
	int               m_max_clip1;							// 0x0014
	int				  m_max_clip2;							// 0x0018
	int				  m_default_clip1;						// 0x001C
	int		          m_default_clip2;						// 0x0020
	int               m_max_reserve;						// 0x0024
	PAD(0x4);												// 0x0028
	const char* m_world_model;						// 0x002C
	const char* m_view_model;							// 0x0030
	const char* m_world_dropped_model;				// 0x0034
	PAD(0x48);											// 0x0038
	const char* m_ammo_type;							// 0x0080
	uint8_t           pad_0084[4];						// 0x0084
	const char* m_sfui_name;							// 0x0088
	const char* m_deprecated_weapon_name;				// 0x008C -- shitty weapon name, shows "weapon_deagle" for revolver / etc.
	uint8_t           pad_0090[56];						// 0x0090
	CSWeaponType      m_weapon_type;						// 0x00C8
	int			      m_in_game_price;						// 0x00CC
	int               m_kill_award;							// 0x00D0
	const char* m_animation_prefix;					// 0x00D4
	float			  m_cycletime;							// 0x00D8
	float			  m_cycletime_alt;						// 0x00DC
	float			  m_time_to_idle;						// 0x00E0
	float			  m_idle_interval;						// 0x00E4
	bool			  m_is_full_auto;						// 0x00E5
	PAD(0x3);												// 0x00E8
	int               m_damage;								// 0x00EC
	float             m_armor_ratio;						// 0x00F0
	int               m_bullets;							// 0x00F4
	float             m_penetration;						// 0x00F8
	float             m_flinch_velocity_modifier_large;		// 0x00FC
	float             m_flinch_velocity_modifier_small;		// 0x0100
	float             m_range;								// 0x0104
	float             m_range_modifier;						// 0x0108
	float			  m_throw_velocity;						// 0x010C
	PAD(0xC);												// 0x0118
	bool			  m_has_silencer;						// 0x0119
	PAD(0x3);												// 0x011C
	const char* m_silencer_model;						// 0x0120
	int				  m_crosshair_min_distance;				// 0x0124
	int				  m_crosshair_delta_distance;			// 0x0128
	float             m_max_player_speed;					// 0x012C
	float             m_max_player_speed_alt;				// 0x0130
	float			  m_spread;								// 0x0134
	float			  m_spread_alt;							// 0x0138
	float             m_inaccuracy_crouch;					// 0x013C
	float             m_inaccuracy_crouch_alt;				// 0x0140
	float             m_inaccuracy_stand;					// 0x0144
	float             m_inaccuracy_stand_alt;				// 0x0148
	float             m_inaccuracy_jump_initial;			// 0x014C
	float             m_inaccuracy_jump;					// 0x0150
	float             m_inaccuracy_jump_alt;				// 0x0154
	float             m_inaccuracy_land;					// 0x0158
	float             m_inaccuracy_land_alt;				// 0x015C
	float             m_inaccuracy_ladder;					// 0x0160
	float             m_inaccuracy_ladder_alt;				// 0x0164
	float             m_inaccuracy_fire;					// 0x0168
	float             m_inaccuracy_fire_alt;				// 0x016C
	float             m_inaccuracy_move;					// 0x0170
	float             m_inaccuracy_move_alt;				// 0x0174
	float             m_inaccuracy_reload;					// 0x0178
	int               m_recoil_seed;						// 0x017C
	float			  m_recoil_angle;						// 0x0180
	float             m_recoil_angle_alt;					// 0x0184
	float             m_recoil_angle_variance;				// 0x0188
	float             m_recoil_angle_variance_alt;			// 0x018C
	float             m_recoil_magnitude;					// 0x0190
	float             m_recoil_magnitude_alt;				// 0x0194
	float             m_recoil_magnitude_variance;			// 0x0198
	float             m_recoil_magnitude_variance_alt;		// 0x019C
	float             m_recovery_time_crouch;				// 0x01A0
	float             m_recovery_time_stand;				// 0x01A4
	float             m_recovery_time_crouch_final;			// 0x01A8
	float             m_recovery_time_stand_final;			// 0x01AC
	float             m_recovery_transition_start_bullet;	// 0x01B0
	float             m_recovery_transition_end_bullet;		// 0x01B4
	bool			  m_unzoom_after_shot;					// 0x01B5
	PAD(0x3);												// 0x01B8
	bool		      m_hide_view_model_zoomed;				// 0x01B9
	bool			  m_zoom_levels;						// 0x01BA
	PAD(0x2);												// 0x01BC
	int				  m_zoom_fov[2];						// 0x01C4
	float			  m_zoom_time[3];						// 0x01D0
	PAD(0x8);												// 0x01D8
	float             m_addon_scale;						// 0x01DC
	PAD(0x8);												// 0x01E4
	int				  m_tracer_frequency;					// 0x01E8
	int				  m_tracer_frequency_alt;				// 0x01EC
	PAD(0x18);											// 0x0200
	int				  m_health_per_shot;					// 0x0204
	PAD(0x8);												// 0x020C
	float			  m_inaccuracy_pitch_shift;				// 0x0210
	float			  m_inaccuracy_alt_sound_threshold;		// 0x0214
	float			  m_bot_audible_range;					// 0x0218
	PAD(0x8);												// 0x0220
	const char* m_wrong_team_msg;						// 0x0224
	bool			  m_has_burst_mode;						// 0x0225
	PAD(0x3);												// 0x0228
	bool			  m_is_revolver;						// 0x0229
	bool			  m_can_shoot_underwater;				// 0x022A
	PAD(0x2);												// 0x022C			
};

class IRefCounted {
private:
	volatile long refCount;

public:
	virtual void destructor(char bDelete) = 0;
	virtual bool OnFinalRelease() = 0;

	void unreference() {
		if (InterlockedDecrement(&refCount) == 0 && OnFinalRelease()) {
			destructor(1);
		}
	}
};

class Weapon : public Entity {
public:
	using ref_vec_t = CUtlVector< IRefCounted* >;

	// netvars / etc.
	__forceinline ref_vec_t& m_CustomMaterials() {
		return get< ref_vec_t >(g_entoffsets.m_CustomMaterials);
	}

	__forceinline ref_vec_t& m_CustomMaterials2() {
		return get< ref_vec_t >(g_entoffsets.m_CustomMaterials2);
	}

	__forceinline ref_vec_t& m_VisualsDataProcessors() {
		return get< ref_vec_t >(g_entoffsets.m_VisualsDataProcessors);
	}

	__forceinline bool& m_bCustomMaterialInitialized() {
		return get< bool >(g_entoffsets.m_bCustomMaterialInitialized);
	}

	__forceinline int& m_iItemDefinitionIndex() {
		return get< int >(g_entoffsets.m_iItemDefinitionIndex);
	}

	__forceinline int& m_iClip1() {
		return get< int >(g_entoffsets.m_iClip1);
	}

	__forceinline int& m_iPrimaryReserveAmmoCount() {
		return get< int >(g_entoffsets.m_iPrimaryReserveAmmoCount);
	}

	__forceinline int& m_Activity() {
		return get< int >(g_entoffsets.m_Activity);
	}

	__forceinline float& m_fFireDuration() {
		return get< float >(g_entoffsets.m_fFireDuration);
	}

	__forceinline int& m_iBurstShotsRemaining() {
		return get< int >(g_entoffsets.m_iBurstShotsRemaining);
	}

	__forceinline float& m_flNextPrimaryAttack() {
		return get< float >(g_entoffsets.m_flNextPrimaryAttack);
	}

	__forceinline float& m_flNextSecondaryAttack() {
		return get< float >(g_entoffsets.m_flNextSecondaryAttack);
	}

	__forceinline float& m_flThrowStrength() {
		return get< float >(g_entoffsets.m_flThrowStrength);
	}

	__forceinline float& m_fNextBurstShot() {
		return get< float >(g_entoffsets.m_fNextBurstShot);
	}

	__forceinline int& m_zoomLevel() {
		return get< int >(g_entoffsets.m_zoomLevel);
	}

	__forceinline float& m_flRecoilIndex() {
		return get< float >(g_entoffsets.m_flRecoilIndex);
	}

	__forceinline int& m_weaponMode() {
		return get< int >(g_entoffsets.m_weaponMode);
	}

	__forceinline int& m_nFallbackPaintKit() {
		return get< int >(g_entoffsets.m_nFallbackPaintKit);
	}

	__forceinline int& m_nFallbackStatTrak() {
		return get< int >(g_entoffsets.m_nFallbackStatTrak);
	}

	__forceinline int& m_nFallbackSeed() {
		return get< int >(g_entoffsets.m_nFallbackSeed);
	}

	__forceinline float& m_flFallbackWear() {
		return get< float >(g_entoffsets.m_flFallbackWear);
	}

	__forceinline int& m_iViewModelIndex() {
		return get< int >(g_entoffsets.m_iViewModelIndex);
	}

	__forceinline int& m_iWorldModelIndex() {
		return get< int >(g_entoffsets.m_iWorldModelIndex);
	}

	__forceinline int& m_iAccountID() {
		return get< int >(g_entoffsets.m_iAccountID);
	}

	__forceinline int& m_iItemIDHigh() {
		return get< int >(g_entoffsets.m_iItemIDHigh);
	}

	__forceinline int& m_iEntityQuality() {
		return get< int >(g_entoffsets.m_iEntityQuality);
	}

	__forceinline int& m_OriginalOwnerXuidLow() {
		return get< int >(g_entoffsets.m_OriginalOwnerXuidLow);
	}

	__forceinline int& m_OriginalOwnerXuidHigh() {
		return get< int >(g_entoffsets.m_OriginalOwnerXuidHigh);
	}

	__forceinline bool& m_bPinPulled() {
		return get< bool >(g_entoffsets.m_bPinPulled);
	}

	__forceinline float& m_fThrowTime() {
		return get< float >(g_entoffsets.m_fThrowTime);
	}

	__forceinline EHANDLE& m_hWeapon() {
		return get< EHANDLE >(g_entoffsets.m_hWeapon);
	}

	__forceinline EHANDLE& m_hWeaponWorldModel() {
		return get< EHANDLE >(g_entoffsets.m_hWeaponWorldModel);
	}

	__forceinline EHANDLE& m_hOwnerEntity() {
		return get< EHANDLE >(g_entoffsets.m_hOwnerEntity);
	}

	__forceinline float& m_flConstraintRadius() {
		return get< float >(g_entoffsets.m_flConstraintRadius);
	}

	__forceinline float& m_fLastShotTime() {
		return get< float >(g_entoffsets.m_fLastShotTime);
	}

public:
	enum indices : size_t {
		SETMODELINDEX = 75,
		GETMAXCLIP1 = 367,
		GETSPREAD = 439,
		GETWPNDATA = 446, // C_WeaponCSBaseGun::GetCSWpnData
		GETINACCURACY = 469,
		UPDATEACCURACYPENALTY = 471,
	};

public:
	// virtuals.
	__forceinline int GetMaxClip1() {
		return util::get_method< int(__thiscall*)(void*) >(this, GETMAXCLIP1)(this);
	}

	__forceinline void SetGloveModelIndex(int index) {
		return util::get_method< void(__thiscall*)(void*, int) >(this, SETMODELINDEX)(this, index);
	}

	__forceinline WeaponInfo* GetWpnData() {
		return util::get_method< WeaponInfo* (__thiscall*)(void*) >(this, GETWPNDATA)(this);
	}

	__forceinline float GetInaccuracy() {
		return util::get_method< float(__thiscall*)(void*) >(this, GETINACCURACY)(this);
	}

	__forceinline float GetSpread() {
		return util::get_method< float(__thiscall*)(void*) >(this, GETSPREAD)(this);
	}

	__forceinline void UpdateAccuracyPenalty() {
		return util::get_method< void(__thiscall*)(void*) >(this, UPDATEACCURACYPENALTY)(this);
	}

	// misc funcs.
	__forceinline Weapon* GetWeapon() {
		return g_csgo.m_entlist->GetClientEntityFromHandle< Weapon* >(m_hWeapon());
	}

	__forceinline Weapon* GetWeaponWorldModel() {
		return g_csgo.m_entlist->GetClientEntityFromHandle< Weapon* >(m_hWeaponWorldModel());
	}

	__forceinline bool IsKnife() {
		return (GetWpnData()->m_weapon_type == WEAPONTYPE_KNIFE && m_iItemDefinitionIndex() != ZEUS);
	}

	__forceinline vec3_t CalculateSpread(int seed, float inaccuracy, float spread, bool revolver2 = false) {
		WeaponInfo* wep_info;
		int        item_def_index;
		float      recoil_index, r1, r2, r3, r4, s1, c1, s2, c2;

		// if we have no bullets, we have no spread.
		wep_info = GetWpnData();
		if ( !wep_info || !wep_info->m_bullets )
			return {};

		// get some data for later.
		item_def_index = m_iItemDefinitionIndex();
		recoil_index = m_flRecoilIndex();

		// seed randomseed.
		g_csgo.RandomSeed(seed);

		// generate needed floats.
		r1 = g_csgo.RandomFloat(0.f, 1.f);
		r2 = g_csgo.RandomFloat(0.f, math::pi_2);
		r3 = g_csgo.RandomFloat(0.f, 1.f);
		r4 = g_csgo.RandomFloat(0.f, math::pi_2);

		// revolver secondary spread.
		if ( item_def_index == REVOLVER && revolver2 ) {
			r1 = 1.f - (r1 * r1);
			r3 = 1.f - (r3 * r3);
		}

		// negev spread.
		else if ( item_def_index == NEGEV && recoil_index < 3.f ) {
			for ( int i{ 3 }; i > recoil_index; --i ) {
				r1 *= r1;
				r3 *= r3;
			}

			r1 = 1.f - r1;
			r3 = 1.f - r3;
		}

		// get needed sine / cosine values.
		c1 = std::cos(r2);
		c2 = std::cos(r4);
		s1 = std::sin(r2);
		s2 = std::sin(r4);

		// calculate spread vector.
		return {
			(c1 * (r1 * inaccuracy)) + (c2 * (r3 * spread)),
			(s1 * (r1 * inaccuracy)) + (s2 * (r3 * spread)),
			0.f
		};
	}

	__forceinline vec3_t CalculateSpread(int seed, bool revolver2 = false) {
		return CalculateSpread(seed, GetInaccuracy(), GetSpread(), revolver2);
	}

	__forceinline std::string GetLocalizedName() {
		C_EconItemView* item_view;
		CEconItemDefinition* item_def;

		item_view = g_csgo.GetEconItemView(this);
		if (!item_view)
			return XOR("error");

		item_def = g_csgo.GetStaticData(item_view);
		if (!item_def)
			return XOR("error");

		return util::WideToMultiByte(g_csgo.m_localize->Find(item_def->GetItemBaseName()));
	}
};

class CTraceFilterSimple_game {
public:
	void* m_vmt;
	const Entity* m_pass_ent1;
	int             m_collision_group;
	ShouldHitFunc_t m_shouldhit_check_fn;

public:
	__forceinline CTraceFilterSimple_game() :
		m_vmt{ g_csgo.CTraceFilterSimple_vmt.as< void* >() },
		m_pass_ent1{},
		m_collision_group{},
		m_shouldhit_check_fn{} {}

	__forceinline CTraceFilterSimple_game(const Entity* pass_ent1, int collision_group = COLLISION_GROUP_NONE, ShouldHitFunc_t shouldhit_check_fn = nullptr) :
		m_vmt{ g_csgo.CTraceFilterSimple_vmt.as< void* >() },
		m_pass_ent1{ pass_ent1 },
		m_collision_group{ collision_group },
		m_shouldhit_check_fn{ shouldhit_check_fn } {}

	__forceinline bool ShouldHitEntity(Entity* entity, int contents_mask) {
		// note - dex; game is dumb, this gets the real vmt.
		void* real_vmt = *(void**)m_vmt;

		return util::get_method< bool(__thiscall*)(void*, Entity*, int) >(real_vmt, 0)(real_vmt, entity, contents_mask);
	}

	// note - dex; don't really care about calling the virtuals for these two functions, they only set members in the class for us.
	__forceinline void SetPassEntity(Entity* pass_ent1) {
		m_pass_ent1 = pass_ent1;

		// util::get_method< void (__thiscall *)( void *, Entity* ) >( m_vmt, 2 )( m_vmt, pass_ent1 );
	}

	__forceinline void SetCollisionGroup(int collision_group) {
		m_collision_group = collision_group;

		// util::get_method< void (__thiscall *)( void *, int ) >( m_vmt, 3 )( m_vmt, collision_group );
	}
};

class CTraceFilterSkipTwoEntities_game {
public:
	void* m_vmt;
	const Entity* m_pass_ent1;
	int             m_collision_group;
	ShouldHitFunc_t m_shouldhit_check_fn;
	const Entity* m_pass_ent2;

public:
	__forceinline CTraceFilterSkipTwoEntities_game() :
		m_vmt{ g_csgo.CTraceFilterSkipTwoEntities_vmt.as< void* >() },
		m_pass_ent1{},
		m_collision_group{},
		m_shouldhit_check_fn{},
		m_pass_ent2{} {}

	__forceinline CTraceFilterSkipTwoEntities_game(const Entity* pass_ent1, const Entity* pass_ent2, int collision_group = COLLISION_GROUP_NONE, ShouldHitFunc_t shouldhit_check_fn = nullptr) :
		m_vmt{ g_csgo.CTraceFilterSimple_vmt.as< void* >() },
		m_pass_ent1{ pass_ent1 },
		m_collision_group{ collision_group },
		m_shouldhit_check_fn{ shouldhit_check_fn },
		m_pass_ent2{ pass_ent2 } {}

	__forceinline bool ShouldHitEntity(Entity* entity, int contents_mask) {
		// note - dex; game is dumb, this gets the real vmt.
		void* real_vmt = *(void**)m_vmt;

		return util::get_method< bool(__thiscall*)(void*, Entity*, int) >(m_vmt, 0)(m_vmt, entity, contents_mask);
	}

	// note - dex; don't really care about calling the virtuals for these two functions, they only set members in the class for us.
	__forceinline void SetPassEntity(Entity* pass_ent1) {
		m_pass_ent1 = pass_ent1;

		// util::get_method< void (__thiscall *)( void *, Entity* ) >( m_vmt, 2 )( m_vmt, pass_ent1 );
	}

	__forceinline void SetCollisionGroup(int collision_group) {
		m_collision_group = collision_group;

		// util::get_method< void (__thiscall *)( void *, int ) >( m_vmt, 3 )( m_vmt, collision_group );
	}

	__forceinline void SetPassEntity2(Entity* pass_ent2) {
		m_pass_ent2 = pass_ent2;
		// util::get_method< void (__thiscall *)( void *, Entity* ) >( m_vmt, 4 )( m_vmt, pass_ent2 );
	}
};