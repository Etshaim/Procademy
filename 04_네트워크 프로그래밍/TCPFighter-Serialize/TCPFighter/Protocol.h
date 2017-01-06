#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define dfSIZE_PACKET_HEADER					4
#define dfSIZE_PACKET_END						1

// 패킷 사이즈
// 서버 -> 클라
#define dfSIZE_PACKET_CS_MOVE_START				9
#define dfSIZE_PACKET_CS_MOVE_STOP				9
#define dfSIZE_PACKET_CS_ATTACK					9

// 클라 -> 서버
#define dfSIZE_PACKET_SC_MOVE_START				5
#define dfSIZE_PACKET_SC_MOVE_STOP				5
#define dfSIZE_PACKET_SC_ATTACK					5



#define	dfPORT									5000
#define dfPACKET_CODE							0X89
#define dfNETWORK_PACKET_END					((BYTE)0x79)

#define dfPACKET_SC_CREATE_MY_CHARACTER			0
#define dfPACKET_SC_CREATE_OTHER_CHARACTER		1
#define dfPACKET_SC_DELETE_CHARACTER			2

#define dfPACKET_CS_MOVE_START					10
#define dfPACKET_SC_MOVE_START					11

#define dfPACKET_MOVE_DIR_LL				0
#define dfPACKET_MOVE_DIR_LU				1
#define dfPACKET_MOVE_DIR_UU				2
#define dfPACKET_MOVE_DIR_RU				3
#define dfPACKET_MOVE_DIR_RR				4
#define dfPACKET_MOVE_DIR_RD				5
#define dfPACKET_MOVE_DIR_DD				6
#define dfPACKET_MOVE_DIR_LD				7

#define dfPACKET_CS_MOVE_STOP					12
#define dfPACKET_SC_MOVE_STOP					13

#define dfPACKET_CS_ATTACK1						20
#define dfPACKET_SC_ATTACK1						21
#define dfPACKET_CS_ATTACK2						22
#define dfPACKET_SC_ATTACK2						23
#define dfPACKET_CS_ATTACK3						24
#define dfPACKET_SC_ATTACK3						25

#define dfPACKET_SC_DAMAGE						30

#pragma pack(push, 1)
struct st_PACKET_HEADER
{
	BYTE	byCode;		// 패킷 코드 0x89 고정
	BYTE	bySize;		// 패킷 페이로드 사이즈
	BYTE	byType;		// 패킷 타입
	BYTE	byTemp;		// 사용 안 함
};
#pragma pack(pop)

//
//struct	st_PACKET_MY_CHARACTER
//{
//	int		iID;
//	BYTE	byDirection;
//	WORD	wPosX;
//	WORD	wPosY;
//	BYTE	byHP;
//};
//
//struct st_PACKET_OTHER_CHARACTER
//{
//	int		iID;
//	BYTE	byDirection;
//	WORD	wPosX;
//	WORD	wPosY;
//	BYTE	byHP;
//};
//
//struct st_PACKET_DELETE_CHARACTER
//{
//	int		iID;
//};
//
//struct st_PACKET_CS_MOVE_START
//{
//	BYTE	byDirection;
//	WORD	wPosX;
//	WORD	wPosY;
//};
//
//struct st_PACKET_SC_MOVE_START
//{
//	int		iID;
//	BYTE	byDirection;
//	WORD	wPosX;
//	WORD	wPosY;
//};
//
//struct st_PACKET_CS_MOVE_STOP
//{
//	BYTE	byDirection;
//	WORD	wPosX;
//	WORD	wPosY;
//};
//
//struct st_PACKET_SC_MOVE_STOP
//{
//	int		iID;
//	BYTE	byDirection;
//	WORD	wPosX;
//	WORD	wPosY;
//};
//
//struct st_PACKET_CS_ATTACK
//{
//	BYTE	byDirection;
//	WORD	wPosX;
//	WORD	wPosY;
//};
//
//struct st_PACKET_SC_ATTACK
//{
//	int		iID;
//	BYTE	byDirection;
//	WORD	wPosX;
//	WORD	wPosY;
//};
//
//struct st_PACKET_DAMAGE
//{
//	int		iAttackID;
//	int		iDamageID;
//	BYTE	byDamageHP;
//};



#endif // !__PROTOCOL_H__



