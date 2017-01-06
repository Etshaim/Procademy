#include "stdafx.h"

#include "BaseObject.h"
#include "Common.h"


CBaseObject::CBaseObject(int objectType, WORD wPosX, WORD wPosY, int iObjectID)
	: m_iObjectType(objectType), m_iObjectID(iObjectID)
{
	SetPosition(wPosX, wPosY);
	

	m_bEndFrame = FALSE;
}

CBaseObject::~CBaseObject()
{

}



WORD CBaseObject::GetCurX(void)
{
	return m_wCurX;
}

WORD CBaseObject::GetCurY(void)
{
	return m_wCurY;
}

int CBaseObject::GetObjectID(void)
{
	return m_iObjectID;
}

int CBaseObject::GetObjectType(void)
{
	return m_iObjectType;
}

WORD CBaseObject::GetOldX(void)
{
	return m_wOldX;
}

WORD CBaseObject::GetOldY(void)
{
	return m_wOldY;
}

int CBaseObject::GetSprite(void)
{
	return m_iSpriteNow;
}

BOOL CBaseObject::IsEndFrame(void)
{
	return m_bEndFrame;
}

void CBaseObject::NextFrame(void)
{
	if (0 > m_iSpriteStart)
	{
		// 현재 스프라이트 지정을 안 한 상태
		return;
	}

	//---------------------------------------------------
	// 프레임 딜레이 값을 넘어야 다음 프레임으로 넘어간다
	//---------------------------------------------------
	m_iDelayCount++;	// Framedelay만큼 반복해서 Draw함. m_iDelayCount는 카운팅용 변수

	if (m_iDelayCount >= m_iFrameDelay)
	{
		// FrameDelay만큼 Draw했으면
		m_iDelayCount = 0;

		//------------------------------------------------
		// 애니메이션 끝으로 이동되면 처음으로 돌린다
		//------------------------------------------------
		m_iSpriteNow++;

		if (m_iSpriteNow >= m_iSpriteMax)
		{
			m_iSpriteNow = m_iSpriteStart;
			m_bEndFrame	 = TRUE;
		}
	}
}



void CBaseObject::SetCurPosition(WORD posX, WORD posY)
{
	m_wCurX = posX;
	m_wCurY = posY;
}

void CBaseObject::SetObjectID(int objectID)
{
	m_iObjectID = objectID;
}

void CBaseObject::SetObejctType(int objectType)
{
	m_iObjectType = objectType;
}

void CBaseObject::SetOldPositon(WORD posX, WORD posY)
{
	m_wOldX = posX;
	m_wOldY = posY;
}

void CBaseObject::SetPosition(WORD posX, WORD posY)
{
	// 초기 위치
	m_wOldX = m_wCurX;
	m_wOldY = m_wCurY;

	m_wCurX = posX;
	m_wCurY = posY;
}

void CBaseObject::SetSprite(int iSpriteStart, int iSpriteMax, int iFramDelay)
{
	m_iSpriteStart	= iSpriteStart;
	m_iSpriteMax	= iSpriteMax;
	m_iFrameDelay	= iFramDelay;

	m_iSpriteNow	= iSpriteStart;
	m_iDelayCount	= 0;
	m_bEndFrame		= FALSE;
}