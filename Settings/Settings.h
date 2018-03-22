#pragma once
#include <vector>
#include "../Main/Main.h"

#define FIX_MIN_FOV_HEAD	30
#define FIX_MIN_FOV_CHEST	50
#define FOV_BASE_DISTANCE	200

namespace Settings
{
	void LoadSettings(string szIniFile);
	void SaveSettings(string szIniFile);
	int TriggerCharToKey(const char* Key);

	namespace InventoryChanger {
		extern bool enabled;
		extern std::vector<uint32_t> medals;
	}

	namespace Aimbot
	{
		struct weapon_aim_s
		{
			//[swap_lines]
			bool aim_Active;
			int aim_Smooth;
			int aim_Fov;
			int aim_FovType;
			int aim_BestHit;
			int aim_Spot;
			int aim_Delay;
			int aim_Rcs;
			int aim_RcsFov;
			int aim_RcsSmooth;
			int aim_RcsClampType;
			bool aim_AutoPistol;
			//[/swap_lines]
		};
		//[swap_lines]
		extern int aim_Backtracktickrate;
		extern bool aim_Backtrack;
		extern bool aim_DrawBacktrack;
		extern bool aim_Deathmatch;
		extern bool aim_WallAttack;
		extern bool aim_CheckSmoke;
		extern bool aim_AntiJump;
		extern int aim_RcsType;
		extern bool aim_DrawFov;
		extern bool aim_DrawSpot;

		extern weapon_aim_s weapon_aim_settings[33];
		//[/swap_lines]
	}



	namespace Esp
	{
		
		extern bool esp_Watermark;
		extern bool esp_Name;
		extern bool esp_Rank;
		extern int esp_Health;
		extern bool esp_Weapon;
		extern bool esp_Ammo;

		extern float esp_Ambient[3];



		extern bool esp_Skeleton;
		extern bool esp_Team;
		extern bool esp_Enemy;
		extern int esp_Visible;
		extern int esp_ChamsVisible;

		extern int esp_Chams;
		extern bool esp_Bomb;
		extern int esp_BombTimer;

		extern float esp_Color_CT[3];
		extern float esp_Color_TT[3];

		extern float chams_Color_CT[3];
		extern float chams_Color_TT[3];
		//[/swap_lines]
	}

	namespace Radar
	{

		extern int rad_Range;
		extern bool rad_InGame;
	}

	namespace Skin
	{
		//[swap_lines]
		extern int knf_ct_model;
		extern int knf_ct_skin;
		extern int knf_tt_model;
		extern int knf_tt_skin;
		extern int gloves_skin;
		//[/swap_lines]
	}

	namespace Misc
	{
		//[swap_lines]
		extern QAngle qLastTickAngle;
		extern bool misc_SkinChanger;
		extern bool misc_KnifeChanger;
		extern bool misc_Bhop;
		extern bool misc_Punch;
		extern bool misc_AwpAim;
		extern bool misc_NoFlash;
		extern bool misc_NoSmoke;
		extern bool misc_AutoStrafe;
		extern bool misc_AutoAccept;
		extern bool misc_Spectators;
		extern bool misc_FovChanger;
		extern int misc_FovView;
		extern int misc_FovModelView;
	}


}
#define CHEAT_NAME "fawn.pw"
#define WATER_MARK "fawn.pw"
#define AIMBOT_TEXT "AIM"
#define CONFIG_TEXT "CONFIG"
#define VISUAL_TEXT "ESP"
#define RADAR_TEXT "Radar"
#define SKIN_TEXT "SKIN"
#define MISC_TEXT "MISC"

#define CVAR_AIMBOT_WALLATTACK "aim_WallAttack"
#define CVAR_AIMBOT_CHECKSMOKE "aim_CheckSmoke"
#define CVAR_AIMBOT_ANTIJUMP	"aim_IgnoreJump"
#define CVAR_AIMBOT_RCSTYPE "aim_RcsType"
#define CVAR_AIMBOT_DRAWFOV "aim_DrawFov"
#define CVAR_AIMBOT_DRAWSPOT "aim_DrawSpot"

#define CVAR_AIMBOT_ACTIVE "aim_Active"
#define CVAR_AIMBOT_SMOOTH "aim_Smooth"
#define CVAR_AIMBOT_FOV "aim_Fov"
#define CVAR_AIMBOT_FOVTYPE "aim_FovType"
#define CVAR_AIMBOT_BESTHIT "aim_Besthit"
#define CVAR_AIMBOT_SPOT "aim_Spot"
#define CVAR_AIMBOT_DELAY "aim_Delay"

#define CVAR_AIMBOT_RCS "aim_Rcs"
#define CVAR_AIMBOT_RCSFOV "aim_RcsFov"
#define CVAR_AIMBOT_RCSSMOOTH "aim_RcsSmooth"
#define CVAR_AIMBOT_RCSCLAMPTYPE "aim_RcsClampType"
#define CVAR_AIMBOT_AUTOPISTOL "aim_AutoPistol"


#define CVAR_KEY_MOUSE3 "MOUSE3"
#define CVAR_KEY_MOUSE4 "MOUSE4"
#define CVAR_KEY_MOUSE5 "MOUSE5"

#define CVAR_ESP_NAME "esp_Name"
#define CVAR_ESP_RANK "esp_Rank"
#define CVAR_ESP_WATER "esp_Watermark"

#define CVAR_ESP_HEALTH "esp_Health"
#define CVAR_ESP_WEAPON "esp_Weapon"
#define CVAR_ESP_AMMO "esp_Ammo"

#define CVAR_ESP_SKELETON "esp_Skeleton"
#define CVAR_ESP_TEAM "esp_Team"
#define CVAR_ESP_ENEMY "esp_Enemy"
#define CVAR_ESP_VISIBLE "esp_Visible"
#define CVAR_ESP_CHAMSVISIBLE "esp_ChamsVisible"

#define CVAR_ESP_CHAMS "esp_Chams"
#define CVAR_ESP_BOMB "esp_Bomb"
#define CVAR_ESP_BOMBTIMER "esp_BombTimer"

#define CVAR_ESP_COLOR_CT "esp_Color_CT"
#define CVAR_ESP_COLOR_TT "esp_Color_TT"

#define CVAR_CHAMS_COLOR_CT "chams_Color_CT"
#define CVAR_CHAMS_COLOR_TT "chams_Color_TT"

#define C4_TIMER_STRING "C4 Timer: %0.2f"


#define CVAR_RAD_RANGE "rad_Range"
#define CVAR_RAD_INGAME "rad_InGame"

#define CVAR_SKIN_CT_MODEL "knf_ct_model"
#define CVAR_SKIN_CT_SKIN "knf_ct_skin"
#define CVAR_SKIN_TT_MODEL "knf_tt_model"
#define CVAR_SKIN_TT_SKIN "knf_tt_skin"
#define CVAR_SKIN_GLOVES "gloves_skin"

#define CVAR_MISC_SKIN_CHANGER "misc_SkinChanger"
#define CVAR_MISC_KNIFE_CHANGER "misc_KnifeChanger"
#define CVAR_AIMBOT_BACKTRACK "aim_Backtrack"
#define CVAR_AIMBOT_BACKTRACK_TICK "aim_Backtracktickrate"
#define CVAR_MISC_BHOP "misc_Bhop"
#define CVAR_MISC_PUNCH "misc_Punch"
#define CVAR_MISC_AWPAIM "misc_AwpAim"
#define CVAR_MISC_NOFLASH "misc_NoFlash"
#define CVAR_MISC_NOSMOKE "misc_NoSmoke"
#define CVAR_MISC_AUTOSTRAFE "misc_AutoStrafe"
#define CVAR_MISC_AUTOACCEPT "misc_AutoAccept"
#define CVAR_MISC_SPECTATORS "misc_Spectators"
#define CVAR_MISC_FOV_CHANGER "misc_FovChanger"
#define CVAR_MISC_FOV_VIEW "misc_FovView"
#define CVAR_MISC_FOV_MDL_VIEW "misc_FovModelView"
#define CVAR_MISC_AWPAIM_COLOR "misc_AwpAimColor"