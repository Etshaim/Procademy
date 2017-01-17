#pragma once

#include "Protocol.h"
#include "StreamSQ.h"

class CProtocolBuffer;

struct st_DATA_ACCOUNT
{
	UINT64	AccountNo;
	WCHAR	szID[dfNICK_MAX_LEN];
};

// Ŭ���̾�Ʈ ����ü
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

// ��Ʈ��ũ �ʱ�ȭ
BOOL	NetworkInitial(void);
void	NetworkProcess(void);
void	SelectSocket(DWORD *dwpTableNO, SOCKET *pTableSocket, FD_SET *pReadSet, FD_SET *pWriteSet);

// ���� ���
void	err_quit(WCHAR *msg);
void	err_display(WCHAR *msg);

// ���� ����
void	CreateAccount(WCHAR *szNickName);

// client ����
BOOL	DisconnectClient(SOCKET socket);

// Ŭ���̾�Ʈ ã��
st_CLIENT *FindClient(SOCKET socket);

// ��Ŷ ���μ���
BOOL	PacketProc(st_CLIENT *pClient, WORD wMsgType, CProtocolBuffer *pPacket);

// ������ ��Ŷ �����
int		MakeRecvPacket(st_CLIENT *pClient);

// net ���μ���
void	netProc_Recv(SOCKET socket);
void	netProc_Send(SOCKET socket);
void	netProc_Accept(void);

// Request
BOOL 	netPacket_ReqAccountAdd(st_CLIENT *pClient, CProtocolBuffer *pPacket);				//ȸ�� ����		 ��û(REQ)
BOOL	netPacket_ReqLogin(st_CLIENT *pClient, CProtocolBuffer *pPacket);				//ȸ�� �α���	 ��û(REQ)
BOOL	netPacket_ReqAccountList(st_CLIENT *pClient, CProtocolBuffer *pPacket);			//ȸ�� ��� 	 ��û(REQ)

BOOL	netPacket_ReqFriendList(st_CLIENT *pClient, CProtocolBuffer *pPacket);			//ģ�� ��� 	 ��û(REQ)
BOOL	netPacket_ReqFriendList_Request(st_CLIENT *pClient, CProtocolBuffer *pPacket);	//���� ģ����û ��� ��û(REQ)
BOOL	netPacket_ReqFriendList_Reply(st_CLIENT *pclient, CProtocolBuffer *pPacket);	//���� ģ����û ��� ��û(REQ)

BOOL 	netPacket_ReqFriendRemove(st_CLIENT *pclient, CProtocolBuffer *pPacket);		//ģ�� ���� 	 ��û(REQ)
BOOL	netPacket_ReqFriendRequest(st_CLIENT *pclient, CProtocolBuffer *pPacket);		//ģ�� 			 ��û(REQ)
BOOL	netPacket_ReqFriendCancel(st_CLIENT *pclient, CProtocolBuffer *pPacket);		//ģ�� ��û ��� ��û(REQ)

BOOL	netPacket_ReqFriendDeny(st_CLIENT *pclient, CProtocolBuffer *pPacket);			//ģ�� ��û �ź� ��û(REQ)
BOOL	netPacket_ReqFriendAgree(st_CLIENT *pclient, CProtocolBuffer *pPacket);			//ģ�� ��û ���� ��û(REQ)

// Response
void	Send_ResAccountAdd(st_CLIENT *pClient);		//ȸ�� ���� 			���(RES)
void	Send_ResLogin(st_CLIENT *pClient);			//ȸ�� �α��� 			���(RES)
void	Send_ResAccountList(st_CLIENT *pClient);					//ȸ�� ����Ʈ 			���(RES)

void	Send_ResFriendList(st_CLIENT *pClient);						//ģ�� ��� 			���(RES)
void	Send_ResFriendRequestList(st_CLIENT *pClient);				//���� ģ����û ��� 	���(RES)
void 	Send_ResFriendList_Reply(st_CLIENT *pclient);				//���� ģ����û ��� 	���(RES)

void	Send_ResFriendRemove(st_CLIENT *pClient);					//ģ�� ���� 			���(RES)
void	Send_ResFriendRequest(st_CLIENT *pclient, BYTE byResult);	//ģ�� ��û 			���(RES)
void	Send_ResFriendCancel(st_CLIENT *pClient);					//ģ�� ��û ��� 		���(RES)

void	Send_ResFriendDeny(st_CLIENT *pClient);						//ģ�� ��û �ź� 		���(RES)
void	Send_ResFriendAgree(st_CLIENT *pClient, BYTE byResult);		//ģ�� ��û ���� 		���(RES)
		
		
// ��Ŷ �����
void	makePacket_ResAccountAdd(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, UINT64 AccountNo);
void	makePacket_ResLogin(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_DATA_ACCOUNT *pAccount );
void	makePacket_ResAccountList(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);

void	makePacket_ResFriendList(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_DATA_ACCOUNT *pAccount);
void	makePacket_ResFriendRequestList(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);
void	makePacket_ResFriendList_Reply(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);

void	makePacket_ResFriendRemove(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);
void	makePacket_FriendRequest(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);
void	makePacket_ResFriendCancel(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);

void	makePacket_ResFriendDeny(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);
void	makePacket_ResFriendAgree(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket);

// Ư�� Ŭ���̾�Ʈ���� ������
void	SendUnicast(st_CLIENT *pClient, st_PACKET_HEADER *pHeader, 
	CProtocolBuffer *pPacket);
