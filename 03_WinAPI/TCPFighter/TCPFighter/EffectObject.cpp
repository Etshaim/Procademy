#include "stdafx.h"
#include "EffectObject.h"
#include "SpriteDIB.h"
#include "LinkedList.h"
#include "Common.h"

extern CSpriteDib						g_cSpriteDib;
extern CDoubleLinkedList<CBaseObject*>	objectList;

CEffectObject::CEffectObject
(int objectType, int iPosX, int iPosY, int dwAttackID, int iDirection)
	: CBaseObject(objectType, iPosX, iPosY, 0)
	, m_dwAttackID(dwAttackID)
	, m_bEffectStart(FALSE)
	// ObjectID ���� Effect�� �߰� �� ��
	, m_iAttackDir(iDirection)
{

}

CEffectObject::~CEffectObject()
{

}

void	CEffectObject::Action(void)
{
	// �������� �������� ����
	if (m_bEndFrame)
	{
		m_iObjectType = eEND_EFFECT;
		return;
	}

	NextFrame();

	// �浹 üũ
	// �浹�� ���� �ʾ����� ����

	//<< ���⼭���ʹ� �浹�� �� ��츸 �ش�
	// ������ ã�Ƽ� �׼Ǻ� �ش� �����ӿ� Ȱ��ȭ
	
	int iSpriteIndex = -1;

	CDoubleLinkedList<CBaseObject*>::Iterator iterator;

	for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
	{
		if ((*iterator)->GetObjectID() == m_dwAttackID)
		{
			iSpriteIndex = (*iterator)->GetSprite();
		}
	}
	
	// �ش� �����ӿ� Ȱ��ȭ
	switch (iSpriteIndex)
	{
	case ePLAYER_ATTACK1_L01:
	case ePLAYER_ATTACK1_R01:
	case ePLAYER_ATTACK2_L02:
	case ePLAYER_ATTACK2_R02:
	case ePLAYER_ATTACK3_L04:
	case ePLAYER_ATTACK3_R04:
		m_bEffectStart = TRUE;

		SetSprite(eEFFECT_SPARK_01, eEFFECT_SPARK_MAX, dfDELAY_EFFECT);
		break;
	}
}

void	CEffectObject::Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	// �������� �������� ����
	if (m_bEndFrame)
	{
		return;
	}

	if (!m_bEffectStart)
	{
		return;
	}
	
	if (dfDIRECTION_LEFT == m_iAttackDir)
	{
		g_cSpriteDib.DrawSprite(m_iSpriteNow, GetCurX() - 70, GetCurY() - 70, bypDest, iDestWidth, iDestHeight, iDestPitch);
	}
	else if(dfDIRECTION_RIGHT == m_iAttackDir)
	{
		g_cSpriteDib.DrawSprite(m_iSpriteNow, GetCurX() + 70, GetCurY() - 70, bypDest, iDestWidth, iDestHeight, iDestPitch);
	}
	
}