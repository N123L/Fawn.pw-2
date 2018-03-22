#pragma once

#include "../../Engine/Engine.h"

#include <algorithm>

#include <Windows.h>
#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib") 

#define CHECK_VALID( _v)	0

FORCEINLINE vec_t DotProduct(const Vector& a, const Vector& b)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	return(a.x*b.x + a.y*b.y + a.z*b.z);
}

namespace Engine
{
	class CBaseEntity;
	class CPlayer;
}

using namespace Engine;


class CEsp
{
public:
	CEsp();
//[swap_lines]

	Color CT_HP_ColorM;
	Color TT_HP_ColorM;

	Color CT_AR_ColorM;
	Color TT_AR_ColorM;

	IMaterial* visible_flat;
	IMaterial* visible_tex;
	IMaterial* hidden_flat;
	IMaterial* hidden_tex;

	float fExplodeC4Timer;
	float fC4Timer;

	Color GetPlayerColor( CPlayer* pPlayer );
	Color GetPlayerVisibleColor( CPlayer* pPlayer );

	bool CheckPlayerTeam( CPlayer* pPlayer );

	void OnRender();
	void OnCreateMove( CUserCmd* pCmd );
	void OnReset();
	void OnEvents( IGameEvent* pEvent );
	void OnDrawModelExecute( IMatRenderContext* ctx , const DrawModelState_t &state , const ModelRenderInfo_t &pInfo , matrix3x4_t *pCustomBoneToWorld = NULL );

	//[/swap_lines]
private:
//[swap_lines]
	void DrawPlayerEsp( CPlayer* pPlayer );
	void DrawPlayerSkeleton( CPlayer* pPlayer );
	void DrawPlayerBulletTrace( CPlayer* pPlayer );

	void DrawHitBoxLine( Vector* vHitBoxArray , Color color );
//[/swap_lines]
};