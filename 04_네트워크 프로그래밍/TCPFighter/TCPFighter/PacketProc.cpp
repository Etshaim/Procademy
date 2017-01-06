#include "stdafx.h"

#include "Common.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "EffectObject.h"

#include "LinkedList.h"

#include "StreamSQ.h"
#include "PacketProc.h"

#include "Protocol.h"

extern BOOL				g_bConnected;
extern BOOL				g_SendFlag;
extern CPlayerObject	*g_pPlayerObject;
extern CDoubleLinkedList<CBaseObject*> objectList;
extern	SOCKET			g_Socket;

void PacketProc_CreateMyCharacter(char *pPacket)
{
	st_PACKET_MY_CHARACTER *pCharacter = (st_PACKET_MY_CHARACTER*)pPacket;

	// CPlayerObject 생성
	// ( int objectType, int iCurX, int iCurY )
	g_pPlayerObject = new CPlayerObject(	e_OBJECT_TYPE::ePLAYER,
											pCharacter->wPosX, 
											pCharacter->wPosY, 
											pCharacter->byHP,
											pCharacter->byDirection, 
											pCharacter->iID
										);

	g_pPlayerObject->SetMyCharacter();
	//g_pPlayerObject->InputAction(pCharacter->byDirection);
	g_pPlayerObject->SetSideDirection(pCharacter->byDirection);

	objectList.PushBack(g_pPlayerObject);


	g_bConnected	= TRUE;
	g_SendFlag		= TRUE;
}

void PacketProc_CreateOtherCharacter(char *pPacket)
{
	st_PACKET_OTHER_CHARACTER *pCharacter = (st_PACKET_OTHER_CHARACTER*)pPacket;

	CPlayerObject *pTemp = new CPlayerObject(	e_OBJECT_TYPE::eOTHER_PLAYER,
												pCharacter->wPosX,
												pCharacter->wPosY,
												pCharacter->byHP,
												pCharacter->byDirection,
												pCharacter->iID
											);

	//g_pPlayerObject->InputAction(pCharacter->byDirection);
	g_pPlayerObject->SetSideDirection(pCharacter->byDirection);

	objectList.PushBack(pTemp);
}


void PacketProc_DeleteCharacter(char *pPacket)
{
	st_PACKET_DELETE_CHARACTER *pDelete = (st_PACKET_DELETE_CHARACTER*)pPacket;

	
	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	for (	iter = objectList.Begin(); iter != objectList.End(); iter++	)
	{
		if (pDelete->iID == (*iter)->GetObjectID() )
		{
			if (pDelete->iID == g_pPlayerObject->GetObjectID())
			{
				closesocket(g_Socket);
				g_bConnected = FALSE;
			}
			objectList.Erase(iter);
			break;
		}
	}
}

void PacketProc_MoveStart(char* pPacket)
{
	st_PACKET_SC_MOVE_START *pTemp = (st_PACKET_SC_MOVE_START*)pPacket;

	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		if (pTemp->iID == (*iter)->GetObjectID())
		{
			//// x,y 좌표 설정
			//(*iter)->SetPosition(pTemp->wPosX, pTemp->wPosY);
			//((CPlayerObject*)(*iter))->SetLeftRight(pTemp->byDirection);

			//if (pTemp->iID == g_pPlayerObject->GetObjectID())
			//{
			//	return;
			//}
			((CPlayerObject*)(*iter))->InputAction(pTemp->byDirection);
			//((CPlayerObject*)(*iter))->SetSideDirection(pTemp->byDirection);
			
			break;
		} 
	}

}

void PacketProc_MoveStop(char* pPacket)
{
	st_PACKET_SC_MOVE_STOP *pTemp = (st_PACKET_SC_MOVE_STOP*)pPacket;

	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		if (pTemp->iID == (*iter)->GetObjectID())
		{
			/*if (pTemp->iID == g_pPlayerObject->GetObjectID())
			{
				return;
			}*/

			// x,y 좌표 설정
			(*iter)->SetPosition(pTemp->wPosX, pTemp->wPosY);

			((CPlayerObject*)(*iter))->InputAction(dfACTION_STAND);
			((CPlayerObject*)(*iter))->SetSideDirection(pTemp->byDirection);

			break;
		}
	}
}

void PacketProc_Attack1(char* pPacket)
{
	st_PACKET_SC_ATTACK *pTemp = (st_PACKET_SC_ATTACK*)pPacket;

	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	// 캐릭터를 찾는다
	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		if (pTemp->iID == (*iter)->GetObjectID())
		{
			/*if (pTemp->iID == g_pPlayerObject->GetObjectID())
			{
				return;
			}*/

			// x,y 좌표 설정
			(*iter)->SetPosition(pTemp->wPosX, pTemp->wPosY);
			((CPlayerObject*)(*iter))->InputAction(dfACTION_ATTACK1);
			((CPlayerObject*)(*iter))->SetSideDirection(pTemp->byDirection);
			break;
		}
	}
}

void PacketProc_Attack2(char* pPacket)
{
	st_PACKET_SC_ATTACK *pTemp = (st_PACKET_SC_ATTACK*)pPacket;

	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	// 캐릭터를 찾는다
	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		if (pTemp->iID == (*iter)->GetObjectID())
		{
			/*if (pTemp->iID == g_pPlayerObject->GetObjectID())
			{
				return;
			}
*/
			// x,y 좌표 설정
			(*iter)->SetPosition(pTemp->wPosX, pTemp->wPosY);
			((CPlayerObject*)(*iter))->InputAction(dfACTION_ATTACK2);
			((CPlayerObject*)(*iter))->SetSideDirection(pTemp->byDirection);
			break;
		}
	}
}

void PacketProc_Attack3(char* pPacket)
{
	st_PACKET_SC_ATTACK *pTemp = (st_PACKET_SC_ATTACK*)pPacket;

	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	// 캐릭터를 찾는다
	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		if (pTemp->iID == (*iter)->GetObjectID())
		{
			/*if (pTemp->iID == g_pPlayerObject->GetObjectID())
			{
				return;
			}*/

			// x,y 좌표 설정
			(*iter)->SetPosition(pTemp->wPosX, pTemp->wPosY);
			((CPlayerObject*)(*iter))->InputAction(dfACTION_ATTACK3);
			((CPlayerObject*)(*iter))->SetSideDirection(pTemp->byDirection);
			break;
		}
	}
}

void PacketProc_Damage(char* pPacket)
{
	st_PACKET_DAMAGE *pTemp = (st_PACKET_DAMAGE*)pPacket;

	// effect 객체 만들기
	// 공격자 ID로 공격자 프레임 주시하고 있다가 애니메이션 시작
	CEffectObject *pEffect = new CEffectObject(
		e_OBJECT_TYPE::eEFFECT, pTemp->iAttackID, pTemp->iDamageID);

	objectList.PushBack(pEffect);

	// 이미 피격된 상태이므로 바로 hp를 깎아준다
	CDoubleLinkedList<CBaseObject*>::Iterator iter;

	// 캐릭터를 찾아서
	for (iter = objectList.Begin(); iter != objectList.End(); iter++)
	{
		// HP를 깎아준다
		if (pTemp->iDamageID == (*iter)->GetObjectID())
		{
			((CPlayerObject*)(*iter))->SetHP(pTemp->byDamageHP);
			break;
		}
	}
}