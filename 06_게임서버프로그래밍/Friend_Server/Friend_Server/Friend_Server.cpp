#include "stdafx.h"
#include "Friend_Server.h"
#include <map>

using namespace std;

extern	UINT64		g_AccountIncrement				= 1;
extern	UINT64		g_FriendIncrement				= 1;
extern	UINT64		g_FriendRequestIncrement		= 1;
extern	SOCKET		g_ListenSocket					= INVALID_SOCKET;

extern	map<SOCKET, st_CLIENT*>						 g_ClientMap;     // key���� �������� ���� ����?
extern	map<UINT64, st_DATA_ACCOUNT*>				 g_AccountMap;
													 
// ģ�� ���谡 ������ ��							    
extern	multimap<UINT64, st_DATA_FRIEND*>			 g_FriendMap;
extern	multimap<UINT64, UINT64>					 g_FriendIndex_From;
extern	multimap<UINT64, UINT64>					 g_FriendIndex_To;


// ģ�� ��û
extern	multimap<UINT64, st_DATA_FRIEND_REQUEST*>    g_FriendRequestMap;
extern	multimap<UINT64, UINT64>                     g_FriendRequestIndex_From;
extern	multimap<UINT64, UINT64>                     g_FriendRequestIndex_To;


BOOL NewworkInitial(void)
{
	// ���� �ʱ�ȭ
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return FALSE;
	}

	// socket()
	g_ListenSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (g_ListenSocket == INVALID_SOCKET)
	{
		err_display(L"socket()");
		return FALSE;
	}

	// bind()
	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(dfNETWORK_PORT);

	int iRetVal = bind(g_ListenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (iRetVal == SOCKET_ERROR)
	{
		err_display(L"bind()");
		return FALSE;
	}

	// listen()
	iRetVal = listen(g_ListenSocket, SOMAXCONN);

	if (iRetVal == SOCKET_ERROR)
	{
		err_display(L"listen()");
		return FALSE;
	}

	return TRUE;
}

void NetworkProcess(void)
{
	st_CLIENT *pClient;

	DWORD	userTable_NO[FD_SETSIZE] = { -1, };
	SOCKET	userTable_SOCKET[FD_SETSIZE] = { INVALID_SOCKET };

	int		iSocketCount = 0;	// 64�� �Ǹ� ȣ��

	FD_SET	readSet;
	FD_SET	writeSet;

	FD_ZERO( &readSet );
	FD_ZERO( &writeSet );

	memset(userTable_NO, -1, FD_SETSIZE);
	memset(userTable_SOCKET, INVALID_SOCKET, FD_SETSIZE);


	// ù ��° �ε������� listen socket ���� ����
	FD_SET(g_ListenSocket, &readSet);
	userTable_NO[iSocketCount] = 0;
	userTable_SOCKET[iSocketCount] = g_ListenSocket;

	iSocketCount++;


	// Client�� ��ȸ�ϸ鼭 ó��
	map< SOCKET, st_CLIENT* >::iterator iter;

	for (iter = g_ClientMap.begin(); iter != g_ClientMap.end(); ++iter)
	{
		pClient = (*iter).second;
		userTable_NO[iSocketCount]		= pClient->pAccount->AccountNo;
		userTable_SOCKET[iSocketCount]	= pClient->sock;

		FD_SET(pClient->sock, &readSet);
		FD_SET(pClient->sock, &writeSet);

		iSocketCount++;

		if (iSocketCount >= FD_SETSIZE)
		{
			// FD_ISSET���� recv send ó��
			SelectSocket(userTable_NO, userTable_SOCKET, &readSet, &writeSet);

			FD_ZERO(&readSet);
			FD_ZERO(&writeSet);

			memset(userTable_NO, -1, FD_SETSIZE);
			memset(userTable_SOCKET, INVALID_SOCKET, FD_SETSIZE);

			iSocketCount = 0;
		}
	}

	// �������� iSocketCount�� FD_SETSIZE��ŭ
	// �������� ������ ��쿡�� �ٽ� �ѹ� ó������
	if (0 < iSocketCount)
	{
		SelectSocket(userTable_NO, userTable_SOCKET, &readSet, &writeSet);
	}
}

void SelectSocket(DWORD * dwpTableNO, SOCKET * pTableSocket, FD_SET * pReadSet, FD_SET * pWriteSet)
{

}

void err_quit(WCHAR * msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(WCHAR * msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	wprintf(L"[%s] %s", msg, (WCHAR *)lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}


BOOL newPacket_ReqJoin(st_CLIENT * pClient, CProtocolBuffer *pPacket)
{
	return 0;
}



BOOL netPacket_ReqJoin(st_CLIENT * pClient, CProtocolBuffer * pPacket)
{
	return 0;
}

BOOL netPacket_ReqAccountAdd(st_CLIENT * pClient, CProtocolBuffer * pPacket)
{
	return 0;
}

BOOL netPacket_ReqLogin(st_CLIENT * pClient, CProtocolBuffer *pPacket)
{
	return 0;
}

BOOL netPacket_ReqMemberList(st_CLIENT * pClient, CProtocolBuffer *pPacket)
{
	return 0;
}

BOOL netPacket_ReqFriendList(st_CLIENT * pClient, CProtocolBuffer *pPacket)
{
	return 0;
}

BOOL netPacket_ReqFriendList_Request(st_CLIENT * pClient, CProtocolBuffer *pPacket)
{
	return 0;
}

BOOL netPacket_ReqFriendList_Reply(st_CLIENT * pclient, CProtocolBuffer *pPacket)
{
	return 0;
}

BOOL netPacket_ReqFriendRemove(st_CLIENT * pclient, CProtocolBuffer *pPacket)
{
	return 0;
}

BOOL netPacket_ReqFriendRequest(st_CLIENT * pclient, CProtocolBuffer *pPacket)
{
	return 0;
}

BOOL netPacket_ReqFriendCancel(st_CLIENT *pclient, CProtocolBuffer *pPacket)
{
	return 0;
}

BOOL netPacket_ReqFriendDeny(st_CLIENT *pclient, CProtocolBuffer *pPacket)
{
	return 0;
}

BOOL netPacket_ReqFriendAgree(st_CLIENT *pclient, CProtocolBuffer *pPacket)
{
	return 0;
}

void Send_ResAccountAdd(st_CLIENT * pClient, BYTE byResult)
{
}

void Send_ResLogin(st_CLIENT * pClient, BYTE byResult)
{
}

void Send_ResAccountList(st_CLIENT * pClient)
{
}

void Send_ResFriendList(st_CLIENT * pClient)
{
}

void Send_ResFriendRequestList(st_CLIENT * pClient)
{
}

void Send_ResFriendList_Reply(st_CLIENT * pclient)
{
}

void Send_ResFriendRemove(st_CLIENT * pClient)
{
}

void Send_ResFriendRequest(st_CLIENT * pclient, BYTE byResult)
{
}

void Send_ResFriendCancel(st_CLIENT * pClient)
{
}

void Send_ResFriendDeny(st_CLIENT * pClient)
{
}

void Send_ResFriendAgree(st_CLIENT * pClient, BYTE byResult)
{
}
