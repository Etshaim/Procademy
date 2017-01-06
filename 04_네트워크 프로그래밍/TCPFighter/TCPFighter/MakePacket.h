#ifndef __MAKE_PACKET_H__
#define __MAKE_PACKET_H__


struct st_PACKET_HEADER;
struct st_PACKET_HEADER;
struct st_PACKET_MY_CHARACTER;
struct st_PACKET_OTHER_CHARACTER;
struct st_PACKET_DELETE_CHARACTER;
struct st_PACKET_CS_MOVE_START;
struct st_PACKET_SC_MOVE_START;
struct st_PACKET_CS_MOVE_STOP;
struct st_PACKET_SC_MOVE_STOP;
struct st_PACKET_CS_ATTACK;
struct st_PACKET_SC_ATTACK;
struct st_PACKET_DAMAGE;

void MakePacket(BYTE m_dwActionCur, WORD wPosX, WORD wPosY, BYTE bySideDir);

void MakePacket_MoveStart(st_PACKET_HEADER *pHeader, 
	st_PACKET_CS_MOVE_START *pPacket, BYTE byDirection, WORD wPosX, WORD wPosY);

void MakePacket_MoveStop(st_PACKET_HEADER *pHeader, 
	st_PACKET_CS_MOVE_STOP *pPacket, BYTE byDirection, WORD wPosX, WORD wPosY);

void MakePacket_Attack1(st_PACKET_HEADER *pHeader,
	st_PACKET_CS_ATTACK *pPacket, BYTE byDirection, WORD wPosX, WORD wPosY);

void MakePacket_Attack2(st_PACKET_HEADER *pHeader,
	st_PACKET_CS_ATTACK *pPacket, BYTE byDirection, WORD wPosX, WORD wPosY);

void MakePacket_Attack3(st_PACKET_HEADER *pHeader,
	st_PACKET_CS_ATTACK *pPacket, BYTE byDirection, WORD wPosX, WORD wPosY);

#endif // !__MAKE_PACKET_H__

