#include "settings.h"

void GetVal(Json::Value &config, int* setting)
{
	if (config.isNull())
		return;

	*setting = config.asInt();
}

void GetVal(Json::Value &config, bool* setting)
{
	if (config.isNull())
		return;

	*setting = config.asBool();
}

void GetVal(Json::Value &config, float* setting)
{
	if (config.isNull())
		return;

	*setting = config.asFloat();
}

void GetVal(Json::Value &config, ImColor* setting)
{
	if (config.isNull())
		return;

	GetVal(config[XORSTR("r")], &setting->Value.x);
	GetVal(config[XORSTR("g")], &setting->Value.y);
	GetVal(config[XORSTR("b")], &setting->Value.z);
	GetVal(config[XORSTR("a")], &setting->Value.w);
}

void GetVal(Json::Value &config, char** setting)
{
	if (config.isNull())
		return;

	*setting = strdup(config.asCString());
}

void GetVal(Json::Value &config, char* setting)
{
	if (config.isNull())
		return;

	strcpy(setting, config.asCString());
}

void GetVal(Json::Value &config, ColorVar* setting)
{
	if (config.isNull())
		return;

	GetVal(config[XORSTR("r")], &setting->color.Value.x);
	GetVal(config[XORSTR("g")], &setting->color.Value.y);
	GetVal(config[XORSTR("b")], &setting->color.Value.z);
	GetVal(config[XORSTR("a")], &setting->color.Value.w);
	GetVal(config[XORSTR("rainbow")], &setting->rainbow);
	GetVal(config[XORSTR("rainbowSpeed")], &setting->rainbowSpeed);
}

void GetVal(Json::Value &config, HealthColorVar* setting)
{
	if (config.isNull())
		return;

	GetVal(config[XORSTR("r")], &setting->color.Value.x);
	GetVal(config[XORSTR("g")], &setting->color.Value.y);
	GetVal(config[XORSTR("b")], &setting->color.Value.z);
	GetVal(config[XORSTR("a")], &setting->color.Value.w);
	GetVal(config[XORSTR("rainbow")], &setting->rainbow);
	GetVal(config[XORSTR("rainbowSpeed")], &setting->rainbowSpeed);
	GetVal(config[XORSTR("hp")], &setting->hp);
}

template <typename Ord, Ord (*lookupFunction)(std::string)>
void GetOrdinal(Json::Value& config, Ord* setting)
{
	if (config.isNull())
		return;

	Ord value;
	if (config.isString())
		value = lookupFunction(config.asString());
	else
		value = (Ord) config.asInt();

	*setting = value;
}

void GetButtonCode(Json::Value &config, enum ButtonCode_t* setting)
{
	GetOrdinal<enum ButtonCode_t, Util::GetButtonCode>(config, setting);
}

void LoadColor(Json::Value &config, ImColor color)
{
	config[XORSTR("r")] = color.Value.x;
	config[XORSTR("g")] = color.Value.y;
	config[XORSTR("b")] = color.Value.z;
	config[XORSTR("a")] = color.Value.w;
}

void LoadColor(Json::Value &config, ColorVar color)
{
	config[XORSTR("r")] = color.color.Value.x;
	config[XORSTR("g")] = color.color.Value.y;
	config[XORSTR("b")] = color.color.Value.z;
	config[XORSTR("a")] = color.color.Value.w;
	config[XORSTR("rainbow")] = color.rainbow;
	config[XORSTR("rainbowSpeed")] = color.rainbowSpeed;
}

void LoadColor(Json::Value &config, HealthColorVar color)
{
	config[XORSTR("r")] = color.color.Value.x;
	config[XORSTR("g")] = color.color.Value.y;
	config[XORSTR("b")] = color.color.Value.z;
	config[XORSTR("a")] = color.color.Value.w;
	config[XORSTR("rainbow")] = color.rainbow;
	config[XORSTR("rainbowSpeed")] = color.rainbowSpeed;
	config[XORSTR("hp")] = color.hp;
}

void Settings::LoadDefaultsOrSave(std::string path)
{
	Json::Value settings;
	Json::StyledWriter styledWriter;

	LoadColor(settings[XORSTR("UI")][XORSTR("mainColor")], Settings::UI::mainColor);
	LoadColor(settings[XORSTR("UI")][XORSTR("bodyColor")], Settings::UI::bodyColor);
	LoadColor(settings[XORSTR("UI")][XORSTR("fontColor")], Settings::UI::fontColor);
	LoadColor(settings[XORSTR("UI")][XORSTR("accentColor")], Settings::UI::accentColor);
	settings[XORSTR("UI")][XORSTR("Fonts")][XORSTR("ESP")][XORSTR("family")] = Settings::UI::Fonts::ESP::family;
	settings[XORSTR("UI")][XORSTR("Fonts")][XORSTR("ESP")][XORSTR("size")] = Settings::UI::Fonts::ESP::size;
	settings[XORSTR("UI")][XORSTR("Fonts")][XORSTR("ESP")][XORSTR("flags")] = Settings::UI::Fonts::ESP::flags;

	for (auto i : Settings::Aimbot::weapons)
	{
		// TODO this is kind of a hack and i'm too tired to find a better way to do this
		// yes i tried defining a variable, skinSetting, and giving it the same value but woooooo operator overloading
		// in C++ and weird shit
		#define weaponSetting settings["Aimbot"]["weapons"][Util::Items::GetItemName((enum ItemDefinitionIndex) i.first)]
		weaponSetting["Enabled"] = i.second.enabled;
		weaponSetting["Silent"] = i.second.silent;
		weaponSetting["pSilent"] = i.second.pSilent;
		weaponSetting["Friendly"] = i.second.friendly;
		weaponSetting["ClosestBone"] = i.second.closestBone;
		weaponSetting["engageLock"] = i.second.engageLock;
		weaponSetting["engageLockTR"] = i.second.engageLockTR;
		weaponSetting["engageLockTTR"] = i.second.engageLockTTR;
		weaponSetting["TargetBone"] = (int) i.second.bone;
		weaponSetting["AimKey"] = Util::GetButtonName(i.second.aimkey);
		weaponSetting["AimKeyOnly"] = i.second.aimkeyOnly;
		weaponSetting["Smooth"]["Enabled"] = i.second.smoothEnabled;
		weaponSetting["Smooth"]["Amount"] = i.second.smoothAmount;
		weaponSetting["Smooth"]["Type"] = (int) i.second.smoothType;
		weaponSetting["Smooth"]["Salting"]["Enabled"] = i.second.smoothSaltEnabled;
		weaponSetting["Smooth"]["Salting"]["Multiplier"] = i.second.smoothSaltMultiplier;
		weaponSetting["ErrorMargin"]["Enabled"] = i.second.errorMarginEnabled;
		weaponSetting["ErrorMargin"]["Value"] = i.second.errorMarginValue;
		weaponSetting["Curve"]["Enabled"] = i.second.curveEnabled;
		weaponSetting["Curve"]["Amount"] = i.second.curveAmount;
		weaponSetting["AutoAim"]["Enabled"] = i.second.autoAimEnabled;
		weaponSetting["AutoAim"]["FOV"] = i.second.autoAimFov;
		weaponSetting["AimStep"]["Enabled"] = i.second.aimStepEnabled;
		weaponSetting["AimStep"]["Amount"] = i.second.aimStepValue;
		weaponSetting["RCS"]["Enabled"] = i.second.rcsEnabled;
		weaponSetting["RCS"]["AlwaysOn"] = i.second.rcsAlwaysOn;
		weaponSetting["RCS"]["AmountX"] = i.second.rcsAmountX;
		weaponSetting["RCS"]["AmountY"] = i.second.rcsAmountY;
		weaponSetting["AutoCockRevolver"]["Enabled"] = i.second.autoCockRevolver;
		weaponSetting["AutoPistol"]["Enabled"] = i.second.autoPistolEnabled;
		weaponSetting["AutoShoot"]["Enabled"] = i.second.autoShootEnabled;
		weaponSetting["AutoScope"]["Enabled"] = i.second.autoScopeEnabled;
		weaponSetting["NoShoot"]["Enabled"] = i.second.noShootEnabled;
		weaponSetting["IgnoreJump"]["Enabled"] = i.second.ignoreJumpEnabled;
		weaponSetting["SmokeCheck"]["Enabled"] = i.second.smokeCheck;
		weaponSetting["FlashCheck"]["Enabled"] = i.second.flashCheck;
		weaponSetting["SpreadLimit"]["Enabled"] = i.second.spreadLimitEnabled;
		weaponSetting["SpreadLimit"]["DistanceBased"] = i.second.spreadLimitDistance;
		weaponSetting["SpreadLimit"]["Value"] = i.second.spreadLimit;
		weaponSetting["AutoWall"]["Enabled"] = i.second.autoWallEnabled;
		weaponSetting["AutoWall"]["Value"] = i.second.autoWallValue;
		weaponSetting["AutoSlow"]["enabled"] = i.second.autoSlow;
		weaponSetting["Prediction"]["enabled"] = i.second.predEnabled;
		weaponSetting["moveMouse"] = i.second.moveMouse;
		weaponSetting["HitChance"]["enabled"] = i.second.hitChanceEnabled;
		weaponSetting["HitChance"]["hitRays"] = i.second.hitChanceRays;
		weaponSetting["HitChance"]["value"] = i.second.hitChanceValue;

		for (int bone = (int) Hitbox::HITBOX_HEAD; bone <= (int) Hitbox::HITBOX_ARMS; bone++)
			weaponSetting["AutoWall"]["Bones"][bone] = i.second.autoWallBones[bone];

		for (int bone = (int) DesiredBones::BONE_PELVIS; bone <= (int) DesiredBones::BONE_RIGHT_SOLE; bone++)
			weaponSetting["DesiredBones"]["Bones"][bone] = i.second.desiredBones[bone];

		weaponSetting["AutoAim"]["RealDistance"] = i.second.autoAimRealDistance;

		#undef weaponSetting
	}

	settings["Aimbot"]["AutoCrouch"]["enabled"] = Settings::Aimbot::AutoCrouch::enabled;
	settings["Aimbot"]["AutoShoot"]["velocityCheck"] = Settings::Aimbot::AutoShoot::velocityCheck;

	settings[XORSTR("Resolver")][XORSTR("ticks")] = Settings::Resolver::ticks;
	settings[XORSTR("Resolver")][XORSTR("modulo")] = Settings::Resolver::modulo;
	settings[XORSTR("Resolver")][XORSTR("mode")] = (int) Settings::Resolver::mode;


	settings[XORSTR("Triggerbot")][XORSTR("enabled")] = Settings::Triggerbot::enabled;
	settings[XORSTR("Triggerbot")][XORSTR("key")] = Util::GetButtonName(Settings::Triggerbot::key);
	settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("enemies")] = Settings::Triggerbot::Filters::enemies;
	settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("allies")] = Settings::Triggerbot::Filters::allies;
	settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("walls")] = Settings::Triggerbot::Filters::walls;
	settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("smoke_check")] = Settings::Triggerbot::Filters::smokeCheck;
	settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("flash_check")] = Settings::Triggerbot::Filters::flashCheck;
	settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("head")] = Settings::Triggerbot::Filters::head;
	settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("chest")] = Settings::Triggerbot::Filters::chest;
	settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("stomach")] = Settings::Triggerbot::Filters::stomach;
	settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("arms")] = Settings::Triggerbot::Filters::arms;
	settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("legs")] = Settings::Triggerbot::Filters::legs;
	settings[XORSTR("Triggerbot")][XORSTR("RandomDelay")][XORSTR("enabled")] = Settings::Triggerbot::RandomDelay::enabled;
	settings[XORSTR("Triggerbot")][XORSTR("RandomDelay")][XORSTR("lowBound")] = Settings::Triggerbot::RandomDelay::lowBound;
	settings[XORSTR("Triggerbot")][XORSTR("RandomDelay")][XORSTR("highBound")] = Settings::Triggerbot::RandomDelay::highBound;

	settings[XORSTR("AntiAim")][XORSTR("Yaw")][XORSTR("enabled")] = Settings::AntiAim::Yaw::enabled;
	settings[XORSTR("AntiAim")][XORSTR("Yaw")][XORSTR("type")] = (int) Settings::AntiAim::Yaw::type;
	settings[XORSTR("AntiAim")][XORSTR("Yaw")][XORSTR("type_fake")] = (int) Settings::AntiAim::Yaw::typeFake;
	settings[XORSTR("AntiAim")][XORSTR("Yaw")][XORSTR("antiResolver")] = Settings::AntiAim::Yaw::antiResolver;
	settings[XORSTR("AntiAim")][XORSTR("Yaw")][XORSTR("dynamic")] = Settings::AntiAim::Yaw::dynamic;
	settings[XORSTR("AntiAim")][XORSTR("Pitch")][XORSTR("enabled")] = Settings::AntiAim::Pitch::enabled;
	settings[XORSTR("AntiAim")][XORSTR("Pitch")][XORSTR("type")] = (int) Settings::AntiAim::Pitch::type;
	settings[XORSTR("AntiAim")][XORSTR("HeadEdge")][XORSTR("enabled")] = Settings::AntiAim::HeadEdge::enabled;
	settings[XORSTR("AntiAim")][XORSTR("HeadEdge")][XORSTR("distance")] = Settings::AntiAim::HeadEdge::distance;
	settings[XORSTR("AntiAim")][XORSTR("AutoDisable")][XORSTR("no_enemy")] = Settings::AntiAim::AutoDisable::noEnemy;
	settings[XORSTR("AntiAim")][XORSTR("AutoDisable")][XORSTR("knife_held")] = Settings::AntiAim::AutoDisable::knifeHeld;
	settings[XORSTR("AntiAim")][XORSTR("MoonWalk")] = Settings::AntiAim::moonwalk;
	
	settings[XORSTR("AntiAim")][XORSTR("Lua")][XORSTR("debugMode")] = Settings::AntiAim::Lua::debugMode;
	settings[XORSTR("AntiAim")][XORSTR("Lua")][XORSTR("scriptX")] = Settings::AntiAim::Lua::scriptX;
	settings[XORSTR("AntiAim")][XORSTR("Lua")][XORSTR("scriptY")] = Settings::AntiAim::Lua::scriptY;
	settings[XORSTR("AntiAim")][XORSTR("Lua")][XORSTR("scriptY2")] = Settings::AntiAim::Lua::scriptY2;
	
	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("mode")] = (int) Settings::AntiAim::Custom::YawOne::mode;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("angle")] = Settings::AntiAim::Custom::YawOne::angle;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("jitter_min")] = Settings::AntiAim::Custom::YawOne::jitterMin;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("jitter_max")] = Settings::AntiAim::Custom::YawOne::jitterMax;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("spin_factor")] = Settings::AntiAim::Custom::YawOne::spinFactor;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("is_static")] = Settings::AntiAim::Custom::YawOne::isStatic;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("slow_spin")] = Settings::AntiAim::Custom::YawOne::slowSpin;

    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("mode")] = (int) Settings::AntiAim::Custom::YawTwo::mode;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("angle")] = Settings::AntiAim::Custom::YawTwo::angle;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("jitter_min")] = Settings::AntiAim::Custom::YawTwo::jitterMin;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("jitter_max")] = Settings::AntiAim::Custom::YawTwo::jitterMax;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("spin_factor")] = Settings::AntiAim::Custom::YawTwo::spinFactor;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("is_static")] = Settings::AntiAim::Custom::YawTwo::isStatic;
    	settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("slow_spin")] = Settings::AntiAim::Custom::YawTwo::slowSpin;

	settings[XORSTR("ESP")][XORSTR("enabled")] = Settings::ESP::enabled;
	settings[XORSTR("ESP")][XORSTR("key")] = Util::GetButtonName(Settings::ESP::key);
	LoadColor(settings[XORSTR("ESP")][XORSTR("enemy_color")], Settings::ESP::enemyColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("enemy_visible_color")], Settings::ESP::enemyVisibleColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("ally_color")], Settings::ESP::allyColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("ally_visible_color")], Settings::ESP::allyVisibleColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("t_color")], Settings::ESP::tColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("t_visible_color")], Settings::ESP::tVisibleColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("ct_color")], Settings::ESP::ctColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("ct_visible_color")], Settings::ESP::ctVisibleColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("localplayer_color")], Settings::ESP::localplayerColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("bomb_color")], Settings::ESP::bombColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("bomb_defusing_color")], Settings::ESP::bombDefusingColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("hostage_color")], Settings::ESP::hostageColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("defuser_color")], Settings::ESP::defuserColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("weapon_color")], Settings::ESP::weaponColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("chicken_color")], Settings::ESP::chickenColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("fish_color")], Settings::ESP::fishColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("smoke_color")], Settings::ESP::smokeColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("decoy_color")], Settings::ESP::decoyColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("flashbang_color")], Settings::ESP::flashbangColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("grenade_color")], Settings::ESP::grenadeColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("molotov_color")], Settings::ESP::molotovColor);
	settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("enabled")] = Settings::ESP::Glow::enabled;
	LoadColor(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("ally_color")], Settings::ESP::Glow::allyColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("enemy_color")], Settings::ESP::Glow::enemyColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("enemy_visible_color")], Settings::ESP::Glow::enemyVisibleColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("localplayer_color")], Settings::ESP::Glow::localplayerColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("weapon_color")], Settings::ESP::Glow::weaponColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("grenade_color")], Settings::ESP::Glow::grenadeColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("defuser_color")], Settings::ESP::Glow::defuserColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("chicken_color")], Settings::ESP::Glow::chickenColor);
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("legit")] = Settings::ESP::Filters::legit;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("visibility_check")] = Settings::ESP::Filters::visibilityCheck;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("smoke_check")] = Settings::ESP::Filters::smokeCheck;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("enemies")] = Settings::ESP::Filters::enemies;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("allies")] = Settings::ESP::Filters::allies;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("bomb")] = Settings::ESP::Filters::bomb;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("hostages")] = Settings::ESP::Filters::hostages;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("defusers")] = Settings::ESP::Filters::defusers;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("weapons")] = Settings::ESP::Filters::weapons;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("chickens")] = Settings::ESP::Filters::chickens;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("fishes")] = Settings::ESP::Filters::fishes;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("throwables")] = Settings::ESP::Filters::throwables;
	settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("localplayer")] = Settings::ESP::Filters::localplayer;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("name")] = Settings::ESP::Info::name;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("clan")] = Settings::ESP::Info::clan;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("steam_id")] = Settings::ESP::Info::steamId;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("rank")] = Settings::ESP::Info::rank;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("health")] = Settings::ESP::Info::health;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("weapon")] = Settings::ESP::Info::weapon;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("scoped")] = Settings::ESP::Info::scoped;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("reloading")] = Settings::ESP::Info::reloading;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("flashed")] = Settings::ESP::Info::flashed;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("planting")] = Settings::ESP::Info::planting;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("has_defuser")] = Settings::ESP::Info::hasDefuser;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("defusing")] = Settings::ESP::Info::defusing;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("grabbing_hostage")] = Settings::ESP::Info::grabbingHostage;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("rescuing")] = Settings::ESP::Info::rescuing;
	settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("location")] = Settings::ESP::Info::location;
	settings[XORSTR("ESP")][XORSTR("Boxes")][XORSTR("enabled")] = Settings::ESP::Boxes::enabled;
	settings[XORSTR("ESP")][XORSTR("Boxes")][XORSTR("type")] = (int) Settings::ESP::Boxes::type;
	settings[XORSTR("ESP")][XORSTR("Skeleton")][XORSTR("enabled")] = Settings::ESP::Skeleton::enabled;
	LoadColor(settings[XORSTR("ESP")][XORSTR("Skeleton")][XORSTR("color")], Settings::ESP::Skeleton::color);
	settings[XORSTR("ESP")][XORSTR("Bars")][XORSTR("enabled")] = Settings::ESP::Bars::enabled;
	settings[XORSTR("ESP")][XORSTR("Bars")][XORSTR("color_type")] = (int) Settings::ESP::Bars::colorType;
	settings[XORSTR("ESP")][XORSTR("Bars")][XORSTR("type")] = (int) Settings::ESP::Bars::type;
	settings[XORSTR("ESP")][XORSTR("Tracers")][XORSTR("enabled")] = Settings::ESP::Tracers::enabled;
	settings[XORSTR("ESP")][XORSTR("Tracers")][XORSTR("type")] = (int) Settings::ESP::Tracers::type;
	settings[XORSTR("ESP")][XORSTR("BulletTracers")][XORSTR("enabled")] = Settings::ESP::BulletTracers::enabled;
	settings[XORSTR("ESP")][XORSTR("FOVCrosshair")][XORSTR("enabled")] = Settings::ESP::FOVCrosshair::enabled;
	settings[XORSTR("ESP")][XORSTR("FOVCrosshair")][XORSTR("filled")] = Settings::ESP::FOVCrosshair::filled;
	LoadColor(settings[XORSTR("ESP")][XORSTR("FOVCrosshair")][XORSTR("color")], Settings::ESP::FOVCrosshair::color);
	
	settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("Arms")][XORSTR("enabled")] = Settings::ESP::Chams::Arms::enabled;
	settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("Arms")][XORSTR("type")] = (int) Settings::ESP::Chams::Arms::type;
	settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("Weapon")][XORSTR("enabled")] = Settings::ESP::Chams::Weapon::enabled;
	LoadColor(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("Weapon")][XORSTR("color")], Settings::ESP::Chams::Weapon::color);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("Arms")][XORSTR("color")], Settings::ESP::Chams::Arms::color);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("players_ally_color")], Settings::ESP::Chams::allyColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("players_ally_visible_color")], Settings::ESP::Chams::allyVisibleColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("players_enemy_color")], Settings::ESP::Chams::enemyColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("players_enemy_visible_color")], Settings::ESP::Chams::enemyVisibleColor);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("localplayer_color")], Settings::ESP::Chams::localplayerColor);
	settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("type")] = (int) Settings::ESP::Chams::type;
	settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("enabled")] = Settings::ESP::Chams::enabled;
	settings[XORSTR("ESP")][XORSTR("Sounds")][XORSTR("enabled")] = Settings::ESP::Sounds::enabled;
	settings[XORSTR("ESP")][XORSTR("Sounds")][XORSTR("time")] = Settings::ESP::Sounds::time;
	settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("enabled")] = Settings::ESP::Hitmarker::enabled;
	settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("enemies")] = Settings::ESP::Hitmarker::enemies;
	settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("allies")] = Settings::ESP::Hitmarker::allies;
	LoadColor(settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("color")], Settings::ESP::Hitmarker::color);
	settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("duration")] = Settings::ESP::Hitmarker::duration;
	settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("size")] = Settings::ESP::Hitmarker::size;
	settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("inner_gap")] = Settings::ESP::Hitmarker::innerGap;
	settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("Damage")][XORSTR("enabled")] = Settings::ESP::Hitmarker::Damage::enabled;
	settings[XORSTR("ESP")][XORSTR("HeadDot")][XORSTR("enabled")] = Settings::ESP::HeadDot::enabled;
	settings[XORSTR("ESP")][XORSTR("HeadDot")][XORSTR("size")] = Settings::ESP::HeadDot::size;
	settings[XORSTR("ESP")][XORSTR("Spread")][XORSTR("enabled")] = Settings::ESP::Spread::enabled;
	settings[XORSTR("ESP")][XORSTR("Spread")][XORSTR("spreadLimit")] = Settings::ESP::Spread::spreadLimit;
	LoadColor(settings[XORSTR("ESP")][XORSTR("Spread")][XORSTR("color")], Settings::ESP::Spread::color);
	LoadColor(settings[XORSTR("ESP")][XORSTR("Spread")][XORSTR("spreadLimitColor")], Settings::ESP::Spread::spreadLimitColor);

	settings[XORSTR("Dlights")][XORSTR("enabled")] = Settings::Dlights::enabled;
	settings[XORSTR("Dlights")][XORSTR("radius")] = Settings::Dlights::radius;
	settings[XORSTR("TracerEffects")][XORSTR("enabled")] = Settings::TracerEffects::enabled;
	settings[XORSTR("TracerEffects")][XORSTR("serverSide")] = Settings::TracerEffects::serverSide;
	settings[XORSTR("TracerEffects")][XORSTR("effect")] = (int) Settings::TracerEffects::effect;
	settings[XORSTR("TracerEffects")][XORSTR("frequency")] = Settings::TracerEffects::frequency;
	

	settings[XORSTR("Spammer")][XORSTR("spammer_type")] = (int) Settings::Spammer::type;
	settings[XORSTR("Spammer")][XORSTR("say_team")] = Settings::Spammer::say_team;

	settings[XORSTR("Spammer")][XORSTR("KillSpammer")][XORSTR("enabled")] = Settings::Spammer::KillSpammer::enabled;
	settings[XORSTR("Spammer")][XORSTR("KillSpammer")][XORSTR("say_team")] = Settings::Spammer::KillSpammer::sayTeam;
	Json::Value killSpammerMessages;
	for (auto it : Settings::Spammer::KillSpammer::messages)
		killSpammerMessages.append(it);
	settings[XORSTR("Spammer")][XORSTR("KillSpammer")][XORSTR("messages")] = killSpammerMessages;

	Json::Value normalSpammerMessages;
	for (auto it : Settings::Spammer::NormalSpammer::messages)
		normalSpammerMessages.append(it);
	settings[XORSTR("Spammer")][XORSTR("NormalSpammer")][XORSTR("messages")] = normalSpammerMessages;

	settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_name")] = Settings::Spammer::PositionSpammer::showName;
	settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_weapon")] = Settings::Spammer::PositionSpammer::showWeapon;
	settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_rank")] = Settings::Spammer::PositionSpammer::showRank;
	settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_wins")] = Settings::Spammer::PositionSpammer::showWins;
	settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_health")] = Settings::Spammer::PositionSpammer::showHealth;
	settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_money")] = Settings::Spammer::PositionSpammer::showMoney;
	settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_lastplace")] = Settings::Spammer::PositionSpammer::showLastplace;

	settings[XORSTR("BHop")][XORSTR("enabled")] = Settings::BHop::enabled;
	settings[XORSTR("BHop")][XORSTR("Chance")][XORSTR("enabled")] = Settings::BHop::Chance::enabled;
	settings[XORSTR("BHop")][XORSTR("Chance")][XORSTR("value")] = Settings::BHop::Chance::value;
	settings[XORSTR("BHop")][XORSTR("Hops")][XORSTR("enabledMax")] = Settings::BHop::Hops::enabledMax;
	settings[XORSTR("BHop")][XORSTR("Hops")][XORSTR("Max")] = Settings::BHop::Hops::Max;
	settings[XORSTR("BHop")][XORSTR("Hops")][XORSTR("enabledMin")] = Settings::BHop::Hops::enabledMin;
	settings[XORSTR("BHop")][XORSTR("Hops")][XORSTR("Min")] = Settings::BHop::Hops::Min;

	settings[XORSTR("AutoStrafe")][XORSTR("enabled")] = Settings::AutoStrafe::enabled;
	settings[XORSTR("AutoStrafe")][XORSTR("type")] = (int) Settings::AutoStrafe::type;
	settings[XORSTR("AutoStrafe")][XORSTR("silent")] = Settings::AutoStrafe::silent;

	settings[XORSTR("Noflash")][XORSTR("enabled")] = Settings::Noflash::enabled;
	settings[XORSTR("Noflash")][XORSTR("value")] = Settings::Noflash::value;

	settings[XORSTR("Radar")][XORSTR("enabled")] = Settings::Radar::enabled;
	settings[XORSTR("Radar")][XORSTR("zoom")] = Settings::Radar::zoom;
	settings[XORSTR("Radar")][XORSTR("enemies")] = Settings::Radar::enemies;
	settings[XORSTR("Radar")][XORSTR("allies")] = Settings::Radar::allies;
	settings[XORSTR("Radar")][XORSTR("legit")] = Settings::Radar::legit;
	settings[XORSTR("Radar")][XORSTR("visibility_check")] = Settings::Radar::visibilityCheck;
	settings[XORSTR("Radar")][XORSTR("smoke_check")] = Settings::Radar::smokeCheck;
	settings[XORSTR("Radar")][XORSTR("InGame")][XORSTR("enabled")] = Settings::Radar::InGame::enabled;
	LoadColor(settings[XORSTR("Radar")][XORSTR("enemy_color")], Settings::Radar::enemyColor);
	LoadColor(settings[XORSTR("Radar")][XORSTR("enemy_visible_color")], Settings::Radar::enemyVisibleColor);
	LoadColor(settings[XORSTR("Radar")][XORSTR("ally_color")], Settings::Radar::allyColor);
	LoadColor(settings[XORSTR("Radar")][XORSTR("ally_visible_color")], Settings::Radar::allyVisibleColor);
	LoadColor(settings[XORSTR("Radar")][XORSTR("t_color")], Settings::Radar::tColor);
	LoadColor(settings[XORSTR("Radar")][XORSTR("t_visible_color")], Settings::Radar::tVisibleColor);
	LoadColor(settings[XORSTR("Radar")][XORSTR("ct_color")], Settings::Radar::ctColor);
	LoadColor(settings[XORSTR("Radar")][XORSTR("ct_visible_color")], Settings::Radar::ctVisibleColor);
	LoadColor(settings[XORSTR("Radar")][XORSTR("bomb_color")], Settings::Radar::bombColor);
	LoadColor(settings[XORSTR("Radar")][XORSTR("bomb_defusing_color")], Settings::Radar::bombDefusingColor);
	settings[XORSTR("Radar")][XORSTR("icons_scale")] = Settings::Radar::iconsScale;

	settings[XORSTR("Recoilcrosshair")][XORSTR("enabled")] = Settings::Recoilcrosshair::enabled;

	settings[XORSTR("Recoilcrosshair")][XORSTR("showOnlyWhenShooting")] = Settings::Recoilcrosshair::showOnlyWhenShooting;

	settings[XORSTR("FOVChanger")][XORSTR("enabled")] = Settings::FOVChanger::enabled;
	settings[XORSTR("FOVChanger")][XORSTR("value")] = Settings::FOVChanger::value;
	settings[XORSTR("FOVChanger")][XORSTR("viewmodel_enabled")] = Settings::FOVChanger::viewmodelEnabled;
	settings[XORSTR("FOVChanger")][XORSTR("viewmodel_value")] = Settings::FOVChanger::viewmodelValue;
	settings[XORSTR("FOVChanger")][XORSTR("ignore_scope")] = Settings::FOVChanger::ignoreScope;

	settings[XORSTR("Airstuck")][XORSTR("enabled")] = Settings::Airstuck::enabled;
	settings[XORSTR("Airstuck")][XORSTR("key")] = Util::GetButtonName(Settings::Airstuck::key);

	settings[XORSTR("SkinChanger")][XORSTR("Skins")][XORSTR("enabled")] = Settings::Skinchanger::Skins::enabled;
	settings[XORSTR("SkinChanger")][XORSTR("Models")][XORSTR("enabled")] = Settings::Skinchanger::Models::enabled;
	settings[XORSTR("SkinChanger")][XORSTR("Skins")][XORSTR("perTeam")] = Settings::Skinchanger::Skins::perTeam;

	for (const auto& item: Settings::Skinchanger::skinsCT)
	{
		const AttribItem_t& skin = item.second;

		#define skinSetting settings[XORSTR("SkinChanger")][XORSTR("skinsCT")][Util::Items::GetItemConfigEntityName(item.first)]
		skinSetting[XORSTR("ItemDefinitionIndex")] = Util::Items::GetItemConfigEntityName(skin.itemDefinitionIndex);
		skinSetting[XORSTR("PaintKit")] = skin.fallbackPaintKit;
		skinSetting[XORSTR("Wear")] = skin.fallbackWear;
		skinSetting[XORSTR("Seed")] = skin.fallbackSeed;
		skinSetting[XORSTR("StatTrak")] = skin.fallbackStatTrak;
		skinSetting[XORSTR("CustomName")] = skin.customName;
		#undef skinSetting
	}

	for (const auto& item: Settings::Skinchanger::skinsT)
	{
		const AttribItem_t& skin = item.second;

		#define skinSetting settings[XORSTR("SkinChanger")][XORSTR("skinsT")][Util::Items::GetItemConfigEntityName(item.first)]
		skinSetting[XORSTR("ItemDefinitionIndex")] = Util::Items::GetItemConfigEntityName(skin.itemDefinitionIndex);
		skinSetting[XORSTR("PaintKit")] = skin.fallbackPaintKit;
		skinSetting[XORSTR("Wear")] = skin.fallbackWear;
		skinSetting[XORSTR("Seed")] = skin.fallbackSeed;
		skinSetting[XORSTR("StatTrak")] = skin.fallbackStatTrak;
		skinSetting[XORSTR("CustomName")] = skin.customName;
		#undef skinSetting
	}

	settings[XORSTR("ShowRanks")][XORSTR("enabled")] = Settings::ShowRanks::enabled;

	settings[XORSTR("ShowSpectators")][XORSTR("enabled")] = Settings::ShowSpectators::enabled;

	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Colors")][XORSTR("posX")] = Settings::UI::Windows::Colors::posX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Colors")][XORSTR("posY")] = Settings::UI::Windows::Colors::posY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Colors")][XORSTR("sizeX")] = Settings::UI::Windows::Colors::sizeX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Colors")][XORSTR("sizeY")] = Settings::UI::Windows::Colors::sizeY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Colors")][XORSTR("open")] = Settings::UI::Windows::Colors::open;

	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Config")][XORSTR("posX")] = Settings::UI::Windows::Config::posX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Config")][XORSTR("posY")] = Settings::UI::Windows::Config::posY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Config")][XORSTR("sizeX")] = Settings::UI::Windows::Config::sizeX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Config")][XORSTR("sizeY")] = Settings::UI::Windows::Config::sizeY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Config")][XORSTR("open")] = Settings::UI::Windows::Config::open;

	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Main")][XORSTR("posX")] = Settings::UI::Windows::Main::posX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Main")][XORSTR("posY")] = Settings::UI::Windows::Main::posY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Main")][XORSTR("sizeX")] = Settings::UI::Windows::Main::sizeX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Main")][XORSTR("sizeY")] = Settings::UI::Windows::Main::sizeY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Main")][XORSTR("open")] = Settings::UI::Windows::Main::open;

	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Playerlist")][XORSTR("posX")] = Settings::UI::Windows::Playerlist::posX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Playerlist")][XORSTR("posY")] = Settings::UI::Windows::Playerlist::posY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Playerlist")][XORSTR("sizeX")] = Settings::UI::Windows::Playerlist::sizeX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Playerlist")][XORSTR("sizeY")] = Settings::UI::Windows::Playerlist::sizeY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Playerlist")][XORSTR("open")] = Settings::UI::Windows::Playerlist::open;

	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Skinmodel")][XORSTR("posX")] = Settings::UI::Windows::Skinmodel::posX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Skinmodel")][XORSTR("posY")] = Settings::UI::Windows::Skinmodel::posY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Skinmodel")][XORSTR("sizeX")] = Settings::UI::Windows::Skinmodel::sizeX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Skinmodel")][XORSTR("sizeY")] = Settings::UI::Windows::Skinmodel::sizeY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Skinmodel")][XORSTR("open")] = Settings::UI::Windows::Skinmodel::open;

	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Spectators")][XORSTR("posX")] = Settings::UI::Windows::Spectators::posX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Spectators")][XORSTR("posY")] = Settings::UI::Windows::Spectators::posY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Spectators")][XORSTR("sizeX")] = Settings::UI::Windows::Spectators::sizeX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Spectators")][XORSTR("sizeY")] = Settings::UI::Windows::Spectators::sizeY;

	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Walkbot")][XORSTR("posX")] = Settings::UI::Windows::Walkbot::posX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Walkbot")][XORSTR("posY")] = Settings::UI::Windows::Walkbot::posY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Walkbot")][XORSTR("sizeX")] = Settings::UI::Windows::Walkbot::sizeX;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Walkbot")][XORSTR("sizeY")] = Settings::UI::Windows::Walkbot::sizeY;
	settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Walkbot")][XORSTR("open")] = Settings::UI::Windows::Walkbot::open;


	settings[XORSTR("ClanTagChanger")][XORSTR("value")] = Settings::ClanTagChanger::value;
	settings[XORSTR("ClanTagChanger")][XORSTR("enabled")] = Settings::ClanTagChanger::enabled;
	settings[XORSTR("ClanTagChanger")][XORSTR("animation")] = Settings::ClanTagChanger::animation;
	settings[XORSTR("ClanTagChanger")][XORSTR("animation_speed")] = Settings::ClanTagChanger::animationSpeed;
	settings[XORSTR("ClanTagChanger")][XORSTR("type")] = (int) Settings::ClanTagChanger::type;

	settings[XORSTR("View")][XORSTR("NoViewPunch")][XORSTR("enabled")] = Settings::View::NoViewPunch::enabled;
	settings[XORSTR("View")][XORSTR("NoAimPunch")][XORSTR("enabled")] = Settings::View::NoAimPunch::enabled;

	settings[XORSTR("FakeLag")][XORSTR("enabled")] = Settings::FakeLag::enabled;
	settings[XORSTR("FakeLag")][XORSTR("value")] = Settings::FakeLag::value;
	settings[XORSTR("FakeLag")][XORSTR("adaptive")] = Settings::FakeLag::adaptive;

	settings[XORSTR("AutoAccept")][XORSTR("enabled")] = Settings::AutoAccept::enabled;

	settings[XORSTR("NoSky")][XORSTR("enabled")] = Settings::NoSky::enabled;
	LoadColor(settings[XORSTR("NoSky")][XORSTR("color")], Settings::NoSky::color);

	settings[XORSTR("SkyChanger")][XORSTR("enabled")] = Settings::SkyChanger::enabled;
	settings[XORSTR("SkyChanger")][XORSTR("skyName")] = Settings::SkyChanger::skyName;

	settings[XORSTR("ASUSWalls")][XORSTR("enabled")] = Settings::ASUSWalls::enabled;
	LoadColor(settings[XORSTR("ASUSWalls")][XORSTR("color")], Settings::ASUSWalls::color);

	settings[XORSTR("NoScopeBorder")][XORSTR("enabled")] = Settings::NoScopeBorder::enabled;

	settings[XORSTR("SniperCrosshair")][XORSTR("enabled")] = Settings::SniperCrosshair::enabled;

	settings[XORSTR("Autoblock")][XORSTR("enabled")] = Settings::Autoblock::enabled;
	settings[XORSTR("Autoblock")][XORSTR("key")] = Settings::Autoblock::key;

	settings[XORSTR("AutoDefuse")][XORSTR("enabled")] = Settings::AutoDefuse::enabled;
	settings[XORSTR("AutoDefuse")][XORSTR("silent")] = Settings::AutoDefuse::silent;

	settings[XORSTR("NoSmoke")][XORSTR("enabled")] = Settings::NoSmoke::enabled;

	settings[XORSTR("ScreenshotCleaner")][XORSTR("enabled")] = Settings::ScreenshotCleaner::enabled;

	settings[XORSTR("EdgeJump")][XORSTR("enabled")] = Settings::EdgeJump::enabled;
	settings[XORSTR("EdgeJump")][XORSTR("key")] = Util::GetButtonName(Settings::EdgeJump::key);

	settings[XORSTR("NameStealer")][XORSTR("enabled")] = Settings::NameStealer::enabled;
	settings[XORSTR("NameStealer")][XORSTR("team")] = Settings::NameStealer::team;

	settings[XORSTR("ThirdPerson")][XORSTR("enabled")] = Settings::ThirdPerson::enabled;
	settings[XORSTR("ThirdPerson")][XORSTR("distance")] = Settings::ThirdPerson::distance;

	settings[XORSTR("JumpThrow")][XORSTR("enabled")] = Settings::JumpThrow::enabled;
	settings[XORSTR("JumpThrow")][XORSTR("key")] = Util::GetButtonName(Settings::JumpThrow::key);

	settings[XORSTR("DisablePostProcessing")][XORSTR("enabled")] = Settings::DisablePostProcessing::enabled;

	// settings[XORSTR("WalkBot")][XORSTR("enabled")] = Settings::WalkBot::enabled;
	settings[XORSTR("WalkBot")][XORSTR("autobuy")] = Settings::WalkBot::autobuy;
	settings[XORSTR("WalkBot")][XORSTR("autobuyAt")] = Settings::WalkBot::autobuyAt;

	settings[XORSTR("GrenadeHelper")][XORSTR("enabled")] = Settings::GrenadeHelper::enabled;
	settings[XORSTR("GrenadeHelper")][XORSTR("aimAssist")] = Settings::GrenadeHelper::aimAssist;
	settings[XORSTR("GrenadeHelper")][XORSTR("OnlyMatching")] = Settings::GrenadeHelper::onlyMatchingInfos;
	settings[XORSTR("GrenadeHelper")][XORSTR("aimStep")] = Settings::GrenadeHelper::aimStep;
	settings[XORSTR("GrenadeHelper")][XORSTR("aimDistance")] = Settings::GrenadeHelper::aimDistance;
	settings[XORSTR("GrenadeHelper")][XORSTR("aimFov")] = Settings::GrenadeHelper::aimFov;
	LoadColor(settings[XORSTR("GrenadeHelper")][XORSTR("aimDot")], Settings::GrenadeHelper::aimDot);
	LoadColor(settings[XORSTR("GrenadeHelper")][XORSTR("aimLine")], Settings::GrenadeHelper::aimLine);
	LoadColor(settings[XORSTR("GrenadeHelper")][XORSTR("infoHe")], Settings::GrenadeHelper::infoHE);
	LoadColor(settings[XORSTR("GrenadeHelper")][XORSTR("infoSmoke")], Settings::GrenadeHelper::infoSmoke);
	LoadColor(settings[XORSTR("GrenadeHelper")][XORSTR("infoMolotov")], Settings::GrenadeHelper::infoMolotov);
	LoadColor(settings[XORSTR("GrenadeHelper")][XORSTR("infoFlash")], Settings::GrenadeHelper::infoFlash);

	settings[XORSTR("AutoKnife")][XORSTR("enabled")] = Settings::AutoKnife::enabled;
 	settings[XORSTR("AutoKnife")][XORSTR("Filters")][XORSTR("enemies")] = Settings::AutoKnife::Filters::enemies;
 	settings[XORSTR("AutoKnife")][XORSTR("Filters")][XORSTR("allies")] = Settings::AutoKnife::Filters::allies;
 	settings[XORSTR("AutoKnife")][XORSTR("onKey")] = Settings::AutoKnife::onKey;

	std::ofstream(path) << styledWriter.write(settings);
}

void Settings::LoadConfig(std::string path)
{
	TracerEffect::RestoreTracers();
	if (!std::ifstream(path).good())
	{
		Settings::LoadDefaultsOrSave(path);
		return;
	}

	Json::Value settings;
	std::ifstream configDoc(path, std::ifstream::binary);
	configDoc >> settings;

	GetVal(settings[XORSTR("UI")][XORSTR("mainColor")], &Settings::UI::mainColor);
	GetVal(settings[XORSTR("UI")][XORSTR("bodyColor")], &Settings::UI::bodyColor);
	GetVal(settings[XORSTR("UI")][XORSTR("fontColor")], &Settings::UI::fontColor);
	GetVal(settings[XORSTR("UI")][XORSTR("accentColor")], &Settings::UI::accentColor);
	GetVal(settings[XORSTR("UI")][XORSTR("Fonts")][XORSTR("ESP")][XORSTR("family")], &Settings::UI::Fonts::ESP::family);
	GetVal(settings[XORSTR("UI")][XORSTR("Fonts")][XORSTR("ESP")][XORSTR("size")], &Settings::UI::Fonts::ESP::size);
	GetVal(settings[XORSTR("UI")][XORSTR("Fonts")][XORSTR("ESP")][XORSTR("flags")], &Settings::UI::Fonts::ESP::flags);

	Fonts::SetupFonts();

	Settings::Aimbot::weapons = {
			{ ItemDefinitionIndex::INVALID, { false, false, false, false, false, false, false, 700, Bone::BONE_HEAD, ButtonCode_t::MOUSE_MIDDLE, false, false, 1.0f, SmoothType::SLOW_END, false, 0.0f, false, 0.0f, false, 0.0f, true, 180.0f, false, 25.0f, false, false, 2.0f, 2.0f, false, false, false, false, false, false, false, false, false, false, 0.1f, false, 10.0f, false, false, 5.0f, false, false, 100, 0.5f } },
	};

	for (Json::ValueIterator itr = settings[XORSTR("Aimbot")][XORSTR("weapons")].begin(); itr != settings[XORSTR("Aimbot")][XORSTR("weapons")].end(); itr++)
	{
		std::string weaponDataKey = itr.key().asString();
		auto weaponSetting = settings[XORSTR("Aimbot")][XORSTR("weapons")][weaponDataKey];

		// XXX Using exception handling to deal with this is stupid, but I don't care to find a better solution
		// XXX We can't use GetOrdinal() since the key type is a string...
		ItemDefinitionIndex weaponID;
		try
		{
			weaponID = (ItemDefinitionIndex) std::stoi(weaponDataKey);
		}
		catch (std::invalid_argument) // Not a number
		{
			weaponID = Util::Items::GetItemIndex(weaponDataKey);
		}

		if (Settings::Aimbot::weapons.find(weaponID) == Settings::Aimbot::weapons.end())
			Settings::Aimbot::weapons[weaponID] = AimbotWeapon_t();

		AimbotWeapon_t weapon = {
weaponSetting["Enabled"].asBool(),
				weaponSetting["Silent"].asBool(),
				weaponSetting["pSilent"].asBool(),
				weaponSetting["Friendly"].asBool(),
				weaponSetting["ClosestBone"].asBool(),
				weaponSetting["engageLock"].asBool(),
				weaponSetting["engageLockTR"].asBool(),
				weaponSetting["engageLockTTR"].asInt(),
				(Bone) weaponSetting["TargetBone"].asInt(),
				Util::GetButtonCode(weaponSetting["AimKey"].asCString()),
				weaponSetting["AimKeyOnly"].asBool(),
				weaponSetting["Smooth"]["Enabled"].asBool(),
				weaponSetting["Smooth"]["Amount"].asFloat(),
				(SmoothType) weaponSetting["Smooth"]["Type"].asInt(),
				weaponSetting["Smooth"]["Salting"]["Enabled"].asBool(),
				weaponSetting["Smooth"]["Salting"]["Multiplier"].asFloat(),
				weaponSetting["ErrorMargin"]["Enabled"].asBool(),
				weaponSetting["ErrorMargin"]["Value"].asFloat(),
				weaponSetting["Curve"]["Enabled"].asBool(),
				weaponSetting["Curve"]["Amount"].asFloat(),
				weaponSetting["AutoAim"]["Enabled"].asBool(),
				weaponSetting["AutoAim"]["FOV"].asFloat(),
				weaponSetting["AimStep"]["Enabled"].asBool(),
				weaponSetting["AimStep"]["Amount"].asFloat(),
				weaponSetting["RCS"]["Enabled"].asBool(),
				weaponSetting["RCS"]["AlwaysOn"].asBool(),
				weaponSetting["RCS"]["AmountX"].asFloat(),
				weaponSetting["RCS"]["AmountY"].asFloat(),
				weaponSetting["AutoCockRevolver"]["Enabled"].asBool(),
				weaponSetting["AutoPistol"]["Enabled"].asBool(),
				weaponSetting["AutoShoot"]["Enabled"].asBool(),
				weaponSetting["AutoScope"]["Enabled"].asBool(),
				weaponSetting["NoShoot"]["Enabled"].asBool(),
				weaponSetting["IgnoreJump"]["Enabled"].asBool(),
				weaponSetting["SmokeCheck"]["Enabled"].asBool(),
				weaponSetting["FlashCheck"]["Enabled"].asBool(),
				weaponSetting["SpreadLimit"]["Enabled"].asBool(),
				weaponSetting["SpreadLimit"]["DistanceBased"].asBool(),
				weaponSetting["SpreadLimit"]["Value"].asFloat(),
				weaponSetting["AutoWall"]["Enabled"].asBool(),
				weaponSetting["AutoWall"]["Value"].asFloat(),
				weaponSetting["AutoAim"]["RealDistance"].asBool(),
				weaponSetting["AutoSlow"]["enabled"].asBool(),
				weaponSetting["Prediction"]["enabled"].asBool(),
				weaponSetting["moveMouse"].asBool(),
				weaponSetting["HitChance"]["enabled"].asBool(),
				weaponSetting["HitChance"]["hitRays"].asInt(),
				weaponSetting["HitChance"]["value"].asFloat()
		};

		for (int bone = (int) Hitbox::HITBOX_HEAD; bone <= (int) Hitbox::HITBOX_ARMS; bone++)
			weapon.autoWallBones[bone] = weaponSetting["AutoWall"]["Bones"][bone].asBool();
		for (int bone = (int) DesiredBones::BONE_PELVIS; bone <= (int) DesiredBones::BONE_RIGHT_SOLE; bone++)
			weapon.desiredBones[bone] = weaponSetting["DesiredBones"]["Bones"][bone].asBool();
		Settings::Aimbot::weapons.at(weaponID) = weapon;
	}

	GetVal(settings["Aimbot"]["AutoCrouch"]["enabled"], &Settings::Aimbot::AutoCrouch::enabled);
	GetVal(settings["Aimbot"]["AutoShoot"]["velocityCheck"], &Settings::Aimbot::AutoShoot::velocityCheck);


	GetVal(settings[XORSTR("Resolver")][XORSTR("resolve_all")], &Settings::Resolver::resolveAll);

	GetVal(settings[XORSTR("Triggerbot")][XORSTR("enabled")], &Settings::Triggerbot::enabled);
	GetButtonCode(settings[XORSTR("Triggerbot")][XORSTR("key")], &Settings::Triggerbot::key);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("enemies")], &Settings::Triggerbot::Filters::enemies);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("allies")], &Settings::Triggerbot::Filters::allies);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("walls")], &Settings::Triggerbot::Filters::walls);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("smoke_check")], &Settings::Triggerbot::Filters::smokeCheck);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("flash_check")], &Settings::Triggerbot::Filters::flashCheck);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("head")], &Settings::Triggerbot::Filters::head);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("chest")], &Settings::Triggerbot::Filters::chest);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("stomach")], &Settings::Triggerbot::Filters::stomach);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("arms")], &Settings::Triggerbot::Filters::arms);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("Filters")][XORSTR("legs")], &Settings::Triggerbot::Filters::legs);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("RandomDelay")][XORSTR("enabled")], &Settings::Triggerbot::RandomDelay::enabled);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("RandomDelay")][XORSTR("lowBound")], &Settings::Triggerbot::RandomDelay::lowBound);
	GetVal(settings[XORSTR("Triggerbot")][XORSTR("RandomDelay")][XORSTR("highBound")], &Settings::Triggerbot::RandomDelay::highBound);

	GetVal(settings[XORSTR("AntiAim")][XORSTR("Yaw")][XORSTR("enabled")], &Settings::AntiAim::Yaw::enabled);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("Yaw")][XORSTR("type")], (int*)& Settings::AntiAim::Yaw::type);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("Yaw")][XORSTR("type_fake")], (int*)& Settings::AntiAim::Yaw::typeFake);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("Yaw")][XORSTR("antiResolver")], &Settings::AntiAim::Yaw::antiResolver);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("Yaw")][XORSTR("dynamic")], &Settings::AntiAim::Yaw::dynamic);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("Pitch")][XORSTR("enabled")], &Settings::AntiAim::Pitch::enabled);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("Pitch")][XORSTR("type")], (int*)& Settings::AntiAim::Pitch::type);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("HeadEdge")][XORSTR("enabled")], &Settings::AntiAim::HeadEdge::enabled);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("HeadEdge")][XORSTR("distance")], &Settings::AntiAim::HeadEdge::distance);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("AutoDisable")][XORSTR("knife_held")], &Settings::AntiAim::AutoDisable::knifeHeld);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("AutoDisable")][XORSTR("no_enemy")], &Settings::AntiAim::AutoDisable::noEnemy);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("MoonWalk")], &Settings::AntiAim::moonwalk);
	
	GetVal(settings[XORSTR("AntiAim")][XORSTR("Lua")][XORSTR("debugMode")], &Settings::AntiAim::Lua::debugMode);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("Lua")][XORSTR("scriptX")], Settings::AntiAim::Lua::scriptX);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("Lua")][XORSTR("scriptY")], Settings::AntiAim::Lua::scriptY);
	GetVal(settings[XORSTR("AntiAim")][XORSTR("Lua")][XORSTR("scriptY2")], Settings::AntiAim::Lua::scriptY2);
	
	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("mode")], (int*)& Settings::AntiAim::Custom::YawOne::mode);
    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("angle")], &Settings::AntiAim::Custom::YawOne::angle);
    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("jitter_min")], &Settings::AntiAim::Custom::YawOne::jitterMin);
    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("jitter_max")], &Settings::AntiAim::Custom::YawOne::jitterMax);
    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("spin_factor")], &Settings::AntiAim::Custom::YawOne::spinFactor);
    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("is_static")], &Settings::AntiAim::Custom::YawOne::isStatic);
    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawOne")][XORSTR("slow_spin")], &Settings::AntiAim::Custom::YawOne::slowSpin);

    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("mode")], (int*)& Settings::AntiAim::Custom::YawTwo::mode);
   	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("angle")], &Settings::AntiAim::Custom::YawTwo::angle);
    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("jitter_min")], &Settings::AntiAim::Custom::YawTwo::jitterMin);
    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("jitter_max")], &Settings::AntiAim::Custom::YawTwo::jitterMax);
    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("spin_factor")], &Settings::AntiAim::Custom::YawTwo::spinFactor);
    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("is_static")], &Settings::AntiAim::Custom::YawTwo::isStatic);
    	GetVal(settings[XORSTR("AntiAim")][XORSTR("Custom")][XORSTR("YawTwo")][XORSTR("slow_spin")], &Settings::AntiAim::Custom::YawTwo::slowSpin);

	GetVal(settings[XORSTR("ESP")][XORSTR("enabled")], &Settings::ESP::enabled);
	GetButtonCode(settings[XORSTR("ESP")][XORSTR("key")], &Settings::ESP::key);
	GetVal(settings[XORSTR("ESP")][XORSTR("enemy_color")], &Settings::ESP::enemyColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("enemy_visible_color")], &Settings::ESP::enemyVisibleColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("ally_color")], &Settings::ESP::allyColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("ally_visible_color")], &Settings::ESP::allyVisibleColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("t_color")], &Settings::ESP::tColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("t_visible_color")], &Settings::ESP::tVisibleColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("ct_color")], &Settings::ESP::ctColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("ct_visible_color")], &Settings::ESP::ctVisibleColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("localplayer_color")], &Settings::ESP::localplayerColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("bomb_color")], &Settings::ESP::bombColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("bomb_defusing_color")], &Settings::ESP::bombDefusingColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("hostage_color")], &Settings::ESP::hostageColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("defuser_color")], &Settings::ESP::defuserColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("weapon_color")], &Settings::ESP::weaponColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("chicken_color")], &Settings::ESP::chickenColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("fish_color")], &Settings::ESP::fishColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("smoke_color")], &Settings::ESP::smokeColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("decoy_color")], &Settings::ESP::decoyColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("flashbang_color")], &Settings::ESP::flashbangColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("grenade_color")], &Settings::ESP::grenadeColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("molotov_color")], &Settings::ESP::molotovColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("enabled")], &Settings::ESP::Glow::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("ally_color")], &Settings::ESP::Glow::allyColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("enemy_color")], &Settings::ESP::Glow::enemyColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("enemy_visible_color")], &Settings::ESP::Glow::enemyVisibleColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("localplayer_color")], &Settings::ESP::Glow::localplayerColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("weapon_color")], &Settings::ESP::Glow::weaponColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("grenade_color")], &Settings::ESP::Glow::grenadeColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("defuser_color")], &Settings::ESP::Glow::defuserColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Glow")][XORSTR("chicken_color")], &Settings::ESP::Glow::chickenColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("legit")], &Settings::ESP::Filters::legit);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("visibility_check")], &Settings::ESP::Filters::visibilityCheck);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("smoke_check")], &Settings::ESP::Filters::smokeCheck);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("enemies")], &Settings::ESP::Filters::enemies);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("allies")], &Settings::ESP::Filters::allies);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("bomb")], &Settings::ESP::Filters::bomb);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("hostages")], &Settings::ESP::Filters::hostages);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("defusers")], &Settings::ESP::Filters::defusers);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("weapons")], &Settings::ESP::Filters::weapons);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("chickens")], &Settings::ESP::Filters::chickens);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("fishes")], &Settings::ESP::Filters::fishes);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("throwables")], &Settings::ESP::Filters::throwables);
	GetVal(settings[XORSTR("ESP")][XORSTR("Filters")][XORSTR("localplayer")], &Settings::ESP::Filters::localplayer);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("name")], &Settings::ESP::Info::name);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("clan")], &Settings::ESP::Info::clan);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("steam_id")], &Settings::ESP::Info::steamId);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("rank")], &Settings::ESP::Info::rank);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("health")], &Settings::ESP::Info::health);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("weapon")], &Settings::ESP::Info::weapon);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("scoped")], &Settings::ESP::Info::scoped);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("reloading")], &Settings::ESP::Info::reloading);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("flashed")], &Settings::ESP::Info::flashed);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("planting")], &Settings::ESP::Info::planting);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("has_defuser")], &Settings::ESP::Info::hasDefuser);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("defusing")], &Settings::ESP::Info::defusing);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("grabbing_hostage")], &Settings::ESP::Info::grabbingHostage);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("rescuing")], &Settings::ESP::Info::rescuing);
	GetVal(settings[XORSTR("ESP")][XORSTR("Info")][XORSTR("location")], &Settings::ESP::Info::location);
	GetVal(settings[XORSTR("ESP")][XORSTR("Boxes")][XORSTR("enabled")], &Settings::ESP::Boxes::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("Boxes")][XORSTR("type")], (int*)& Settings::ESP::Boxes::type);
	GetVal(settings[XORSTR("ESP")][XORSTR("Skeleton")][XORSTR("enabled")], &Settings::ESP::Skeleton::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("Skeleton")][XORSTR("color")], &Settings::ESP::Skeleton::color);
	GetVal(settings[XORSTR("ESP")][XORSTR("Bars")][XORSTR("enabled")], &Settings::ESP::Bars::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("Bars")][XORSTR("color_type")], (int*)& Settings::ESP::Bars::colorType);
	GetVal(settings[XORSTR("ESP")][XORSTR("Bars")][XORSTR("type")], (int*)& Settings::ESP::Bars::type);
	GetVal(settings[XORSTR("ESP")][XORSTR("Tracers")][XORSTR("enabled")], &Settings::ESP::Tracers::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("Tracers")][XORSTR("type")], (int*)& Settings::ESP::Tracers::type);
	GetVal(settings[XORSTR("ESP")][XORSTR("BulletTracers")][XORSTR("enabled")], &Settings::ESP::BulletTracers::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("FOVCrosshair")][XORSTR("enabled")], &Settings::ESP::FOVCrosshair::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("FOVCrosshair")][XORSTR("filled")], &Settings::ESP::FOVCrosshair::filled);
	GetVal(settings[XORSTR("ESP")][XORSTR("FOVCrosshair")][XORSTR("color")], &Settings::ESP::FOVCrosshair::color);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("Arms")][XORSTR("enabled")], &Settings::ESP::Chams::Arms::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("Arms")][XORSTR("type")], (int*)& Settings::ESP::Chams::Arms::type);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("Arms")][XORSTR("color")], &Settings::ESP::Chams::Arms::color);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("Weapon")][XORSTR("enabled")], &Settings::ESP::Chams::Weapon::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("Weapon")][XORSTR("color")], &Settings::ESP::Chams::Weapon::color);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("players_ally_color")], &Settings::ESP::Chams::allyColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("players_ally_visible_color")], &Settings::ESP::Chams::allyVisibleColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("players_enemy_color")], &Settings::ESP::Chams::enemyColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("players_enemy_visible_color")], &Settings::ESP::Chams::enemyVisibleColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("localplayer_color")], &Settings::ESP::Chams::localplayerColor);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("type")], (int*)& Settings::ESP::Chams::type);
	GetVal(settings[XORSTR("ESP")][XORSTR("Chams")][XORSTR("enabled")], &Settings::ESP::Chams::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("Sounds")][XORSTR("enabled")], &Settings::ESP::Sounds::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("Sounds")][XORSTR("time")], &Settings::ESP::Sounds::time);
	GetVal(settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("enabled")], &Settings::ESP::Hitmarker::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("enemies")], &Settings::ESP::Hitmarker::enemies);
	GetVal(settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("allies")], &Settings::ESP::Hitmarker::allies);
	GetVal(settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("color")], &Settings::ESP::Hitmarker::color);
	GetVal(settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("duration")], &Settings::ESP::Hitmarker::duration);
	GetVal(settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("size")], &Settings::ESP::Hitmarker::size);
	GetVal(settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("inner_gap")], &Settings::ESP::Hitmarker::innerGap);
	GetVal(settings[XORSTR("ESP")][XORSTR("Hitmarker")][XORSTR("Damage")][XORSTR("enabled")], &Settings::ESP::Hitmarker::Damage::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("HeadDot")][XORSTR("enabled")], &Settings::ESP::HeadDot::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("HeadDot")][XORSTR("size")], &Settings::ESP::HeadDot::size);
	GetVal(settings[XORSTR("ESP")][XORSTR("Spread")][XORSTR("enabled")], &Settings::ESP::Spread::enabled);
	GetVal(settings[XORSTR("ESP")][XORSTR("Spread")][XORSTR("spreadLimit")], &Settings::ESP::Spread::spreadLimit);
	GetVal(settings[XORSTR("ESP")][XORSTR("Spread")][XORSTR("color")], &Settings::ESP::Spread::color);
	GetVal(settings[XORSTR("ESP")][XORSTR("Spread")][XORSTR("spreadLimitColor")], &Settings::ESP::Spread::spreadLimitColor);
	GetVal(settings[XORSTR("TracerEffects")][XORSTR("enabled")], &Settings::TracerEffects::enabled);
	GetVal(settings[XORSTR("TracerEffects")][XORSTR("serverSide")], &Settings::TracerEffects::serverSide);
	GetVal(settings[XORSTR("TracerEffects")][XORSTR("effect")], (int*)&Settings::TracerEffects::effect);
	GetVal(settings[XORSTR("TracerEffects")][XORSTR("frequency")], &Settings::TracerEffects::frequency);
		
	GetVal(settings[XORSTR("Dlights")][XORSTR("enabled")], &Settings::Dlights::enabled);
	GetVal(settings[XORSTR("Dlights")][XORSTR("radius")], &Settings::Dlights::radius);

	GetVal(settings[XORSTR("Spammer")][XORSTR("spammer_type")], (int*)& Settings::Spammer::type);
	GetVal(settings[XORSTR("Spammer")][XORSTR("say_team")], &Settings::Spammer::say_team);
	GetVal(settings[XORSTR("Spammer")][XORSTR("KillSpammer")][XORSTR("enabled")], &Settings::Spammer::KillSpammer::enabled);
	GetVal(settings[XORSTR("Spammer")][XORSTR("KillSpammer")][XORSTR("say_team")], &Settings::Spammer::KillSpammer::sayTeam);
	if (!settings[XORSTR("Spammer")][XORSTR("KillSpammer")][XORSTR("messages")].isNull())
	{
		Settings::Spammer::KillSpammer::messages.clear();
		for (const Json::Value& message : settings[XORSTR("Spammer")][XORSTR("KillSpammer")][XORSTR("messages")])
			Settings::Spammer::KillSpammer::messages.push_back(message.asString());
	}
	if (!settings[XORSTR("Spammer")][XORSTR("NormalSpammer")][XORSTR("messages")].isNull())
	{
		Settings::Spammer::NormalSpammer::messages.clear();
		for (const Json::Value& message : settings[XORSTR("Spammer")][XORSTR("NormalSpammer")][XORSTR("messages")])
			Settings::Spammer::NormalSpammer::messages.push_back(message.asString());
	}
	GetVal(settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_name")], &Settings::Spammer::PositionSpammer::showName);
	GetVal(settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_weapon")], &Settings::Spammer::PositionSpammer::showWeapon);
	GetVal(settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_rank")], &Settings::Spammer::PositionSpammer::showRank);
	GetVal(settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_wins")], &Settings::Spammer::PositionSpammer::showWins);
	GetVal(settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_health")], &Settings::Spammer::PositionSpammer::showHealth);
	GetVal(settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_money")], &Settings::Spammer::PositionSpammer::showMoney);
	GetVal(settings[XORSTR("Spammer")][XORSTR("PositionSpammer")][XORSTR("show_lastplace")], &Settings::Spammer::PositionSpammer::showLastplace);

	GetVal(settings[XORSTR("BHop")][XORSTR("enabled")], &Settings::BHop::enabled);
	GetVal(settings[XORSTR("BHop")][XORSTR("Chance")][XORSTR("enabled")], &Settings::BHop::Chance::enabled);
	GetVal(settings[XORSTR("BHop")][XORSTR("Chance")][XORSTR("value")], &Settings::BHop::Chance::value);
	GetVal(settings[XORSTR("BHop")][XORSTR("Hops")][XORSTR("enabledMax")], &Settings::BHop::Hops::enabledMax);
	GetVal(settings[XORSTR("BHop")][XORSTR("Hops")][XORSTR("Max")], &Settings::BHop::Hops::Max);
	GetVal(settings[XORSTR("BHop")][XORSTR("Hops")][XORSTR("enabledMin")], &Settings::BHop::Hops::enabledMin);
	GetVal(settings[XORSTR("BHop")][XORSTR("Hops")][XORSTR("Min")], &Settings::BHop::Hops::Min);

	GetVal(settings[XORSTR("AutoStrafe")][XORSTR("enabled")], &Settings::AutoStrafe::enabled);
	GetVal(settings[XORSTR("AutoStrafe")][XORSTR("type")], (int*)& Settings::AutoStrafe::type);
	GetVal(settings[XORSTR("AutoStrafe")][XORSTR("silent")], &Settings::AutoStrafe::silent);

	GetVal(settings[XORSTR("Noflash")][XORSTR("enabled")], &Settings::Noflash::enabled);
	GetVal(settings[XORSTR("Noflash")][XORSTR("value")], &Settings::Noflash::value);

	GetVal(settings[XORSTR("Radar")][XORSTR("enabled")], &Settings::Radar::enabled);
	GetVal(settings[XORSTR("Radar")][XORSTR("zoom")], &Settings::Radar::zoom);
	GetVal(settings[XORSTR("Radar")][XORSTR("enemies")], &Settings::Radar::enemies);
	GetVal(settings[XORSTR("Radar")][XORSTR("allies")], &Settings::Radar::allies);
	GetVal(settings[XORSTR("Radar")][XORSTR("legit")], &Settings::Radar::legit);
	GetVal(settings[XORSTR("Radar")][XORSTR("visibility_check")], &Settings::Radar::visibilityCheck);
	GetVal(settings[XORSTR("Radar")][XORSTR("smoke_check")], &Settings::Radar::smokeCheck);
	GetVal(settings[XORSTR("Radar")][XORSTR("InGame")][XORSTR("enabled")], &Settings::Radar::InGame::enabled);
	GetVal(settings[XORSTR("Radar")][XORSTR("enemy_color")], &Settings::Radar::enemyColor);
	GetVal(settings[XORSTR("Radar")][XORSTR("enemy_visible_color")], &Settings::Radar::enemyVisibleColor);
	GetVal(settings[XORSTR("Radar")][XORSTR("ally_color")], &Settings::Radar::allyColor);
	GetVal(settings[XORSTR("Radar")][XORSTR("ally_visible_color")], &Settings::Radar::allyVisibleColor);
	GetVal(settings[XORSTR("Radar")][XORSTR("t_color")], &Settings::Radar::tColor);
	GetVal(settings[XORSTR("Radar")][XORSTR("t_visible_color")], &Settings::Radar::tVisibleColor);
	GetVal(settings[XORSTR("Radar")][XORSTR("ct_color")], &Settings::Radar::ctColor);
	GetVal(settings[XORSTR("Radar")][XORSTR("ct_visible_color")], &Settings::Radar::ctVisibleColor);
	GetVal(settings[XORSTR("Radar")][XORSTR("bomb_color")], &Settings::Radar::bombColor);
	GetVal(settings[XORSTR("Radar")][XORSTR("bomb_defusing_color")], &Settings::Radar::bombDefusingColor);
	GetVal(settings[XORSTR("Radar")][XORSTR("icons_scale")], &Settings::Radar::iconsScale);


	GetVal(settings[XORSTR("Recoilcrosshair")][XORSTR("enabled")], &Settings::Recoilcrosshair::enabled);
	GetVal(settings[XORSTR("Recoilcrosshair")][XORSTR("showOnlyWhenShooting")], &Settings::Recoilcrosshair::showOnlyWhenShooting);

	GetVal(settings[XORSTR("FOVChanger")][XORSTR("enabled")], &Settings::FOVChanger::enabled);
	GetVal(settings[XORSTR("FOVChanger")][XORSTR("value")], &Settings::FOVChanger::value);
	GetVal(settings[XORSTR("FOVChanger")][XORSTR("viewmodel_enabled")], &Settings::FOVChanger::viewmodelEnabled);
	GetVal(settings[XORSTR("FOVChanger")][XORSTR("viewmodel_value")], &Settings::FOVChanger::viewmodelValue);
	GetVal(settings[XORSTR("FOVChanger")][XORSTR("ignore_scope")], &Settings::FOVChanger::ignoreScope);

	GetVal(settings[XORSTR("Airstuck")][XORSTR("enabled")], &Settings::Airstuck::enabled);
	GetButtonCode(settings[XORSTR("Airstuck")][XORSTR("key")], &Settings::Airstuck::key);

	Settings::Skinchanger::Skins::enabled = false;
	Settings::Skinchanger::skinsCT.clear();
	Settings::Skinchanger::skinsT.clear();

	for (Json::ValueIterator itr = settings[XORSTR("SkinChanger")][XORSTR("skinsCT")].begin(); itr != settings[XORSTR("SkinChanger")][XORSTR("skinsCT")].end(); itr++)
	{
		std::string skinDataKey = itr.key().asString();
		auto skinSetting = settings[XORSTR("SkinChanger")][XORSTR("skinsCT")][skinDataKey];

		// XXX Using exception handling to deal with this is stupid, but I don't care to find a better solution
		// XXX We can't use GetOrdinal() since the key type is a string...
		unsigned int weaponID;

		try
		{
			weaponID = std::stoi(skinDataKey);
		}
		catch(std::invalid_argument)
		{
			weaponID = (int) Util::Items::GetItemIndex(skinDataKey);
		}

		ItemDefinitionIndex defIndex;
		GetOrdinal<ItemDefinitionIndex, Util::Items::GetItemIndex>(skinSetting[XORSTR("ItemDefinitionIndex")], &defIndex);

		if (Settings::Skinchanger::skinsCT.find((ItemDefinitionIndex) weaponID) == Settings::Skinchanger::skinsCT.end())
			Settings::Skinchanger::skinsCT[(ItemDefinitionIndex) weaponID] = AttribItem_t();

		AttribItem_t skin = {
				defIndex,
				skinSetting[XORSTR("PaintKit")].asInt(),
				skinSetting[XORSTR("Wear")].asFloat(),
				skinSetting[XORSTR("Seed")].asInt(),
				skinSetting[XORSTR("StatTrak")].asInt(),
				-1,
				skinSetting[XORSTR("CustomName")].asString(),
		};

		Settings::Skinchanger::skinsCT.at((ItemDefinitionIndex) weaponID) = skin;
	}

	for (Json::ValueIterator itr = settings[XORSTR("SkinChanger")][XORSTR("skinsT")].begin(); itr != settings[XORSTR("SkinChanger")][XORSTR("skinsT")].end(); itr++)
	{
		std::string skinDataKey = itr.key().asString();
		auto skinSetting = settings[XORSTR("SkinChanger")][XORSTR("skinsT")][skinDataKey];

		// XXX Using exception handling to deal with this is stupid, but I don't care to find a better solution
		// XXX We can't use GetOrdinal() since the key type is a string...
		unsigned int weaponID;

		try
		{
			weaponID = std::stoi(skinDataKey);
		}
		catch(std::invalid_argument)
		{
			weaponID = (int) Util::Items::GetItemIndex(skinDataKey);
		}

		ItemDefinitionIndex defIndex;
		GetOrdinal<ItemDefinitionIndex, Util::Items::GetItemIndex>(skinSetting[XORSTR("ItemDefinitionIndex")], &defIndex);

		if (Settings::Skinchanger::skinsT.find((ItemDefinitionIndex) weaponID) == Settings::Skinchanger::skinsT.end())
			Settings::Skinchanger::skinsT[(ItemDefinitionIndex) weaponID] = AttribItem_t();

		AttribItem_t skin = {
				defIndex,
				skinSetting[XORSTR("PaintKit")].asInt(),
				skinSetting[XORSTR("Wear")].asFloat(),
				skinSetting[XORSTR("Seed")].asInt(),
				skinSetting[XORSTR("StatTrak")].asInt(),
				-1,
				skinSetting[XORSTR("CustomName")].asString(),
		};

		Settings::Skinchanger::skinsT.at((ItemDefinitionIndex) weaponID) = skin;
	}

	SkinChanger::forceFullUpdate = true;

	GetVal(settings[XORSTR("SkinChanger")][XORSTR("Skins")][XORSTR("enabled")], &Settings::Skinchanger::Skins::enabled);
	GetVal(settings[XORSTR("SkinChanger")][XORSTR("Models")][XORSTR("enabled")], &Settings::Skinchanger::Models::enabled);
	GetVal(settings[XORSTR("SkinChanger")][XORSTR("Skins")][XORSTR("perTeam")], &Settings::Skinchanger::Skins::perTeam);

	GetVal(settings[XORSTR("ShowRanks")][XORSTR("enabled")], &Settings::ShowRanks::enabled);

	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Colors")][XORSTR("posX")], &Settings::UI::Windows::Colors::posX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Colors")][XORSTR("posY")], &Settings::UI::Windows::Colors::posY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Colors")][XORSTR("sizeX")], &Settings::UI::Windows::Colors::sizeX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Colors")][XORSTR("sizeY")], &Settings::UI::Windows::Colors::sizeY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Colors")][XORSTR("open")], &Settings::UI::Windows::Colors::open);

	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Config")][XORSTR("posX")], &Settings::UI::Windows::Config::posX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Config")][XORSTR("posY")], &Settings::UI::Windows::Config::posY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Config")][XORSTR("sizeX")], &Settings::UI::Windows::Config::sizeX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Config")][XORSTR("sizeY")], &Settings::UI::Windows::Config::sizeY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Config")][XORSTR("open")], &Settings::UI::Windows::Config::open);

	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Main")][XORSTR("posX")], &Settings::UI::Windows::Main::posX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Main")][XORSTR("posY")], &Settings::UI::Windows::Main::posY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Main")][XORSTR("sizeX")], &Settings::UI::Windows::Main::sizeX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Main")][XORSTR("sizeY")], &Settings::UI::Windows::Main::sizeY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Main")][XORSTR("open")], &Settings::UI::Windows::Main::open);

	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Playerlist")][XORSTR("posX")], &Settings::UI::Windows::Playerlist::posX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Playerlist")][XORSTR("posY")], &Settings::UI::Windows::Playerlist::posY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Playerlist")][XORSTR("sizeX")], &Settings::UI::Windows::Playerlist::sizeX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Playerlist")][XORSTR("sizeY")], &Settings::UI::Windows::Playerlist::sizeY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Playerlist")][XORSTR("open")], &Settings::UI::Windows::Playerlist::open);

	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Skinmodel")][XORSTR("posX")], &Settings::UI::Windows::Skinmodel::posX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Skinmodel")][XORSTR("posY")], &Settings::UI::Windows::Skinmodel::posY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Skinmodel")][XORSTR("sizeX")], &Settings::UI::Windows::Skinmodel::sizeX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Skinmodel")][XORSTR("sizeY")], &Settings::UI::Windows::Skinmodel::sizeY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Skinmodel")][XORSTR("open")], &Settings::UI::Windows::Skinmodel::open);

	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Spectators")][XORSTR("posX")], &Settings::UI::Windows::Spectators::posX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Spectators")][XORSTR("posY")], &Settings::UI::Windows::Spectators::posY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Spectators")][XORSTR("sizeX")], &Settings::UI::Windows::Spectators::sizeX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Spectators")][XORSTR("sizeY")], &Settings::UI::Windows::Spectators::sizeY);

	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Walkbot")][XORSTR("posX")], &Settings::UI::Windows::Walkbot::posX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Walkbot")][XORSTR("posY")], &Settings::UI::Windows::Walkbot::posY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Walkbot")][XORSTR("sizeX")], &Settings::UI::Windows::Walkbot::sizeX);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Walkbot")][XORSTR("sizeY")], &Settings::UI::Windows::Walkbot::sizeY);
	GetVal(settings[XORSTR("UI")][XORSTR("Windows")][XORSTR("Walkbot")][XORSTR("open")], &Settings::UI::Windows::Walkbot::open);

	Settings::UI::Windows::Main::reload = true;
	Settings::UI::Windows::Colors::reload = true;
	Settings::UI::Windows::Config::reload = true;
	Settings::UI::Windows::Playerlist::reload = true;
	Settings::UI::Windows::Skinmodel::reload = true;
	Settings::UI::Windows::Spectators::reload = true;
	Settings::UI::Windows::Walkbot::reload = true;

	GetVal(settings[XORSTR("ShowSpectators")][XORSTR("enabled")], &Settings::ShowSpectators::enabled);

	GetVal(settings[XORSTR("ClanTagChanger")][XORSTR("value")], (char *)& Settings::ClanTagChanger::value);
	GetVal(settings[XORSTR("ClanTagChanger")][XORSTR("enabled")], &Settings::ClanTagChanger::enabled);
	GetVal(settings[XORSTR("ClanTagChanger")][XORSTR("animation")], &Settings::ClanTagChanger::animation);
	GetVal(settings[XORSTR("ClanTagChanger")][XORSTR("animation_speed")], &Settings::ClanTagChanger::animationSpeed);
	GetVal(settings[XORSTR("ClanTagChanger")][XORSTR("type")], (int*)& Settings::ClanTagChanger::type);
	::ClanTagChanger::UpdateClanTagCallback();

	GetVal(settings[XORSTR("View")][XORSTR("NoViewPunch")][XORSTR("enabled")], &Settings::View::NoViewPunch::enabled);
	GetVal(settings[XORSTR("View")][XORSTR("NoAimPunch")][XORSTR("enabled")], &Settings::View::NoAimPunch::enabled);

	GetVal(settings[XORSTR("FakeLag")][XORSTR("enabled")], &Settings::FakeLag::enabled);
	GetVal(settings[XORSTR("FakeLag")][XORSTR("value")], &Settings::FakeLag::value);
	GetVal(settings[XORSTR("FakeLag")][XORSTR("adaptive")], &Settings::FakeLag::adaptive);

	GetVal(settings[XORSTR("AutoAccept")][XORSTR("enabled")], &Settings::AutoAccept::enabled);

	GetVal(settings[XORSTR("NoSky")][XORSTR("enabled")], &Settings::NoSky::enabled);
	GetVal(settings[XORSTR("NoSky")][XORSTR("color")], &Settings::NoSky::color);

	GetVal(settings[XORSTR("SkyChager")][XORSTR("enabled")], &Settings::SkyChanger::enabled);
	GetVal(settings[XORSTR("SkyChanger")][XORSTR("skyName")], Settings::SkyChanger::skyName);

	GetVal(settings[XORSTR("ASUSWalls")][XORSTR("enabled")], &Settings::ASUSWalls::enabled);
	GetVal(settings[XORSTR("ASUSWalls")][XORSTR("color")], &Settings::ASUSWalls::color);

	GetVal(settings[XORSTR("NoScopeBorder")][XORSTR("enabled")], &Settings::NoScopeBorder::enabled);

	GetVal(settings[XORSTR("SniperCrosshair")][XORSTR("enabled")], &Settings::SniperCrosshair::enabled);

	GetVal(settings[XORSTR("Autoblock")][XORSTR("enabled")], &Settings::Autoblock::enabled);
	GetButtonCode(settings[XORSTR("Autoblock")][XORSTR("key")], &Settings::Autoblock::key);

	GetVal(settings[XORSTR("AutoDefuse")][XORSTR("enabled")], &Settings::AutoDefuse::enabled);
	GetVal(settings[XORSTR("AutoDefuse")][XORSTR("silent")], &Settings::AutoDefuse::silent);

	GetVal(settings[XORSTR("NoSmoke")][XORSTR("enabled")], &Settings::NoSmoke::enabled);

	GetVal(settings[XORSTR("ScreenshotCleaner")][XORSTR("enabled")], &Settings::ScreenshotCleaner::enabled);

	GetVal(settings[XORSTR("EdgeJump")][XORSTR("enabled")], &Settings::EdgeJump::enabled);
	GetButtonCode(settings[XORSTR("EdgeJump")][XORSTR("key")], &Settings::EdgeJump::key);

	GetVal(settings[XORSTR("NameStealer")][XORSTR("enabled")], &Settings::NameStealer::enabled);
	GetVal(settings[XORSTR("NameStealer")][XORSTR("team")], &Settings::NameStealer::team);

	GetVal(settings[XORSTR("ThirdPerson")][XORSTR("enabled")], &Settings::ThirdPerson::enabled);
	GetVal(settings[XORSTR("ThirdPerson")][XORSTR("distance")], &Settings::ThirdPerson::distance);

	GetVal(settings[XORSTR("JumpThrow")][XORSTR("enabled")], &Settings::JumpThrow::enabled);
	GetButtonCode(settings[XORSTR("JumpThrow")][XORSTR("key")], &Settings::JumpThrow::key);

	GetVal(settings[XORSTR("DisablePostProcessing")][XORSTR("enabled")], &Settings::DisablePostProcessing::enabled);

	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("enabled")], &Settings::GrenadeHelper::enabled);
	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("aimAssist")], &Settings::GrenadeHelper::aimAssist);
	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("OnlyMatching")], &Settings::GrenadeHelper::onlyMatchingInfos);
	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("aimStep")], &Settings::GrenadeHelper::aimStep);
	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("aimDistance")], &Settings::GrenadeHelper::aimDistance);
	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("aimFov")], &Settings::GrenadeHelper::aimFov);

	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("aimDot")], &Settings::GrenadeHelper::aimDot);
	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("aimLine")], &Settings::GrenadeHelper::aimLine);
	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("infoHE")], &Settings::GrenadeHelper::infoHE);
	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("infoSmoke")], &Settings::GrenadeHelper::infoSmoke);
	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("infoFlash")], &Settings::GrenadeHelper::infoFlash);
	GetVal(settings[XORSTR("GrenadeHelper")][XORSTR("infoMolotov")], &Settings::GrenadeHelper::infoMolotov);

	// GetVal(settings[XORSTR("WalkBot")][XORSTR("enabled")], &Settings::WalkBot::enabled);
	GetVal(settings[XORSTR("WalkBot")][XORSTR("autobuy")], &Settings::WalkBot::autobuy);
	GetVal(settings[XORSTR("WalkBot")][XORSTR("autobuyAt")], &Settings::WalkBot::autobuyAt);

	GetVal(settings[XORSTR("AutoKnife")][XORSTR("enabled")], &Settings::AutoKnife::enabled);
 	GetVal(settings[XORSTR("AutoKnife")][XORSTR("Filters")][XORSTR("enemies")], &Settings::AutoKnife::Filters::enemies);
 	GetVal(settings[XORSTR("AutoKnife")][XORSTR("Filters")][XORSTR("allies")], &Settings::AutoKnife::Filters::allies);
 	GetVal(settings[XORSTR("AutoKnife")][XORSTR("onKey")], &Settings::AutoKnife::onKey);


}

/*
void Settings::LoadSettings()
{
	passwd *pw = getpwuid(getuid());
	pstring directory = pw->pw_dir;
	directory << "/.config";

	if (!DoesDirectoryExist(directory.c_str()))
		mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	directory << "/fuzion/";

	if (!DoesDirectoryExist(directory.c_str()))
		mkdir(directory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}
*/

void Settings::SaveGrenadeInfo(std::string path)
{
	Json::Value grenadeInfos;
	for (auto grenadeInfo = GrenadeHelper::grenadeInfos.begin(); grenadeInfo != GrenadeHelper::grenadeInfos.end(); grenadeInfo++)
	{
		Json::Value act;
		act[XORSTR("name")] = grenadeInfo->name.c_str();
		act[XORSTR("gType")] = grenadeInfo->gType;
		act[XORSTR("tType")] = grenadeInfo->tType;
		act[XORSTR("pos")][XORSTR("x")] = grenadeInfo->pos.x;
		act[XORSTR("pos")][XORSTR("y")] = grenadeInfo->pos.y;
		act[XORSTR("pos")][XORSTR("z")] = grenadeInfo->pos.z;

		act[XORSTR("angle")][XORSTR("x")] = grenadeInfo->angle.x;
		act[XORSTR("angle")][XORSTR("y")] = grenadeInfo->angle.y;

		grenadeInfos.append(act);
	}

	Json::Value data;
	Json::StyledWriter styledWriter;

	data[XORSTR("smokeinfos")] = grenadeInfos;

	std::ofstream(path) << styledWriter.write(data);
}

void Settings::LoadGrenadeInfo(std::string path)
{
	if (!std::ifstream(path).good() || !DoesFileExist(path.c_str()))
		return;
	Json::Value data;
	std::ifstream configDoc(path, std::ifstream::binary);
	try {
		configDoc >> data;
	}
	catch (...)
	{
		cvar->ConsoleDPrintf(XORSTR("Error parsing the config file.\n"));
		return;
	}

	Json::Value array = data[XORSTR("smokeinfos")];
	Settings::GrenadeHelper::grenadeInfos = {};

	for(Json::Value::iterator it = array.begin(); it!=array.end(); ++it)
	{
		Json::Value act = *it;
		const char* name = act[XORSTR("name")].asCString();
		GrenadeType gType = (GrenadeType)act[XORSTR("gType")].asInt();
		ThrowType tType = (ThrowType)act[XORSTR("tType")].asInt();
		Json::Value pos = act[XORSTR("pos")];
		Vector posVec = Vector(pos[XORSTR("x")].asFloat(), pos[XORSTR("y")].asFloat(), pos[XORSTR("z")].asFloat());
		Json::Value angle = act[XORSTR("angle")];
		QAngle vAngle = QAngle(angle[XORSTR("x")].asFloat(), angle[XORSTR("y")].asFloat(), 0);
		Settings::GrenadeHelper::grenadeInfos.push_back(GrenadeInfo(gType, posVec, vAngle, tType, pstring(name)));
	}
}

void remove_directory(const char* path)
{
	DIR* dir;
	dirent* pdir;

	dir = opendir(path);

	while ((pdir = readdir(dir)))
	{
		if (strcmp(pdir->d_name, ".") == 0 || strcmp(pdir->d_name, "..") == 0)
			continue;

		if (pdir->d_type == DT_DIR)
		{
			pstring _dir;
			_dir << path << "/" << pdir->d_name;

			remove_directory(_dir.c_str());
		}
		else if (pdir->d_type == DT_REG)
		{
			pstring file;
			file << path << "/" << pdir->d_name;

			unlink(file.c_str());
		}
	}

	rmdir(path);
}

void Settings::DeleteConfig(std::string path)
{
	remove_directory(path.c_str());
}
