#include "stdafx.h"
#include "Friend_Server.h"
#include "ProtocolBuffer.h"

using namespace std;

extern	UINT64		g_AccountIncrement;
extern	UINT64		g_FriendIncrement;
extern	UINT64		g_FriendRequestIncrement;
extern	SOCKET		g_ListenSocket;

extern	map<SOCKET, st_CLIENT*>						 g_ClientMap;     // key값을 소켓으로 쓰는 이유?
extern	map<UINT64, st_DATA_ACCOUNT*>				 g_AccountMap;
													 
// 친구 관계가 성립된 것							    
extern	multimap<UINT64, st_DATA_FRIEND*>			 g_FriendMap;
extern	multimap<UINT64, UINT64>					 g_FriendIndex_From;
extern	multimap<UINT64, UINT64>					 g_FriendIndex_To;


// 친구 요청
extern	multimap<UINT64, st_DATA_FRIEND_REQUEST*>    g_FriendRequestMap;
extern	multimap<UINT64, UINT64>                     g_FriendRequestIndex_From;
extern	multimap<UINT64, UINT64>                     g_FriendRequestIndex_To;


BOOL NetworkInitial(void)
{
	// 윈속 초기화
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

	DWORD	userTable_NO[FD_SETSIZE] = { (DWORD)-1, };
	SOCKET	userTable_SOCKET[FD_SETSIZE] = { INVALID_SOCKET };

	int		iSocketCount = 0;	// 64명 되면 호출

	FD_SET	readSet;
	FD_SET	writeSet;

	FD_ZERO( &readSet );
	FD_ZERO( &writeSet );

	memset(userTable_NO, -1, FD_SETSIZE);
	memset(userTable_SOCKET, INVALID_SOCKET, FD_SETSIZE);

	 
	// 첫 번째 인덱스에는 listen socket 넣을 것임
	FD_SET(g_ListenSocket, &readSet);
	userTable_NO[iSocketCount] = 0;
	userTable_SOCKET[iSocketCount] = g_ListenSocket;

	iSocketCount++;


	// Client를 순회하면서 처리
	map< SOCKET, st_CLIENT* >::iterator iter;

	for (iter = g_ClientMap.begin(); iter != g_ClientMap.end(); ++iter)
	{
		pClient = (*iter).second;

		// 아직 계정을 생성 안 했을 수도 있으므로
		if (nullptr != pClient->pAccount)
		{
			userTable_NO[iSocketCount] = (DWORD)pClient->pAccount->AccountNo;
		}
		
		userTable_SOCKET[iSocketCount]	= pClient->sock;

		FD_SET(pClient->sock, &readSet);
		FD_SET(pClient->sock, &writeSet);

		iSocketCount++;

		if (iSocketCount >= FD_SETSIZE)
		{
			// FD_ISSET으로 recv send 처리
			SelectSocket(userTable_NO, userTable_SOCKET, &readSet, &writeSet);

			FD_ZERO(&readSet);
			FD_ZERO(&writeSet);

			memset(userTable_NO, -1, FD_SETSIZE);
			memset(userTable_SOCKET, INVALID_SOCKET, FD_SETSIZE);

			iSocketCount = 0;
		}
	}

	// 마지막에 iSocketCount가 FD_SETSIZE만큼
	// 도달하지 못했을 경우에는 다시 한번 처리해줌
	if (0 < iSocketCount)
	{
		SelectSocket(userTable_NO, userTable_SOCKET, &readSet, &writeSet);
	}
}

void SelectSocket(DWORD * dwpTableNO, SOCKET * pTableSocket, FD_SET * pReadSet, FD_SET * pWriteSet)
{
	timeval Time;
	int		iRetVal;
	int		iCount;

	//-----------------------------------------------------
	// select 함수의 대기 시간
	//-----------------------------------------------------
	Time.tv_sec		= 0;
	Time.tv_usec	= 0;

	// select 호출
	iRetVal = select(0, pReadSet, pWriteSet, nullptr, &Time);

	//-----------------------------------------------------
	// iRetVal 값이 0 이상이면 반응이 있는 것
	//-----------------------------------------------------
	if ( 0 < iRetVal)
	{
		// 테이블 순회하면서 FD_ISSET 으로 rset wset 검사
		for (iCount = 0; iCount < FD_SETSIZE; ++iCount)
		{
			if (INVALID_SOCKET == pTableSocket[iCount])
			{
				continue;
			}

			// rset 반응 있으면 
			if (FD_ISSET(pTableSocket[iCount], pReadSet))
			{
				// listen 소켓이 반응했으면 accept
				if (0 == dwpTableNO[iCount])
				{
					netProc_Accept();
				}
				// 아니면 패킷 프로세스 처리
				else
				{
					//netProc_Recv
				}
			}

			// wset 반응 있으면
			if (FD_ISSET(pTableSocket[iCount], pWriteSet))
			{
				//netProc_Send
			}
		}
	}
	else if (SOCKET_ERROR == iRetVal)
	{
		wprintf(L"SelectSocket()");
	}
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

void CreateAccount(WCHAR * szNickName)
{
	st_DATA_ACCOUNT *pAccount = new st_DATA_ACCOUNT;

	wcscpy_s(pAccount->szID, szNickName);
	pAccount->AccountNo = g_AccountIncrement;
	g_AccountIncrement++;

	g_AccountMap.insert( pair<UINT64, st_DATA_ACCOUNT*>( pAccount->AccountNo, pAccount ) );
}

BOOL DisconnectClient(DWORD dwUserNo)
{
	st_CLIENT *pClient = g_ClientMap[dwUserNo];

	if (nullptr == pClient)
	{
		return FALSE;
	}

	delete g_ClientMap[dwUserNo];

	g_ClientMap.erase(dwUserNo);

	return TRUE;
}

BOOL PacketProc(st_CLIENT * pClient, WORD wMsgType, CProtocolBuffer * pPacket)
{
	wprintf(L"PacketRecv [UserNO:%d][Type:%d]\n", pClient->pAccount->AccountNo, wMsgType);

	switch (wMsgType)
	{
	case df_REQ_ACCOUNT_ADD:
		return netPacket_ReqAccountAdd(pClient, pPacket);
	
	case df_REQ_LOGIN:
		return netPacket_ReqLogin(pClient, pPacket);

	case df_REQ_ACCOUNT_LIST:
		return netPacket_ReqAccountList(pClient, pPacket);

	case df_REQ_FRIEND_LIST:
		return netPacket_ReqFriendList(pClient, pPacket);

	case df_REQ_FRIEND_REQUEST_LIST:
		return netPacket_ReqFriendList_Request(pClient, pPacket);

	case df_REQ_FRIEND_REPLY_LIST:
		return netPacket_ReqFriendList_Reply(pClient, pPacket);

	case df_REQ_FRIEND_REMOVE:
		return netPacket_ReqFriendRemove(pClient, pPacket);

	case df_REQ_FRIEND_REQUEST:
		return netPacket_ReqFriendRequest(pClient, pPacket);

	case df_REQ_FRIEND_CANCEL:
		return netPacket_ReqFriendCancel(pClient, pPacket);

	case df_REQ_FRIEND_DENY:
		return netPacket_ReqFriendDeny(pClient, pPacket);

	case df_REQ_FRIEND_AGREE: 
		netPacket_ReqFriendAgree(pClient, pPacket);
	}
	return TRUE;
}

int MakeRecvPacket(st_CLIENT * pClient)
{
	// return 1		패킷 미완성
	// return 0		패킷 처리
	// return -1	오류

	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		clPacket;

	int iRecvQSize = pClient->RecvQ.GetUseSize();

	// 헤더 사이즈보다 작으면 처리할 게 없다고 판단
	if (sizeof(st_PACKET_HEADER) > iRecvQSize)
	{
		// 다음에 처리
		return 1;
	}

	// 패킷 코드 검사
	pClient->RecvQ.Peek((char*)&stHeader, sizeof(st_PACKET_HEADER));

	if (dfPACKET_CODE != stHeader.byCode)
	{
		return -1;
	}

	// 헤더 + 페이로드보다 recvQ 크기가 작으면 미완성
	if (stHeader.wPayloadSize + sizeof(stHeader) > iRecvQSize)
	{
		// 다음에 처리
		return 1;
	}

	// 헤더부분은 빼버리고 페이로드만 가져옴
	pClient->RecvQ.RemoveData( sizeof( stHeader ) );

	if (stHeader.wPayloadSize != pClient->RecvQ.Get(clPacket.GetBufferPtr(), stHeader.wPayloadSize))
	{
		return -1;
	}

	clPacket.MoveWritePos(stHeader.wPayloadSize);

	// 실질적인 패킷 처리 함수 호출
	if (!PacketProc(pClient, stHeader.wMsgType, &clPacket))
	{
		return -1;
	}

	// 여기까지 왔다는 것은 정상적인 패킷 하나 처리
	// 이어서 계속 처리할 수 있도록 return 0을 해줌
	return 0;
}

void netProc_Recv(DWORD dwUserNo)
{
	st_CLIENT *pClient  = g_ClientMap[dwUserNo];
	int iResult			= 0;

	if (nullptr == pClient)
	{
		return;
	}

	iResult = recv(pClient->sock, pClient->RecvQ.GetWriteBufferPtr(),
		pClient->RecvQ.GetFreeSize(), 0);

	if (SOCKET_ERROR == iResult)
	{
		closesocket(pClient->sock);
		DisconnectClient(dwUserNo);

		return;
	}

	if (0 < iResult)
	{
		// write한만큼 recvQ 이동
		pClient->RecvQ.MoveWritePos(iResult);

		// 처리할 수 있는만큼 한번에 처리
		while (1)
		{
			iResult = MakeRecvPacket(pClient);

			// 아직 온전한 패킷을 만들 사이즈가 아님
			if (1 == iResult)
			{
				break;
			}
			// 에러
			if (-1 == iResult)
			{
				err_display(L"MakeRecvPacket()");
				return;
			}
		}
	}
}

void netProc_Send(DWORD dwUserNO)
{
}

void netProc_Accept(void)
{
	st_CLIENT *pClient	= new st_CLIENT;
	int iAddrLen		= 0;
	WCHAR szTemp[32]	= { 0, };


	iAddrLen = sizeof(pClient->connectAddr);

	pClient->sock = accept(g_ListenSocket,
		(SOCKADDR*)&pClient->connectAddr, &iAddrLen);

	pClient->pAccount = nullptr;

	if (INVALID_SOCKET == pClient->sock)
	{
		int iErr = WSAGetLastError();

		if (WSAEWOULDBLOCK != iErr)
		{
			err_display(L"accpet()");
		}
	}
	else
	{
		g_ClientMap.insert( pair< SOCKET, st_CLIENT*>( pClient->sock, pClient ) );

		InetNtopW(AF_INET, &pClient->connectAddr.sin_addr, szTemp, 32);
		wprintf(L"Accept - %s:%d [UserNO:%d]\n",
			szTemp, ntohs(pClient->connectAddr.sin_port), g_AccountIncrement);
	}
}

BOOL newPacket_ReqJoin(st_CLIENT * pClient, CProtocolBuffer *pPacket)
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

BOOL netPacket_ReqAccountList(st_CLIENT * pClient, CProtocolBuffer *pPacket)
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
