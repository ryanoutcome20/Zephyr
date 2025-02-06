#pragma once

class BonesObject {
public:
	vec3_t origin;
	ang_t absangle;
	ang_t eyeangle;
	int last_skipped;
	float poses[ 24 ];
	C_AnimationLayer layers[ 13 ];
	matrix3x4_t transform;
};

class Bones {
public:
	bool m_running;

public:
	bool setup( Player* player, BoneArray* out, LagRecord* record );
	void backup( Player* player, int mask, BoneArray* out, LagRecord* record, BonesObject* object );
	void restore(Player* player, int mask, BoneArray* out, LagRecord* record, BonesObject* object);
	bool BuildBones( Player* target, int mask, BoneArray* out, LagRecord* record );
	bool BuildBonesDirty( Player* target, int mask, BoneArray* out );
};

extern Bones g_bones;