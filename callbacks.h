#pragma once

namespace callbacks {
	void SkinUpdate( );
	void ForceFullUpdate( );
	void ToggleMinimalDamageOverride( );
	void ToggleExtendedLag( );
	void ToggleThirdPerson( );
	void ToggleFakeLatency( );
    void ToggleKillfeed( );

	void ConfigLoad( );
	void ConfigSave( );
	void ConfigCreate();
	void ConfigDelete();
	void ConfigRefresh();

	bool IsBaimHealth( );
	bool IsFovOn( );
	bool IsHitchanceOn( );
	bool IsPenetrationOn( );
	bool IsMultipointOn( );
	bool IsMultipointBodyOn( );

	bool IsAntiAimModeStand( );
	bool HasStandYaw( );
	bool IsStandYawJitter( );
	bool IsStandYawRotate( );
	bool IsStandYawRnadom( );
	bool IsStandDirAuto( );
	bool IsStandDirCustom( );
	bool IsStandBodyCustom( );
	bool IsStandBodyDouble();
	bool IsAntiAimModeWalk( );
	bool WalkHasYaw( );
	bool IsWalkYawJitter( );
	bool IsWalkYawRotate( );
	bool IsWalkYawRandom( );
	bool IsWalkDirAuto( );
	bool IsWalkDirCustom( );
	bool IsAntiAimModeAir( );
	bool AirHasYaw( );
	bool IsAirYawJitter( );
	bool IsAirYawRotate( );
	bool IsAirYawRandom( );
	bool IsAirDirAuto( );
	bool IsAirDirCustom( );
	bool IsFakeAntiAimRelative( );
	bool IsFakeAntiAimJitter( );
	bool IsFakeLagOn( );
	
	bool IsChamsEnemy( );
	bool IsChamsFriendly( );
	bool IsChamsLocal( );

	bool IsBoxEnemy( );
	bool IsBoxFriendly( );
	bool IsBoxLocal( );
	bool IsNameEnemy( );
	bool IsNameFriendly( );
	bool IsNameLocal( );
	bool IsWeaponOn( );
	bool IsWeaponEnemy( );
	bool IsWeaponFriendly( );
	bool IsWeaponLocal( );
	bool IsGlowEnemy( );
	bool IsGlowFriendly( );
	bool IsGlowLocal( );
	bool IsSkeletonEnemy( );
	bool IsSkeletonFriendly( );
	bool IsSkeletonLocal( );
	bool IsOffscreenOn( );
	bool IsHealthOn( );
	bool IsHealthOverrideOn( );
	bool IsLBYUpdateOn( );
	bool IsShotMatrixOn( );

	bool IsItemsOn( );
	bool IsAmmoOn( );
	bool IsAmmoBarOn( );
	bool IsDroppedC4On( );
	bool IsProjectileOn();
	bool IsProjectileSphereOn( );
	bool IsProjectileGroundOn();
	bool IsAutopeekVisualizeOn( );
	bool IsImpactBeamsOn( );
	bool IsImpactBoxesServer( );
	bool IsImpactBoxesClient( );
	bool IsImpactBoxesOn( );

	bool IsAutoBuyOn( );
	bool IsNotificationsOn( );

	bool IsConfigMM( );
	bool IsConfigNS( );

	// weapon cfgs.
	bool DEAGLE( );
	bool ELITE( );
	bool FIVESEVEN( );
	bool GLOCK( );
	bool AK47( );
	bool AUG( );
	bool AWP( );
	bool FAMAS( );
	bool G3SG1( );
	bool GALIL( );
	bool M249( );
	bool M4A4( );
	bool MAC10( );
	bool P90( );
	bool UMP45( );
	bool XM1014( );
	bool BIZON( );
	bool MAG7( );
	bool NEGEV( );
	bool SAWEDOFF( );
	bool TEC9( );
	bool P2000( );
	bool MP7( );
	bool MP9( );
	bool NOVA( );
	bool P250( );
	bool SCAR20( );
	bool SG553( );
	bool SSG08( );
	bool M4A1S( );
	bool USPS( );
	bool CZ75A( );
	bool REVOLVER( );
	bool KNIFE_BAYONET( );
	bool KNIFE_FLIP( );
	bool KNIFE_GUT( );
	bool KNIFE_KARAMBIT( );
	bool KNIFE_M9_BAYONET( );
	bool KNIFE_HUNTSMAN( );
	bool KNIFE_FALCHION( );
	bool KNIFE_BOWIE( );
	bool KNIFE_BUTTERFLY( );
	bool KNIFE_SHADOW_DAGGERS( );
	bool AUTO_STOP( );
}