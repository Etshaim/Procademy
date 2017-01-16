#include "stdafx.h"
#include "Map.h"
#include "Common.h"
#include "ScreenDIB.h"
#include "SpriteDIB.h"
#include "PlayerObject.h"

extern CScreenDib		g_cScreenDib;
extern CSpriteDib		g_cSpriteDib;
extern CPlayerObject	*g_pPlayerObject;

CMap::CMap()
{

}

CMap::~CMap()
{

}

void CMap::DrawTile(BYTE * bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	int iTileX = m_iDrawPosX / 64;
	int iTileY = m_iDrawPosY / 64;

	int iDrawX = m_iDrawPosX -(m_iDrawPosX % 64);
	int iDrawY = m_iDrawPosY -(m_iDrawPosY % 64);

	int iCurDrawX = 0;
	int iCurDrawY = 0;

	for (int iCountY = 0; iCountY < 9; iCountY++)
	{
		iCurDrawY = iDrawY + iCountY * 64;

		for (int iCountX = 0; iCountX < 11; iCountX++)
		{
			iCurDrawX = iDrawX + iCountX * 64;

			g_cSpriteDib.DrawSprite(e_SPRITE::eMAP, iCurDrawX, iCurDrawY, bypDest, iDestWidth, iDestHeight, iDestPitch);
		}
	}
}

void CMap::SetDrawPos(int iPosX, int iPosY)
{
	m_iDrawPosX = iPosX - dfWINDOW_WIDTH / 2;
	m_iDrawPosX = max(m_iDrawPosX, 0);
	m_iDrawPosX = min(m_iDrawPosX, dfRANGE_MOVE_RIGHT - dfWINDOW_WIDTH);

	m_iDrawPosY = iPosY - dfWINDOW_HEIGHT / 2 - 40;	// 캐릭터 중점이 발이기 때문에 좀 더 올려줌
	m_iDrawPosY = max(m_iDrawPosY, 0);
	m_iDrawPosY = min(m_iDrawPosY, dfRANGE_MOVE_BOTTOM - dfWINDOW_HEIGHT);
}

int CMap::GetDrawPosX(void)
{
	return m_iDrawPosX;
}

int CMap::GetDrawPosY(void)
{
	return m_iDrawPosY;
}
