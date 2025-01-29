#include "includes.h"

Bones g_bones{};;

bool Bones::setup( Player* player, BoneArray* out, LagRecord* record ) {
	// if the record isnt setup yet.
	if( !record->m_setup ) {
		// run setupbones rebuilt.
		if( !BuildBones( player, BONE_USED_BY_ANYTHING, record->m_bones, record ) )
			return false;

		// we have setup this record bones.
		record->m_setup = true;
	}

	// record is setup.
	if( out && record->m_setup )
		std::memcpy( out, record->m_bones, sizeof( BoneArray ) * 128 );

	return true;
}

void Bones::backup(Player* player, int mask, BoneArray* out, LagRecord* record, BonesObject* object) {
	// set our backups.
	object->origin = player->GetAbsOrigin();
	object->angle = player->GetAbsAngles();
	object->last_skipped = player->m_nLastNonSkippedFrame();

	player->GetPoseParameters(object->poses);
	player->GetAnimLayers(object->layers);

	// prevent the game from running ShouldSkipAnimationFrame.
	player->m_nLastNonSkippedFrame() = NULL;

	// compute transform from raw data.
	matrix3x4_t transform;
	math::AngleMatrix(record->m_abs_ang, record->m_pred_origin, transform);
	object->transform = transform;

	// force some needed entity flags. these won't be restored.
	player->AddEffect(EF_NOINTERP);

	player->AddEntityFlag(EFL_DIRTY_ABSVELOCITY);
	player->AddEntityFlag(EFL_DIRTY_ABSANGVELOCITY);
	player->AddEntityFlag(EFL_DIRTY_ABSTRANSFORM);

	// set non interpolated data.
	player->SetAbsOrigin(record->m_pred_origin);
	player->SetAbsAngles(record->m_abs_ang);
	player->SetPoseParameters(record->m_poses);
	player->SetAnimLayers(record->m_layers);

	// force game to call AccumulateLayers - pvs fix.
	m_running = true;
}

void Bones::restore(Player* player, int mask, BoneArray* out, LagRecord* record, BonesObject* object) {
	// restore original interpolated entity data.
	player->SetAbsOrigin(object->origin);
	player->SetAbsAngles(object->angle);
	player->SetPoseParameters(object->poses);
	player->SetAnimLayers(object->layers);

	// revert to old game behavior.
	m_running = false;

	// allow the game to call ShouldSkipAnimationFrame.
	player->m_nLastNonSkippedFrame() = object->last_skipped;
}

bool Bones::BuildBones( Player* target, int mask, BoneArray* out, LagRecord* record ) {
	vec3_t		     pos[ 128 ];
	quaternion_t     q[ 128 ];

	// get hdr.
	CStudioHdr* hdr = target->GetModelPtr( );
	if( !hdr )
		return false;

	// get ptr to bone accessor.
	CBoneAccessor* accessor = &target->m_BoneAccessor( );
	if( !accessor )
		return false;

	// store origial output matrix.
	// likely cachedbonedata.
	BoneArray* backup_matrix = accessor->m_pBones;
	if( !backup_matrix )
		return false;

	// make our bone object.
	BonesObject object;

	// call our backup function
	backup( target, mask, out, record, &object );

	// set bone array for write.
	accessor->m_pBones = out;

	// compute and build bones.
	target->StandardBlendingRules( hdr, pos, q, record->m_pred_time, mask );

	uint8_t computed[ 0x100 ];
	std::memset( computed, 0, 0x100 );
	target->BuildTransformations( hdr, pos, q, object.transform, mask, computed );

	// restore old matrix.
	accessor->m_pBones = backup_matrix;

	// restore our old variables.
	restore( target, mask, out, record, &object );

	return true;
}
