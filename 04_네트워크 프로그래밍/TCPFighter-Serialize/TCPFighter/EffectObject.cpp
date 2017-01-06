#include "stdafx.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"
#include "SpriteDIB.h"
#include "LinkedList.h"
#include "Common.h"

extern CSpriteDib						g_cSpriteDib;
extern CDoubleLinkedList<CBaseObject*>	objectList;

CEffectObject::CEffectObject
(int objectType, DWORD dwAttackID, DWORD dwDamageID)
	: CBaseObject(objectType, 0, 0, 0)
	, m_dwAttackID(dwAttackID)
	, m_dwDamageID(dwDamageID)
	, m_bEffectStart(FALSE)
	// ObjectID 설정 Effect는 추가 안 함
{

}

CEffectObject::~CEffectObject()
{

}

void	CEffectObject::Action(void)
{
	// 프레임이 끝났으면 삭제
	if (m_bEndFrame)
	{
		m_iObjectType = eEND_EFFECT;
		return;
	}

	NextFrame();

	// 충돌 체크
	// 충돌이 되지 않았으면 삭제

	//<< 여기서부터는 충돌이 된 경우만 해당
	// 공격자 찾아서 액션별 해당 프레임에 활성화
	
	int iSpriteIndex = -1;

	CDoubleLinkedList<CBaseObject*>::Iterator iterator;

	for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
	{
		if ((*iterator)->GetObjectID() == m_dwAttackID)
		{
			// sprite 얻기
			iSpriteIndex = (*iterator)->GetSprite();

			//((CPlayerObject*)(*iterator))->SetSideDirection(m_iAttackDir);

			m_iAttackDir = ((CPlayerObject*)(*iterator))->GetSideDirection();
			break;
		}
	}
	
	// 해당 프레임에 활성화
	switch (iSpriteIndex)
	{
	case ePLAYER_ATTACK1_L01:
	case ePLAYER_ATTACK1_R01:
	case ePLAYER_ATTACK2_L02:
	case ePLAYER_ATTACK2_R02:
	case ePLAYER_ATTACK3_L04:
	case ePLAYER_ATTACK3_R04:
	{
		m_bEffectStart = TRUE;
		SetSprite(eEFFECT_SPARK_01, eEFFECT_SPARK_MAX, dfDELAY_EFFECT);

		// 맞은 플레이어 위치 찾기
		for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
		{
			if ((*iterator)->GetObjectID() == m_dwDamageID)
			{
				// 맞은 플레이어 위치 얻기
				m_wCurX = ((CPlayerObject*)(*iterator))->GetCurX();
				m_wCurY = ((CPlayerObject*)(*iterator))->GetCurY();
				break;
			}
		}
	}
		break;
	default:
		break;
	}
}

void	CEffectObject::Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	// 프레임이 끝났으면 삭제
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
		g_cSpriteDib.DrawSprite(m_iSpriteNow, GetCurX(), GetCurY() - 70, bypDest, iDestWidth, iDestHeight, iDestPitch);
	}
	else if(dfDIRECTION_RIGHT == m_iAttackDir)
	{
		g_cSpriteDib.DrawSprite(m_iSpriteNow, GetCurX(), GetCurY() - 70, bypDest, iDestWidth, iDestHeight, iDestPitch);
	}
	
}