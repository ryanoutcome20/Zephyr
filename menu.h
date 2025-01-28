#pragma once

class AimbotTab : public Tab {
public:
	// col1.
	Checkbox	  enable;
	Checkbox	  silent;
	Dropdown	  selection;
	Checkbox	  fov;
	Slider		  fov_amount;
	MultiDropdown hitbox;
	MultiDropdown hitbox_history;
	MultiDropdown multipoint;
	Slider		  scale;
	Slider		  body_scale;
	Slider		  minimal_damage;
	Checkbox	  minimal_damage_hp;
	Checkbox	  penetrate;
	Slider		  penetrate_minimal_damage;
	Checkbox	  penetrate_minimal_damage_hp;
	Checkbox      knifebot;
	Checkbox	  zeusbot;
	Checkbox	  ignore_moving_limbs;

	// col2.
	Dropdown      zoom;
	Checkbox      nospread;
	Checkbox      norecoil;
	Checkbox      hitchance;
	Slider	      hitchance_amount;
	Checkbox      lagfix;
	Checkbox	  correct;
	MultiDropdown baim1;
	MultiDropdown baim2;
	Slider        baim_hp;
	Keybind       baim_key;
	Keybind		minimal_damage_override;
	Slider		  minimal_damage_override_damage;
	Slider		  minimal_damage_override_penetrate_damage;

public:
	void init() {
		// title.
		SetTitle(XOR("aimbot"));

		enable.setup(XOR("enable"), XOR("enable"));
		RegisterElement(&enable);

		silent.setup(XOR("silent aimbot"), XOR("silent"));
		RegisterElement(&silent);

		selection.setup(XOR("target selection"), XOR("selection"), { XOR("distance"), XOR("crosshair"), XOR("damage"), XOR("health"), XOR("lag"), XOR("height") });
		RegisterElement(&selection);

		fov.setup(XOR("angle limit"), XOR("fov"));
		RegisterElement(&fov);

		fov_amount.setup( "", XOR( "fov_amount" ), 1.f, 180.f, false, 0, 180.f, 1.f, XOR( L"°" ) );
		fov_amount.AddShowCallback(callbacks::IsFovOn);
		RegisterElement(&fov_amount);

		hitbox.setup(XOR("hitbox"), XOR("hitbox"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		RegisterElement(&hitbox);

		hitbox_history.setup(XOR("hitbox history"), XOR("hitbox_history"), { XOR("head"), XOR("chest"), XOR("body"), XOR("arms"), XOR("legs") });
		RegisterElement(&hitbox_history);

		multipoint.setup(XOR("multi-point"), XOR("multipoint"), { XOR("head"), XOR("chest"), XOR("body"), XOR("legs") });
		RegisterElement(&multipoint);

		scale.setup("", XOR("hitbox_scale"), 1.f, 100.f, false, 0, 90.f, 1.f, XOR(L"%"));
		scale.AddShowCallback(callbacks::IsMultipointOn);
		RegisterElement(&scale);

		body_scale.setup(XOR("body hitbox scale"), XOR("body_hitbox_scale"), 1.f, 100.f, true, 0, 50.f, 1.f, XOR(L"%"));
		body_scale.AddShowCallback(callbacks::IsMultipointBodyOn);
		RegisterElement(&body_scale);

		minimal_damage.setup(XOR("minimal damage"), XOR("minimal_damage"), 1.f, 100.f, true, 0, 40.f, 1.f);
		RegisterElement(&minimal_damage);

		minimal_damage_hp.setup(XOR("scale damage on hp"), XOR("minimal_damage_hp"));
		RegisterElement(&minimal_damage_hp);

		penetrate.setup(XOR("penetrate walls"), XOR("penetrate"));
		RegisterElement(&penetrate);

		penetrate_minimal_damage.setup("", XOR("penetrate_minimal_damage"), 1.f, 100.f, false, 0, 30.f, 1.f);
		penetrate_minimal_damage.AddShowCallback(callbacks::IsPenetrationOn);
		RegisterElement(&penetrate_minimal_damage);

		penetrate_minimal_damage_hp.setup(XOR("scale penetration damage on hp"), XOR("penetrate_minimal_damage_hp"));
		penetrate_minimal_damage_hp.AddShowCallback(callbacks::IsPenetrationOn);
		RegisterElement(&penetrate_minimal_damage_hp);

		knifebot.setup(XOR("aimbot with knife"), XOR("knifebot"));
		RegisterElement(&knifebot);

		zeusbot.setup(XOR("aimbot with taser"), XOR("zeusbot"));
		RegisterElement(&zeusbot);

		ignore_moving_limbs.setup(XOR("ignore moving limbs"), XOR("ignore_moving_limbs"));
		RegisterElement(&ignore_moving_limbs);

		// col2.
		zoom.setup(XOR("auto scope"), XOR("zoom"), { XOR("off"), XOR("always"), XOR("hitchance fail") });
		RegisterElement(&zoom, 1);

		nospread.setup(XOR("compensate spread"), XOR("nospread"));
		nospread.AddShowCallback(callbacks::IsConfigNS);
		RegisterElement(&nospread, 1);

		norecoil.setup(XOR("compensate recoil"), XOR("norecoil"));
		RegisterElement(&norecoil, 1);

		hitchance.setup(XOR("hitchance"), XOR("hitchance"));
		hitchance.AddShowCallback(callbacks::IsConfigMM);
		RegisterElement(&hitchance, 1);

		hitchance_amount.setup("", XOR("hitchance_amount"), 1.f, 100.f, false, 0, 50.f, 1.f, XOR(L"%"));
		hitchance_amount.AddShowCallback(callbacks::IsConfigMM);
		hitchance_amount.AddShowCallback(callbacks::IsHitchanceOn);
		RegisterElement(&hitchance_amount, 1);

		lagfix.setup(XOR("predict fake-lag"), XOR("lagfix"));
		RegisterElement(&lagfix, 1);

		correct.setup(XOR("correct anti-aim"), XOR("correct"));
		RegisterElement(&correct, 1);

		baim1.setup(XOR("prefer body aim"), XOR("baim1"), { XOR("always"), XOR("lethal"), XOR("lethal x2"), XOR("fake"), XOR("in air") });
		RegisterElement(&baim1, 1);

		baim2.setup(XOR("only body aim"), XOR("baim2"), { XOR("always"), XOR("health"), XOR("fake"), XOR("in air") });
		RegisterElement(&baim2, 1);

		baim_hp.setup("", XOR("baim_hp"), 1.f, 50.f, false, 0, 20.f, 1.f, XOR(L"hp"));
		baim_hp.AddShowCallback(callbacks::IsBaimHealth);
		RegisterElement(&baim_hp, 1);

		baim_key.setup(XOR("body aim on key"), XOR("baim_key"));
		RegisterElement(&baim_key, 1);

		minimal_damage_override.setup(XOR("minimal damage override"), XOR("minimal_damage_override"));
		minimal_damage_override.SetToggleCallback( callbacks::ToggleMinimalDamageOverride );
		RegisterElement(&minimal_damage_override, 1);

		minimal_damage_override_damage.setup(XOR("minimal damage"), XOR("minimal_damage_override_damage"), 1.f, 100.f, true, 0, 40.f, 1.f);
		RegisterElement(&minimal_damage_override_damage, 1);

		minimal_damage_override_penetrate_damage.setup(XOR("penetrate damage"), XOR("minimal_damage_override_penetrate_damage"), 1.f, 100.f, true, 0, 30.f, 1.f);
		minimal_damage_override_penetrate_damage.AddShowCallback(callbacks::IsPenetrationOn);
		RegisterElement(&minimal_damage_override_penetrate_damage, 1);
	}
};

class AntiAimTab : public Tab {
public:
	// col 1.
	Checkbox enable;
	Checkbox edge;
	Dropdown mode;

	Dropdown pitch_stand;
	Dropdown yaw_stand;
	Slider   jitter_range_stand;
	Slider   rot_range_stand;
	Slider   rot_speed_stand;
	Slider   rand_update_stand;
	Dropdown dir_stand;
	Slider   dir_time_stand;
	Slider   dir_custom_stand;
	Checkbox dir_lock;
	Dropdown base_angle_stand;
	Dropdown body_fake_stand;
	Slider		  body_fake_stand_custom;
	Checkbox	  body_fake_stand_custom_double;
	Slider		  body_fake_stand_custom_double_amount;

	Dropdown pitch_walk;
	Dropdown yaw_walk;
	Slider   jitter_range_walk;
	Slider   rot_range_walk;
	Slider   rot_speed_walk;
	Slider   rand_update_walk;
	Dropdown dir_walk;
	Slider	 dir_time_walk;
	Slider   dir_custom_walk;
	Dropdown base_angle_walk;

	Dropdown pitch_air;
	Dropdown yaw_air;
	Slider   jitter_range_air;
	Slider   rot_range_air;
	Slider   rot_speed_air;
	Slider   rand_update_air;
	Dropdown dir_air;
	Slider   dir_time_air;
	Slider   dir_custom_air;
	Dropdown base_angle_air;
	Dropdown body_fake_air;

	// col 2.
	Dropdown fake_yaw;
	Slider	 fake_relative;
	Slider	 fake_jitter_range;

	Checkbox      lag_enable;
	MultiDropdown lag_active;
	Dropdown      lag_mode;
	Slider        lag_limit;
	Keybind		  lag_extend;
	Slider			  lag_extended_limit;	
	Checkbox      lag_land;
	Checkbox      lag_revolver;
	Checkbox		  anti_lastmove;

public:
	void init() {
		SetTitle(XOR("anti-aim"));

		enable.setup(XOR("enable"), XOR("enable"));
		RegisterElement(&enable);

		edge.setup(XOR("edge"), XOR("edge"));
		RegisterElement(&edge);

		mode.setup("", XOR("mode"), { XOR("stand"), XOR("walk"), XOR("air") }, false);
		RegisterElement(&mode);

		// stand.
		pitch_stand.setup(XOR("pitch"), XOR("pitch_stnd"), { XOR("off"), XOR("down"), XOR("up"), XOR("random"), XOR("ideal") });
		pitch_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		RegisterElement(&pitch_stand);

		yaw_stand.setup(XOR("yaw"), XOR("yaw_stnd"), { XOR("off"), XOR("direction"), XOR("jitter"), XOR("rotate"), XOR("random") });
		yaw_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		RegisterElement(&yaw_stand);

		jitter_range_stand.setup("", XOR("jitter_range_stnd"), 1.f, 180.f, false, 0, 45.f, 5.f, XOR(L"°"));
		jitter_range_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		jitter_range_stand.AddShowCallback(callbacks::IsStandYawJitter);
		RegisterElement(&jitter_range_stand);

		rot_range_stand.setup("", XOR("rot_range_stnd"), 0.f, 360.f, false, 0, 360.f, 5.f, XOR(L"°"));
		rot_range_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		rot_range_stand.AddShowCallback(callbacks::IsStandYawRotate);
		RegisterElement(&rot_range_stand);

		rot_speed_stand.setup("", XOR("rot_speed_stnd"), 1.f, 100.f, false, 0, 10.f, 1.f, XOR(L"%"));
		rot_speed_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		rot_speed_stand.AddShowCallback(callbacks::IsStandYawRotate);
		RegisterElement(&rot_speed_stand);

		rand_update_stand.setup("", XOR("rand_update_stnd"), 0.f, 1.f, false, 1, 0.f, 0.1f);
		rand_update_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		rand_update_stand.AddShowCallback(callbacks::IsStandYawRnadom);
		RegisterElement(&rand_update_stand);

		dir_stand.setup(XOR("direction"), XOR("dir_stnd"), { XOR("auto"), XOR("backwards"), XOR("left"), XOR("right"), XOR("custom") });
		dir_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		dir_stand.AddShowCallback(callbacks::HasStandYaw);
		RegisterElement(&dir_stand);

		dir_time_stand.setup("", XOR("dir_time_stnd"), 0.f, 10.f, false, 0, 0.f, 1.f, XOR(L"s"));
		dir_time_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		dir_time_stand.AddShowCallback(callbacks::HasStandYaw);
		dir_time_stand.AddShowCallback(callbacks::IsStandDirAuto);
		RegisterElement(&dir_time_stand);

		dir_custom_stand.setup("", XOR("dir_custom_stnd"), -180.f, 180.f, false, 0, 0.f, 5.f, XOR(L"°"));
		dir_custom_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		dir_custom_stand.AddShowCallback(callbacks::HasStandYaw);
		dir_custom_stand.AddShowCallback(callbacks::IsStandDirCustom);
		RegisterElement(&dir_custom_stand);

		base_angle_stand.setup(XOR("base angle"), XOR("base_angle_stand"), { XOR("off"), XOR("static"), XOR("away crosshair"), XOR("away distance") });
		base_angle_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		base_angle_stand.AddShowCallback(callbacks::HasStandYaw);
		RegisterElement(&base_angle_stand);

		dir_lock.setup(XOR("lock direction"), XOR("dir_lock"));
		dir_lock.AddShowCallback(callbacks::IsAntiAimModeStand);
		dir_lock.AddShowCallback(callbacks::HasStandYaw);
		RegisterElement(&dir_lock);

		body_fake_stand.setup(XOR("fake body"), XOR("body_fake_stnd"), { XOR("off"), XOR("left"), XOR("right"), XOR("opposite"), XOR("z"), XOR("custom") });
		body_fake_stand.AddShowCallback(callbacks::IsAntiAimModeStand);
		body_fake_stand.AddShowCallback(callbacks::HasStandYaw);
		RegisterElement(&body_fake_stand);

		body_fake_stand_custom.setup("", XOR("body_fake_stand_custom"), -180.f, 180.f, false, 0, 90.f, 5.f, XOR(L"°"));
		body_fake_stand_custom.AddShowCallback(callbacks::IsAntiAimModeStand);
		body_fake_stand_custom.AddShowCallback(callbacks::HasStandYaw);
		body_fake_stand_custom.AddShowCallback(callbacks::IsStandBodyCustom);
		RegisterElement(&body_fake_stand_custom);

		body_fake_stand_custom_double.setup(XOR("early flick"), XOR("body_fake_stand_custom_double"));
		body_fake_stand_custom_double.AddShowCallback(callbacks::IsAntiAimModeStand);
		body_fake_stand_custom_double.AddShowCallback(callbacks::HasStandYaw);
		body_fake_stand_custom_double.AddShowCallback(callbacks::IsStandBodyCustom);
		RegisterElement(&body_fake_stand_custom_double);

		body_fake_stand_custom_double_amount.setup("", XOR("body_fake_stand_custom_double_amount"), -180.f, 180.f, false, 0, -90.f, 5.f, XOR(L"°"));
		body_fake_stand_custom_double_amount.AddShowCallback(callbacks::IsAntiAimModeStand);
		body_fake_stand_custom_double_amount.AddShowCallback(callbacks::HasStandYaw);
		body_fake_stand_custom_double_amount.AddShowCallback(callbacks::IsStandBodyCustom);
		body_fake_stand_custom_double_amount.AddShowCallback(callbacks::IsStandBodyDouble);
		RegisterElement(&body_fake_stand_custom_double_amount);

		// walk.
		pitch_walk.setup(XOR("pitch"), XOR("pitch_walk"), { XOR("off"), XOR("down"), XOR("up"), XOR("random"), XOR("ideal") });
		pitch_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		RegisterElement(&pitch_walk);

		yaw_walk.setup(XOR("yaw"), XOR("yaw_walk"), { XOR("off"), XOR("direction"), XOR("jitter"), XOR("rotate"), XOR("random") });
		yaw_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		RegisterElement(&yaw_walk);

		jitter_range_walk.setup("", XOR("jitter_range_walk"), 1.f, 180.f, false, 0, 45.f, 5.f, XOR(L"°"));
		jitter_range_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		jitter_range_walk.AddShowCallback(callbacks::IsWalkYawJitter);
		RegisterElement(&jitter_range_walk);

		rot_range_walk.setup("", XOR("rot_range_walk"), 0.f, 360.f, false, 0, 360.f, 5.f, XOR(L"°"));
		rot_range_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		rot_range_walk.AddShowCallback(callbacks::IsWalkYawRotate);
		RegisterElement(&rot_range_walk);

		rot_speed_walk.setup("", XOR("rot_speed_walk"), 1.f, 100.f, false, 0, 10.f, 1.f, XOR(L"%"));
		rot_speed_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		rot_speed_walk.AddShowCallback(callbacks::IsWalkYawRotate);
		RegisterElement(&rot_speed_walk);

		rand_update_walk.setup("", XOR("rand_update_walk"), 0.f, 1.f, false, 1, 0.f, 0.1f);
		rand_update_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		rand_update_walk.AddShowCallback(callbacks::IsWalkYawRandom);
		RegisterElement(&rand_update_walk);

		dir_walk.setup(XOR("direction"), XOR("dir_walk"), { XOR("auto"), XOR("backwards"), XOR("left"), XOR("right"), XOR("custom") });
		dir_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		dir_walk.AddShowCallback(callbacks::WalkHasYaw);
		RegisterElement(&dir_walk);

		dir_time_walk.setup("", XOR("dir_time_walk"), 0.f, 10.f, false, 0, 0.f, 1.f, XOR(L"s"));
		dir_time_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		dir_time_walk.AddShowCallback(callbacks::WalkHasYaw);
		dir_time_walk.AddShowCallback(callbacks::IsWalkDirAuto);
		RegisterElement(&dir_time_walk);

		dir_custom_walk.setup("", XOR("dir_custom_walk"), -180.f, 180.f, false, 0, 0.f, 5.f, XOR(L"°"));
		dir_custom_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		dir_custom_walk.AddShowCallback(callbacks::WalkHasYaw);
		dir_custom_walk.AddShowCallback(callbacks::IsWalkDirCustom);
		RegisterElement(&dir_custom_walk);

		base_angle_walk.setup(XOR("base angle"), XOR("base_angle_walk"), { XOR("off"), XOR("static"), XOR("away crosshair"), XOR("away distance") });
		base_angle_walk.AddShowCallback(callbacks::IsAntiAimModeWalk);
		base_angle_walk.AddShowCallback(callbacks::WalkHasYaw);
		RegisterElement(&base_angle_walk);

		// air.
		pitch_air.setup(XOR("pitch"), XOR("pitch_air"), { XOR("off"), XOR("down"), XOR("up"), XOR("random"), XOR("ideal") });
		pitch_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		RegisterElement(&pitch_air);

		yaw_air.setup(XOR("yaw"), XOR("yaw_air"), { XOR("off"), XOR("direction"), XOR("jitter"), XOR("rotate"), XOR("random") });
		yaw_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		RegisterElement(&yaw_air);

		jitter_range_air.setup("", XOR("jitter_range_air"), 1.f, 180.f, false, 0, 45.f, 5.f, XOR(L"°"));
		jitter_range_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		jitter_range_air.AddShowCallback(callbacks::IsAirYawJitter);
		RegisterElement(&jitter_range_air);

		rot_range_air.setup("", XOR("rot_range_air"), 0.f, 360.f, false, 0, 360.f, 5.f, XOR(L"°"));
		rot_range_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		rot_range_air.AddShowCallback(callbacks::IsAirYawRotate);
		RegisterElement(&rot_range_air);

		rot_speed_air.setup("", XOR("rot_speed_air"), 1.f, 100.f, false, 0, 10.f, 1.f, XOR(L"%"));
		rot_speed_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		rot_speed_air.AddShowCallback(callbacks::IsAirYawRotate);
		RegisterElement(&rot_speed_air);

		rand_update_air.setup("", XOR("rand_update_air"), 0.f, 1.f, false, 1, 0.f, 0.1f);
		rand_update_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		rand_update_air.AddShowCallback(callbacks::IsAirYawRandom);
		RegisterElement(&rand_update_air);

		dir_air.setup(XOR("direction"), XOR("dir_air"), { XOR("auto"), XOR("backwards"), XOR("left"), XOR("right"), XOR("custom") });
		dir_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		dir_air.AddShowCallback(callbacks::AirHasYaw);
		RegisterElement(&dir_air);

		dir_time_air.setup("", XOR("dir_time_air"), 0.f, 10.f, false, 0, 0.f, 1.f, XOR(L"s"));
		dir_time_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		dir_time_air.AddShowCallback(callbacks::AirHasYaw);
		dir_time_air.AddShowCallback(callbacks::IsAirDirAuto);
		RegisterElement(&dir_time_air);

		dir_custom_air.setup("", XOR("dir_custom_air"), -180.f, 180.f, false, 0, 0.f, 5.f, XOR(L"°"));
		dir_custom_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		dir_custom_air.AddShowCallback(callbacks::AirHasYaw);
		dir_custom_air.AddShowCallback(callbacks::IsAirDirCustom);
		RegisterElement(&dir_custom_air);

		base_angle_air.setup(XOR("base angle"), XOR("base_angle_air"), { XOR("off"), XOR("static"), XOR("away crosshair"), XOR("away distance") });
		base_angle_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		base_angle_air.AddShowCallback(callbacks::AirHasYaw);
		RegisterElement(&base_angle_air);

		body_fake_air.setup(XOR("fake body"), XOR("body_fake_air"), { XOR("off"), XOR("left"), XOR("right"), XOR("opposite") });
		body_fake_air.AddShowCallback(callbacks::IsAntiAimModeAir);
		body_fake_air.AddShowCallback(callbacks::AirHasYaw);
		RegisterElement(&body_fake_air);

		// col2.
		fake_yaw.setup(XOR("fake yaw"), XOR("fake_yaw"), { XOR("off"), XOR("default"), XOR("relative"), XOR("jitter"), XOR("rotate"), XOR("random"), XOR("local view") });
		RegisterElement(&fake_yaw, 1);

		fake_relative.setup("", XOR("fake_relative"), -90.f, 90.f, false, 0, 0.f, 5.f, XOR(L"°"));
		fake_relative.AddShowCallback(callbacks::IsFakeAntiAimRelative);
		RegisterElement(&fake_relative, 1);

		fake_jitter_range.setup("", XOR("fake_jitter_range"), 1.f, 90.f, false, 0, 0.f, 5.f, XOR(L"°"));
		fake_jitter_range.AddShowCallback(callbacks::IsFakeAntiAimJitter);
		RegisterElement(&fake_jitter_range, 1);

		lag_enable.setup(XOR("fake-lag"), XOR("lag_enable"));
		RegisterElement(&lag_enable, 1);

		lag_active.setup("", XOR("lag_active"), { XOR("move"), XOR("air"), XOR("crouch") }, false);
		lag_active.AddShowCallback( callbacks::IsFakeLagOn );
		RegisterElement(&lag_active, 1);

		lag_mode.setup("", XOR("lag_mode"), { XOR("max"), XOR("break"), XOR("random"), XOR("break step") }, false);
		lag_mode.AddShowCallback( callbacks::IsFakeLagOn );
		RegisterElement(&lag_mode, 1);

		lag_limit.setup(XOR("limit"), XOR("lag_limit"), 2, 16, true, 0, 2, 1.f);
		lag_limit.AddShowCallback( callbacks::IsFakeLagOn );
		RegisterElement(&lag_limit, 1);

		lag_extend.setup(XOR("extend lag"), XOR("lag_extend"));
		lag_extend.AddShowCallback( callbacks::IsFakeLagOn );
		lag_extend.SetToggleCallback(callbacks::ToggleExtendedLag);
		RegisterElement(&lag_extend, 1);

		lag_extended_limit.setup("", XOR("lag_extended_limit"), 2, 16, false, 0, 2, 1.f);
		lag_extended_limit.AddShowCallback( callbacks::IsFakeLagOn );
		RegisterElement(&lag_extended_limit, 1);

		lag_land.setup(XOR("fake-lag on land"), XOR("lag_land"));
		RegisterElement(&lag_land, 1);

		lag_revolver.setup(XOR("disable on revolver"), XOR("lag_revolver"));
		RegisterElement(&lag_revolver, 1);

		anti_lastmove.setup(XOR("anti-lastmove"), XOR("anti_lastmove"));
		RegisterElement( &anti_lastmove, 1 );
	}
};

class PlayersTab : public Tab {
public:
	MultiDropdown box;
	Colorpicker   box_enemy;
	Colorpicker   box_friendly;
	Colorpicker   box_local;
	MultiDropdown name;
	Colorpicker   name_enemy;
	Colorpicker   name_friendly;
	Colorpicker   name_local;
	MultiDropdown health;
	Checkbox			   health_color_override;
	Colorpicker		   health_color;
	MultiDropdown flags_enemy;
	MultiDropdown flags_friendly;
	MultiDropdown weapon;
	Dropdown      weapon_mode;
	Checkbox      ammo;
	Colorpicker   ammo_enemy;
	Colorpicker   ammo_friendly;
	Colorpicker   ammo_local;
	Checkbox      lby_update;
	Colorpicker   lby_update_color;
	Checkbox		 shot_matrix;
	Colorpicker   shot_matrix_color;
	Slider			 shot_matrix_time;
	Checkbox      offscreen;
	Colorpicker   offscreen_color;
	Slider			 offscreen_dist;
	Slider			 offscreen_size;
	MultiDropdown glow;
	Colorpicker   glow_enemy;
	Colorpicker   glow_friendly;
	Colorpicker   glow_local;
	Slider		     glow_blend;
	Dropdown	 glow_style;
	MultiDropdown skeleton;
	Colorpicker   skeleton_enemy;
	Colorpicker   skeleton_friendly;
	Colorpicker   skeleton_local;
	Checkbox      dormant;

	// col2.
	Dropdown		  chams_selection;
	
	// enemy subtab.
	Checkbox			  chams_enemy_visible;
	Dropdown		  chams_enemy_visible_material;
	Colorpicker		  chams_enemy_visible_color;
	Slider				  chams_enemy_visible_blend;
	Checkbox			  chams_enemy_visible_overlay;
	Dropdown		  chams_enemy_visible_overlay_material;
	Colorpicker		  chams_enemy_visible_overlay_color;
	Slider				  chams_enemy_visible_overlay_blend;
	Checkbox			  chams_enemy_invisible;
	Dropdown		  chams_enemy_invisible_material;
	Colorpicker		  chams_enemy_invisible_color;
	Slider				  chams_enemy_invisible_blend;
	Checkbox			  chams_enemy_history;
	Dropdown		  chams_enemy_history_material;
	Colorpicker		  chams_enemy_history_color;
	Slider				  chams_enemy_history_blend;

	// friendly subtab.
	Checkbox			  chams_friendly_visible;
	Dropdown		  chams_friendly_visible_material;
	Colorpicker		  chams_friendly_visible_color;
	Slider				  chams_friendly_visible_blend;
	Checkbox			  chams_friendly_visible_overlay;
	Dropdown		  chams_friendly_visible_overlay_material;
	Colorpicker		  chams_friendly_visible_overlay_color;
	Slider				  chams_friendly_visible_overlay_blend;
	Checkbox			  chams_friendly_invisible;
	Dropdown		  chams_friendly_invisible_material;
	Colorpicker		  chams_friendly_invisible_color;
	Slider				  chams_friendly_invisible_blend;

	// local subtab.
	Checkbox			  chams_local_real;
	Dropdown		  chams_local_real_material;
	Colorpicker		  chams_local_real_color;
	Slider				  chams_local_real_blend;
	Checkbox			  chams_local_real_overlay;
	Dropdown		  chams_local_real_overlay_material;
	Colorpicker		  chams_local_real_overlay_color;
	Slider				  chams_local_real_overlay_blend;
	Checkbox		      chams_local_real_blend_scope;
	Checkbox			  chams_local_fake;
	Dropdown		  chams_local_fake_material;
	Colorpicker		  chams_local_fake_color;
	Slider				  chams_local_fake_blend;
	Checkbox			  chams_local_fake_overlay;
	Dropdown		  chams_local_fake_overlay_material;
	Colorpicker		  chams_local_fake_overlay_color;
	Slider        		  chams_local_fake_overlay_blend;

public:
	void init() {
		SetTitle(XOR("players"));

		box.setup(XOR("boxes"), XOR("box"), { XOR("enemy"), XOR("friendly"), XOR("local") });
		RegisterElement(&box);

		box_enemy.setup(XOR("box enemy color"), XOR("box_enemy"), { 150, 200, 60 });
		box_enemy.AddShowCallback( callbacks::IsBoxEnemy );
		RegisterElement(&box_enemy);

		box_friendly.setup(XOR("box friendly color"), XOR("box_friendly"), { 255, 200, 0 });
		box_friendly.AddShowCallback( callbacks::IsBoxFriendly );
		RegisterElement(&box_friendly);
		
		box_local.setup(XOR("box local color"), XOR("box_local"), { 225, 69, 0 });
		box_local.AddShowCallback( callbacks::IsBoxLocal );
		RegisterElement(&box_local);

		name.setup(XOR("name"), XOR("name"), { XOR("enemy"), XOR("friendly"), XOR("local") });
		RegisterElement(&name);

		name_enemy.setup(XOR("name enemy color"), XOR("name_enemy"), colors::white);
		name_enemy.AddShowCallback( callbacks::IsNameEnemy );
		RegisterElement(&name_enemy);

		name_friendly.setup(XOR("name friendly color"), XOR("name_friendly"), colors::white);
		name_friendly.AddShowCallback( callbacks::IsNameFriendly );
		RegisterElement(&name_friendly);

		name_local.setup(XOR("name local color"), XOR("name_local"), colors::white);
		name_local.AddShowCallback( callbacks::IsNameLocal );
		RegisterElement(&name_local);

		health.setup(XOR("health"), XOR("health"), { XOR("enemy"), XOR("friendly"), XOR("local") });
		RegisterElement(&health);

		health_color_override.setup( XOR("override color"), XOR("health_color_override") );
		health_color_override.AddShowCallback( callbacks::IsHealthOn );
		RegisterElement( &health_color_override );

		health_color.setup( XOR("color"), XOR("health_color"), colors::burgundy );
		health_color.AddShowCallback( callbacks::IsHealthOverrideOn );
		RegisterElement( &health_color );

		flags_enemy.setup(XOR("flags enemy"), XOR("flags_enemy"), { XOR("money"), XOR("armor"), XOR("scoped"), XOR("flashed"), XOR("reloading"), XOR("bomb"), XOR("lc") });
		RegisterElement(&flags_enemy);

		flags_friendly.setup(XOR("flags friendly"), XOR("flags_friendly"), { XOR("money"), XOR("armor"), XOR("scoped"), XOR("flashed"), XOR("reloading"), XOR("bomb") });
		RegisterElement(&flags_friendly);

		weapon.setup(XOR("weapon"), XOR("weapon"), { XOR("enemy"), XOR("friendly"), XOR("local") });
		RegisterElement(&weapon);

		weapon_mode.setup("", XOR("weapon_mode"), { XOR("text"), XOR("icon") }, false);
		weapon_mode.AddShowCallback( callbacks::IsWeaponOn );
		RegisterElement(&weapon_mode);

		ammo.setup(XOR("ammo"), XOR("ammo"));
		ammo.AddShowCallback( callbacks::IsWeaponOn );
		RegisterElement(&ammo);

		ammo_enemy.setup(XOR("ammo enemy color"), XOR("ammo_enemy"), colors::light_blue);
		ammo_enemy.AddShowCallback( callbacks::IsWeaponEnemy );
		RegisterElement(&ammo_enemy);
		
		ammo_friendly.setup(XOR("ammo friendly color"), XOR("ammo_friendly"), colors::burgundy);
		ammo_friendly.AddShowCallback( callbacks::IsWeaponFriendly );
		RegisterElement(&ammo_friendly);
		
		ammo_local.setup(XOR("ammo local color"), XOR("ammo_local"), colors::white);
		ammo_local.AddShowCallback( callbacks::IsWeaponLocal );
		RegisterElement(&ammo_local);

		lby_update.setup(XOR("lby update"), XOR("lby_update"));
		RegisterElement(&lby_update);

		lby_update_color.setup(XOR("color"), XOR("lby_update_color"), colors::orange);
		lby_update_color.AddShowCallback( callbacks::IsLBYUpdateOn );
		RegisterElement(&lby_update_color);

		shot_matrix.setup(XOR("shot matrix"), XOR("shot_matrix"));
		RegisterElement(&shot_matrix);

		shot_matrix_color.setup(XOR("color"), XOR("shot_matrix_color"), colors::white);
		shot_matrix_color.AddShowCallback( callbacks::IsShotMatrixOn );
		RegisterElement(&shot_matrix_color);

		shot_matrix_time.setup( XOR("shot matrix time"), XOR("shot_matrix_time"), 0.1f, 10.f, true, 1, 2.f, 0.1f, XOR(L"s"));
		shot_matrix_time.AddShowCallback( callbacks::IsShotMatrixOn );
		RegisterElement(&shot_matrix_time);

		offscreen.setup(XOR("enemy offscreen esp"), XOR("offscreen"));
		RegisterElement(&offscreen);

		offscreen_color.setup(XOR("offscreen esp color"), XOR("offscreen_color"), colors::white);
		offscreen_color.AddShowCallback(callbacks::IsOffscreenOn);
		RegisterElement(&offscreen_color);

		offscreen_dist.setup("", XOR("offscreen_distance"), 30.f, 300.f, false, 0, 250.f, 5.f);
		offscreen_dist.AddShowCallback(callbacks::IsOffscreenOn);
		RegisterElement(&offscreen_dist);

		offscreen_size.setup("", XOR("offscreen_size"), 6.f, 18.f, false, 0.f, 12.f);
		offscreen_size.AddShowCallback(callbacks::IsOffscreenOn);
		RegisterElement(&offscreen_size);

		glow.setup(XOR("glow"), XOR("glow"), { XOR("enemy"), XOR("friendly"), XOR("local") });
		RegisterElement(&glow);

		glow_enemy.setup(XOR("enemy color"), XOR("glow_enemy"), { 150, 200, 60 });
		glow_enemy.AddShowCallback( callbacks::IsGlowEnemy );
		RegisterElement(&glow_enemy);

		glow_friendly.setup(XOR("friendly color"), XOR("glow_friendly"), { 150, 200, 60 });
		glow_friendly.AddShowCallback(callbacks::IsGlowFriendly);
		RegisterElement(&glow_friendly);
		
		glow_local.setup(XOR("local color"), XOR("glow_local"), { 150, 200, 60 });
		glow_local.AddShowCallback(callbacks::IsGlowLocal);
		RegisterElement(&glow_local);

		glow_blend.setup("", XOR("glow_blend"), 10.f, 100.f, false, 0, 60.f, 1.f, XOR(L"%"));
		RegisterElement(&glow_blend);

		glow_style.setup( XOR("glow style"), XOR("glow_style"), {  XOR("outline"), XOR("model"), XOR("inline"), XOR("pulse") });
		RegisterElement(&glow_style);

		skeleton.setup(XOR("skeleton"), XOR("skeleton"), { XOR("enemy"), XOR("friendly"), XOR("local") });
		RegisterElement(&skeleton);

		skeleton_enemy.setup(XOR("enemy color"), XOR("skeleton_enemy"), { 255, 255, 255 });
		skeleton_enemy.AddShowCallback(callbacks::IsSkeletonEnemy);
		RegisterElement(&skeleton_enemy);

		skeleton_friendly.setup(XOR("friendly color"), XOR("skeleton_friendly"), { 255, 255, 255 });
		skeleton_friendly.AddShowCallback(callbacks::IsSkeletonFriendly);
		RegisterElement(&skeleton_friendly);

		skeleton_local.setup(XOR("local color"), XOR("skeleton_local"), { 255, 255, 255 });
		skeleton_local.AddShowCallback(callbacks::IsSkeletonLocal);
		RegisterElement(&skeleton_local);

		dormant.setup(XOR("dormant enemies"), XOR("dormant"));
		RegisterElement(&dormant);

		// col2.
		chams_selection.setup( XOR("selection"), XOR("chams_selection"), { XOR("enemy"), XOR("friendly"), XOR("local") });
		RegisterElement(&chams_selection, 1);

		// enemy subtab.
		chams_enemy_visible.setup(XOR("visible"), XOR("chams_enemy_visible"));
		chams_enemy_visible.AddShowCallback(callbacks::IsChamsEnemy);
		RegisterElement(&chams_enemy_visible, 1);

		chams_enemy_visible_material.setup(XOR("material"), XOR("chams_enemy_visible_material"), { XOR("regular"), XOR("flat"), XOR("palm") });
		chams_enemy_visible_material.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_visible_material.AddShowCallback(callbacks::IsChamsEnemyVisible);
		RegisterElement(&chams_enemy_visible_material, 1);

		chams_enemy_visible_color.setup(XOR("color"), XOR("chams_enemy_visible_color"), { 150, 200, 60 });
		chams_enemy_visible_color.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_visible_color.AddShowCallback(callbacks::IsChamsEnemyVisible);
		RegisterElement(&chams_enemy_visible_color, 1);

		chams_enemy_visible_blend.setup("", XOR("chams_enemy_visible_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		chams_enemy_visible_blend.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_visible_blend.AddShowCallback(callbacks::IsChamsEnemyVisible);
		RegisterElement(&chams_enemy_visible_blend, 1);

		chams_enemy_visible_overlay.setup(XOR("overlay"), XOR("chams_enemy_visible_overlay"));
		chams_enemy_visible_overlay.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_visible_overlay.AddShowCallback(callbacks::IsChamsEnemyVisible);
		RegisterElement(&chams_enemy_visible_overlay, 1);

		chams_enemy_visible_overlay_material.setup(XOR("material"), XOR("chams_enemy_visible_overlay_material"), { XOR("pulse"), XOR("glow"), XOR("animation"), XOR("circle"), XOR("liner") });
		chams_enemy_visible_overlay_material.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_visible_overlay_material.AddShowCallback(callbacks::IsChamsEnemyVisible);
		RegisterElement(&chams_enemy_visible_overlay_material, 1);

		chams_enemy_visible_overlay_color.setup(XOR("tint color"), XOR("chams_enemy_visible_overlay_color"), colors::white);
		chams_enemy_visible_overlay_color.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_visible_overlay_color.AddShowCallback(callbacks::IsChamsEnemyVisible);
		RegisterElement(&chams_enemy_visible_overlay_color, 1);

		chams_enemy_visible_overlay_blend.setup("", XOR("chams_enemy_visible_overlay_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		chams_enemy_visible_overlay_blend.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_visible_overlay_blend.AddShowCallback(callbacks::IsChamsEnemyVisible);
		RegisterElement(&chams_enemy_visible_overlay_blend, 1);


		chams_enemy_invisible.setup(XOR("invisible"), XOR("chams_enemy_invisible"));
		chams_enemy_invisible.AddShowCallback(callbacks::IsChamsEnemy);
		RegisterElement(&chams_enemy_invisible, 1);

		chams_enemy_invisible_material.setup(XOR("material"), XOR("chams_enemy_invisible_material"), { XOR("regular"), XOR("flat"), XOR("palm") });
		chams_enemy_invisible_material.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_invisible_material.AddShowCallback(callbacks::IsChamsEnemyInvisible);
		RegisterElement(&chams_enemy_invisible_material, 1);

		chams_enemy_invisible_color.setup(XOR("color"), XOR("chams_enemy_invisible_color"), { 150, 200, 60 });
		chams_enemy_invisible_color.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_invisible_color.AddShowCallback(callbacks::IsChamsEnemyInvisible);
		RegisterElement(&chams_enemy_invisible_color, 1);

		chams_enemy_invisible_blend.setup("", XOR("chams_enemy_invisible_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		chams_enemy_invisible_blend.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_invisible_blend.AddShowCallback(callbacks::IsChamsEnemyInvisible);
		RegisterElement(&chams_enemy_invisible_blend, 1);


		chams_enemy_history.setup(XOR("history"), XOR("chams_enemy_history"));
		chams_enemy_history.AddShowCallback(callbacks::IsChamsEnemy);
		RegisterElement(&chams_enemy_history, 1);

		chams_enemy_history_material.setup(XOR("material"), XOR("chams_enemy_history_material"), { XOR("regular"), XOR("flat"), XOR("palm") });
		chams_enemy_history_material.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_history_material.AddShowCallback(callbacks::IsChamsEnemyHistory);
		RegisterElement(&chams_enemy_history_material, 1);

		chams_enemy_history_color.setup(XOR("color"), XOR("chams_enemy_history_color"), { 150, 200, 60 });
		chams_enemy_history_color.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_history_color.AddShowCallback(callbacks::IsChamsEnemyHistory);
		RegisterElement(&chams_enemy_history_color, 1);

		chams_enemy_history_blend.setup("", XOR("chams_enemy_history_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		chams_enemy_history_blend.AddShowCallback(callbacks::IsChamsEnemy);
		chams_enemy_history_blend.AddShowCallback(callbacks::IsChamsEnemyHistory);
		RegisterElement(&chams_enemy_history_blend, 1);

		// friendly subtab.
		chams_friendly_visible.setup(XOR("visible"), XOR("chams_friendly_visible"));
		chams_friendly_visible.AddShowCallback(callbacks::IsChamsFriendly);
		RegisterElement(&chams_friendly_visible, 1);

		chams_friendly_visible_material.setup(XOR("material"), XOR("chams_friendly_visible_material"), { XOR("regular"), XOR("flat"), XOR("palm") });
		chams_friendly_visible_material.AddShowCallback(callbacks::IsChamsFriendly);
		chams_friendly_visible_material.AddShowCallback(callbacks::IsChamsFriendlyVisible);
		RegisterElement(&chams_friendly_visible_material, 1);

		chams_friendly_visible_color.setup(XOR("color"), XOR("chams_friendly_visible_color"), { 150, 200, 60 });
		chams_friendly_visible_color.AddShowCallback(callbacks::IsChamsFriendly);
		chams_friendly_visible_color.AddShowCallback(callbacks::IsChamsFriendlyVisible);
		RegisterElement(&chams_friendly_visible_color, 1);

		chams_friendly_visible_blend.setup("", XOR("chams_friendly_visible_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		chams_friendly_visible_blend.AddShowCallback(callbacks::IsChamsFriendly);
		chams_friendly_visible_blend.AddShowCallback(callbacks::IsChamsFriendlyVisible);
		RegisterElement(&chams_friendly_visible_blend, 1);

		chams_friendly_visible_overlay.setup(XOR("overlay"), XOR("chams_friendly_visible_overlay"));
		chams_friendly_visible_overlay.AddShowCallback(callbacks::IsChamsFriendly);
		chams_friendly_visible_overlay.AddShowCallback(callbacks::IsChamsFriendlyVisible);
		RegisterElement(&chams_friendly_visible_overlay, 1);

		chams_friendly_visible_overlay_material.setup(XOR("material"), XOR("chams_friendly_visible_overlay_material"), { XOR("pulse"), XOR("glow"), XOR("animation"), XOR("circle"), XOR("liner") });
		chams_friendly_visible_overlay_material.AddShowCallback(callbacks::IsChamsFriendly);
		chams_friendly_visible_overlay_material.AddShowCallback(callbacks::IsChamsFriendlyVisible);
		RegisterElement(&chams_friendly_visible_overlay_material, 1);

		chams_friendly_visible_overlay_color.setup(XOR("tint color"), XOR("chams_friendly_visible_overlay_color"), colors::white);
		chams_friendly_visible_overlay_color.AddShowCallback(callbacks::IsChamsFriendly);
		chams_friendly_visible_overlay_color.AddShowCallback(callbacks::IsChamsFriendlyVisible);
		RegisterElement(&chams_friendly_visible_overlay_color, 1);

		chams_friendly_visible_overlay_blend.setup("", XOR("chams_friendly_visible_overlay_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		chams_friendly_visible_overlay_blend.AddShowCallback(callbacks::IsChamsFriendly);
		chams_friendly_visible_overlay_blend.AddShowCallback(callbacks::IsChamsFriendlyVisible);
		RegisterElement(&chams_friendly_visible_overlay_blend, 1);


		chams_friendly_invisible.setup(XOR("invisible"), XOR("chams_friendly_invisible"));
		chams_friendly_invisible.AddShowCallback(callbacks::IsChamsFriendly);
		RegisterElement(&chams_friendly_invisible, 1);

		chams_friendly_invisible_material.setup(XOR("material"), XOR("chams_friendly_invisible_material"), { XOR("regular"), XOR("flat"), XOR("palm") });
		chams_friendly_invisible_material.AddShowCallback(callbacks::IsChamsFriendly);
		chams_friendly_invisible_material.AddShowCallback(callbacks::IsChamsFriendlyInvisible);
		RegisterElement(&chams_friendly_invisible_material, 1);

		chams_friendly_invisible_color.setup(XOR("color"), XOR("chams_friendly_invisible_color"), { 150, 200, 60 });
		chams_friendly_invisible_color.AddShowCallback(callbacks::IsChamsFriendly);
		chams_friendly_invisible_color.AddShowCallback(callbacks::IsChamsFriendlyInvisible);
		RegisterElement(&chams_friendly_invisible_color, 1);

		chams_friendly_invisible_blend.setup("", XOR("chams_friendly_invisible_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		chams_friendly_invisible_blend.AddShowCallback(callbacks::IsChamsFriendly);
		chams_friendly_invisible_blend.AddShowCallback(callbacks::IsChamsFriendlyInvisible);
		RegisterElement(&chams_friendly_invisible_blend, 1);

		// local subtab.
		chams_local_real.setup(XOR("real"), XOR("chams_local_real"));
		chams_local_real.AddShowCallback(callbacks::IsChamsLocal);
		RegisterElement(&chams_local_real, 1);

		chams_local_real_material.setup(XOR("material"), XOR("chams_local_real_material"), { XOR("regular"), XOR("flat"), XOR("palm") });
		chams_local_real_material.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_real_material.AddShowCallback(callbacks::IsChamsLocalReal);
		RegisterElement(&chams_local_real_material, 1);

		chams_local_real_color.setup(XOR("color"), XOR("chams_local_real_color"), { 150, 200, 60 });
		chams_local_real_color.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_real_color.AddShowCallback(callbacks::IsChamsLocalReal);
		RegisterElement(&chams_local_real_color, 1);

		chams_local_real_blend.setup("", XOR("chams_local_real_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		chams_local_real_blend.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_real_blend.AddShowCallback(callbacks::IsChamsLocalReal);
		RegisterElement(&chams_local_real_blend, 1);

		chams_local_real_overlay.setup(XOR("real overlay"), XOR("chams_local_real_overlay"));
		chams_local_real_overlay.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_real_overlay.AddShowCallback(callbacks::IsChamsLocalReal);
		RegisterElement(&chams_local_real_overlay, 1);

		chams_local_real_overlay_material.setup(XOR("material"), XOR("chams_local_real_overlay_material"), { XOR("pulse"), XOR("glow"), XOR("animation"), XOR("circle"), XOR("liner") });
		chams_local_real_overlay_material.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_real_overlay_material.AddShowCallback(callbacks::IsChamsLocalReal);
		RegisterElement(&chams_local_real_overlay_material, 1);

		chams_local_real_overlay_color.setup(XOR("tint color"), XOR("chams_local_real_overlay_color"), colors::white);
		chams_local_real_overlay_color.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_real_overlay_color.AddShowCallback(callbacks::IsChamsLocalReal);
		RegisterElement(&chams_local_real_overlay_color, 1);

		chams_local_real_overlay_blend.setup("", XOR("chams_local_real_overlay_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		chams_local_real_overlay_blend.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_real_overlay_blend.AddShowCallback(callbacks::IsChamsLocalReal);
		RegisterElement(&chams_local_real_overlay_blend, 1);

		chams_local_real_blend_scope.setup(XOR("blend when scoped"), XOR("chams_local_real_blend_scope"));
		chams_local_real_blend_scope.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_real_blend_scope.AddShowCallback(callbacks::IsChamsLocalReal);
		RegisterElement(&chams_local_real_blend_scope, 1);


		chams_local_fake.setup(XOR("fake"), XOR("chams_local_fake"));
		chams_local_fake.AddShowCallback(callbacks::IsChamsLocal);
		RegisterElement(&chams_local_fake, 1);

		chams_local_fake_material.setup(XOR("material"), XOR("chams_local_fake_material"), { XOR("regular"), XOR("flat"), XOR("palm") });
		chams_local_fake_material.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_fake_material.AddShowCallback(callbacks::IsChamsLocalFake);
		RegisterElement(&chams_local_fake_material, 1);

		chams_local_fake_color.setup(XOR("color"), XOR("chams_local_fake_color"), { 150, 200, 60 });
		chams_local_fake_color.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_fake_color.AddShowCallback(callbacks::IsChamsLocalFake);
		RegisterElement(&chams_local_fake_color, 1);

		chams_local_fake_blend.setup("", XOR("chams_local_fake_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		chams_local_fake_blend.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_fake_blend.AddShowCallback(callbacks::IsChamsLocalFake);
		RegisterElement(&chams_local_fake_blend, 1);
	
		chams_local_fake_overlay.setup(XOR("fake overlay"), XOR("chams_local_fake_overlay"));
		chams_local_fake_overlay.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_fake_overlay.AddShowCallback(callbacks::IsChamsLocalFake);
		RegisterElement(&chams_local_fake_overlay, 1);

		chams_local_fake_overlay_material.setup(XOR("material"), XOR("chams_local_fake_overlay_material"), { XOR("pulse"), XOR("glow"), XOR("animation"), XOR("circle"), XOR("liner") });
		chams_local_fake_overlay_material.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_fake_overlay_material.AddShowCallback(callbacks::IsChamsLocalFake);
		RegisterElement(&chams_local_fake_overlay_material, 1);

		chams_local_fake_overlay_color.setup(XOR("tint color"), XOR("chams_local_fake_overlay_color"), colors::white);
		chams_local_fake_overlay_color.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_fake_overlay_color.AddShowCallback(callbacks::IsChamsLocalFake);
		RegisterElement(&chams_local_fake_overlay_color, 1);

		chams_local_fake_overlay_blend.setup("", XOR("chams_local_fake_overlay_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		chams_local_fake_overlay_blend.AddShowCallback(callbacks::IsChamsLocal);
		chams_local_fake_overlay_blend.AddShowCallback(callbacks::IsChamsLocalFake);
		RegisterElement(&chams_local_fake_overlay_blend, 1);
	}
};

class VisualsTab : public Tab {
public:
	Checkbox      items;
	Colorpicker   item_color;
	Checkbox      ammo;
	Colorpicker   ammo_color;
	Checkbox		 ammo_bar;
	Colorpicker	 ammo_bar_color;
	Checkbox	     dropped_c4;
	Colorpicker   dropped_c4_color;
	Checkbox      proj;
	Colorpicker   proj_color;
	MultiDropdown proj_sphere;
	Colorpicker   proj_sphere_color;
	MultiDropdown proj_ground;
	Colorpicker   proj_ground_color;
	MultiDropdown planted_c4;
	Dropdown	  world;
	Checkbox      transparent_props;
	Checkbox      enemy_radar;

	// col2.
	MultiDropdown removals;
	Checkbox      fov;
	Slider        fov_amt;
	Checkbox      fov_scoped;
	Checkbox      viewmodel_fov;
	Slider        viewmodel_fov_amt;
	Checkbox      spectators;
	Checkbox      force_xhair;
	Checkbox      spread_xhair;
	Colorpicker   spread_xhair_col;
	Slider        spread_xhair_blend;
	Checkbox      pen_crosshair;
	MultiDropdown indicators;
	Checkbox           autopeek_indicator;
	Colorpicker        autopeek_color;
	Checkbox           tracers;
	MultiDropdown impact_boxes;
	Colorpicker		   impact_boxes_server;
	Colorpicker		   impact_boxes_client;
	Slider				   impact_boxes_time;
	Checkbox      impact_beams;
	Colorpicker   impact_beams_color;
	Colorpicker   impact_beams_hurt_color;
	Slider        impact_beams_time;
	Keybind       thirdperson;
	Slider		  thirdperson_distance;

public:
	void init() {
		SetTitle(XOR("visuals"));

		items.setup(XOR("dropped weapons"), XOR("items"));
		RegisterElement(&items);

		item_color.setup(XOR("color"), XOR("item_color"), colors::white);
		item_color.AddShowCallback( callbacks::IsItemsOn );
		RegisterElement(&item_color);

		ammo.setup(XOR("dropped weapons ammo"), XOR("ammo"));
		RegisterElement(&ammo);

		ammo_color.setup(XOR("color"), XOR("ammo_color"), colors::white);
		ammo_color.AddShowCallback( callbacks::IsAmmoOn );
		RegisterElement(&ammo_color);

		ammo_bar.setup(XOR("dropped weapons ammo bar"), XOR("ammo_bar"));
		RegisterElement(&ammo_bar);

		ammo_bar_color.setup(XOR("color"), XOR("ammo_bar_color"), colors::burgundy);
		ammo_bar_color.AddShowCallback( callbacks::IsAmmoBarOn );
		RegisterElement(&ammo_bar_color);

		dropped_c4.setup( XOR("dropped c4"), XOR("dropped_c4"));
		RegisterElement(&dropped_c4);

		dropped_c4_color.setup(XOR("color"), XOR("dropped_c4_color"), colors::burgundy);
		dropped_c4_color.AddShowCallback( callbacks::IsDroppedC4On );
		RegisterElement(&dropped_c4_color);

		proj.setup(XOR("projectiles"), XOR("proj"));
		RegisterElement(&proj);

		proj_color.setup(XOR("color"), XOR("proj_color"), colors::white);
		proj_color.AddShowCallback( callbacks::IsProjectileOn );
		RegisterElement(&proj_color);

		proj_sphere.setup(XOR("projectile sphere"), XOR("proj_sphere"), { XOR("frag"), XOR("molly") });
		proj_sphere.AddShowCallback(callbacks::IsProjectileOn);
		RegisterElement(&proj_sphere);

		proj_sphere_color.setup(XOR("color"), XOR("proj_sphere_color"), colors::burgundy);
		proj_sphere_color.AddShowCallback(callbacks::IsProjectileOn);
		proj_sphere_color.AddShowCallback( callbacks::IsProjectileSphereOn );
		RegisterElement(&proj_sphere_color);

		proj_ground.setup(XOR("projectile ground"), XOR("proj_ground"), { XOR("smoke"), XOR("molly") });
		proj_ground.AddShowCallback(callbacks::IsProjectileOn);
		RegisterElement(&proj_ground);

		proj_ground_color.setup(XOR("color"), XOR("proj_ground_color"), colors::burgundy);
		proj_ground_color.AddShowCallback(callbacks::IsProjectileOn);
		proj_ground_color.AddShowCallback(callbacks::IsProjectileGroundOn);
		RegisterElement(&proj_ground_color);

		enemy_radar.setup(XOR("force enemies on radar"), XOR("enemy_radar"));
		RegisterElement(&enemy_radar);

		planted_c4.setup(XOR("planted c4"), XOR("planted_c4"), { XOR("timer (2D)"), XOR("timer (3D)"), XOR("damage (2D)"), XOR("damage (3D)") });
		RegisterElement(&planted_c4);

		world.setup(XOR("world"), XOR("world"), { XOR("off"), XOR("night"), XOR("fullbright") });
		world.SetCallback(Visuals::ModulateWorld);
		RegisterElement(&world);

		transparent_props.setup(XOR("transparent props"), XOR("transparent_props"));
		transparent_props.SetCallback(Visuals::ModulateWorld);
		RegisterElement(&transparent_props);

		// col2.
		removals.setup( XOR("removals"), XOR("removals"), { 
			XOR("visual kick"), 
			XOR("visual punch"), 
			XOR("smoke"),
			XOR("fog"),
			XOR("particles"),
			XOR("post-processing"),
			XOR("flash"),
			XOR("scope"),
			XOR("shadows")
		});
		RegisterElement(&removals, 1);

		fov.setup(XOR("override fov"), XOR("fov"));
		RegisterElement(&fov, 1);

		fov_amt.setup("", XOR("fov_amt"), 60.f, 140.f, false, 0, 90.f, 1.f, XOR(L"°"));
		RegisterElement(&fov_amt, 1);

		fov_scoped.setup(XOR("override fov when scoped"), XOR("fov_scoped"));
		RegisterElement(&fov_scoped, 1);

		viewmodel_fov.setup(XOR("override viewmodel fov"), XOR("viewmodel_fov"));
		RegisterElement(&viewmodel_fov, 1);

		viewmodel_fov_amt.setup("", XOR("viewmodel_fov_amt"), 60.f, 140.f, false, 0, 90.f, 1.f, XOR(L"°"));
		RegisterElement(&viewmodel_fov_amt, 1);

		spectators.setup(XOR("show spectator list"), XOR("spectators"));
		RegisterElement(&spectators, 1);

		force_xhair.setup(XOR("force crosshair"), XOR("force_xhair"));
		RegisterElement(&force_xhair, 1);

		spread_xhair.setup(XOR("visualize spread"), XOR("spread_xhair"));
		RegisterElement(&spread_xhair, 1);

		spread_xhair_col.setup(XOR("visualize spread color"), XOR("spread_xhair_col"), colors::burgundy);
		RegisterElement(&spread_xhair_col, 1);

		spread_xhair_blend.setup("", XOR("spread_xhair_blend"), 10.f, 100.f, false, 0, 100.f, 1.f, XOR(L"%"));
		RegisterElement(&spread_xhair_blend, 1);

		pen_crosshair.setup(XOR("penetration crosshair"), XOR("pen_xhair"));
		RegisterElement(&pen_crosshair, 1);

		indicators.setup(XOR("indicators"), XOR("indicators"), { XOR("lby"), XOR("lag compensation"), XOR("fake latency"), XOR("fake lag extended"), XOR("minimal damage") });
		RegisterElement(&indicators, 1);

		autopeek_indicator.setup(XOR("visualize autopeek"), XOR("autopeek_indicator"));
		RegisterElement(&autopeek_indicator, 1);

		autopeek_color.setup(XOR("color"), XOR("autopeek_color"), colors::white);
		autopeek_color.AddShowCallback(callbacks::IsAutopeekVisualizeOn);
		RegisterElement(&autopeek_color, 1);

		tracers.setup(XOR("grenade simulation"), XOR("tracers"));
		RegisterElement(&tracers, 1);

		impact_boxes.setup(XOR("impact boxes"), XOR("impact_boxes"), { XOR("server"), XOR("client") });
		RegisterElement(&impact_boxes, 1);

		impact_boxes_server.setup( XOR("server color"), XOR("impact_boxes_server"), colors::blue ); 
		impact_boxes_server.AddShowCallback(callbacks::IsImpactBoxesServer);
		RegisterElement(&impact_boxes_server, 1);

		impact_boxes_client.setup(XOR("client color"), XOR("impact_boxes_client"), colors::red );
		impact_boxes_client.AddShowCallback(callbacks::IsImpactBoxesClient);
		RegisterElement(&impact_boxes_client, 1);

		impact_boxes_time.setup(XOR("impact boxes time"), XOR("impact_boxes_time"), 1.f, 10.f, true, 0, 1.f, 1.f, XOR(L"s"));
		impact_boxes_time.AddShowCallback(callbacks::IsImpactBoxesOn);
		RegisterElement(&impact_boxes_time, 1);

		impact_beams.setup(XOR("impact beams"), XOR("impact_beams"));
		RegisterElement(&impact_beams, 1);

		impact_beams_color.setup(XOR("impact beams color"), XOR("impact_beams_color"), colors::light_blue);
		impact_beams_color.AddShowCallback( callbacks::IsImpactBeamsOn );
		RegisterElement(&impact_beams_color, 1);

		impact_beams_hurt_color.setup(XOR("impact beams hurt color"), XOR("impact_beams_hurt_color"), colors::red);
		impact_beams_hurt_color.AddShowCallback( callbacks::IsImpactBeamsOn );
		RegisterElement(&impact_beams_hurt_color, 1);

		impact_beams_time.setup(XOR("impact beams time"), XOR("impact_beams_time"), 1.f, 10.f, true, 0, 1.f, 1.f, XOR(L"s"));
		impact_beams_time.AddShowCallback( callbacks::IsImpactBeamsOn );
		RegisterElement(&impact_beams_time, 1);

		thirdperson.setup(XOR("thirdperson"), XOR("thirdperson"));
		thirdperson.SetToggleCallback(callbacks::ToggleThirdPerson);
		RegisterElement(&thirdperson, 1);

		thirdperson_distance.setup(XOR(" "), XOR("thirdperson_distance"), 50.f, 300.f, false, 0, 150.f, 5.f, XOR(L"°"));
		RegisterElement(&thirdperson_distance, 1);
	}
};

class MovementTab : public Tab {
public:
	Checkbox bhop;
	Checkbox autostrafe;
	Keybind  snakestrafe;
	Slider   snake_freq;
	Slider   snake_dist;
	Checkbox airduck;
	Checkbox duck_delay;

	Keybind  fakewalk;
	Keybind  autopeek;
	Keybind  autostop;
	Checkbox autostop_always_on;

public:
	void init() {
		SetTitle(XOR("movement"));

		// col1.
		bhop.setup(XOR("automatic jump"), XOR("bhop"));
		RegisterElement(&bhop);

		autostrafe.setup(XOR("automatic strafe"), XOR("autostrafe"));
		RegisterElement(&autostrafe);

		snakestrafe.setup(XOR("snake-strafe"), XOR("snakestrafe"));
		RegisterElement(&snakestrafe);

		snake_freq.setup("", XOR("snake_freq"), 1.f, 100.f, false, 0, 50.f, 0.5f, XOR(L"hz"));
		RegisterElement(&snake_freq);

		snake_dist.setup("", XOR("snake_dist"), 1.f, 100.f, false, 0, 20.f, 0.5f, XOR(L"%"));
		RegisterElement(&snake_dist);

		airduck.setup(XOR("duck in air"), XOR("airduck"));
		RegisterElement(&airduck);

		duck_delay.setup(XOR("remove duck delay"), XOR("duck_delay"));
		RegisterElement(&duck_delay);

		// col2.
		fakewalk.setup(XOR("fake-walk"), XOR("fakewalk"));
		RegisterElement(&fakewalk, 1);

		autopeek.setup(XOR("automatic peek"), XOR("autopeek"));
		RegisterElement(&autopeek, 1);

		autostop_always_on.setup(XOR("automatic stop always on"), XOR("auto_stop_always"));
		RegisterElement(&autostop_always_on, 1);

		autostop.setup(XOR("automatic stop"), XOR("autostop"));
		autostop.AddShowCallback(callbacks::AUTO_STOP);
		RegisterElement(&autostop, 1);
	}
};

class SkinsTab : public Tab {
public:
	Checkbox enable;

	Edit     id_deagle;
	Checkbox stattrak_deagle;
	Slider   quality_deagle;
	Slider	 seed_deagle;

	Edit     id_elite;
	Checkbox stattrak_elite;
	Slider   quality_elite;
	Slider	 seed_elite;

	Edit     id_fiveseven;
	Checkbox stattrak_fiveseven;
	Slider   quality_fiveseven;
	Slider	 seed_fiveseven;

	Edit     id_glock;
	Checkbox stattrak_glock;
	Slider   quality_glock;
	Slider	 seed_glock;

	Edit     id_ak47;
	Checkbox stattrak_ak47;
	Slider   quality_ak47;
	Slider	 seed_ak47;

	Edit     id_aug;
	Checkbox stattrak_aug;
	Slider   quality_aug;
	Slider	 seed_aug;

	Edit     id_awp;
	Checkbox stattrak_awp;
	Slider   quality_awp;
	Slider	 seed_awp;

	Edit     id_famas;
	Checkbox stattrak_famas;
	Slider   quality_famas;
	Slider	 seed_famas;

	Edit     id_g3sg1;
	Checkbox stattrak_g3sg1;
	Slider   quality_g3sg1;
	Slider	 seed_g3sg1;

	Edit     id_galil;
	Checkbox stattrak_galil;
	Slider   quality_galil;
	Slider	 seed_galil;

	Edit     id_m249;
	Checkbox stattrak_m249;
	Slider   quality_m249;
	Slider	 seed_m249;

	Edit     id_m4a4;
	Checkbox stattrak_m4a4;
	Slider   quality_m4a4;
	Slider	 seed_m4a4;

	Edit     id_mac10;
	Checkbox stattrak_mac10;
	Slider   quality_mac10;
	Slider	 seed_mac10;

	Edit     id_p90;
	Checkbox stattrak_p90;
	Slider   quality_p90;
	Slider	 seed_p90;

	Edit     id_ump45;
	Checkbox stattrak_ump45;
	Slider   quality_ump45;
	Slider	 seed_ump45;

	Edit     id_xm1014;
	Checkbox stattrak_xm1014;
	Slider   quality_xm1014;
	Slider	 seed_xm1014;

	Edit     id_bizon;
	Checkbox stattrak_bizon;
	Slider   quality_bizon;
	Slider	 seed_bizon;

	Edit     id_mag7;
	Checkbox stattrak_mag7;
	Slider   quality_mag7;
	Slider	 seed_mag7;

	Edit     id_negev;
	Checkbox stattrak_negev;
	Slider   quality_negev;
	Slider	 seed_negev;

	Edit     id_sawedoff;
	Checkbox stattrak_sawedoff;
	Slider   quality_sawedoff;
	Slider	 seed_sawedoff;

	Edit     id_tec9;
	Checkbox stattrak_tec9;
	Slider   quality_tec9;
	Slider	 seed_tec9;

	Edit     id_p2000;
	Checkbox stattrak_p2000;
	Slider   quality_p2000;
	Slider	 seed_p2000;

	Edit     id_mp7;
	Checkbox stattrak_mp7;
	Slider   quality_mp7;
	Slider	 seed_mp7;

	Edit     id_mp9;
	Checkbox stattrak_mp9;
	Slider   quality_mp9;
	Slider	 seed_mp9;

	Edit     id_nova;
	Checkbox stattrak_nova;
	Slider   quality_nova;
	Slider	 seed_nova;

	Edit     id_p250;
	Checkbox stattrak_p250;
	Slider   quality_p250;
	Slider	 seed_p250;

	Edit     id_scar20;
	Checkbox stattrak_scar20;
	Slider   quality_scar20;
	Slider	 seed_scar20;

	Edit     id_sg553;
	Checkbox stattrak_sg553;
	Slider   quality_sg553;
	Slider	 seed_sg553;

	Edit     id_ssg08;
	Checkbox stattrak_ssg08;
	Slider   quality_ssg08;
	Slider	 seed_ssg08;

	Edit     id_m4a1s;
	Checkbox stattrak_m4a1s;
	Slider   quality_m4a1s;
	Slider	 seed_m4a1s;

	Edit     id_usps;
	Checkbox stattrak_usps;
	Slider   quality_usps;
	Slider	 seed_usps;

	Edit     id_cz75a;
	Checkbox stattrak_cz75a;
	Slider   quality_cz75a;
	Slider	 seed_cz75a;

	Edit     id_revolver;
	Checkbox stattrak_revolver;
	Slider   quality_revolver;
	Slider	 seed_revolver;

	Edit     id_bayonet;
	Checkbox stattrak_bayonet;
	Slider   quality_bayonet;
	Slider	 seed_bayonet;

	Edit     id_flip;
	Checkbox stattrak_flip;
	Slider   quality_flip;
	Slider	 seed_flip;

	Edit     id_gut;
	Checkbox stattrak_gut;
	Slider   quality_gut;
	Slider	 seed_gut;

	Edit     id_karambit;
	Checkbox stattrak_karambit;
	Slider   quality_karambit;
	Slider	 seed_karambit;

	Edit     id_m9;
	Checkbox stattrak_m9;
	Slider   quality_m9;
	Slider	 seed_m9;

	Edit     id_huntsman;
	Checkbox stattrak_huntsman;
	Slider   quality_huntsman;
	Slider	 seed_huntsman;

	Edit     id_falchion;
	Checkbox stattrak_falchion;
	Slider   quality_falchion;
	Slider	 seed_falchion;

	Edit     id_bowie;
	Checkbox stattrak_bowie;
	Slider   quality_bowie;
	Slider	 seed_bowie;

	Edit     id_butterfly;
	Checkbox stattrak_butterfly;
	Slider   quality_butterfly;
	Slider	 seed_butterfly;

	Edit     id_daggers;
	Checkbox stattrak_daggers;
	Slider   quality_daggers;
	Slider	 seed_daggers;

	// col 2.
	Dropdown knife;
	Dropdown glove;
	Edit	 glove_id;

public:
	void init() {
		SetTitle(XOR("skins"));

		enable.setup(XOR("enable"), XOR("skins_enable"));
		enable.SetCallback(callbacks::ForceFullUpdate);
		RegisterElement(&enable);

		// weapons...
		id_deagle.setup(XOR("paintkit id"), XOR("id_deagle"), 3);
		id_deagle.SetCallback(callbacks::SkinUpdate);
		id_deagle.AddShowCallback(callbacks::DEAGLE);
		RegisterElement(&id_deagle);

		stattrak_deagle.setup(XOR("stattrak"), XOR("stattrak_deagle"));
		stattrak_deagle.SetCallback(callbacks::SkinUpdate);
		stattrak_deagle.AddShowCallback(callbacks::DEAGLE);
		RegisterElement(&stattrak_deagle);

		quality_deagle.setup(XOR("quality"), XOR("quality_deagle"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_deagle.SetCallback(callbacks::SkinUpdate);
		quality_deagle.AddShowCallback(callbacks::DEAGLE);
		RegisterElement(&quality_deagle);

		seed_deagle.setup(XOR("seed"), XOR("seed_deagle"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_deagle.SetCallback(callbacks::SkinUpdate);
		seed_deagle.AddShowCallback(callbacks::DEAGLE);
		RegisterElement(&seed_deagle);

		id_elite.setup(XOR("paintkit id"), XOR("id_elite"), 3);
		id_elite.SetCallback(callbacks::SkinUpdate);
		id_elite.AddShowCallback(callbacks::ELITE);
		RegisterElement(&id_elite);

		stattrak_elite.setup(XOR("stattrak"), XOR("stattrak_elite"));
		stattrak_elite.SetCallback(callbacks::SkinUpdate);
		stattrak_elite.AddShowCallback(callbacks::ELITE);
		RegisterElement(&stattrak_elite);

		quality_elite.setup(XOR("quality"), XOR("quality_elite"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_elite.SetCallback(callbacks::SkinUpdate);
		quality_elite.AddShowCallback(callbacks::ELITE);
		RegisterElement(&quality_elite);

		seed_elite.setup(XOR("seed"), XOR("seed_elite"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_elite.SetCallback(callbacks::SkinUpdate);
		seed_elite.AddShowCallback(callbacks::ELITE);
		RegisterElement(&seed_elite);

		id_fiveseven.setup(XOR("paintkit id"), XOR("id_fiveseven"), 3);
		id_fiveseven.SetCallback(callbacks::SkinUpdate);
		id_fiveseven.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&id_fiveseven);

		stattrak_fiveseven.setup(XOR("stattrak"), XOR("stattrak_fiveseven"));
		stattrak_fiveseven.SetCallback(callbacks::SkinUpdate);
		stattrak_fiveseven.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&stattrak_fiveseven);

		quality_fiveseven.setup(XOR("quality"), XOR("quality_fiveseven"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_fiveseven.SetCallback(callbacks::SkinUpdate);
		quality_fiveseven.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&quality_fiveseven);

		seed_fiveseven.setup(XOR("seed"), XOR("seed_fiveseven"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_fiveseven.SetCallback(callbacks::SkinUpdate);
		seed_fiveseven.AddShowCallback(callbacks::FIVESEVEN);
		RegisterElement(&seed_fiveseven);

		id_glock.setup(XOR("paintkit id"), XOR("id_glock"), 3);
		id_glock.SetCallback(callbacks::SkinUpdate);
		id_glock.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&id_glock);

		stattrak_glock.setup(XOR("stattrak"), XOR("stattrak_glock"));
		stattrak_glock.SetCallback(callbacks::SkinUpdate);
		stattrak_glock.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&stattrak_glock);

		quality_glock.setup(XOR("quality"), XOR("quality_glock"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_glock.SetCallback(callbacks::SkinUpdate);
		quality_glock.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&quality_glock);

		seed_glock.setup(XOR("seed"), XOR("seed_glock"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_glock.SetCallback(callbacks::SkinUpdate);
		seed_glock.AddShowCallback(callbacks::GLOCK);
		RegisterElement(&seed_glock);

		id_ak47.setup(XOR("paintkit id"), XOR("id_ak47"), 3);
		id_ak47.SetCallback(callbacks::SkinUpdate);
		id_ak47.AddShowCallback(callbacks::AK47);
		RegisterElement(&id_ak47);

		stattrak_ak47.setup(XOR("stattrak"), XOR("stattrak_ak47"));
		stattrak_ak47.SetCallback(callbacks::SkinUpdate);
		stattrak_ak47.AddShowCallback(callbacks::AK47);
		RegisterElement(&stattrak_ak47);

		quality_ak47.setup(XOR("quality"), XOR("quality_ak47"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_ak47.SetCallback(callbacks::SkinUpdate);
		quality_ak47.AddShowCallback(callbacks::AK47);
		RegisterElement(&quality_ak47);

		seed_ak47.setup(XOR("seed"), XOR("seed_ak47"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_ak47.SetCallback(callbacks::SkinUpdate);
		seed_ak47.AddShowCallback(callbacks::AK47);
		RegisterElement(&seed_ak47);

		id_aug.setup(XOR("paintkit id"), XOR("id_aug"), 3);
		id_aug.SetCallback(callbacks::SkinUpdate);
		id_aug.AddShowCallback(callbacks::AUG);
		RegisterElement(&id_aug);

		stattrak_aug.setup(XOR("stattrak"), XOR("stattrak_aug"));
		stattrak_aug.SetCallback(callbacks::SkinUpdate);
		stattrak_aug.AddShowCallback(callbacks::AUG);
		RegisterElement(&stattrak_aug);

		quality_aug.setup(XOR("quality"), XOR("quality_aug"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_aug.SetCallback(callbacks::SkinUpdate);
		quality_aug.AddShowCallback(callbacks::AUG);
		RegisterElement(&quality_aug);

		seed_aug.setup(XOR("seed"), XOR("seed_aug"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_aug.SetCallback(callbacks::SkinUpdate);
		seed_aug.AddShowCallback(callbacks::AUG);
		RegisterElement(&seed_aug);

		id_awp.setup(XOR("paintkit id"), XOR("id_awp"), 3);
		id_awp.SetCallback(callbacks::SkinUpdate);
		id_awp.AddShowCallback(callbacks::AWP);
		RegisterElement(&id_awp);

		stattrak_awp.setup(XOR("stattrak"), XOR("stattrak_awp"));
		stattrak_awp.SetCallback(callbacks::SkinUpdate);
		stattrak_awp.AddShowCallback(callbacks::AWP);
		RegisterElement(&stattrak_awp);

		quality_awp.setup(XOR("quality"), XOR("quality_awp"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_awp.SetCallback(callbacks::SkinUpdate);
		quality_awp.AddShowCallback(callbacks::AWP);
		RegisterElement(&quality_awp);

		seed_awp.setup(XOR("seed"), XOR("seed_awp"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_awp.SetCallback(callbacks::SkinUpdate);
		seed_awp.AddShowCallback(callbacks::AWP);
		RegisterElement(&seed_awp);

		id_famas.setup(XOR("paintkit id"), XOR("id_famas"), 3);
		id_famas.SetCallback(callbacks::SkinUpdate);
		id_famas.AddShowCallback(callbacks::FAMAS);
		RegisterElement(&id_famas);

		stattrak_famas.setup(XOR("stattrak"), XOR("stattrak_famas"));
		stattrak_famas.SetCallback(callbacks::SkinUpdate);
		stattrak_famas.AddShowCallback(callbacks::FAMAS);
		RegisterElement(&stattrak_famas);

		quality_famas.setup(XOR("quality"), XOR("quality_famas"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_famas.SetCallback(callbacks::SkinUpdate);
		quality_famas.AddShowCallback(callbacks::FAMAS);
		RegisterElement(&quality_famas);

		seed_famas.setup(XOR("seed"), XOR("seed_famas"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_famas.SetCallback(callbacks::SkinUpdate);
		seed_famas.AddShowCallback(callbacks::FAMAS);
		RegisterElement(&seed_famas);

		id_g3sg1.setup(XOR("paintkit id"), XOR("id_g3sg1"), 3);
		id_g3sg1.SetCallback(callbacks::SkinUpdate);
		id_g3sg1.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&id_g3sg1);

		stattrak_g3sg1.setup(XOR("stattrak"), XOR("stattrak_g3sg1"));
		stattrak_g3sg1.SetCallback(callbacks::SkinUpdate);
		stattrak_g3sg1.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&stattrak_g3sg1);

		quality_g3sg1.setup(XOR("quality"), XOR("quality_g3sg1"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_g3sg1.SetCallback(callbacks::SkinUpdate);
		quality_g3sg1.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&quality_g3sg1);

		seed_g3sg1.setup(XOR("seed"), XOR("seed_g3sg1"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_g3sg1.SetCallback(callbacks::SkinUpdate);
		seed_g3sg1.AddShowCallback(callbacks::G3SG1);
		RegisterElement(&seed_g3sg1);

		id_galil.setup(XOR("paintkit id"), XOR("id_galil"), 3);
		id_galil.SetCallback(callbacks::SkinUpdate);
		id_galil.AddShowCallback(callbacks::GALIL);
		RegisterElement(&id_galil);

		stattrak_galil.setup(XOR("stattrak"), XOR("stattrak_galil"));
		stattrak_galil.SetCallback(callbacks::SkinUpdate);
		stattrak_galil.AddShowCallback(callbacks::GALIL);
		RegisterElement(&stattrak_galil);

		quality_galil.setup(XOR("quality"), XOR("quality_galil"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_galil.SetCallback(callbacks::SkinUpdate);
		quality_galil.AddShowCallback(callbacks::GALIL);
		RegisterElement(&quality_galil);

		seed_galil.setup(XOR("seed"), XOR("seed_galil"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_galil.SetCallback(callbacks::SkinUpdate);
		seed_galil.AddShowCallback(callbacks::GALIL);
		RegisterElement(&seed_galil);

		id_m249.setup(XOR("paintkit id"), XOR("id_m249"), 3);
		id_m249.SetCallback(callbacks::SkinUpdate);
		id_m249.AddShowCallback(callbacks::M249);
		RegisterElement(&id_m249);

		stattrak_m249.setup(XOR("stattrak"), XOR("stattrak_m249"));
		stattrak_m249.SetCallback(callbacks::SkinUpdate);
		stattrak_m249.AddShowCallback(callbacks::M249);
		RegisterElement(&stattrak_m249);

		quality_m249.setup(XOR("quality"), XOR("quality_m249"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_m249.SetCallback(callbacks::SkinUpdate);
		quality_m249.AddShowCallback(callbacks::M249);
		RegisterElement(&quality_m249);

		seed_m249.setup(XOR("seed"), XOR("seed_m249"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_m249.SetCallback(callbacks::SkinUpdate);
		seed_m249.AddShowCallback(callbacks::M249);
		RegisterElement(&seed_m249);

		id_m4a4.setup(XOR("paintkit id"), XOR("id_m4a4"), 3);
		id_m4a4.SetCallback(callbacks::SkinUpdate);
		id_m4a4.AddShowCallback(callbacks::M4A4);
		RegisterElement(&id_m4a4);

		stattrak_m4a4.setup(XOR("stattrak"), XOR("stattrak_m4a4"));
		stattrak_m4a4.SetCallback(callbacks::SkinUpdate);
		stattrak_m4a4.AddShowCallback(callbacks::M4A4);
		RegisterElement(&stattrak_m4a4);

		quality_m4a4.setup(XOR("quality"), XOR("quality_m4a4"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_m4a4.SetCallback(callbacks::SkinUpdate);
		quality_m4a4.AddShowCallback(callbacks::M4A4);
		RegisterElement(&quality_m4a4);

		seed_m4a4.setup(XOR("seed"), XOR("seed_m4a4"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_m4a4.SetCallback(callbacks::SkinUpdate);
		seed_m4a4.AddShowCallback(callbacks::M4A4);
		RegisterElement(&seed_m4a4);

		id_mac10.setup(XOR("paintkit id"), XOR("id_mac10"), 3);
		id_mac10.SetCallback(callbacks::SkinUpdate);
		id_mac10.AddShowCallback(callbacks::MAC10);
		RegisterElement(&id_mac10);

		stattrak_mac10.setup(XOR("stattrak"), XOR("stattrak_mac10"));
		stattrak_mac10.SetCallback(callbacks::SkinUpdate);
		stattrak_mac10.AddShowCallback(callbacks::MAC10);
		RegisterElement(&stattrak_mac10);

		quality_mac10.setup(XOR("quality"), XOR("quality_mac10"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_mac10.SetCallback(callbacks::SkinUpdate);
		quality_mac10.AddShowCallback(callbacks::MAC10);
		RegisterElement(&quality_mac10);

		seed_mac10.setup(XOR("seed"), XOR("seed_mac10"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_mac10.SetCallback(callbacks::SkinUpdate);
		seed_mac10.AddShowCallback(callbacks::MAC10);
		RegisterElement(&seed_mac10);

		id_p90.setup(XOR("paintkit id"), XOR("id_p90"), 3);
		id_p90.SetCallback(callbacks::SkinUpdate);
		id_p90.AddShowCallback(callbacks::P90);
		RegisterElement(&id_p90);

		stattrak_p90.setup(XOR("stattrak"), XOR("stattrak_p90"));
		stattrak_p90.SetCallback(callbacks::SkinUpdate);
		stattrak_p90.AddShowCallback(callbacks::P90);
		RegisterElement(&stattrak_p90);

		quality_p90.setup(XOR("quality"), XOR("quality_p90"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_p90.SetCallback(callbacks::SkinUpdate);
		quality_p90.AddShowCallback(callbacks::P90);
		RegisterElement(&quality_p90);

		seed_p90.setup(XOR("seed"), XOR("seed_p90"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_p90.SetCallback(callbacks::SkinUpdate);
		seed_p90.AddShowCallback(callbacks::P90);
		RegisterElement(&seed_p90);

		id_ump45.setup(XOR("paintkit id"), XOR("id_ump45"), 3);
		id_ump45.SetCallback(callbacks::SkinUpdate);
		id_ump45.AddShowCallback(callbacks::UMP45);
		RegisterElement(&id_ump45);

		stattrak_ump45.setup(XOR("stattrak"), XOR("stattrak_ump45"));
		stattrak_ump45.SetCallback(callbacks::SkinUpdate);
		stattrak_ump45.AddShowCallback(callbacks::UMP45);
		RegisterElement(&stattrak_ump45);

		quality_ump45.setup(XOR("quality"), XOR("quality_ump45"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_ump45.SetCallback(callbacks::SkinUpdate);
		quality_ump45.AddShowCallback(callbacks::UMP45);
		RegisterElement(&quality_ump45);

		seed_ump45.setup(XOR("seed"), XOR("seed_ump45"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_ump45.SetCallback(callbacks::SkinUpdate);
		seed_ump45.AddShowCallback(callbacks::UMP45);
		RegisterElement(&seed_ump45);

		id_xm1014.setup(XOR("paintkit id"), XOR("id_xm1014"), 3);
		id_xm1014.SetCallback(callbacks::SkinUpdate);
		id_xm1014.AddShowCallback(callbacks::XM1014);
		RegisterElement(&id_xm1014);

		stattrak_xm1014.setup(XOR("stattrak"), XOR("stattrak_xm1014"));
		stattrak_xm1014.SetCallback(callbacks::SkinUpdate);
		stattrak_xm1014.AddShowCallback(callbacks::XM1014);
		RegisterElement(&stattrak_xm1014);

		quality_xm1014.setup(XOR("quality"), XOR("quality_xm1014"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_xm1014.SetCallback(callbacks::SkinUpdate);
		quality_xm1014.AddShowCallback(callbacks::XM1014);
		RegisterElement(&quality_xm1014);

		seed_xm1014.setup(XOR("seed"), XOR("seed_xm1014"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_xm1014.SetCallback(callbacks::SkinUpdate);
		seed_xm1014.AddShowCallback(callbacks::XM1014);
		RegisterElement(&seed_xm1014);

		id_bizon.setup(XOR("paintkit id"), XOR("id_bizon"), 3);
		id_bizon.SetCallback(callbacks::SkinUpdate);
		id_bizon.AddShowCallback(callbacks::BIZON);
		RegisterElement(&id_bizon);

		stattrak_bizon.setup(XOR("stattrak"), XOR("stattrak_bizon"));
		stattrak_bizon.SetCallback(callbacks::SkinUpdate);
		stattrak_bizon.AddShowCallback(callbacks::BIZON);
		RegisterElement(&stattrak_bizon);

		quality_bizon.setup(XOR("quality"), XOR("quality_bizon"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_bizon.SetCallback(callbacks::SkinUpdate);
		quality_bizon.AddShowCallback(callbacks::BIZON);
		RegisterElement(&quality_bizon);

		seed_bizon.setup(XOR("seed"), XOR("seed_bizon"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_bizon.SetCallback(callbacks::SkinUpdate);
		seed_bizon.AddShowCallback(callbacks::BIZON);
		RegisterElement(&seed_bizon);

		id_mag7.setup(XOR("paintkit id"), XOR("id_mag7"), 3);
		id_mag7.SetCallback(callbacks::SkinUpdate);
		id_mag7.AddShowCallback(callbacks::MAG7);
		RegisterElement(&id_mag7);

		stattrak_mag7.setup(XOR("stattrak"), XOR("stattrak_mag7"));
		stattrak_mag7.SetCallback(callbacks::SkinUpdate);
		stattrak_mag7.AddShowCallback(callbacks::MAG7);
		RegisterElement(&stattrak_mag7);

		quality_mag7.setup(XOR("quality"), XOR("quality_mag7"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_mag7.SetCallback(callbacks::SkinUpdate);
		quality_mag7.AddShowCallback(callbacks::MAG7);
		RegisterElement(&quality_mag7);

		seed_mag7.setup(XOR("seed"), XOR("seed_mag7"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_mag7.SetCallback(callbacks::SkinUpdate);
		seed_mag7.AddShowCallback(callbacks::MAG7);
		RegisterElement(&seed_mag7);

		id_negev.setup(XOR("paintkit id"), XOR("id_negev"), 3);
		id_negev.SetCallback(callbacks::SkinUpdate);
		id_negev.AddShowCallback(callbacks::NEGEV);
		RegisterElement(&id_negev);

		stattrak_negev.setup(XOR("stattrak"), XOR("stattrak_negev"));
		stattrak_negev.SetCallback(callbacks::SkinUpdate);
		stattrak_negev.AddShowCallback(callbacks::NEGEV);
		RegisterElement(&stattrak_negev);

		quality_negev.setup(XOR("quality"), XOR("quality_negev"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_negev.SetCallback(callbacks::SkinUpdate);
		quality_negev.AddShowCallback(callbacks::NEGEV);
		RegisterElement(&quality_negev);

		seed_negev.setup(XOR("seed"), XOR("seed_negev"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_negev.SetCallback(callbacks::SkinUpdate);
		seed_negev.AddShowCallback(callbacks::NEGEV);
		RegisterElement(&seed_negev);

		id_sawedoff.setup(XOR("paintkit id"), XOR("id_sawedoff"), 3);
		id_sawedoff.SetCallback(callbacks::SkinUpdate);
		id_sawedoff.AddShowCallback(callbacks::SAWEDOFF);
		RegisterElement(&id_sawedoff);

		stattrak_sawedoff.setup(XOR("stattrak"), XOR("stattrak_sawedoff"));
		stattrak_sawedoff.SetCallback(callbacks::SkinUpdate);
		stattrak_sawedoff.AddShowCallback(callbacks::SAWEDOFF);
		RegisterElement(&stattrak_sawedoff);

		quality_sawedoff.setup(XOR("quality"), XOR("quality_sawedoff"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_sawedoff.SetCallback(callbacks::SkinUpdate);
		quality_sawedoff.AddShowCallback(callbacks::SAWEDOFF);
		RegisterElement(&quality_sawedoff);

		seed_sawedoff.setup(XOR("seed"), XOR("seed_sawedoff"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_sawedoff.SetCallback(callbacks::SkinUpdate);
		seed_sawedoff.AddShowCallback(callbacks::SAWEDOFF);
		RegisterElement(&seed_sawedoff);

		id_tec9.setup(XOR("paintkit id"), XOR("id_tec9"), 3);
		id_tec9.SetCallback(callbacks::SkinUpdate);
		id_tec9.AddShowCallback(callbacks::TEC9);
		RegisterElement(&id_tec9);

		stattrak_tec9.setup(XOR("stattrak"), XOR("stattrak_tec9"));
		stattrak_tec9.SetCallback(callbacks::SkinUpdate);
		stattrak_tec9.AddShowCallback(callbacks::TEC9);
		RegisterElement(&stattrak_tec9);

		quality_tec9.setup(XOR("quality"), XOR("quality_tec9"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_tec9.SetCallback(callbacks::SkinUpdate);
		quality_tec9.AddShowCallback(callbacks::TEC9);
		RegisterElement(&quality_tec9);

		seed_tec9.setup(XOR("seed"), XOR("seed_tec9"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_tec9.SetCallback(callbacks::SkinUpdate);
		seed_tec9.AddShowCallback(callbacks::TEC9);
		RegisterElement(&seed_tec9);

		id_p2000.setup(XOR("paintkit id"), XOR("id_p2000"), 3);
		id_p2000.SetCallback(callbacks::SkinUpdate);
		id_p2000.AddShowCallback(callbacks::P2000);
		RegisterElement(&id_p2000);

		stattrak_p2000.setup(XOR("stattrak"), XOR("stattrak_p2000"));
		stattrak_p2000.SetCallback(callbacks::SkinUpdate);
		stattrak_p2000.AddShowCallback(callbacks::P2000);
		RegisterElement(&stattrak_p2000);

		quality_p2000.setup(XOR("quality"), XOR("quality_p2000"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_p2000.SetCallback(callbacks::SkinUpdate);
		quality_p2000.AddShowCallback(callbacks::P2000);
		RegisterElement(&quality_p2000);

		seed_p2000.setup(XOR("seed"), XOR("seed_p2000"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_p2000.SetCallback(callbacks::SkinUpdate);
		seed_p2000.AddShowCallback(callbacks::P2000);
		RegisterElement(&seed_p2000);

		id_mp7.setup(XOR("paintkit id"), XOR("id_mp7"), 3);
		id_mp7.SetCallback(callbacks::SkinUpdate);
		id_mp7.AddShowCallback(callbacks::MP7);
		RegisterElement(&id_mp7);

		stattrak_mp7.setup(XOR("stattrak"), XOR("stattrak_mp7"));
		stattrak_mp7.SetCallback(callbacks::SkinUpdate);
		stattrak_mp7.AddShowCallback(callbacks::MP7);
		RegisterElement(&stattrak_mp7);

		quality_mp7.setup(XOR("quality"), XOR("quality_mp7"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_mp7.SetCallback(callbacks::SkinUpdate);
		quality_mp7.AddShowCallback(callbacks::MP7);
		RegisterElement(&quality_mp7);

		seed_mp7.setup(XOR("seed"), XOR("seed_mp7"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_mp7.SetCallback(callbacks::SkinUpdate);
		seed_mp7.AddShowCallback(callbacks::MP7);
		RegisterElement(&seed_mp7);

		id_mp9.setup(XOR("paintkit id"), XOR("id_mp9"), 3);
		id_mp9.SetCallback(callbacks::SkinUpdate);
		id_mp9.AddShowCallback(callbacks::MP9);
		RegisterElement(&id_mp9);

		stattrak_mp9.setup(XOR("stattrak"), XOR("stattrak_mp9"));
		stattrak_mp9.SetCallback(callbacks::SkinUpdate);
		stattrak_mp9.AddShowCallback(callbacks::MP9);
		RegisterElement(&stattrak_mp9);

		quality_mp9.setup(XOR("quality"), XOR("quality_mp9"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_mp9.SetCallback(callbacks::SkinUpdate);
		quality_mp9.AddShowCallback(callbacks::MP9);
		RegisterElement(&quality_mp9);

		seed_mp9.setup(XOR("seed"), XOR("seed_mp9"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_mp9.SetCallback(callbacks::SkinUpdate);
		seed_mp9.AddShowCallback(callbacks::MP9);
		RegisterElement(&seed_mp9);

		id_nova.setup(XOR("paintkit id"), XOR("id_nova"), 3);
		id_nova.SetCallback(callbacks::SkinUpdate);
		id_nova.AddShowCallback(callbacks::NOVA);
		RegisterElement(&id_nova);

		stattrak_nova.setup(XOR("stattrak"), XOR("stattrak_nova"));
		stattrak_nova.SetCallback(callbacks::SkinUpdate);
		stattrak_nova.AddShowCallback(callbacks::NOVA);
		RegisterElement(&stattrak_nova);

		quality_nova.setup(XOR("quality"), XOR("quality_nova"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_nova.SetCallback(callbacks::SkinUpdate);
		quality_nova.AddShowCallback(callbacks::NOVA);
		RegisterElement(&quality_nova);

		seed_nova.setup(XOR("seed"), XOR("seed_nova"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_nova.SetCallback(callbacks::SkinUpdate);
		seed_nova.AddShowCallback(callbacks::NOVA);
		RegisterElement(&seed_nova);

		id_p250.setup(XOR("paintkit id"), XOR("id_p250"), 3);
		id_p250.SetCallback(callbacks::SkinUpdate);
		id_p250.AddShowCallback(callbacks::P250);
		RegisterElement(&id_p250);

		stattrak_p250.setup(XOR("stattrak"), XOR("stattrak_p250"));
		stattrak_p250.SetCallback(callbacks::SkinUpdate);
		stattrak_p250.AddShowCallback(callbacks::P250);
		RegisterElement(&stattrak_p250);

		quality_p250.setup(XOR("quality"), XOR("quality_p250"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_p250.SetCallback(callbacks::SkinUpdate);
		quality_p250.AddShowCallback(callbacks::P250);
		RegisterElement(&quality_p250);

		seed_p250.setup(XOR("seed"), XOR("seed_p250"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_p250.SetCallback(callbacks::SkinUpdate);
		seed_p250.AddShowCallback(callbacks::P250);
		RegisterElement(&seed_p250);

		id_scar20.setup(XOR("paintkit id"), XOR("id_scar20"), 3);
		id_scar20.SetCallback(callbacks::SkinUpdate);
		id_scar20.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&id_scar20);

		stattrak_scar20.setup(XOR("stattrak"), XOR("stattrak_scar20"));
		stattrak_scar20.SetCallback(callbacks::SkinUpdate);
		stattrak_scar20.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&stattrak_scar20);

		quality_scar20.setup(XOR("quality"), XOR("quality_scar20"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_scar20.SetCallback(callbacks::SkinUpdate);
		quality_scar20.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&quality_scar20);

		seed_scar20.setup(XOR("seed"), XOR("seed_scar20"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_scar20.SetCallback(callbacks::SkinUpdate);
		seed_scar20.AddShowCallback(callbacks::SCAR20);
		RegisterElement(&seed_scar20);

		id_sg553.setup(XOR("paintkit id"), XOR("id_sg553"), 3);
		id_sg553.SetCallback(callbacks::SkinUpdate);
		id_sg553.AddShowCallback(callbacks::SG553);
		RegisterElement(&id_sg553);

		stattrak_sg553.setup(XOR("stattrak"), XOR("stattrak_sg553"));
		stattrak_sg553.SetCallback(callbacks::SkinUpdate);
		stattrak_sg553.AddShowCallback(callbacks::SG553);
		RegisterElement(&stattrak_sg553);

		quality_sg553.setup(XOR("quality"), XOR("quality_sg553"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_sg553.SetCallback(callbacks::SkinUpdate);
		quality_sg553.AddShowCallback(callbacks::SG553);
		RegisterElement(&quality_sg553);

		seed_sg553.setup(XOR("seed"), XOR("seed_sg553"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_sg553.SetCallback(callbacks::SkinUpdate);
		seed_sg553.AddShowCallback(callbacks::SG553);
		RegisterElement(&seed_sg553);

		id_ssg08.setup(XOR("paintkit id"), XOR("id_ssg08"), 3);
		id_ssg08.SetCallback(callbacks::SkinUpdate);
		id_ssg08.AddShowCallback(callbacks::SSG08);
		RegisterElement(&id_ssg08);

		stattrak_ssg08.setup(XOR("stattrak"), XOR("stattrak_ssg08"));
		stattrak_ssg08.SetCallback(callbacks::SkinUpdate);
		stattrak_ssg08.AddShowCallback(callbacks::SSG08);
		RegisterElement(&stattrak_ssg08);

		quality_ssg08.setup(XOR("quality"), XOR("quality_ssg08"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_ssg08.SetCallback(callbacks::SkinUpdate);
		quality_ssg08.AddShowCallback(callbacks::SSG08);
		RegisterElement(&quality_ssg08);

		seed_ssg08.setup(XOR("seed"), XOR("seed_ssg08"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_ssg08.SetCallback(callbacks::SkinUpdate);
		seed_ssg08.AddShowCallback(callbacks::SSG08);
		RegisterElement(&seed_ssg08);

		id_m4a1s.setup(XOR("paintkit id"), XOR("id_m4a1s"), 3);
		id_m4a1s.SetCallback(callbacks::SkinUpdate);
		id_m4a1s.AddShowCallback(callbacks::M4A1S);
		RegisterElement(&id_m4a1s);

		stattrak_m4a1s.setup(XOR("stattrak"), XOR("stattrak_m4a1s"));
		stattrak_m4a1s.SetCallback(callbacks::SkinUpdate);
		stattrak_m4a1s.AddShowCallback(callbacks::M4A1S);
		RegisterElement(&stattrak_m4a1s);

		quality_m4a1s.setup(XOR("quality"), XOR("quality_m4a1s"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_m4a1s.SetCallback(callbacks::SkinUpdate);
		quality_m4a1s.AddShowCallback(callbacks::M4A1S);
		RegisterElement(&quality_m4a1s);

		seed_m4a1s.setup(XOR("seed"), XOR("seed_m4a1s"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_m4a1s.SetCallback(callbacks::SkinUpdate);
		seed_m4a1s.AddShowCallback(callbacks::M4A1S);
		RegisterElement(&seed_m4a1s);

		id_usps.setup(XOR("paintkit id"), XOR("id_usps"), 3);
		id_usps.SetCallback(callbacks::SkinUpdate);
		id_usps.AddShowCallback(callbacks::USPS);
		RegisterElement(&id_usps);

		stattrak_usps.setup(XOR("stattrak"), XOR("stattrak_usps"));
		stattrak_usps.SetCallback(callbacks::SkinUpdate);
		stattrak_usps.AddShowCallback(callbacks::USPS);
		RegisterElement(&stattrak_usps);

		quality_usps.setup(XOR("quality"), XOR("quality_usps"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_usps.SetCallback(callbacks::SkinUpdate);
		quality_usps.AddShowCallback(callbacks::USPS);
		RegisterElement(&quality_usps);

		seed_usps.setup(XOR("seed"), XOR("seed_usps"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_usps.SetCallback(callbacks::SkinUpdate);
		seed_usps.AddShowCallback(callbacks::USPS);
		RegisterElement(&seed_usps);

		id_cz75a.setup(XOR("paintkit id"), XOR("id_cz75a"), 3);
		id_cz75a.SetCallback(callbacks::SkinUpdate);
		id_cz75a.AddShowCallback(callbacks::CZ75A);
		RegisterElement(&id_cz75a);

		stattrak_cz75a.setup(XOR("stattrak"), XOR("stattrak_cz75a"));
		stattrak_cz75a.SetCallback(callbacks::SkinUpdate);
		stattrak_cz75a.AddShowCallback(callbacks::CZ75A);
		RegisterElement(&stattrak_cz75a);

		quality_cz75a.setup(XOR("quality"), XOR("quality_cz75a"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_cz75a.SetCallback(callbacks::SkinUpdate);
		quality_cz75a.AddShowCallback(callbacks::CZ75A);
		RegisterElement(&quality_cz75a);

		seed_cz75a.setup(XOR("seed"), XOR("seed_cz75a"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_cz75a.SetCallback(callbacks::SkinUpdate);
		seed_cz75a.AddShowCallback(callbacks::CZ75A);
		RegisterElement(&seed_cz75a);

		id_revolver.setup(XOR("paintkit id"), XOR("id_revolver"), 3);
		id_revolver.SetCallback(callbacks::SkinUpdate);
		id_revolver.AddShowCallback(callbacks::REVOLVER);
		RegisterElement(&id_revolver);

		stattrak_revolver.setup(XOR("stattrak"), XOR("stattrak_revolver"));
		stattrak_revolver.SetCallback(callbacks::SkinUpdate);
		stattrak_revolver.AddShowCallback(callbacks::REVOLVER);
		RegisterElement(&stattrak_revolver);

		quality_revolver.setup(XOR("quality"), XOR("quality_revolver"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_revolver.SetCallback(callbacks::SkinUpdate);
		quality_revolver.AddShowCallback(callbacks::REVOLVER);
		RegisterElement(&quality_revolver);

		seed_revolver.setup(XOR("seed"), XOR("seed_revolver"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_revolver.SetCallback(callbacks::SkinUpdate);
		seed_revolver.AddShowCallback(callbacks::REVOLVER);
		RegisterElement(&seed_revolver);

		id_bayonet.setup(XOR("paintkit id"), XOR("id_bayonet"), 3);
		id_bayonet.SetCallback(callbacks::SkinUpdate);
		id_bayonet.AddShowCallback(callbacks::KNIFE_BAYONET);
		RegisterElement(&id_bayonet);

		stattrak_bayonet.setup(XOR("stattrak"), XOR("stattrak_bayonet"));
		stattrak_bayonet.SetCallback(callbacks::SkinUpdate);
		stattrak_bayonet.AddShowCallback(callbacks::KNIFE_BAYONET);
		RegisterElement(&stattrak_bayonet);

		quality_bayonet.setup(XOR("quality"), XOR("quality_bayonet"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_bayonet.SetCallback(callbacks::SkinUpdate);
		quality_bayonet.AddShowCallback(callbacks::KNIFE_BAYONET);
		RegisterElement(&quality_bayonet);

		seed_bayonet.setup(XOR("seed"), XOR("seed_bayonet"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_bayonet.SetCallback(callbacks::SkinUpdate);
		seed_bayonet.AddShowCallback(callbacks::KNIFE_BAYONET);
		RegisterElement(&seed_bayonet);

		id_flip.setup(XOR("paintkit id"), XOR("id_flip"), 3);
		id_flip.SetCallback(callbacks::SkinUpdate);
		id_flip.AddShowCallback(callbacks::KNIFE_FLIP);
		RegisterElement(&id_flip);

		stattrak_flip.setup(XOR("stattrak"), XOR("stattrak_flip"));
		stattrak_flip.SetCallback(callbacks::SkinUpdate);
		stattrak_flip.AddShowCallback(callbacks::KNIFE_FLIP);
		RegisterElement(&stattrak_flip);

		quality_flip.setup(XOR("quality"), XOR("quality_flip"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_flip.SetCallback(callbacks::SkinUpdate);
		quality_flip.AddShowCallback(callbacks::KNIFE_FLIP);
		RegisterElement(&quality_flip);

		seed_flip.setup(XOR("seed"), XOR("seed_flip"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_flip.SetCallback(callbacks::SkinUpdate);
		seed_flip.AddShowCallback(callbacks::KNIFE_FLIP);
		RegisterElement(&seed_flip);

		id_gut.setup(XOR("paintkit id"), XOR("id_gut"), 3);
		id_gut.SetCallback(callbacks::SkinUpdate);
		id_gut.AddShowCallback(callbacks::KNIFE_GUT);
		RegisterElement(&id_gut);

		stattrak_gut.setup(XOR("stattrak"), XOR("stattrak_gut"));
		stattrak_gut.SetCallback(callbacks::SkinUpdate);
		stattrak_gut.AddShowCallback(callbacks::KNIFE_GUT);
		RegisterElement(&stattrak_gut);

		quality_gut.setup(XOR("quality"), XOR("quality_gut"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_gut.SetCallback(callbacks::SkinUpdate);
		quality_gut.AddShowCallback(callbacks::KNIFE_GUT);
		RegisterElement(&quality_gut);

		seed_gut.setup(XOR("seed"), XOR("seed_gut"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_gut.SetCallback(callbacks::SkinUpdate);
		seed_gut.AddShowCallback(callbacks::KNIFE_GUT);
		RegisterElement(&seed_gut);

		id_karambit.setup(XOR("paintkit id"), XOR("id_karambit"), 3);
		id_karambit.SetCallback(callbacks::SkinUpdate);
		id_karambit.AddShowCallback(callbacks::KNIFE_KARAMBIT);
		RegisterElement(&id_karambit);

		stattrak_karambit.setup(XOR("stattrak"), XOR("stattrak_karambit"));
		stattrak_karambit.SetCallback(callbacks::SkinUpdate);
		stattrak_karambit.AddShowCallback(callbacks::KNIFE_KARAMBIT);
		RegisterElement(&stattrak_karambit);

		quality_karambit.setup(XOR("quality"), XOR("quality_karambit"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_karambit.SetCallback(callbacks::SkinUpdate);
		quality_karambit.AddShowCallback(callbacks::KNIFE_KARAMBIT);
		RegisterElement(&quality_karambit);

		seed_karambit.setup(XOR("seed"), XOR("seed_karambit"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_karambit.SetCallback(callbacks::SkinUpdate);
		seed_karambit.AddShowCallback(callbacks::KNIFE_KARAMBIT);
		RegisterElement(&seed_karambit);

		id_m9.setup(XOR("paintkit id"), XOR("id_m9"), 3);
		id_m9.SetCallback(callbacks::SkinUpdate);
		id_m9.AddShowCallback(callbacks::KNIFE_M9_BAYONET);
		RegisterElement(&id_m9);

		stattrak_m9.setup(XOR("stattrak"), XOR("stattrak_m9"));
		stattrak_m9.SetCallback(callbacks::SkinUpdate);
		stattrak_m9.AddShowCallback(callbacks::KNIFE_M9_BAYONET);
		RegisterElement(&stattrak_m9);

		quality_m9.setup(XOR("quality"), XOR("quality_m9"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_m9.SetCallback(callbacks::SkinUpdate);
		quality_m9.AddShowCallback(callbacks::KNIFE_M9_BAYONET);
		RegisterElement(&quality_m9);

		seed_m9.setup(XOR("seed"), XOR("seed_m9"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_m9.SetCallback(callbacks::SkinUpdate);
		seed_m9.AddShowCallback(callbacks::KNIFE_M9_BAYONET);
		RegisterElement(&seed_m9);

		id_huntsman.setup(XOR("paintkit id"), XOR("id_huntsman"), 3);
		id_huntsman.SetCallback(callbacks::SkinUpdate);
		id_huntsman.AddShowCallback(callbacks::KNIFE_HUNTSMAN);
		RegisterElement(&id_huntsman);

		stattrak_huntsman.setup(XOR("stattrak"), XOR("stattrak_huntsman"));
		stattrak_huntsman.SetCallback(callbacks::SkinUpdate);
		stattrak_huntsman.AddShowCallback(callbacks::KNIFE_HUNTSMAN);
		RegisterElement(&stattrak_huntsman);

		quality_huntsman.setup(XOR("quality"), XOR("quality_huntsman"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_huntsman.SetCallback(callbacks::SkinUpdate);
		quality_huntsman.AddShowCallback(callbacks::KNIFE_HUNTSMAN);
		RegisterElement(&quality_huntsman);

		seed_huntsman.setup(XOR("seed"), XOR("seed_huntsman"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_huntsman.SetCallback(callbacks::SkinUpdate);
		seed_huntsman.AddShowCallback(callbacks::KNIFE_HUNTSMAN);
		RegisterElement(&seed_huntsman);

		id_falchion.setup(XOR("paintkit id"), XOR("id_falchion"), 3);
		id_falchion.SetCallback(callbacks::SkinUpdate);
		id_falchion.AddShowCallback(callbacks::KNIFE_FALCHION);
		RegisterElement(&id_falchion);

		stattrak_falchion.setup(XOR("stattrak"), XOR("stattrak_falchion"));
		stattrak_falchion.SetCallback(callbacks::SkinUpdate);
		stattrak_falchion.AddShowCallback(callbacks::KNIFE_FALCHION);
		RegisterElement(&stattrak_falchion);

		quality_falchion.setup(XOR("quality"), XOR("quality_falchion"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_falchion.SetCallback(callbacks::SkinUpdate);
		quality_falchion.AddShowCallback(callbacks::KNIFE_FALCHION);
		RegisterElement(&quality_falchion);

		seed_falchion.setup(XOR("seed"), XOR("seed_falchion"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_falchion.SetCallback(callbacks::SkinUpdate);
		seed_falchion.AddShowCallback(callbacks::KNIFE_FALCHION);
		RegisterElement(&seed_falchion);

		id_bowie.setup(XOR("paintkit id"), XOR("id_bowie"), 3);
		id_bowie.SetCallback(callbacks::SkinUpdate);
		id_bowie.AddShowCallback(callbacks::KNIFE_BOWIE);
		RegisterElement(&id_bowie);

		stattrak_bowie.setup(XOR("stattrak"), XOR("stattrak_bowie"));
		stattrak_bowie.SetCallback(callbacks::SkinUpdate);
		stattrak_bowie.AddShowCallback(callbacks::KNIFE_BOWIE);
		RegisterElement(&stattrak_bowie);

		quality_bowie.setup(XOR("quality"), XOR("quality_bowie"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_bowie.SetCallback(callbacks::SkinUpdate);
		quality_bowie.AddShowCallback(callbacks::KNIFE_BOWIE);
		RegisterElement(&quality_bowie);

		seed_bowie.setup(XOR("seed"), XOR("seed_bowie"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_bowie.SetCallback(callbacks::SkinUpdate);
		seed_bowie.AddShowCallback(callbacks::KNIFE_BOWIE);
		RegisterElement(&seed_bowie);

		id_butterfly.setup(XOR("paintkit id"), XOR("id_butterfly"), 3);
		id_butterfly.SetCallback(callbacks::SkinUpdate);
		id_butterfly.AddShowCallback(callbacks::KNIFE_BUTTERFLY);
		RegisterElement(&id_butterfly);

		stattrak_butterfly.setup(XOR("stattrak"), XOR("stattrak_butterfly"));
		stattrak_butterfly.SetCallback(callbacks::SkinUpdate);
		stattrak_butterfly.AddShowCallback(callbacks::KNIFE_BUTTERFLY);
		RegisterElement(&stattrak_butterfly);

		quality_butterfly.setup(XOR("quality"), XOR("quality_butterfly"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_butterfly.SetCallback(callbacks::SkinUpdate);
		quality_butterfly.AddShowCallback(callbacks::KNIFE_BUTTERFLY);
		RegisterElement(&quality_butterfly);

		seed_butterfly.setup(XOR("seed"), XOR("seed_butterfly"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_butterfly.SetCallback(callbacks::SkinUpdate);
		seed_butterfly.AddShowCallback(callbacks::KNIFE_BUTTERFLY);
		RegisterElement(&seed_butterfly);

		id_daggers.setup(XOR("paintkit id"), XOR("id_daggers"), 3);
		id_daggers.SetCallback(callbacks::SkinUpdate);
		id_daggers.AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS);
		RegisterElement(&id_daggers);

		stattrak_daggers.setup(XOR("stattrak"), XOR("stattrak_daggers"));
		stattrak_daggers.SetCallback(callbacks::SkinUpdate);
		stattrak_daggers.AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS);
		RegisterElement(&stattrak_daggers);

		quality_daggers.setup(XOR("quality"), XOR("quality_daggers"), 1.f, 100.f, true, 0, 100.f, 1.f, XOR(L"%"));
		quality_daggers.SetCallback(callbacks::SkinUpdate);
		quality_daggers.AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS);
		RegisterElement(&quality_daggers);

		seed_daggers.setup(XOR("seed"), XOR("seed_daggers"), 0.f, 255.f, true, 0, 0.f, 1.f);
		seed_daggers.SetCallback(callbacks::SkinUpdate);
		seed_daggers.AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS);
		RegisterElement(&seed_daggers);

		// col 2.
		knife.setup(XOR("knife model"), XOR("skins_knife_model"), { XOR("off"), XOR("bayonet"), XOR("bowie"), XOR("butterfly"), XOR("falchion"), XOR("flip"), XOR("gut"), XOR("huntsman"), XOR("karambit"), XOR("m9 bayonet"), XOR("daggers") });
		knife.SetCallback(callbacks::SkinUpdate);
		RegisterElement(&knife, 1);

		glove.setup(XOR("glove model"), XOR("skins_glove_model"), { XOR("off"), XOR("bloodhound"), XOR("sport"), XOR("driver"), XOR("handwraps"), XOR("moto"), XOR("specialist") });
		glove.SetCallback(callbacks::ForceFullUpdate);
		RegisterElement(&glove, 1);

		glove_id.setup(XOR("glove paintkit id"), XOR("skins_glove_id"), 2);
		glove_id.SetCallback(callbacks::ForceFullUpdate);
		RegisterElement(&glove_id, 1);
	}
};

class MiscTab : public Tab {
public:
	// col1.
	Checkbox           buy;
	MultiDropdown buy1;
	MultiDropdown buy2;
	MultiDropdown buy3;
	Checkbox           buy_money;
	Slider				   buy_money_amt;

	MultiDropdown notifications;
	Checkbox			  extend_logs;
	Keybind       last_tick_defuse;

	Checkbox    fake_latency_always;
	Keybind       fake_latency;
	Slider		  fake_latency_amt;

	Colorpicker console_color;
	Slider		   console_blend;

	// col2.
	Checkbox unlock;
	Checkbox hitmarker;
	MultiDropdown ragdoll_modifiers;
	Checkbox killfeed;
	Checkbox clantag;
	Edit			 clantag_text;
public:
	void init() {
		SetTitle(XOR("misc"));

		buy.setup(XOR("auto buy items"), XOR("auto_buy"));
		RegisterElement(&buy);

		buy1.setup("", XOR("auto_buy1"),
			{
				XOR("galilar"),
				XOR("famas"),
				XOR("ak47"),
				XOR("m4a1"),
				XOR("m4a1_silencer"),
				XOR("ssg08"),
				XOR("aug"),
				XOR("sg556"),
				XOR("awp"),
				XOR("scar20"),
				XOR("g3sg1"),
				XOR("nova"),
				XOR("xm1014"),
				XOR("mag7"),
				XOR("m249"),
				XOR("negev"),
				XOR("mac10"),
				XOR("mp9"),
				XOR("mp7"),
				XOR("ump45"),
				XOR("p90"),
				XOR("bizon"),
			}, false);
		buy1.AddShowCallback(callbacks::IsAutoBuyOn);
		RegisterElement(&buy1);

		buy2.setup("", XOR("auto_buy2"),
			{
				XOR("glock"),
				XOR("hkp2000"),
				XOR("usp_silencer"),
				XOR("elite"),
				XOR("p250"),
				XOR("tec9"),
				XOR("fn57"),
				XOR("deagle"),
			}, false);
		buy2.AddShowCallback(callbacks::IsAutoBuyOn);
		RegisterElement(&buy2);

		buy3.setup("", XOR("auto_buy3"),
			{
				XOR("vest"),
				XOR("vesthelm"),
				XOR("taser"),
				XOR("defuser"),
				XOR("molotov"),
				XOR("incgrenade"),
				XOR("decoy"),
				XOR("flashbang"),
				XOR("hegrenade"),
				XOR("smokegrenade"),
			}, false);
		buy3.AddShowCallback(callbacks::IsAutoBuyOn);
		RegisterElement(&buy3);
	
		buy_money.setup(XOR("money override"), XOR("buy_money"));
		buy_money.AddShowCallback(callbacks::IsAutoBuyOn);
		RegisterElement(&buy_money);

		buy_money_amt.setup("", XOR("buy_money_amt"), 0.f, 16000.f, false, 0, 16000.f, 500.f, XOR(L"$"));
		buy_money_amt.AddShowCallback(callbacks::IsAutoBuyOn);
		RegisterElement(&buy_money_amt);

		notifications.setup(XOR("notifications"), XOR("notifications"), { XOR("damage"), XOR("purchases"), XOR("bomb"), XOR("defuse"), XOR("spread") });
		RegisterElement(&notifications);

		extend_logs.setup( XOR("extend miss logs"), XOR("extend_logs"));
		extend_logs.AddShowCallback( callbacks::IsNotificationsOn );
		RegisterElement(&extend_logs);

		last_tick_defuse.setup(XOR("last tick defuse"), XOR("last_tick_defuse"));
		RegisterElement(&last_tick_defuse);

		fake_latency.setup(XOR("fake latency"), XOR("fake_latency"));
		fake_latency.SetToggleCallback(callbacks::ToggleFakeLatency);
		RegisterElement(&fake_latency);

		fake_latency_amt.setup("", XOR("fake_latency_amt"), 50.f, 800.f, false, 0, 200.f, 50.f, XOR(L"ms"));
		RegisterElement(&fake_latency_amt);

		fake_latency_always.setup(XOR("fake latency always on"), XOR("fake_latency_always"));
		RegisterElement(&fake_latency_always);

		console_color.setup(XOR("console color"), XOR("console_color"), colors::white);
		RegisterElement(&console_color);

		console_blend.setup("", XOR("console_blend"), 10.f, 100.f, false, 0, 65.f, 1.f, XOR(L"%"));
		RegisterElement(&console_blend);

		// col2.
		unlock.setup(XOR("unlock inventory in-game"), XOR("unlock_inventory"));
		RegisterElement(&unlock, 1);

		hitmarker.setup(XOR("hitmarker"), XOR("hitmarker"));
		RegisterElement(&hitmarker, 1);

		ragdoll_modifiers.setup(XOR("ragdoll modifiers"), XOR("ragdoll_modifers"), { XOR("force"), XOR("gravity") });
		RegisterElement(&ragdoll_modifiers, 1);

		killfeed.setup(XOR("preserve killfeed"), XOR("killfeed"));
		killfeed.SetCallback(callbacks::ToggleKillfeed);
		RegisterElement(&killfeed, 1);

		clantag.setup(XOR("clantag"), XOR("clantag"));
		RegisterElement(&clantag, 1);

		clantag_text.setup("", XOR("clantag_text"), 16, false, XOR("zephyr"), true);
		RegisterElement(&clantag_text, 1);
	}
};

class ConfigTab : public Tab {
public:
	Colorpicker menu_color;
	Dropdown mode;

	Dropdown config;
	Edit			  config_edit;
	Button   save;
	Button   load;
	Button   create;
	Button   remove;
	Button   refresh;

public:

	void init() {
		SetTitle(XOR("config"));

		// col1.
		menu_color.setup(XOR("menu color"), XOR("menu_color"), colors::burgundy, &g_gui.m_color);
		RegisterElement(&menu_color);

		mode.setup(XOR("safety mode"), XOR("mode"), { XOR("matchmaking"), XOR("no-spread") });
		RegisterElement(&mode);

		// col2.
		config.setup(XOR("configuration"), XOR("cfg"), { });
		config.RemoveFlags(ElementFlags::SAVE);
		RegisterElement(&config, 1);

		config_edit.setup("", XOR("config_edit"), 32u, false, "", true);
		config_edit.RemoveFlags(ElementFlags::SAVE);
		RegisterElement(&config_edit, 1);

		save.setup(XOR("save"));
		save.SetCallback(callbacks::ConfigSave);
		RegisterElement(&save, 1);

		load.setup(XOR("load"));
		load.SetCallback(callbacks::ConfigLoad);
		RegisterElement(&load, 1);

		create.setup(XOR("create"));
		create.SetCallback(callbacks::ConfigCreate);
		RegisterElement(&create, 1);

		remove.setup(XOR("delete"));
		remove.SetCallback(callbacks::ConfigDelete);
		RegisterElement(&remove, 1);

		refresh.setup(XOR("refresh"));
		refresh.SetCallback(callbacks::ConfigRefresh);
		RegisterElement(&refresh, 1);
	}
};

class MainForm : public Form {
public:
	// aimbot.
	AimbotTab    aimbot;
	AntiAimTab   antiaim;

	// visuals.
	PlayersTab	 players;
	VisualsTab	 visuals;

	// misc.
	MovementTab  movement;
	SkinsTab     skins;
	MiscTab	     misc;
	ConfigTab	 config;

public:
	void init() {
		SetPosition(50, 50);
		SetSize(630, 750);

		// aim.
		RegisterTab(&aimbot);
		aimbot.init();

		RegisterTab(&antiaim);
		antiaim.init();

		// visuals.
		RegisterTab(&players);
		players.init();

		RegisterTab(&visuals);
		visuals.init();

		// misc.
		RegisterTab(&movement);
		movement.init();

		RegisterTab(&skins);
		skins.init();

		RegisterTab(&misc);
		misc.init();

		RegisterTab(&config);
		config.init();
	}
};

class Menu {
public:
	MainForm main;

public:
	void init() {
		Colorpicker::init();

		main.init();
		g_gui.RegisterForm(&main, VK_INSERT);
	}
};

extern Menu g_menu;