#include "Client.h"
#include <ctime>

//[enc_string_enable /]
//[junk_enable /]

namespace Client
{
	//[swap_lines]
	int	iScreenWidth = 0;
	int	iScreenHeight = 0;

	string BaseDir = "";
	string LogFile = "";
	string GuiFile = "";
	string IniFile = "";

	vector<string> ConfigList;

	Vector2D	g_vCenterScreen = Vector2D(0.f, 0.f);

	CPlayers*	g_pPlayers = nullptr;
	CRender*	g_pRender = nullptr;
	CGui*		g_pGui = nullptr;

	CAimbot*	g_pAimbot = nullptr;
	CEsp*		g_pEsp = nullptr;
	CRadar*		g_pRadar = nullptr;
	CSkin*		g_pSkin = nullptr;
	CMisc*		g_pMisc = nullptr;
	CInventoryChanger* g_pInventoryChanger = nullptr;

	bool		bC4Timer = false;
	int			iC4Timer = 40;

	int			iWeaponID = 0;
	int			iWeaponSelectIndex = WEAPON_DEAGLE;
	int			iWeaponSelectSkinIndex = -1;
	//[/swap_lines]

	void ReadConfigs(LPCTSTR lpszFileName)
	{
		if (!strstr(lpszFileName, "gui.ini"))
		{
			ConfigList.push_back(lpszFileName);
		}
	}

	void RefreshConfigs()
	{
		ConfigList.clear();
		string ConfigDir = BaseDir + "\\*.ini";
		SearchFiles(ConfigDir.c_str(), ReadConfigs, FALSE);
	}

	bool SendClientHello()
	{
		CMsgClientHello Message;

		Message.set_client_session_need(1);
		Message.clear_socache_have_versions();

		void* ptr = malloc(Message.ByteSize() + 8);

		if (!ptr)
			return false;

		((uint32_t*)ptr)[0] = k_EMsgGCClientHello | ((DWORD)1 << 31);
		((uint32_t*)ptr)[1] = 0;

		Message.SerializeToArray((void*)((DWORD)ptr + 8), Message.ByteSize());

		bool result = Interfaces::SteamGameCoordinator()->SendMessage(k_EMsgGCClientHello | ((DWORD)1 << 31), ptr, Message.ByteSize() + 8) == k_EGCResultOK;

		free(ptr);

		return result;
	}

	bool Initialize(IDirect3DDevice9* pDevice)
	{
		g_pPlayers = new CPlayers();
		g_pRender = new CRender(pDevice);
		g_pGui = new CGui();

		g_pAimbot = new CAimbot();
		g_pEsp = new CEsp();
		g_pRadar = new CRadar();
		g_pSkin = new CSkin();
		g_pMisc = new CMisc();
		g_pInventoryChanger = new CInventoryChanger();

		GuiFile = BaseDir + "\\" + "gui.ini";
		IniFile = BaseDir + "\\" + "settings.ini";

		g_pSkin->InitalizeSkins();

		Settings::LoadSettings(IniFile);

		iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);

		g_pGui->GUI_Init(pDevice);

		RefreshConfigs();

		SendClientHello();

		return true;
	}

	void Shutdown()
	{
		DELETE_MOD(g_pPlayers);
		DELETE_MOD(g_pRender);
		DELETE_MOD(g_pGui);

		DELETE_MOD(g_pAimbot);
		DELETE_MOD(g_pEsp);
		DELETE_MOD(g_pRadar);
		DELETE_MOD(g_pSkin);
		DELETE_MOD(g_pMisc);
	}

	void OnRender()
	{
		if (g_pRender && !Interfaces::Engine()->IsTakingScreenshot() && Interfaces::Engine()->IsActiveApp())
		{
			g_pRender->BeginRender();

			if (g_pGui)
				g_pGui->GUI_Draw_Elements();

			Interfaces::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);

			g_vCenterScreen.x = iScreenWidth / 2.f;
			g_vCenterScreen.y = iScreenHeight / 2.f;

			if (!Interfaces::Engine()->IsConnected())
				g_pRender->Text(3, 3, false, true, Color::Yellow(), WATER_MARK);

			if (Client::g_pPlayers && Client::g_pPlayers->GetLocal() && Interfaces::Engine()->IsInGame())
			{
				if (g_pEsp)
					g_pEsp->OnRender();

				if (g_pMisc)
				{
					g_pMisc->OnRender();
					g_pMisc->OnRenderSpectatorList();
				}
			}

			g_pRender->EndRender();
		}
	}

	void OnLostDevice()
	{
		if (g_pRender)
			g_pRender->OnLostDevice();

		if (g_pGui)
			ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void OnResetDevice()
	{
		if (g_pRender)
			g_pRender->OnResetDevice();

		if (g_pGui)
			ImGui_ImplDX9_CreateDeviceObjects();
	}

	//////////

	void OnRetrieveMessage(void* ecx, void* edx, uint32_t *punMsgType, void *pubDest, uint32_t cubDest, uint32_t *pcubMsgSize)
	{
		g_pInventoryChanger->PostRetrieveMessage(punMsgType, pubDest, cubDest, pcubMsgSize);
	}

	void OnSendMessage(void* ecx, void* edx, uint32_t unMsgType, const void* pubData, uint32_t cubData)
	{
		void* pubDataMutable = const_cast<void*>(pubData);
		g_pInventoryChanger->PreSendMessage(unMsgType, pubDataMutable, cubData);
	}

	void OnCreateMove(CUserCmd* pCmd)
	{
		if (g_pPlayers && Interfaces::Engine()->IsInGame())
		{
			g_pPlayers->Update();

			if (g_pEsp)
				g_pEsp->OnCreateMove(pCmd);

			if (IsLocalAlive())
			{
				if (!bIsGuiVisible)
				{
					int iWeaponSettingsSelectID = GetWeaponSettingsSelectID();

					if (iWeaponSettingsSelectID >= 0)
						iWeaponID = iWeaponSettingsSelectID;
				}

				if (g_pAimbot)
					g_pAimbot->OnCreateMove(pCmd, g_pPlayers->GetLocal());

				if (g_pMisc)
					g_pMisc->OnCreateMove(pCmd);

				backtracking->legitBackTrack(pCmd);

			}
		}
	}

	void OnFireEventClientSideThink(IGameEvent* pEvent)
	{
		if (!strcmp(pEvent->GetName(), "player_connect_full") ||
			!strcmp(pEvent->GetName(), "round_start") ||
			!strcmp(pEvent->GetName(), "cs_game_disconnected"))
		{
			if (g_pPlayers)
				g_pPlayers->Clear();

			if (g_pEsp)
				g_pEsp->OnReset();
		}

		if (Interfaces::Engine()->IsConnected())
		{
			if (g_pEsp)
				g_pEsp->OnEvents(pEvent);

			if (g_pSkin)
				g_pSkin->OnEvents(pEvent);
		}
	}

	void OnFrameStageNotify(ClientFrameStage_t Stage)
	{
		if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		{
			/*
			ConVar* sv_cheats = Interfaces::GetConVar()->FindVar("sv_cheats");
			SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
			sv_cheats_spoofed->SetInt(1);
			*/

			if (g_pMisc)
				g_pMisc->FrameStageNotify(Stage);

			Skin_OnFrameStageNotify(Stage);
			Gloves_OnFrameStageNotify(Stage);
		}
	}

	void OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state,
		const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
	{
		if (Interfaces::Engine()->IsInGame() && ctx && pCustomBoneToWorld)
		{
			if (g_pEsp)
				g_pEsp->OnDrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

			if (g_pMisc)
				g_pMisc->OnDrawModelExecute();
		}
	}

	void OnPlaySound(const Vector* pOrigin, const char* pszSoundName)
	{
		if (!pszSoundName || !Interfaces::Engine()->IsInGame())
			return;

		if (!strstr(pszSoundName, "bulletLtoR") &&
			!strstr(pszSoundName, "rics/ric") &&
			!strstr(pszSoundName, "impact_bullet"))
		{
		}
	}

	void OnPlaySound(const char* pszSoundName)
	{
		if (g_pMisc)
			g_pMisc->OnPlaySound(pszSoundName);
	}

	void OnOverrideView(CViewSetup* pSetup)
	{
		if (g_pMisc)
			g_pMisc->OnOverrideView(pSetup);
	}

	void OnGetViewModelFOV(float& fov)
	{
		if (g_pMisc)
			g_pMisc->OnGetViewModelFOV(fov);
	}

	void OnRenderGUI()
	{
		ImGui::SetNextWindowSize(ImVec2(500.f, 300.f));

		if (ImGui::Begin(CHEAT_NAME, &bIsGuiVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
		{
			ImGui::BeginGroup();
			if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_FovType > 1)
				Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_FovType = 1;

			if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit > 4)
				Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit = 4;

			if (Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Spot > 13)
				Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Spot = 13;

			const char* tabNames[] = {
				AIMBOT_TEXT , VISUAL_TEXT, MISC_TEXT , SKIN_TEXT, CONFIG_TEXT };

			static int tabOrder[] = { 0 , 1 , 2 , 3 , 4, 5 };
			static int tabSelected = 0;
			const bool tabChanged = ImGui::TabLabels(tabNames,
				sizeof(tabNames) / sizeof(tabNames[0]),
				tabSelected, tabOrder);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			float SpaceLineOne = 115.f;
			float SpaceLineTwo = 215.f;
			float SpaceLineTwopointfive = 235.f;
			float SpaceLineThr = 315.f;
			float SpaceLineFor = 390.f;
			float EspLineOne = 130.f;
			float EspLineTwo = 230.f;
			float EspLineThr = 330.f;
			float EspLineFor = 430.f;
			float noone = 240.f;

			if (tabSelected == 0) // Aimbot
			{
				ImGui::Checkbox("  Active", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Active);
				if (iWeaponID <= 9)
				{
					ImGui::SameLine(SpaceLineOne);
					ImGui::Checkbox("Autopistol", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_AutoPistol);
				}
				ImGui::SameLine(SpaceLineTwopointfive);
				ImGui::Checkbox("Friendly Fire", &Settings::Aimbot::aim_Deathmatch);
				ImGui::SameLine(SpaceLineFor);
				ImGui::PushItemWidth(110.f);
				ImGui::Text("Weapon:");
				ImGui::SameLine();
				ImGui::Combo("##AimWeapon", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Checkbox("AntiJump", &Settings::Aimbot::aim_AntiJump);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("WallAttack", &Settings::Aimbot::aim_WallAttack);
				ImGui::SameLine(SpaceLineTwopointfive);
				ImGui::Checkbox("CheckSmoke", &Settings::Aimbot::aim_CheckSmoke);
				ImGui::SameLine();
				ImGui::Checkbox("Fov Span", &Settings::Aimbot::aim_DrawFov);
				ImGui::SameLine();
				ImGui::Checkbox("Fov Spot", &Settings::Aimbot::aim_DrawSpot);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::PushItemWidth(362.f);
				ImGui::SliderInt("Smooth", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Smooth, 1, 300);
				ImGui::SliderInt("Fov", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Fov, 1, 300);
				ImGui::PopItemWidth();

				const char* AimFovType[] = { "Dynamic" , "Static" };
				ImGui::PushItemWidth(362.f);
				ImGui::Combo("Fov Type", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_FovType, AimFovType, IM_ARRAYSIZE(AimFovType));
				ImGui::PopItemWidth();

				const char* BestHit[] = { "Prefer Manually" , "All Spots" , "No Headshot" , "No Arms/Legs" , "No Arms/Legs/Neck" };
				ImGui::PushItemWidth(362.f);
				ImGui::Combo("Hit Scan", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_BestHit, BestHit, IM_ARRAYSIZE(BestHit));

				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("If Preferred Manually then use Aimspot ");

				ImGui::PopItemWidth();

				const char* Aimspot[] = { "Head" , "Neck" , "Lower Neck" , "Body" , "Thorax" , "Chest" , "Right Thigh" ,
					                      "Left Thigh" , "Right Hand" , "Left Hand" , "Right Upperarm" , "Right Forearm" ,
					                      "Left Upperarm" , "Left Forearm" };
				ImGui::PushItemWidth(362.f);
				ImGui::Combo("Aimspot", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Spot, Aimspot, IM_ARRAYSIZE(Aimspot));
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::PushItemWidth(362.f);
				ImGui::SliderInt("ShotDelay", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Delay, 0, 200);
				ImGui::SliderInt("Recoil Control", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Rcs, 0, 100);
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				if (iWeaponID >= 10 && iWeaponID <= 30)
				{
					ImGui::PushItemWidth(362.f);
					ImGui::SliderInt("Rcs Fov", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsFov, 1, 300);
					ImGui::SliderInt("Rcs Smooth", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsSmooth, 1, 300);
					ImGui::PopItemWidth();

					const char* ClampType[] = { "All Target" , "Shot" , "Shot + Target" };
					ImGui::PushItemWidth(362.f);
					ImGui::Combo("Rcs Clamp", &Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_RcsClampType, ClampType, IM_ARRAYSIZE(ClampType));
					ImGui::PopItemWidth();

					ImGui::Spacing();
					ImGui::Separator();
					ImGui::Spacing();
				}
			}

			else if (tabSelected == 1) // Visuals
			{
				string chams_1 = "None";
				string chams_2 = "Active";
				const char* items5[] = { chams_1.c_str() , chams_2.c_str() };
				ImGui::Combo("Chams", &Settings::Esp::esp_Chams, items5, IM_ARRAYSIZE(items5));

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Checkbox("Esp Team", &Settings::Esp::esp_Team);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("Esp Enemy", &Settings::Esp::esp_Enemy);
				ImGui::SameLine(EspLineTwo);
				ImGui::Checkbox("Rank", &Settings::Esp::esp_Rank);
				ImGui::SameLine(EspLineThr);
				ImGui::Checkbox("Radar", &Settings::Radar::rad_InGame);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Checkbox("Name", &Settings::Esp::esp_Name);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("Skeleton", &Settings::Esp::esp_Skeleton);
				ImGui::SameLine(EspLineTwo);
				ImGui::Checkbox("Weapon", &Settings::Esp::esp_Weapon);
				ImGui::SameLine(EspLineThr);
				ImGui::Checkbox("Ammo", &Settings::Esp::esp_Ammo);
				ImGui::SameLine(EspLineFor);
				ImGui::Checkbox("Bomb", &Settings::Esp::esp_Bomb);
				

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				string hpbar_1 = "None";
				string hpbar_2 = "Vertical Bar";
				const char* items3[] = { hpbar_1.c_str() , hpbar_2.c_str() };
				ImGui::Combo("Esp Health", &Settings::Esp::esp_Health, items3, IM_ARRAYSIZE(items3));

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::ColorEdit3("Esp Color CT", Settings::Esp::esp_Color_CT);
				ImGui::ColorEdit3("Esp Color TT", Settings::Esp::esp_Color_TT);
				ImGui::Separator();
				ImGui::ColorEdit3("Chams Color CT", Settings::Esp::chams_Color_CT);
				ImGui::ColorEdit3("Chams Color TT", Settings::Esp::chams_Color_TT);
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
			}

			else if (tabSelected == 2) // Misc
			{
				ImGui::Checkbox("AwpAim", &Settings::Misc::misc_AwpAim);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("AutoAccept", &Settings::Misc::misc_AutoAccept);
				ImGui::SameLine(noone);
				ImGui::Checkbox("Spectators", &Settings::Misc::misc_Spectators);

				ImGui::Checkbox("AutoBhop", &Settings::Misc::misc_Bhop);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("AutoStrafe", &Settings::Misc::misc_AutoStrafe);
				ImGui::SameLine(noone);
				ImGui::Checkbox("Recoil Crosshair", &Settings::Misc::misc_Punch);

				ImGui::Checkbox("No Smoke", &Settings::Misc::misc_NoSmoke);
				ImGui::SameLine(SpaceLineOne);
				ImGui::Checkbox("NoFlash", &Settings::Misc::misc_NoFlash);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Checkbox("Backtrack", &Settings::Aimbot::aim_Backtrack);
				ImGui::SliderInt("Ticks", &Settings::Aimbot::aim_Backtracktickrate, 1, 12);

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::Checkbox("Fov Changer", &Settings::Misc::misc_FovChanger);
				ImGui::PushItemWidth(362.f);
				ImGui::SliderInt("Fov View", &Settings::Misc::misc_FovView, 1, 170);
				ImGui::SliderInt("Fov Model View", &Settings::Misc::misc_FovModelView, 1, 190);
			}

			else if (tabSelected == 3) // Skin
			{
				if (Settings::Misc::misc_SkinChanger && bIsGuiVisible)
				{
					const char* gloves_listbox_items[49] =
					{
						"default",
						"Bloodhound Gloves | Charred","Bloodhound Gloves | Snakebite","Bloodhound Gloves | Bronzed","Bloodhound Gloves | Guerrilla",
						"Driver Gloves | Lunar Weave","Driver Gloves | Convoy","Driver Gloves | Crimson Weave","Driver Gloves | Diamondback","Driver Gloves | King Snake","Driver Gloves | Imperial","Driver Gloves | Overtake","Driver Gloves | Racing Green",
						"Sport Gloves | Superconductor","Sport Gloves | Arid","Sport Gloves | Pandora's Box","Sport Gloves | Hedge Maze","Sport Gloves | Amphibious","Sport Gloves | Bronze Morph","Sport Gloves | Omega","Sport Gloves | Vice",
						"Hand Wraps | Leather","Hand Wraps | Spruce DDPAT","Hand Wraps | Slaughter","Hand Wraps | Badlands","Hand Wraps | Cobalt Skulls","Hand Wraps | Overprint","Hand Wraps | Duct Tape","Hand Wraps | Arboreal",
						"Moto Gloves | Eclipse","Moto Gloves | Spearmint","Moto Gloves | Boom!","Moto Gloves | Cool Mint","Moto Gloves | POW!","Moto Gloves | Turtle","Moto Gloves | Transport","Moto Gloves | Polygon",
						"Specialist Gloves | Forest DDPAT","Specialist Gloves | Crimson Kimono","Specialist Gloves | Emerald Web","Specialist Gloves | Foundation","Specialist Gloves | Crimson Web","Specialist Gloves | Buckshot","Specialist Gloves | Fade","Specialist Gloves | Mogul",
						"Hydra Gloves | Emerald","Hydra Gloves | Mangrove","Hydra Gloves | Rattler","Hydra Gloves | Case Hardened",
					};
					//[enc_string_enable /]

					ImGui::Text("Current Weapon: %s", pWeaponData[iWeaponID]);
					ImGui::PushItemWidth(362.f);
					static int iOldWeaponID = -1;
					ImGui::Combo("Weapon##WeaponSelect", &iWeaponID, pWeaponData, IM_ARRAYSIZE(pWeaponData));
					iWeaponSelectIndex = pWeaponItemIndexData[iWeaponID];
					if (iOldWeaponID != iWeaponID)
						iWeaponSelectSkinIndex = GetWeaponSkinIndexFromPaintKit(g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit);
					iOldWeaponID = iWeaponID;
					string WeaponSkin = pWeaponData[iWeaponID];
					WeaponSkin += " Skin";
					ImGui::ComboBoxArray(WeaponSkin.c_str(), &iWeaponSelectSkinIndex, WeaponSkins[iWeaponID].SkinNames);
					ImGui::Separator();

					const char* knife_models_items[] =
					{
						"Default","Bayonet","Flip","Gut","Karambit" ,"M9 Bayonet",
						"Huntsman","Falchion","Bowie","Butterfly","Shadow Daggers"
					};

					static int iSelectKnifeCTSkinIndex = -1;
					static int iSelectKnifeTTSkinIndex = -1;

					int iKnifeCTModelIndex = Settings::Skin::knf_ct_model;
					int iKnifeTTModelIndex = Settings::Skin::knf_tt_model;

					static int iOldKnifeCTModelIndex = -1;
					static int iOldKnifeTTModelIndex = -1;

					if (iOldKnifeCTModelIndex != iKnifeCTModelIndex && Settings::Skin::knf_ct_model)
						iSelectKnifeCTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, false);

					if (iOldKnifeTTModelIndex != iKnifeTTModelIndex && Settings::Skin::knf_tt_model)
						iSelectKnifeTTSkinIndex = GetKnifeSkinIndexFromPaintKit(Settings::Skin::knf_ct_skin, true);

					iOldKnifeCTModelIndex = iKnifeCTModelIndex;
					iOldKnifeTTModelIndex = iKnifeTTModelIndex;

					string KnifeCTModel = knife_models_items[Settings::Skin::knf_ct_model];
					string KnifeTTModel = knife_models_items[Settings::Skin::knf_tt_model];

					KnifeCTModel += " Skin##KCT";
					KnifeTTModel += " Skin##KTT";

					ImGui::Combo("Knife CT Model", &Settings::Skin::knf_ct_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));
					ImGui::ComboBoxArray(KnifeCTModel.c_str(), &iSelectKnifeCTSkinIndex, KnifeSkins[iKnifeCTModelIndex].SkinNames);
					ImGui::Separator();
					ImGui::Combo("Knife TT Model", &Settings::Skin::knf_tt_model, knife_models_items, IM_ARRAYSIZE(knife_models_items));
					ImGui::ComboBoxArray(KnifeTTModel.c_str(), &iSelectKnifeTTSkinIndex, KnifeSkins[iKnifeTTModelIndex].SkinNames);
					ImGui::Separator();
					ImGui::Combo("Gloves Skin", &Settings::Skin::gloves_skin, gloves_listbox_items, IM_ARRAYSIZE(gloves_listbox_items));

					ImGui::Separator();

					ImGui::PopItemWidth();


					if (ImGui::Button("Apply##Skin"))

					{

						if (iWeaponSelectSkinIndex >= 0)
						{
							g_SkinChangerCfg[iWeaponSelectIndex].nFallbackPaintKit = WeaponSkins[iWeaponID].SkinPaintKit[iWeaponSelectSkinIndex];
							ForceFullUpdate();
						}

						if (iSelectKnifeCTSkinIndex >= 0 && Settings::Skin::knf_ct_model > 0)

						{
							Settings::Skin::knf_ct_skin = KnifeSkins[iKnifeCTModelIndex].SkinPaintKit[iSelectKnifeCTSkinIndex];
							ForceFullUpdate();
						}
						else
						{
							Settings::Skin::knf_ct_skin = 0;
							ForceFullUpdate();
						}

						if (iSelectKnifeTTSkinIndex >= 0 && Settings::Skin::knf_tt_model > 0)
						{
							Settings::Skin::knf_tt_skin = KnifeSkins[iKnifeTTModelIndex].SkinPaintKit[iSelectKnifeTTSkinIndex];
							ForceFullUpdate();
						}
						else
						{
							Settings::Skin::knf_tt_skin = 0;
							ForceFullUpdate();
						}
					}
				}
			}

			else if (tabSelected == 4) // Config
			{
				static int iConfigSelect = 0;
				static int iMenuSheme = 1;
				static char ConfigName[64] = { 0 };

				ImGui::ComboBoxArray("Select Config", &iConfigSelect, ConfigList);

				ImGui::Separator();

				if (ImGui::Button("Load"))
				{
					Settings::LoadSettings(BaseDir + "\\" + ConfigList[iConfigSelect]);
				}
				ImGui::SameLine();
				if (ImGui::Button("Save"))
				{
					Settings::SaveSettings(BaseDir + "\\" + ConfigList[iConfigSelect]);
				}
				ImGui::SameLine();
				if (ImGui::Button("Refresh"))
				{
					RefreshConfigs();
				}

				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();

				ImGui::InputText("Config Name", ConfigName, 64);

				if (ImGui::Button("Save New Config"))
				{
					string ConfigFileName = ConfigName;

					if (ConfigFileName.size() < 1)
					{
						ConfigFileName = "settings";
					}

					Settings::SaveSettings(BaseDir + "\\" + ConfigFileName + ".ini");
					RefreshConfigs();
				}
			}
			ImGui::End();
		}
	}
}