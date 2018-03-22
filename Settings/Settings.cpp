#include "Settings.h"

//[enc_string_enable /]
//[junk_enable /]

using namespace Client;

namespace Settings
{
	void LoadSettings(string szIniFile)
	{
		CSX::Cvar::InitPath(szIniFile.c_str());
		Aimbot::aim_Backtrack = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_BACKTRACK, Aimbot::aim_Backtrack) != false;
		Aimbot::aim_Backtracktickrate = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_BACKTRACK_TICK, Aimbot::aim_Backtracktickrate);
		Aimbot::aim_WallAttack = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_WALLATTACK, Aimbot::aim_WallAttack);
		Aimbot::aim_CheckSmoke = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_CHECKSMOKE, Aimbot::aim_CheckSmoke);
		Aimbot::aim_AntiJump = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_ANTIJUMP, Aimbot::aim_AntiJump);
		Aimbot::aim_RcsType = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_RCSTYPE, Aimbot::aim_RcsType);
		Aimbot::aim_DrawFov = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWFOV, Aimbot::aim_DrawFov);
		Aimbot::aim_DrawSpot = CSX::Cvar::LoadCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWSPOT, Aimbot::aim_DrawSpot);

		for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
		{
			// пистолеты
			if (i <= 9)
			{
				Aimbot::weapon_aim_settings[i].aim_Active = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, 0);
				Aimbot::weapon_aim_settings[i].aim_Smooth = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, 1);
				Aimbot::weapon_aim_settings[i].aim_Fov = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOV, 1);
				Aimbot::weapon_aim_settings[i].aim_FovType = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, 0);
				Aimbot::weapon_aim_settings[i].aim_BestHit = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, 0);
				Aimbot::weapon_aim_settings[i].aim_Spot = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, 0);
				Aimbot::weapon_aim_settings[i].aim_Delay = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, 0);
				Aimbot::weapon_aim_settings[i].aim_Rcs = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCS, 0);
				Aimbot::weapon_aim_settings[i].aim_AutoPistol = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_AUTOPISTOL, 0);
			}

			// автоматы
			if (i >= 10 && i <= 30)
			{
				Aimbot::weapon_aim_settings[i].aim_Active = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, 0);
				Aimbot::weapon_aim_settings[i].aim_Smooth = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, 1);
				Aimbot::weapon_aim_settings[i].aim_Fov = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOV, 1);
				Aimbot::weapon_aim_settings[i].aim_FovType = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, 0);
				Aimbot::weapon_aim_settings[i].aim_BestHit = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, 0);
				Aimbot::weapon_aim_settings[i].aim_Spot = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, 0);
				Aimbot::weapon_aim_settings[i].aim_Delay = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, 0);
				Aimbot::weapon_aim_settings[i].aim_Rcs = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCS, 0);
				Aimbot::weapon_aim_settings[i].aim_RcsFov = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSFOV, 1);
				Aimbot::weapon_aim_settings[i].aim_RcsSmooth = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSSMOOTH, 1);
				Aimbot::weapon_aim_settings[i].aim_RcsClampType = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCSCLAMPTYPE, 0);
			}

			// снайперки
			if (i >= 31 && i <= 32)
			{
				Aimbot::weapon_aim_settings[i].aim_Active = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, 0);
				Aimbot::weapon_aim_settings[i].aim_Smooth = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, 1);
				Aimbot::weapon_aim_settings[i].aim_Fov = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOV, 1);
				Aimbot::weapon_aim_settings[i].aim_FovType = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, 0);
				Aimbot::weapon_aim_settings[i].aim_BestHit = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, 0);
				Aimbot::weapon_aim_settings[i].aim_Spot = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, 0);
				Aimbot::weapon_aim_settings[i].aim_Delay = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, 0);
				Aimbot::weapon_aim_settings[i].aim_Rcs = CSX::Cvar::LoadCvar(pWeaponData[i], CVAR_AIMBOT_RCS, 0);
			}
		}


		Esp::esp_Name = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_NAME, Esp::esp_Name) != false;
		Esp::esp_Rank = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_RANK, Esp::esp_Rank) != false;
		Esp::esp_Health = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_HEALTH, Esp::esp_Health);
		Esp::esp_Weapon = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_WEAPON, Esp::esp_Weapon) != false;
		Esp::esp_Ammo = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_AMMO, Esp::esp_Ammo) != false;

		Esp::esp_Skeleton = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_SKELETON, Esp::esp_Skeleton) != false;
		Esp::esp_Team = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_TEAM, Esp::esp_Team) != false;
		Esp::esp_Enemy = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_ENEMY, Esp::esp_Enemy) != false;
		Esp::esp_Visible = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_VISIBLE, Esp::esp_Visible);

		Esp::esp_Chams = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_CHAMS, Esp::esp_Chams);
		Esp::esp_Bomb = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_BOMB, Esp::esp_Bomb) != false;
		Esp::esp_BombTimer = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_BOMBTIMER, Esp::esp_BombTimer);

		string esp_Color_CT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_COLOR_CT, "255,255,255");
		string esp_Color_TT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_ESP_COLOR_TT, "255,255,255");

		string chams_Color_CT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_CT, "255,0,0");
		string chams_Color_TT = CSX::Cvar::LoadCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_TT, "255,0,0");

		ScanColorFromCvar(esp_Color_CT.c_str(), Settings::Esp::esp_Color_CT);
		ScanColorFromCvar(esp_Color_TT.c_str(), Settings::Esp::esp_Color_TT);

		ScanColorFromCvar(chams_Color_CT.c_str(), Settings::Esp::chams_Color_CT);
		ScanColorFromCvar(chams_Color_TT.c_str(), Settings::Esp::chams_Color_TT);



		Radar::rad_Range = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_RANGE, Radar::rad_Range);
		Radar::rad_InGame = CSX::Cvar::LoadCvar(RADAR_TEXT, CVAR_RAD_INGAME, Radar::rad_InGame) != false;


		Skin::knf_ct_model = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_CT_MODEL, Skin::knf_ct_model);
		Skin::knf_ct_skin = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_CT_SKIN, Skin::knf_ct_skin);
		Skin::knf_tt_model = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_TT_MODEL, Skin::knf_tt_model);
		Skin::knf_tt_skin = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_TT_SKIN, Skin::knf_tt_skin);
		Skin::gloves_skin = CSX::Cvar::LoadCvar(SKIN_TEXT, CVAR_SKIN_GLOVES, Skin::gloves_skin);

		for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
		{
			g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit = CSX::Cvar::LoadCvar(SKIN_TEXT, pWeaponData[i], 0);
		}

		if (Interfaces::Engine()->IsConnected())
			ForceFullUpdate();

		Misc::misc_SkinChanger = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_SKIN_CHANGER, Misc::misc_SkinChanger) != false;
		Misc::misc_KnifeChanger = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_KNIFE_CHANGER, Misc::misc_KnifeChanger) != false;
		Misc::misc_Bhop = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_BHOP, Misc::misc_Bhop) != false;
		Misc::misc_Punch = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_PUNCH, Misc::misc_Punch) != false;
		Misc::misc_AwpAim = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_AWPAIM, Misc::misc_AwpAim) != false;
		Misc::misc_NoFlash = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_NOFLASH, Misc::misc_NoFlash) != false;
		Misc::misc_NoSmoke = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_NOSMOKE, Misc::misc_NoSmoke) != false;
		Misc::misc_AutoStrafe = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_AUTOSTRAFE, Misc::misc_AutoStrafe) != false;
		Misc::misc_AutoAccept = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_AUTOACCEPT, Misc::misc_AutoAccept) != false;
		Misc::misc_Spectators = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_SPECTATORS, Misc::misc_Spectators) != false;
		Misc::misc_FovChanger = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_FOV_CHANGER, Misc::misc_FovChanger) != false;
		Misc::misc_FovView = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_FOV_VIEW, Misc::misc_FovView);
		Misc::misc_FovModelView = CSX::Cvar::LoadCvar(MISC_TEXT, CVAR_MISC_FOV_MDL_VIEW, Misc::misc_FovModelView);
	}

	void SaveSettings(string szIniFile)
	{
		CSX::Cvar::InitPath(szIniFile.c_str());
		
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_BACKTRACK, Aimbot::aim_Backtrack);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_BACKTRACK_TICK, Aimbot::aim_Backtracktickrate);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_WALLATTACK, Aimbot::aim_WallAttack);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_CHECKSMOKE, Aimbot::aim_CheckSmoke);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_ANTIJUMP, Aimbot::aim_AntiJump);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_RCSTYPE, Aimbot::aim_RcsType);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWFOV, Aimbot::aim_DrawFov);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_DRAWSPOT, Aimbot::aim_DrawSpot);

		for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
		{
			// пистолеты
			if (i <= 9)
			{
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, Aimbot::weapon_aim_settings[i].aim_Active);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, Aimbot::weapon_aim_settings[i].aim_Smooth);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOV, Aimbot::weapon_aim_settings[i].aim_Fov);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, Aimbot::weapon_aim_settings[i].aim_FovType);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, Aimbot::weapon_aim_settings[i].aim_BestHit);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, Aimbot::weapon_aim_settings[i].aim_Spot);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, Aimbot::weapon_aim_settings[i].aim_Delay);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCS, Aimbot::weapon_aim_settings[i].aim_Rcs);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_AUTOPISTOL, Aimbot::weapon_aim_settings[i].aim_AutoPistol);
			}

			// автоматы
			if (i >= 10 && i <= 30)
			{
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, Aimbot::weapon_aim_settings[i].aim_Active);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, Aimbot::weapon_aim_settings[i].aim_Smooth);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOV, Aimbot::weapon_aim_settings[i].aim_Fov);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, Aimbot::weapon_aim_settings[i].aim_FovType);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, Aimbot::weapon_aim_settings[i].aim_BestHit);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, Aimbot::weapon_aim_settings[i].aim_Spot);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, Aimbot::weapon_aim_settings[i].aim_Delay);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCS, Aimbot::weapon_aim_settings[i].aim_Rcs);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSFOV, Aimbot::weapon_aim_settings[i].aim_RcsFov);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSSMOOTH, Aimbot::weapon_aim_settings[i].aim_RcsSmooth);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCSCLAMPTYPE, Aimbot::weapon_aim_settings[i].aim_RcsClampType);
			}

			// снайперки
			if (i >= 31 && i <= 32)
			{
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_ACTIVE, Aimbot::weapon_aim_settings[i].aim_Active);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SMOOTH, Aimbot::weapon_aim_settings[i].aim_Smooth);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOV, Aimbot::weapon_aim_settings[i].aim_Fov);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_FOVTYPE, Aimbot::weapon_aim_settings[i].aim_FovType);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_BESTHIT, Aimbot::weapon_aim_settings[i].aim_BestHit);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_SPOT, Aimbot::weapon_aim_settings[i].aim_Spot);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_DELAY, Aimbot::weapon_aim_settings[i].aim_Delay);
				CSX::Cvar::SaveCvar(pWeaponData[i], CVAR_AIMBOT_RCS, Aimbot::weapon_aim_settings[i].aim_Rcs);
			}
		}



		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_NAME, Settings::Esp::esp_Name);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_WATER, Settings::Esp::esp_Watermark);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_RANK, Settings::Esp::esp_Rank);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_HEALTH, Settings::Esp::esp_Health);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_WEAPON, Settings::Esp::esp_Weapon);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_AMMO, Settings::Esp::esp_Ammo);

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_SKELETON, Settings::Esp::esp_Skeleton);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_TEAM, Settings::Esp::esp_Team);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_ENEMY, Settings::Esp::esp_Enemy);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_VISIBLE, Settings::Esp::esp_Visible);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_CHAMSVISIBLE, Settings::Esp::esp_ChamsVisible);

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_CHAMS, Settings::Esp::esp_Chams);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_BOMB, Settings::Esp::esp_Bomb);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_BOMBTIMER, Settings::Esp::esp_BombTimer);


		string esp_Color_CT =
			to_string(int(Settings::Esp::esp_Color_CT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_CT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_CT[2] * 255.f));

		string esp_Color_TT =
			to_string(int(Settings::Esp::esp_Color_TT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_TT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::esp_Color_TT[2] * 255.f));

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_COLOR_CT, esp_Color_CT);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_ESP_COLOR_TT, esp_Color_TT);

		string chams_Color_CT =
			to_string(int(Settings::Esp::chams_Color_CT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_CT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_CT[2] * 255.f));

		string chams_Color_TT =
			to_string(int(Settings::Esp::chams_Color_TT[0] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_TT[1] * 255.f)) + "," +
			to_string(int(Settings::Esp::chams_Color_TT[2] * 255.f));

		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_CT, chams_Color_CT);
		CSX::Cvar::SaveCvar(VISUAL_TEXT, CVAR_CHAMS_COLOR_TT, chams_Color_TT);


		CSX::Cvar::SaveCvar(RADAR_TEXT, CVAR_RAD_RANGE, Radar::rad_Range);
		CSX::Cvar::SaveCvar( RADAR_TEXT , CVAR_RAD_INGAME , Radar::rad_InGame );

		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_CT_MODEL, Skin::knf_ct_model);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_CT_SKIN, Skin::knf_ct_skin);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_TT_MODEL, Skin::knf_tt_model);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_TT_SKIN, Skin::knf_tt_skin);
		CSX::Cvar::SaveCvar(SKIN_TEXT, CVAR_SKIN_GLOVES, Skin::gloves_skin);

		for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
		{
			CSX::Cvar::SaveCvar(SKIN_TEXT, pWeaponData[i], g_SkinChangerCfg[pWeaponItemIndexData[i]].nFallbackPaintKit);
		}

		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_SKIN_CHANGER, Misc::misc_SkinChanger);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_KNIFE_CHANGER, Misc::misc_KnifeChanger);
		CSX::Cvar::SaveCvar(AIMBOT_TEXT, CVAR_AIMBOT_BACKTRACK, Aimbot::aim_Backtrack);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_BHOP, Misc::misc_Bhop);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_PUNCH, Misc::misc_Punch);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_NOFLASH, Misc::misc_NoFlash);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_NOSMOKE, Misc::misc_NoSmoke);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_AWPAIM, Misc::misc_AwpAim);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_AUTOSTRAFE, Misc::misc_AutoStrafe);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_AUTOACCEPT, Misc::misc_AutoAccept);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_SPECTATORS, Misc::misc_Spectators);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_FOV_CHANGER, Misc::misc_FovChanger);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_FOV_VIEW, Misc::misc_FovView);
		CSX::Cvar::SaveCvar(MISC_TEXT, CVAR_MISC_FOV_MDL_VIEW, Misc::misc_FovModelView);
	}

	int TriggerCharToKey(const char* Key)
	{
		if (!strcmp(Key, CVAR_KEY_MOUSE3)) return 0;
		if (!strcmp(Key, CVAR_KEY_MOUSE4)) return 1;
		if (!strcmp(Key, CVAR_KEY_MOUSE5)) return 2;

		return 0;
	}

	namespace InventoryChanger {
		bool enabled = true;
		std::vector<uint32_t> medals = { 930,931,948,958,6021,6020,6019,6012,6032,6027,6002 };
	}

	namespace Aimbot
	{
		bool aim_Backtrack = false;
		bool aim_DrawBacktrack = false;
		int aim_Backtracktickrate = 1;
		bool aim_Deathmatch = false;
		bool aim_WallAttack = false;
		bool aim_CheckSmoke = false;
		bool aim_AntiJump = false;
		int aim_RcsType = 0;
		bool aim_DrawFov = false;
		bool aim_DrawSpot = false;

		weapon_aim_s weapon_aim_settings[33] = { 0 };
	}


	namespace Esp
	{
		
		bool esp_Watermark = true;
		bool esp_Name = false;
		bool esp_Rank = false;
		int esp_Health = 0;
		bool esp_Weapon = false;
		bool esp_Ammo = false;

		float esp_Ambient[3];

		bool esp_Skeleton = 0;
		bool esp_Team = 1;
		bool esp_Enemy = 1;
		int esp_Visible = 1;
		int esp_ChamsVisible = 1;

		int esp_Chams = 0;
		bool esp_Bomb = false;
		int esp_BombTimer = 40;

		float esp_Color_CT[3] = { 0.f,0.0f,0.f };
		float esp_Color_TT[3] = { 0.f,0.0f,0.f };

		float chams_Color_CT[3] = { 0.f,0.0f,0.f };
		float chams_Color_TT[3] = { 0.f,0.0f,0.f };
	}

	namespace Radar
	{

		int rad_Range = 3500;
		bool rad_InGame = false;
	}

	namespace Skin
	{
		int knf_ct_model = 0;
		int knf_ct_skin = 0;
		int knf_tt_model = 0;
		int knf_tt_skin = 0;
		int gloves_skin = 0;
	}

	namespace Misc
	{
		QAngle qLastTickAngle;
		float misc_MenuColor[3] = { 0.f, 0.f, 0.f };
		bool misc_SkinChanger = true;
		bool misc_KnifeChanger = true;
		bool misc_Bhop = false;
		bool misc_Punch = false;
		bool misc_NoFlash = false;
		bool misc_NoSmoke = false;
		bool misc_AwpAim = false;
		bool misc_AutoStrafe = false;
		bool misc_AutoAccept = false;
		bool misc_Spectators = false;
		bool misc_FovChanger = false;
		int misc_FovView = 90;
		int misc_FovModelView = 90;
	}
}