#include "stdafx.h"
#include "BaseObject.h"
#include "Common.h"

CBaseObject::CBaseObject(int objectType, int iPosX, int iPosY, int iObjectID)
	: m_iObjectType(objectType), m_iObjectID(iObjectID)
{
	OutputDebugString(L"CBaseObject()\n");
	SetPosition(iPosX, iPosY);
	m_dwActionInput = dfACTION_STAND;
	m_bEndFrame = FALSE;
}

CBaseObject::~CBaseObject()
{
	OutputDebugString(L"~CBaseObject()\n");
}

//-----------------------------------------------------------------------
//외부에서의 액션 입력은 단순 멤버 변수에 세팅만 해준다
// 직접적인 스프라이트 변경, 애니메이션 변경, 좌표 이동 등을 해선 안 되며
// 액션 변경에 대한 실질적인 처리는 Action함수에서만 하도록 한다.
//-----------------------------------------------------------------------
void CBaseObject::ActionInput(DWORD dwAction)
{
	m_dwActionInput = dwAction;
}

int CBaseObject::GetCurX(void)
{
	return m_iCurX;
}

int CBaseObject::GetCurY(void)
{
	return m_iCurY;
}

int CBaseObject::GetObjectID(void)
{
	return m_iObjectID;
}

int CBaseObject::GetObjectType(void)
{
	return m_iObjectType;
}

int CBaseObject::GetOldX(void)
{
	return m_iOldX;
}

int CBaseObject::GetOldY(void)
{
	return m_iOldY;
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

void CBaseObject::SetCurPosition(int posX, int posY)
{
	m_iCurX = posX;
	m_iCurY = posY;
}

void CBaseObject::SetObjectID(int objectID)
{
	m_iObjectID = objectID;
}

void CBaseObject::SetObejctType(int objectType)
{
	m_iObjectType = objectType;
}

void CBaseObject::SetOldPositon(int posX, int posY)
{
	m_iOldX = posX;
	m_iOldY = posY;
}

void CBaseObject::SetPosition(int posX, int posY)
{
	// 초기 위치
	m_iCurX = posX;
	m_iCurY = posY;

	m_iOldX = posX;
	m_iOldY = posY;
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