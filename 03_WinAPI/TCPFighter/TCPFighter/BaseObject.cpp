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
//�ܺο����� �׼� �Է��� �ܼ� ��� ������ ���ø� ���ش�
// �������� ��������Ʈ ����, �ִϸ��̼� ����, ��ǥ �̵� ���� �ؼ� �� �Ǹ�
// �׼� ���濡 ���� �������� ó���� Action�Լ������� �ϵ��� �Ѵ�.
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
		// ���� ��������Ʈ ������ �� �� ����
		return;
	}

	//---------------------------------------------------
	// ������ ������ ���� �Ѿ�� ���� ���������� �Ѿ��
	//---------------------------------------------------
	m_iDelayCount++;	// Framedelay��ŭ �ݺ��ؼ� Draw��. m_iDelayCount�� ī���ÿ� ����

	if (m_iDelayCount >= m_iFrameDelay)
	{
		// FrameDelay��ŭ Draw������
		m_iDelayCount = 0;

		//------------------------------------------------
		// �ִϸ��̼� ������ �̵��Ǹ� ó������ ������
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
	// �ʱ� ��ġ
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