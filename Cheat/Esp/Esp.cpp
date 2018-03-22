#include "Esp.h"

using namespace Client;
//[enc_string_enable /]
//[junk_enable /]


CEsp::CEsp()
{
	// Цвета хп бара

	CT_HP_ColorM.SetColor(255, 64, 64);
	TT_HP_ColorM.SetColor(255, 64, 64);

	// Цвета армор бара

	CT_AR_ColorM.SetColor(255, 64, 64);
	TT_AR_ColorM.SetColor(255, 64, 64);

	visible_flat = nullptr;
	visible_tex = nullptr;
	hidden_flat = nullptr;
	hidden_tex = nullptr;

	fExplodeC4Timer = 0.f;
	fC4Timer = 0.f;
}

Color CEsp::GetPlayerColor(CPlayer* pPlayer)
{
	Color CT_Color(int(Settings::Esp::esp_Color_CT[0] * 255.f), int(Settings::Esp::esp_Color_CT[1] * 255.f), int(Settings::Esp::esp_Color_CT[2] * 255.f));
	Color TT_Color(int(Settings::Esp::esp_Color_TT[0] * 255.f), int(Settings::Esp::esp_Color_TT[1] * 255.f), int(Settings::Esp::esp_Color_TT[2] * 255.f));

	if (pPlayer->Team == TEAM_CT)
	{
		return CT_Color;
	}
	else if (pPlayer->Team == TEAM_TT)
	{
		return TT_Color;
	}

	return Color::White();
}

Color CEsp::GetPlayerVisibleColor(CPlayer* pPlayer)
{
	Color PlayerColor = GetPlayerColor(pPlayer);

	bool SetColor = false;

	// Подсвечивать зелёным если игрок виден:

	if (pPlayer->bVisible)
	{
		if (Settings::Esp::esp_Visible == 0 && pPlayer->Team != g_pPlayers->GetLocal()->Team) // Противников
		{
			SetColor = true;
		}

		Color CT_Color(int(Settings::Esp::esp_Color_CT[0] * 255.f), int(Settings::Esp::esp_Color_CT[1] * 255.f), int(Settings::Esp::esp_Color_CT[2] * 255.f));
		Color TT_Color(int(Settings::Esp::esp_Color_TT[0] * 255.f), int(Settings::Esp::esp_Color_TT[1] * 255.f), int(Settings::Esp::esp_Color_TT[2] * 255.f));

		if (SetColor)
		{
			if (pPlayer->Team == TEAM_CT)
			{
				PlayerColor = CT_Color;
			}
			else if (pPlayer->Team == TEAM_TT)
			{
				PlayerColor = TT_Color;
			}
		}
	}

	return PlayerColor;
}

bool CEsp::CheckPlayerTeam(CPlayer* pPlayer)
{
	bool CheckTeam = false;
	bool PlayerVisible = pPlayer->bVisible;

	if (Settings::Esp::esp_Enemy && pPlayer->Team != g_pPlayers->GetLocal()->Team) 
		CheckTeam = true;

	if (Settings::Esp::esp_Team && pPlayer->Team == g_pPlayers->GetLocal()->Team) 
		CheckTeam = true;

	if (Settings::Esp::esp_Visible >= 3 && !PlayerVisible)
		CheckTeam = false;

	return CheckTeam;
}

void CEsp::OnRender()
{

	if (g_pAimbot)
		g_pAimbot->OnRender();

	if (Settings::Misc::misc_AwpAim && IsLocalAlive() && g_pPlayers->GetLocal()->WeaponIndex == WEAPON_AWP)
	{

		g_pRender->DrawFillBox(iScreenWidth / 2 - 1, iScreenHeight / 2 - 1, 3, 3, Color::Red());
	}

	for (BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++)
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer(PlayerIndex);

		if (pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate && CheckPlayerTeam(pPlayer))
		{
			DrawPlayerEsp(pPlayer);

			if (Settings::Esp::esp_Skeleton)
				DrawPlayerSkeleton(pPlayer);
		}
	}

	if (Settings::Esp::esp_BombTimer)
	{
		if (bC4Timer && iC4Timer)
		{
			float fTimeStamp = Interfaces::Engine()->GetLastTimeStamp();

			if (!fExplodeC4Timer)
			{
				fExplodeC4Timer = fTimeStamp + (float)iC4Timer;
			}
			else
			{
				fC4Timer = fExplodeC4Timer - fTimeStamp;

				if (fC4Timer < 0.f)
					fC4Timer = 0.f;
			}
		}
		else
		{
			fExplodeC4Timer = 0.f;
			fC4Timer = 0.f;
		}
	}

	if (Settings::Esp::esp_Bomb)
	{
		for (int EntIndex = 0; EntIndex < Interfaces::EntityList()->GetHighestEntityIndex(); EntIndex++)
		{
			CBaseEntity* pEntity = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(EntIndex);

			if (!pEntity || pEntity->IsPlayer())
				continue;

			const model_t* pModel = pEntity->GetModel();

			if (pModel)
			{
				const char* pModelName = Interfaces::ModelInfo()->GetModelName(pModel);

				if (pModelName)
				{
					Vector vEntScreen;

					if (WorldToScreen(pEntity->GetRenderOrigin(), vEntScreen))
					{
						if (Settings::Esp::esp_Bomb &&
							(pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CC4 ||
								pEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CPlantedC4))
						{
							if (GetKeyState(VK_MENU))
							{
								g_pRender->Text((int)vEntScreen.x, (int)vEntScreen.y, true, true, Color::Yellow(),
									"C4");
							}
						}
					}
				}
			}
		}
	}
}

void MsgFunc_ServerRankRevealAll()
{
	using tServerRankRevealAllFn = bool(__cdecl*)(int*);
	static tServerRankRevealAllFn ServerRankRevealAll = 0;

	if (!ServerRankRevealAll)
	{
		ServerRankRevealAll = (tServerRankRevealAllFn)(
			CSX::Memory::FindPattern(CLIENT_DLL, "55 8B EC 8B 0D ? ? ? ? 68", 0));
	}

	if (ServerRankRevealAll)
	{
		int fArray[3] = { 0,0,0 };
		ServerRankRevealAll(fArray);
	}
}

void CEsp::OnCreateMove(CUserCmd* pCmd)
{
	if (Settings::Esp::esp_Rank && pCmd->buttons & IN_SCORE)
		MsgFunc_ServerRankRevealAll();
}

void CEsp::OnReset()
{

	if (Settings::Esp::esp_BombTimer)
	{
		if (Settings::Esp::esp_BombTimer > 60)
			Settings::Esp::esp_BombTimer = 60;

		bC4Timer = false;
		iC4Timer = Settings::Esp::esp_BombTimer;
	}
}

void CEsp::OnEvents(IGameEvent* pEvent)
{
	if (g_pEsp && Settings::Esp::esp_BombTimer)
	{
		if (!strcmp(pEvent->GetName(), "bomb_defused") || !strcmp(pEvent->GetName(), "bomb_exploded"))
		{
			bC4Timer = false;
		}
		else if (!strcmp(pEvent->GetName(), "bomb_planted"))
		{
			bC4Timer = true;
		}
	}
}

void CEsp::OnDrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
{
	if (!g_pPlayers || Interfaces::Engine()->IsTakingScreenshot() || !Interfaces::Engine()->IsConnected() || !pInfo.pModel)
		return;

	static bool InitalizeMaterial = false;

	if (!InitalizeMaterial)
	{
		visible_flat = CreateMaterial(true, false);
		visible_tex = CreateMaterial(false, false);
		hidden_flat = CreateMaterial(true, true);
		hidden_tex = CreateMaterial(false, true);

		InitalizeMaterial = true;

		return;
	}

	string strModelName = Interfaces::ModelInfo()->GetModelName(pInfo.pModel);

	if (strModelName.size() <= 1)
		return;

	if (Settings::Esp::esp_Chams && Client::g_pPlayers && Client::g_pPlayers->GetLocal() && strModelName.find("models/player") != string::npos)
	{
		IClientEntity* pBaseEntity = Interfaces::EntityList()->GetClientEntity(pInfo.entity_index);

		if (pBaseEntity && pBaseEntity->GetClientClass()->m_ClassID == (int)CLIENT_CLASS_ID::CCSPlayer)
		{
			CPlayer* pPlayer = g_pPlayers->GetPlayer(pInfo.entity_index);

			if (pPlayer && pPlayer->bUpdate)
			{
				bool CheckTeam = false;

				Color TeamHideColor;
				Color TeamVisibleColor;

				if (Settings::Esp::esp_Enemy && pPlayer->Team != g_pPlayers->GetLocal()->Team) // Противников
					CheckTeam = true;

				if (Settings::Esp::esp_Team && pPlayer->Team == g_pPlayers->GetLocal()->Team) // Своих
					CheckTeam = true;

				if (pPlayer->Team == TEAM_CT)
				{
					TeamHideColor = Color(int(Settings::Esp::chams_Color_CT[0] * 255.f),
						int(Settings::Esp::chams_Color_CT[1] * 255.f),
						int(Settings::Esp::chams_Color_CT[2] * 255.f));
				}
				else if (pPlayer->Team == TEAM_TT)
				{
					TeamHideColor = Color(int(Settings::Esp::chams_Color_TT[0] * 255.f),
						int(Settings::Esp::chams_Color_TT[1] * 255.f),
						int(Settings::Esp::chams_Color_TT[2] * 255.f));
				}

				bool SetColor = false;

				if (Settings::Esp::esp_Visible == 0 && pPlayer->Team != g_pPlayers->GetLocal()->Team) // Противников
				{
					SetColor = true;
				}

				if (SetColor)
				{
					if (pPlayer->Team == TEAM_CT)
					{
						TeamVisibleColor = Color(int(Settings::Esp::chams_Color_CT[0] * 255.f),
							int(Settings::Esp::chams_Color_CT[1] * 255.f),
							int(Settings::Esp::chams_Color_CT[2] * 255.f));
					}
					else if (pPlayer->Team == TEAM_TT)
					{
						TeamVisibleColor = Color(int(Settings::Esp::chams_Color_TT[0] * 255.f),
							int(Settings::Esp::chams_Color_TT[1] * 255.f),
							int(Settings::Esp::chams_Color_TT[2] * 255.f));
					}
				}
				else
					TeamVisibleColor = TeamHideColor;

				if (CheckTeam)
				{
					if (Settings::Esp::esp_Visible <= 0)
					{
						if (Settings::Esp::esp_Chams >= 1)
						{
							ForceMaterial(TeamHideColor, hidden_tex);
							hidden_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
						}
					}
					else
					{
						if (Settings::Esp::esp_Chams >= 1)
						{
							ForceMaterial(TeamHideColor, hidden_tex);
							hidden_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
						}
					}

					Interfaces::ModelRender()->DrawModelExecute(ctx, state, pInfo, pCustomBoneToWorld);

					if (Settings::Esp::esp_Chams == 1)
					{
						ForceMaterial(TeamVisibleColor, visible_tex);
						visible_tex->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
					}
				}
			}
		}
	}
}

void CEsp::DrawPlayerEsp(CPlayer* pPlayer)
{
	bool bOriginScreen = (pPlayer->vOriginScreen.x > 0 && pPlayer->vOriginScreen.y > 0);
	bool bHitBoxScreen = (pPlayer->vHitboxHeadScreen.x > 0 && pPlayer->vHitboxHeadScreen.y > 0);

	if (!bOriginScreen && !bHitBoxScreen)
		return;

	Vector vLineOrigin;

	int Height = (int)pPlayer->vOriginScreen.y - (int)pPlayer->vHitboxHeadScreen.y;

	if (Height < 18)
		Height = 18;

	int Width = Height / 2;

	int x = (int)pPlayer->vHitboxHeadScreen.x - Width / 2;
	int y = (int)pPlayer->vHitboxHeadScreen.y;

	vLineOrigin = pPlayer->vHitboxHeadScreen;
	vLineOrigin.y += Height;

	Color EspPlayerColor = GetPlayerColor(pPlayer);
	Color EspVisibleColor = GetPlayerVisibleColor(pPlayer);

	if (Settings::Esp::esp_Name)
	{
		if (GetKeyState(VK_MENU))
		{
			g_pRender->Text((int)vLineOrigin.x, (int)pPlayer->vHitboxHeadScreen.y - 13, true, true, EspPlayerColor, pPlayer->Name.c_str());

		}
	}
	int iHpAmY = 1;

	if (Settings::Esp::esp_Health >= 1)
	{
		Color Minus = Color::Red();

		if (pPlayer->Team == TEAM_CT)
		{
			Minus = CT_HP_ColorM;
		}
		else if (pPlayer->Team == TEAM_TT)
		{
			Minus = TT_HP_ColorM;
		}

		int iHealth = pPlayer->iHealth;

		if (iHealth)
		{
			if (Settings::Esp::esp_Health == 1)
			{
				if (GetKeyState(VK_MENU))
				{
					g_pRender->DrawVerBar(x - 6, (int)pPlayer->vHitboxHeadScreen.y, 5, Height, iHealth, Color::LawnGreen(), Minus);
				}
			}
		}
	}

	if (Settings::Esp::esp_Weapon && !pPlayer->WeaponName.empty())
	{
		if (GetKeyState(VK_MENU))
		{
			string WeaponStr = pPlayer->WeaponName;

			if (Settings::Esp::esp_Ammo && pPlayer->iWAmmo)
			{
				WeaponStr += " (";
				WeaponStr += to_string(pPlayer->iWAmmo);
				WeaponStr += ")";
			}

			g_pRender->Text((int)vLineOrigin.x, (int)vLineOrigin.y + iHpAmY, true, true, EspPlayerColor, WeaponStr.erase(0, 6).c_str());
			iHpAmY += 10;
		}

	}
}

void CEsp::DrawPlayerSkeleton(CPlayer* pPlayer)
{
	Color SkeletonColor = GetPlayerColor(pPlayer);

	for (BYTE IndexArray = 0; IndexArray < 18; IndexArray++)
	{
		if (GetKeyState(VK_MENU))
		{
			DrawHitBoxLine(pPlayer->vHitboxSkeletonArray[IndexArray], SkeletonColor);
		}
	}
}

void CEsp::DrawPlayerBulletTrace(CPlayer* pPlayer)
{
	Color EspColor = GetPlayerColor(pPlayer);

	if (!pPlayer->vBulletTraceArray[0].IsZero() && !pPlayer->vBulletTraceArray[1].IsZero())
	{
		g_pRender->DrawLine(
			(int)pPlayer->vBulletTraceArray[0].x, (int)pPlayer->vBulletTraceArray[0].y,
			(int)pPlayer->vBulletTraceArray[1].x, (int)pPlayer->vBulletTraceArray[1].y, EspColor);

		g_pRender->DrawFillBox((int)pPlayer->vBulletTraceArray[1].x - 2, (int)pPlayer->vBulletTraceArray[1].y - 2, 4, 4, EspColor);
	}
}

void CEsp::DrawHitBoxLine(Vector* vHitBoxArray, Color color)
{
	Vector vHitBoxOneScreen;
	Vector vHitBoxTwoScreen;

	if (vHitBoxArray[0].IsZero() || !vHitBoxArray[0].IsValid() || vHitBoxArray[1].IsZero() || !vHitBoxArray[1].IsValid())
		return;

	if (WorldToScreen(vHitBoxArray[0], vHitBoxOneScreen) && WorldToScreen(vHitBoxArray[1], vHitBoxTwoScreen))
	{
		g_pRender->DrawLine((int)vHitBoxOneScreen.x, (int)vHitBoxOneScreen.y,
			(int)vHitBoxTwoScreen.x, (int)vHitBoxTwoScreen.y, color);
	}
}

