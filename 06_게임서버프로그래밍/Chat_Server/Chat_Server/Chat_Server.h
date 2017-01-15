#pragma once

#include <Windows.h>
#include <map>
#include <list>
#include "StreamSQ.h"
#include "Protocol.h"

using namespace std;

class CProtocolBuffer;

// 클라이언트 구조체
struct st_CLIENT
{
	SOCKET			sock;
	SOCKADDR_IN		connectAddr;

	CStreamSQ		SendQ;
	CStreamSQ		RecvQ;

	DWORD			dwUserNO;
	DWORD			dwEnterRoomNO;

	WCHAR			szNickName[dfNICK_MAX_LEN];
};

// 채팅방 구조체
struct st_CHAT_ROOM
{
	DWORD			dwRoomNO;
	WCHAR			szTitle[256];
	list<DWORD>		UserList;
};

// 네트워크 초기화
BOOL	NetworkInitial(void);

void	NetworkProcess(void);

void	SelectSocket(DWORD *dwpTableNO, SOCKET *pTableSocket,
	FD_SET *pReadSet, FD_SET *pWriteSet);

int		MakeRecvPacket(st_CLIENT *pClient);

BYTE	MakeCheckSum(CProtocolBuffer *pPacket, WORD wMsgType);

BOOL	PacketProc(st_CLIENT *pClient, WORD wMsgType, CProtocolBuffer *pPacket);

// netproc
void	netProc_Recv(DWORD dwUserNO);
void	netProc_Send(DWORD dwUserNO);
void	netProc_Accept(void);

// Client 찾기
st_CLIENT	*FindClient(DWORD dwUserNO);

// Client 끊기
BOOL	DisconnectClient(DWORD dwUserNO);

BOOL	netPacket_ReqLogin(st_CLIENT *pClient, CProtocolBuffer *pPacket);
BOOL	netPacket_ReqRoomList(st_CLIENT *pClient, CProtocolBuffer *pPacket);
BOOL	netPacket_ReqRoomCreate(st_CLIENT *pClient, CProtocolBuffer *pPacket);
BOOL	netPacket_ReqRoomEnter(st_CLIENT *pClient, CProtocolBuffer *pPacket);
BOOL	netPacket_ReqChat(st_CLIENT *pClient, CProtocolBuffer *pPacket);
BOOL	netPacket_ReqRoomLeave(st_CLIENT *pClient, CProtocolBuffer *pPacket);


void	Send_ResLogin(st_CLIENT *pClient, BYTE byResult);
void	Send_ResRoomList(st_CLIENT *pClient, BYTE byResult);
void	Send_ResRoomCreate(st_CLIENT *pClient, st_CHAT_ROOM *pRoom, BYTE byResult);
void	Send_ResRoomEnter(st_CLIENT *pClient, st_CHAT_ROOM *pRoom, BYTE byResult);
void	Send_ResChat(st_CLIENT *pClient, WORD wMsgSize, PWCHAR szMsg);
void	Send_ResRoomLeave(st_CLIENT *pClient);
void	Send_ResRoomDelete(st_CLIENT *pClient, st_CHAT_ROOM *pRoom);
void	Send_ResUserEnter(st_CLIENT *pClient);


void	MakePacket_ResLogin(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, BYTE byResult, st_CLIENT *pClient);
void	MakePacket_ResRoomList(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, BYTE byResult);
void	MakePacket_ResRoomCreate(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, BYTE byResult, st_CHAT_ROOM *pRoom);
void	MakePacket_ResRoomEnter(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, BYTE byResult, st_CHAT_ROOM *pRoom);
void	MakePacket_ResChat(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_CLIENT *pClient, BYTE byResult, WORD wMsgSize, PWCHAR szMsg);
void	MakePacket_ResUserEnter(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_CLIENT *pClient);
void	MakePacket_ResRoomLeave(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_CLIENT *pClient);
void	MakePacket_ResRoomDelete(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_CLIENT *pClient, st_CHAT_ROOM *pRoom);

// 특정 클라이언트에게 보내기
void	SendUnicast(st_CLIENT *pClient, st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);

// 모든 유저에게 보내기
void	SendBroadcast(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);

// 방 유저에게 보내기( 당사자는 제외)
void	SendBroadcastRoom(st_CHAT_ROOM *pRoom, st_CLIENT *pClient,
	st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);

// packet을 받아서 checksum 만들기
BYTE	MakeCheckSum(CProtocolBuffer *pPacket, WORD dwType);


void err_quit(WCHAR *msg);
void err_display(WCHAR *msg);