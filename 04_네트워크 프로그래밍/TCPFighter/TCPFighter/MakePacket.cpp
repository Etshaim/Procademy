#include "stdafx.h"

#include "MakePacket.h"
#include "Protocol.h"
#include "Common.h"
#include "Network.h"

// 어떤 패킷을 만들어 보낼지 판단하는 기능
void MakePacket(BYTE m_dwActionCur, WORD wPosX, WORD wPosY, BYTE bySideDir)
{
	st_PACKET_HEADER header;

	switch (m_dwActionCur)
	{
	case	dfMOVE_DIR_LL:
	case	dfMOVE_DIR_LU:
	case	dfMOVE_DIR_UU:
	case	dfMOVE_DIR_RU:
	case	dfMOVE_DIR_RR:
	case	dfMOVE_DIR_RD:
	case	dfMOVE_DIR_DD:
	case	dfMOVE_DIR_LD:
	{
		st_PACKET_CS_MOVE_START packet;
		ZeroMemory(&packet, sizeof(packet));

		MakePacket_MoveStart(&header, &packet,
			m_dwActionCur, wPosX, wPosY);

		SendPacket(&header, (char*)&packet);
	}
	break;

	case	dfACTION_ATTACK1:
	{
		st_PACKET_CS_ATTACK packet;
		ZeroMemory(&packet, sizeof(packet));

		MakePacket_Attack1(&header, &packet,
			bySideDir, wPosX, wPosY);

		SendPacket(&header, (char*)&packet);
	}
	break;

	case	dfACTION_ATTACK2:
	{
		st_PACKET_CS_ATTACK packet;
		ZeroMemory(&packet, sizeof(packet));

		MakePacket_Attack2(&header, &packet,
			bySideDir, wPosX, wPosY);

		SendPacket(&header, (char*)&packet);
	}
	break;

	case	dfACTION_ATTACK3:
	{
		st_PACKET_CS_ATTACK packet;
		ZeroMemory(&packet, sizeof(packet));

		MakePacket_Attack3(&header, &packet,
			bySideDir, wPosX, wPosY);

		SendPacket(&header, (char*)&packet);
	}
	break;

	case	dfACTION_STAND:
	{
		st_PACKET_CS_MOVE_STOP packet;
		ZeroMemory(&packet, sizeof(packet));

		MakePacket_MoveStop(&header, &packet,
			bySideDir, wPosX, wPosY);

		SendPacket(&header, (char*)&packet);
	}
	break;

	default:
		return;
	}
}



void MakePacket_MoveStart(st_PACKET_HEADER *pHeader, 
	st_PACKET_CS_MOVE_START *pPacket, BYTE byDirection, WORD wPosX, WORD wPosY)
{
	// 헤더 정보 입력
	pHeader->byCode = dfPACKET_CODE;
	pHeader->bySize = sizeof(st_PACKET_CS_MOVE_START);
	pHeader->byType = dfPACKET_CS_MOVE_START;
	
	// 페이로드 입력
	pPacket->byDirection	= byDirection;
	pPacket->wPosX			= wPosX;
	pPacket->wPosY			= wPosY;

}

void MakePacket_MoveStop(st_PACKET_HEADER *pHeader, 
	st_PACKET_CS_MOVE_STOP *pPacket, BYTE byDirection, WORD wPosX, WORD wPosY)
{
	// 헤더 정보 입력
	pHeader->byCode = dfPACKET_CODE;
	pHeader->bySize = sizeof(st_PACKET_CS_MOVE_STOP);
	pHeader->byType = dfPACKET_CS_MOVE_STOP;

	// 페이로드 입력
	pPacket->byDirection	= byDirection;
	pPacket->wPosX			= wPosX;
	pPacket->wPosY			= wPosY;

}

void MakePacket_Attack1(st_PACKET_HEADER *pHeader,
	st_PACKET_CS_ATTACK *pPacket, BYTE byDirection, WORD wPosX, WORD wPosY)
{
	// 헤더 정보 입력
	pHeader->byCode = dfPACKET_CODE;
	pHeader->bySize = sizeof(st_PACKET_CS_ATTACK);
	pHeader->byType = dfPACKET_CS_ATTACK1;

	// 페이로드 입력
	pPacket->byDirection = byDirection;
	pPacket->wPosX = wPosX;
	pPacket->wPosY = wPosY;

}

void MakePacket_Attack2(st_PACKET_HEADER *pHeader,
	st_PACKET_CS_ATTACK *pPacket, BYTE byDirection, WORD wPosX, WORD wPosY)
{
	// 헤더 정보 입력
	pHeader->byCode = dfPACKET_CODE;
	pHeader->bySize = sizeof(st_PACKET_CS_ATTACK);
	pHeader->byType = dfPACKET_CS_ATTACK2;

	// 페이로드 입력
	pPacket->byDirection = byDirection;
	pPacket->wPosX = wPosX;
	pPacket->wPosY = wPosY;
}

void MakePacket_Attack3(st_PACKET_HEADER *pHeader,
	st_PACKET_CS_ATTACK *pPacket, BYTE byDirection, WORD wPosX, WORD wPosY)
{
	// 헤더 정보 입력
	pHeader->byCode = dfPACKET_CODE;
	pHeader->bySize = sizeof(st_PACKET_CS_ATTACK);
	pHeader->byType = dfPACKET_CS_ATTACK3;

	// 페이로드 입력
	pPacket->byDirection = byDirection;
	pPacket->wPosX = wPosX;
	pPacket->wPosY = wPosY;
}

