#include "Misc.h"


using namespace Client;
//[junk_enable /]
//[enc_string_enable /]

void CMisc::OnRender()
{
	if (Settings::Misc::misc_Punch)
	{
		int punch_x = (int)g_vCenterScreen.x + (int)g_pPlayers->GetLocal()->vAimPunch.y * 10;
		int punch_y = (int)g_vCenterScreen.y + (int)g_pPlayers->GetLocal()->vAimPunch.x * 10;

		g_pRender->DrawFillBox(punch_x - 1, punch_y - 1, 3, 3, Color::Green());
	}
}

void CMisc::OnCreateMove(CUserCmd* pCmd)
{
	if (Settings::Misc::misc_Bhop)
	{
		if (pCmd->buttons & IN_JUMP && !(g_pPlayers->GetLocal()->iFlags & FL_ONGROUND))
		{
			pCmd->buttons &= ~IN_JUMP;
		}
	}

	if (Settings::Misc::misc_AutoStrafe && !(g_pPlayers->GetLocal()->iFlags & FL_ONGROUND))
	{
		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = -450.0f;
		}
		else if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = 450.0f;
		}
	}
}

void CMisc::FrameStageNotify(ClientFrameStage_t Stage)
{
	CBaseEntity* localplayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	if (Interfaces::Engine()->IsInGame() && localplayer && Stage == ClientFrameStage_t::FRAME_RENDER_START)
	{
		static QAngle vecAngles;
		Interfaces::Engine()->GetViewAngles(vecAngles);
	}
}

std::vector<const char*> smoke_materials = {
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	"particle/vistasmokev1/vistasmokev1_fire",
};

void CMisc::OnDrawModelExecute()
{
	static bool NoSmoke = false;
	static bool NoFlashReset = false;

	if (Settings::Misc::misc_NoFlash && !NoFlashReset)
	{
		IMaterial* flash = Interfaces::MaterialSystem()->FindMaterial(
			"effects\\flashbang", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* flashWhite = Interfaces::MaterialSystem()->FindMaterial("effects\\flashbang_white",
			TEXTURE_GROUP_CLIENT_EFFECTS);

		if (flash && flashWhite)
		{
			flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			flashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

			NoFlashReset = true;
		}
	}
	else if (!Settings::Misc::misc_NoFlash && NoFlashReset)
	{
		IMaterial* flash = Interfaces::MaterialSystem()->FindMaterial(
			"effects\\flashbang", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* flashWhite = Interfaces::MaterialSystem()->FindMaterial("effects\\flashbang_white",
			TEXTURE_GROUP_CLIENT_EFFECTS);

		if (flash && flashWhite)
		{
			flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			flashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);

			NoFlashReset = false;
		}
	}

	if (Settings::Misc::misc_NoSmoke)
	{
		IMaterial* vistasmokev1_smokegrenade = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_smokegrenade", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_emods = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_emods", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_emods_impactdust = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_emods_impactdust", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_fire = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_fire", TEXTURE_GROUP_CLIENT_EFFECTS);

		if (vistasmokev1_smokegrenade && vistasmokev1_emods && vistasmokev1_emods_impactdust && vistasmokev1_fire)
		{
			vistasmokev1_smokegrenade->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_emods->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_emods_impactdust->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			vistasmokev1_fire->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

			NoSmoke = true;
		}
	}
	else if (!Settings::Misc::misc_NoSmoke)
	{
		IMaterial* vistasmokev1_smokegrenade = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_smokegrenade", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_emods = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_emods", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_emods_impactdust = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_emods_impactdust", TEXTURE_GROUP_CLIENT_EFFECTS);

		IMaterial* vistasmokev1_fire = Interfaces::MaterialSystem()->FindMaterial("particle/vistasmokev1/vistasmokev1_fire", TEXTURE_GROUP_CLIENT_EFFECTS);

		if (vistasmokev1_smokegrenade && vistasmokev1_emods && vistasmokev1_emods_impactdust && vistasmokev1_fire)
		{
			vistasmokev1_smokegrenade->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			vistasmokev1_emods->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			vistasmokev1_emods_impactdust->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
			vistasmokev1_fire->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);

			NoSmoke = false;

		}
	}
}

void CMisc::OnPlaySound(const char* pszSoundName)
{
	if (Settings::Misc::misc_AutoAccept && !strcmp(pszSoundName, "!UI/competitive_accept_beep.wav"))
	{
		typedef void(*IsReadyCallBackFn)();

		IsReadyCallBackFn IsReadyCallBack = 0;

		if (!IsReadyCallBack)
		{
			IsReadyCallBack = (IsReadyCallBackFn)(
				CSX::Memory::FindPattern(CLIENT_DLL, "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE", 0));

#if ENABLE_DEBUG_FILE == 1
			CSX::Log::Add("::IsReadyCallBack = %X", IsReadyCallBack);
#endif
		}

		if (IsReadyCallBack)
		{
			IsReadyCallBack();
		}
	}
}

void CMisc::OnOverrideView(CViewSetup * pSetup)
{
	if (Settings::Misc::misc_FovChanger && !Interfaces::Engine()->IsTakingScreenshot())
	{
		CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

		if (!pPlayer)
			return;

		if (pPlayer->GetIsScoped())
			return;

		if (pPlayer->IsDead())
		{
			if (pPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && pPlayer->GetObserverTarget())
				pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetObserverTarget());

			if (!pPlayer)
				return;
		}

		pSetup->fov = (float)Settings::Misc::misc_FovView;
	}
}

void CMisc::OnGetViewModelFOV(float& fov)
{
	if (Settings::Misc::misc_FovChanger && !Interfaces::Engine()->IsTakingScreenshot())
	{
		CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

		if (!pPlayer)
			return;

		if (pPlayer->IsDead())
		{
			if (pPlayer->GetObserverMode() == ObserverMode_t::OBS_MODE_IN_EYE && pPlayer->GetObserverTarget())
				pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetObserverTarget());

			if (!pPlayer)
				return;
		}

		fov = (float)Settings::Misc::misc_FovModelView;
	}
}

vector<int> CMisc::GetObservervators(int playerId)
{
	vector<int> SpectatorList;

	CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(playerId);

	if (!pPlayer)
		return SpectatorList;

	if (pPlayer->IsDead())
	{
		CBaseEntity* pObserverTarget = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pPlayer->GetObserverTarget());

		if (!pObserverTarget)
			return SpectatorList;

		pPlayer = pObserverTarget;
	}

	for (int PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++)
	{
		CBaseEntity* pCheckPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(PlayerIndex);

		if (!pCheckPlayer)
			continue;

		if (pCheckPlayer->IsDormant() || !pCheckPlayer->IsDead())
			continue;

		CBaseEntity* pObserverTarget = (CBaseEntity*)Interfaces::EntityList()->GetClientEntityFromHandle(pCheckPlayer->GetObserverTarget());

		if (!pObserverTarget)
			continue;

		if (pPlayer != pObserverTarget)
			continue;

		SpectatorList.push_back(PlayerIndex);
	}

	return SpectatorList;
}

void CMisc::OnRenderSpectatorList()
{
	if (Settings::Misc::misc_Spectators)
	{
		int DrawIndex = 1;

		for (int playerId : GetObservervators(Interfaces::Engine()->GetLocalPlayer()))
		{
			if (playerId == Interfaces::Engine()->GetLocalPlayer())
				continue;

			CBaseEntity* pPlayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(playerId);

			if (!pPlayer)
				continue;

			PlayerInfo Pinfo;
			Interfaces::Engine()->GetPlayerInfo(playerId, &Pinfo);

			if (Pinfo.m_bIsFakePlayer)
				continue;

			if (g_pRender)
			{
				string Name = Pinfo.m_szPlayerName;

				if (Name != "")
				{
					Color PlayerObsColor;
					//[junk_disable /]
					switch (pPlayer->GetObserverMode())
					{
					case ObserverMode_t::OBS_MODE_IN_EYE:
						Name.append(" - Perspective");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_CHASE:
						Name.append(" - 3rd person");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_ROAMING:
						Name.append(" - Free look");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_DEATHCAM:
						Name.append(" - Deathcam");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_FREEZECAM:
						Name.append(" - Freezecam");
						PlayerObsColor = Color::White();
						break;
					case ObserverMode_t::OBS_MODE_FIXED:
						Name.append(" - Fixed");
						PlayerObsColor = Color::White();
						break;
					default:
						break;
					}
					//[junk_enable /]
					g_pRender->Text(iScreenWidth - 300, 300 + (DrawIndex * 13), false, true, PlayerObsColor, "%s", Name.c_str());
					DrawIndex++;
				}
			}
		}
	}
}
