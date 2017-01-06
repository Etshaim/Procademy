#include "stdafx.h"

#include "Common.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"
#include "LinkedList.h"
#include "StreamSQ.h"
#include "PacketProc.h"
#include "Protocol.h"
#include "ProtocolBuffer.h"

extern BOOL				g_bConnected;
extern BOOL				g_SendFlag;
extern CPlayerObject	*g_pPlayerObject;
extern CDoubleLinkedList<CBaseObject*> objectList;
extern	SOCKET			g_Socket;

void PacketProc_CreateMyCharacter(CProtocolBuffer *cpPacket)
{
	int		iID;
	BYTE	byDirection;
	WORD	wPosX;
	WORD	wPosY;
	BYTE	byHP;
	
	(*cpPacket) >> iID;
	(*cpPacket) >> byDirection;
	(*cpPacket) >> wPosX;
	(*cpPacket) >> wPosY;
	(*cpPacket) >> byHP;

	// CPlayerObject 생성
	// ( int objectType, int iCurX, int iCurY )
	g_pPlayerObject = new CPlayerObject(	e_OBJECT_TYPE::ePLAYER,
											wPosX, 
											wPosY,
											byHP,
											byDirection, 
											iID
										);

	g_pPlayerObject->SetMyCharacter();
	g_pPlayerObject->SetSideDirection(byDirection);

	objectList.PushBack(g_pPlayerObject);


	g_bConnected	= TRUE;
	g_SendFlag		= TRUE;
}

void PacketProc_CreateOtherCharacter(CProtocolBuffer *cpPacket)
{
	int		iID;
	BYTE	byDirection;
	WORD	wPosX;
	WORD	wPosY;
	BYTE	byHP;

	(*cpPacket) >> iID;
	(*cpPacket) >> byDirection;
	(*cpPacket) >> wPosX;
	(*cpPacket) >> wPosY;
	(*cpPacket) >> byHP;


	CPlayerObject *pTemp = new CPlayerObject(	e_OBJECT_TYPE::eOTHER_PLAYER,
												wPosX,
												wPosY,
												byHP,
												byDirection,
												iID
											);

	//g_pPlayerObject->InputAction(pCharacter->byDirection);
	g_pPlayerObject->SetSideDirection( byDirection );

	objectList.PushBack(pTemp);
}


void PacketProc_DeleteCharacter(CProtocolBuffer *cpPacket)
{
	int		iID;

	(*cpPacket) >> iID;
	
	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	for (	iter = objectList.Begin(); iter != objectList.End(); iter++	)
	{
		if ( iID == (*iter)->GetObjectID() )
		{
			if ( iID == g_pPlayerObject->GetObjectID() )
			{
				closesocket(g_Socket);
				g_bConnected = FALSE;
			}
			objectList.Erase(iter);
			break;
		}
	}
}

void PacketProc_MoveStart(CProtocolBuffer *cpPacket)
{
	int		iID;
	BYTE	byDirection;
	WORD	wPosX;
	WORD	wPosY;

	(*cpPacket) >> iID;
	(*cpPacket) >> byDirection;
	(*cpPacket) >> wPosX;
	(*cpPacket) >> wPosY;
	
	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		if ( iID == (*iter)->GetObjectID() )
		{
			//// x,y 좌표 설정
			//(*iter)->SetPosition(pTemp->wPosX, pTemp->wPosY);
			//((CPlayerObject*)(*iter))->SetLeftRight(pTemp->byDirection);

			//if (pTemp->iID == g_pPlayerObject->GetObjectID())
			//{
			//	return;
			//}
			((CPlayerObject*)(*iter))->InputAction( byDirection );
			//((CPlayerObject*)(*iter))->SetSideDirection(pTemp->byDirection);
			
			break;
		} 
	}

}

void PacketProc_MoveStop(CProtocolBuffer *cpPacket)
{
	int		iID;
	BYTE	byDirection;
	WORD	wPosX;
	WORD	wPosY;

	(*cpPacket) >> iID;
	(*cpPacket) >> byDirection;
	(*cpPacket) >> wPosX;
	(*cpPacket) >> wPosY;


	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		if ( iID == (*iter)->GetObjectID() )
		{
			// x,y 좌표 설정
			(*iter)->SetPosition( wPosX, wPosY );

			((CPlayerObject*)(*iter))->InputAction(dfACTION_STAND);
			((CPlayerObject*)(*iter))->SetSideDirection( byDirection );

			break;
		}
	}
}

void PacketProc_Attack1(CProtocolBuffer *cpPacket)
{
	int		iID;
	BYTE	byDirection;
	WORD	wPosX;
	WORD	wPosY;

	(*cpPacket) >> iID;
	(*cpPacket) >> byDirection;
	(*cpPacket) >> wPosX;
	(*cpPacket) >> wPosY;

	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	// 캐릭터를 찾는다
	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		if ( iID == (*iter)->GetObjectID())
		{
			// x,y 좌표 설정
			(*iter)->SetPosition( wPosX,  wPosY );
			((CPlayerObject*)(*iter))->InputAction(dfACTION_ATTACK1);
			((CPlayerObject*)(*iter))->SetSideDirection( byDirection);
			break;
		}
	}
}

void PacketProc_Attack2(CProtocolBuffer *cpPacket)
{
	int		iID;
	BYTE	byDirection;
	WORD	wPosX;
	WORD	wPosY;

	(*cpPacket) >> iID;
	(*cpPacket) >> byDirection;
	(*cpPacket) >> wPosX;
	(*cpPacket) >> wPosY;


	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	// 캐릭터를 찾는다
	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		if ( iID == (*iter)->GetObjectID() )
		{
			/*if (pTemp->iID == g_pPlayerObject->GetObjectID())
			{
				return;
			}
*/
			// x,y 좌표 설정
			(*iter)->SetPosition( wPosX,  wPosY );
			((CPlayerObject*)(*iter))->InputAction(dfACTION_ATTACK2);
			((CPlayerObject*)(*iter))->SetSideDirection( byDirection );
			break;
		}
	}
}

void PacketProc_Attack3(CProtocolBuffer *cpPacket)
{
	int		iID;
	BYTE	byDirection;
	WORD	wPosX;
	WORD	wPosY;

	(*cpPacket) >> iID;
	(*cpPacket) >> byDirection;
	(*cpPacket) >> wPosX;
	(*cpPacket) >> wPosY;

	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	// 캐릭터를 찾는다
	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		if ( iID == (*iter)->GetObjectID() )
		{
			/*if (pTemp->iID == g_pPlayerObject->GetObjectID())
			{
				return;
			}*/

			// x,y 좌표 설정
			(*iter)->SetPosition( wPosX, wPosY );
			((CPlayerObject*)(*iter))->InputAction(dfACTION_ATTACK3);
			((CPlayerObject*)(*iter))->SetSideDirection( byDirection );
			break;
		}
	}
}

void PacketProc_Damage(CProtocolBuffer *cpPacket)
{
	int		iAttackID;
	int		iDamageID;
	BYTE	byDamageHP;

	(*cpPacket) >> iAttackID;
	(*cpPacket) >> iDamageID;
	(*cpPacket) >> byDamageHP;


	// effect 객체 만들기
	// 공격자 ID로 공격자 프레임 주시하고 있다가 애니메이션 시작
	CEffectObject *pEffect = new CEffectObject(
		e_OBJECT_TYPE::eEFFECT,  iAttackID,  iDamageID);

	objectList.PushBack(pEffect);

	// 이미 피격된 상태이므로 바로 hp를 깎아준다
	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	// 캐릭터를 찾아서
	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		// HP를 깎아준다
		if ( iDamageID == (*iter)->GetObjectID())
		{
			((CPlayerObject*)(*iter))->SetHP( byDamageHP );
			break;
		}
	}
}