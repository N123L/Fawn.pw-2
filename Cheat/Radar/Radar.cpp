#include "Radar.h"

using namespace Client;
//[junk_enable /]
void CRadar::RadarRange( float* x , float* y , float range )
{
	if ( fabs( ( *x ) ) > range || fabs( ( *y ) ) > range )
	{
		if ( ( *y ) > ( *x ) )
		{
			if ( ( *y ) > -( *x ) )
			{
				( *x ) = range*( *x ) / ( *y );
				( *y ) = range;
			}
			else
			{
				( *y ) = -range*( *y ) / ( *x );
				( *x ) = -range;
			}
		}
		else
		{
			if ( ( *y ) > -( *x ) )
			{
				( *y ) = range*( *y ) / ( *x );
				( *x ) = range;
			}
			else
			{
				( *x ) = -range*( *x ) / ( *y );
				( *y ) = -range;
			}
		}
	}
}

void CRadar::CalcRadarPoint( Vector vOrigin , int& screenx , int& screeny )
{
	Vector vAngle;

	Interfaces::Engine()->GetViewAngles( vAngle );

	float dx = vOrigin.x - g_pPlayers->GetLocal()->vEyeOrigin.x;
	float dy = vOrigin.y - g_pPlayers->GetLocal()->vEyeOrigin.y;

	float fYaw = float( vAngle.y * ( M_PI / 180.0 ) );

	float fsin_yaw = sin( fYaw );
	float fminus_cos_yaw = -cos( fYaw );

	float x = dy*fminus_cos_yaw + dx*fsin_yaw;
	float y = dx*fminus_cos_yaw - dy*fsin_yaw;

	float range = (float)Settings::Radar::rad_Range;

	RadarRange( &x , &y , range);

	ImVec2 DrawPos = ImGui::GetCursorScreenPos();
	ImVec2 DrawSize = ImGui::GetContentRegionAvail();

	int rad_x = (int)DrawPos.x;
	int rad_y = (int)DrawPos.y;

	float r_siz_x = DrawSize.x;
	float r_siz_y = DrawSize.y;

	int x_max = (int)r_siz_x + rad_x - 5;
	int y_max = (int)r_siz_y + rad_y - 5;

	screenx = rad_x + ( (int)r_siz_x / 2 + int( x / range * r_siz_x ) );
	screeny = rad_y + ( (int)r_siz_y / 2 + int( y / range * r_siz_y ) );

	if ( screenx > x_max )
		screenx = x_max;
	
	if ( screenx < rad_x )
		screenx = rad_x;
	
	if ( screeny > y_max )
		screeny = y_max;
	
	if ( screeny < rad_y )
		screeny = rad_y;
}

void CRadar::OnRenderPlayer()
{
	for ( BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize(); PlayerIndex++ )
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer( PlayerIndex );

		if ( pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate )
		{
			if (Settings::Radar::rad_InGame)
			{
				if (*pPlayer->m_pEntity->IsSpotted() == false)
					*pPlayer->m_pEntity->IsSpotted() = true;
			}

			int screenx = 0;
			int screeny = 0;

			CalcRadarPoint( pPlayer->vOrigin , screenx , screeny );
		}
	}
}



void CRadar::OnRender()
{
	if ( Settings::Radar::rad_InGame)
	{
		if (Interfaces::Engine()->IsInGame() == false)
		{
		}
		else if (Interfaces::Engine()->IsInGame())
		{
			if (GetKeyState(VK_MENU))
			{
				OnRenderPlayer();
			}
		}
	}
	}