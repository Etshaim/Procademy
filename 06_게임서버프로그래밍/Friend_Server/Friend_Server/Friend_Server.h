#pragma once

#include "Protocol.h"
#include "StreamSQ.h"

class CProtocolBuffer;

struct st_DATA_ACCOUNT
{
	UINT64	AccountNo;
	WCHAR	szID[dfNICK_MAX_LEN];
};

// 클라이언트 구조체
struct st_CLIENT
{
	st_CLIENT()
	{
		pAccount = nullptr;
	}

	SOCKET			sock;
	SOCKADDR_IN		connectAddr;

	CStreamSQ		SendQ;
	CStreamSQ		RecvQ;

	st_DATA_ACCOUNT	*pAccount;
};

struct st_DATA_FRIEND
{
	UINT64	No;
	UINT64	FromAccountNo;
	UINT64	ToAccountNo;
	time_t	Time;
};

struct st_DATA_FRIEND_REQUEST
{
	UINT64	No;
	UINT64	FromAccountNo;
	UINT64	ToAccountNo;
	time_t	Time;
};

// 네트워크 초기화
BOOL	NetworkInitial(void);
void	NetworkProcess(void);
void	SelectSocket(UINT64 *dwpTableNO, SOCKET *pTableSocket, FD_SET *pReadSet, FD_SET *pWriteSet);

// 오류 출력
void	err_quit(WCHAR *msg);
void	err_display(WCHAR *msg);

// 계정 생성
void	CreateAccount(WCHAR *szNickName);

// client 끊기
BOOL	DisconnectClient(SOCKET socket);

// 클라이언트 찾기
st_CLIENT *FindClient(SOCKET socket);

// 패킷 프로세스
BOOL	PacketProc(st_CLIENT *pClient, WORD wMsgType, CProtocolBuffer *pPacket);

// 온전한 패킷 만들기
int		MakeRecvPacket(st_CLIENT *pClient);

// net 프로세스
void	netProc_Recv(SOCKET socket);
void	netProc_Send(SOCKET socket);
void	netProc_Accept(void);

// Request
BOOL 	netPacket_ReqAccountAdd(st_CLIENT *pClient, CProtocolBuffer *pPacket);				//회원 가입		 요청(REQ)
BOOL	netPacket_ReqLogin(st_CLIENT *pClient, CProtocolBuffer *pPacket);				//회원 로그인	 요청(REQ)
BOOL	netPacket_ReqAccountList(st_CLIENT *pClient, CProtocolBuffer *pPacket);			//회원 목록 	 요청(REQ)

BOOL	netPacket_ReqFriendList(st_CLIENT *pClient, CProtocolBuffer *pPacket);			//친구 목록 	 요청(REQ)
BOOL	netPacket_ReqFriendList_Request(st_CLIENT *pClient, CProtocolBuffer *pPacket);	//보낸 친구요청 목록 요청(REQ)
BOOL	netPacket_ReqFriendList_Reply(st_CLIENT *pClient, CProtocolBuffer *pPacket);	//받은 친구요청 목록 요청(REQ)

BOOL 	netPacket_ReqFriendRemove(st_CLIENT *pClient, CProtocolBuffer *pPacket);		//친구 끊기 	 요청(REQ)
BOOL	netPacket_ReqFriendRequest(st_CLIENT *pClient, CProtocolBuffer *pPacket);		//친구 			 요청(REQ)
BOOL	netPacket_ReqFriendCancel(st_CLIENT *pClient, CProtocolBuffer *pPacket);		//친구 요청 취소 요청(REQ)

BOOL	netPacket_ReqFriendDeny(st_CLIENT *pClient, CProtocolBuffer *pPacket);			//친구 요청 거부 요청(REQ)
BOOL	netPacket_ReqFriendAgree(st_CLIENT *pClient, CProtocolBuffer *pPacket);			//친구 요청 수락 요청(REQ)

// Response
void	Send_ResAccountAdd(st_CLIENT *pClient);		//회원 가입 			결과(RES)
void	Send_ResLogin(st_CLIENT *pClient);			//회원 로그인 			결과(RES)
void	Send_ResAccountList(st_CLIENT *pClient);					//회원 리스트 			결과(RES)

void	Send_ResFriendList(st_CLIENT *pClient);						//친구 목록 			결과(RES)
void	Send_ResFriendList_Request(st_CLIENT *pClient);				//보낸 친구요청 목록 	결과(RES)
void 	Send_ResFriendList_Reply(st_CLIENT *pClient);				//받은 친구요청 목록 	결과(RES)

void	Send_ResFriendRemove(st_CLIENT *pClient);					//친구 끊기 			결과(RES)
void	Send_ResFriendRequest(st_CLIENT *pClient, UINT64 AccountNo, BYTE byResult);	//친구 요청 			결과(RES)
void	Send_ResFriendCancel(st_CLIENT *pClient);					//친구 요청 취소 		결과(RES)

void	Send_ResFriendDeny(st_CLIENT *pClient);						//친구 요청 거부 		결과(RES)
void	Send_ResFriendAgree(st_CLIENT *pClient, UINT64 FriendAccount, BYTE byResult);		//친구 요청 수락 		결과(RES)
		
		
// 패킷 만들기
void	makePacket_ResAccountAdd(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, UINT64 AccountNo);
void	makePacket_ResLogin(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_DATA_ACCOUNT *pAccount );
void	makePacket_ResAccountList(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);

void	makePacket_ResFriendList(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_DATA_ACCOUNT *pAccount);
void	makePacket_ResFriendList_Request(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_DATA_ACCOUNT *pAccount);
void	makePacket_ResFriendList_Reply(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_DATA_ACCOUNT *pAccount);

void	makePacket_ResFriendRemove(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);
void	makePacket_ResFriendRequest(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, UINT64 FriendAccountNo, BYTE byResult);
void	makePacket_ResFriendCancel(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);

void	makePacket_ResFriendDeny(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);
void	makePacket_ResFriendAgree(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, UINT64 FriendAccountNo, BYTE byResult);

// 특정 클라이언트에게 보내기
void	SendUnicast(st_CLIENT *pClient, st_PACKET_HEADER *pHeader, 
	CProtocolBuffer *pPacket);

// 친구 요청 추가
BOOL	AddFriendRequest(UINT64 FromAccountNo, UINT64 ToAccountNo);

// 친구인지 확인
BOOL	CheckFriendBetween(UINT64 FromAccountNo, UINT64 ToAccountNo);