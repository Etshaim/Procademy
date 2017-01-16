#include "stdafx.h"

#include "MakePacket.h"
#include "Protocol.h"
#include "Common.h"
#include "Network.h"
#include "ProtocolBuffer.h"

// 어떤 패킷을 만들어 보낼지 판단하는 기능
void MakePacket(BYTE m_dwActionCur, WORD wPosX, WORD wPosY, BYTE bySideDir)
{
	st_PACKET_HEADER header;
	CProtocolBuffer cBuffer;


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
		MakePacket_MoveStart(&header, &cBuffer,
			m_dwActionCur, wPosX, wPosY);

		SendPacket(&header, &cBuffer);
	}
	break;

	case	dfACTION_ATTACK1:
	{
		MakePacket_Attack1(&header, &cBuffer,
			bySideDir, wPosX, wPosY);

		SendPacket(&header, &cBuffer);
	}
	break;

	case	dfACTION_ATTACK2:
	{
		MakePacket_Attack2(&header, &cBuffer,
			bySideDir, wPosX, wPosY);

		SendPacket(&header, &cBuffer);
	}
	break;

	case	dfACTION_ATTACK3:
	{
		MakePacket_Attack3(&header, &cBuffer,
			bySideDir, wPosX, wPosY);

		SendPacket(&header, &cBuffer);
	}
	break;

	case	dfACTION_STAND:
	{
		MakePacket_MoveStop(&header, &cBuffer,
			bySideDir, wPosX, wPosY);

		SendPacket(&header, &cBuffer);
	}
	break;

	default:
		return;
	}
}



void MakePacket_MoveStart(st_PACKET_HEADER *pHeader, 
	CProtocolBuffer *cpBuffer, BYTE byDirection, WORD wPosX, WORD wPosY)
{
	// 헤더 정보 입력
	pHeader->byCode = dfPACKET_CODE;
	pHeader->bySize = dfSIZE_PACKET_SC_MOVE_START;
	pHeader->byType = dfPACKET_CS_MOVE_START;
	
	// 페이로드 입력
	(*cpBuffer) << byDirection;
	(*cpBuffer) << wPosX;
	(*cpBuffer) << wPosY;

}

void MakePacket_MoveStop(st_PACKET_HEADER *pHeader, 
	CProtocolBuffer *cpBuffer, BYTE byDirection, WORD wPosX, WORD wPosY)
{
	// 헤더 정보 입력
	pHeader->byCode = dfPACKET_CODE;
	pHeader->bySize = dfSIZE_PACKET_SC_MOVE_STOP;
	pHeader->byType = dfPACKET_CS_MOVE_STOP;

	// 페이로드 입력
	(*cpBuffer) << byDirection;
	(*cpBuffer) << wPosX;
	(*cpBuffer) << wPosY;

}

void MakePacket_Attack1(st_PACKET_HEADER *pHeader,
	CProtocolBuffer *cpBuffer, BYTE byDirection, WORD wPosX, WORD wPosY)
{
	// 헤더 정보 입력
	pHeader->byCode = dfPACKET_CODE;
	pHeader->bySize = dfSIZE_PACKET_CS_ATTACK;
	pHeader->byType = dfPACKET_CS_ATTACK1;

	// 페이로드 입력
	(*cpBuffer) << byDirection;
	(*cpBuffer) << wPosX;
	(*cpBuffer) << wPosY;

}

void MakePacket_Attack2(st_PACKET_HEADER *pHeader,
	CProtocolBuffer *cpBuffer, BYTE byDirection, WORD wPosX, WORD wPosY)
{
	// 헤더 정보 입력
	pHeader->byCode = dfPACKET_CODE;
	pHeader->bySize = dfSIZE_PACKET_CS_ATTACK;
	pHeader->byType = dfPACKET_CS_ATTACK2;

	// 페이로드 입력
	(*cpBuffer) << byDirection;
	(*cpBuffer) << wPosX;
	(*cpBuffer) << wPosY;
}

void MakePacket_Attack3(st_PACKET_HEADER *pHeader,
	CProtocolBuffer *cpBuffer, BYTE byDirection, WORD wPosX, WORD wPosY)
{
	// 헤더 정보 입력
	pHeader->byCode = dfPACKET_CODE;
	pHeader->bySize = dfSIZE_PACKET_CS_ATTACK;
	pHeader->byType = dfPACKET_CS_ATTACK3;

	// 페이로드 입력
	(*cpBuffer) << byDirection;
	(*cpBuffer) << wPosX;
	(*cpBuffer) << wPosY;
}

