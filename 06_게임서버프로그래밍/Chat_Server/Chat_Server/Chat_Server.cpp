#include "Chat_Server.h"
#include "ProtocolBuffer.h"
#include <WS2tcpip.h>
#include <map>

// 전역
extern	DWORD		g_dwKey_UserNO;					// User 아이디
extern	DWORD		g_dwKey_RoomNO;					// 채팅방

extern	map<DWORD, st_CLIENT*>		g_ClientMap;	// 사용자 관리 메인 map
extern	map<DWORD, st_CHAT_ROOM*>	g_RoomMap;		// 채팅방 관리 메인 map

extern	SOCKET		g_ListenSocket;					// 사용자 accept용 listen 소켓


BOOL NewworkInitial(void)
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

	int retval = bind(g_ListenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
	{
		err_display(L"bind()");
		return FALSE;
	}

	// listen()
	retval = listen(g_ListenSocket, SOMAXCONN);

	if (retval == SOCKET_ERROR)
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

	int		iSocketCount = 0;	// 64명 되면 호출

	FD_SET	readSet;
	FD_SET	writeSet;

	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);

	memset(userTable_NO, -1, FD_SETSIZE);
	memset(userTable_SOCKET, INVALID_SOCKET, FD_SETSIZE);


	// 첫 번째 인덱스에는 listen socket 넣을 것임
	FD_SET(g_ListenSocket, &readSet);
	userTable_NO[iSocketCount] = 0;
	userTable_SOCKET[iSocketCount] = g_ListenSocket;

	iSocketCount++;


	// Client를 순회하면서 처리
	map<DWORD, st_CLIENT*>::iterator iter;

	for (iter = g_ClientMap.begin(); iter != g_ClientMap.end(); ++iter)
	{
		pClient = (*iter).second;
		userTable_NO[iSocketCount] = pClient->dwUserNO;
		userTable_SOCKET[iSocketCount] = pClient->sock;

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

void SelectSocket(DWORD * dwpTable, SOCKET * pSocketTable,
	FD_SET *pReadSet, FD_SET *pWriteSet)
{
	timeval		time;
	int			iResult;

	// 64명씩 돌면서 하기 때문에 시간을 넣으면 안 됨
	time.tv_sec = 0;
	time.tv_usec = 0;

	iResult = select(0, pReadSet, pWriteSet, NULL, &time);

	// iResult 값이 0 이상이면
	// 64명 테이블을 돌면서 FD_ISSET으로 readSet과 writeSet 검사
	if (0 < iResult)
	{
		for (int iCount = 0; iCount < FD_SETSIZE; iCount++)
		{
			if (INVALID_SOCKET == pSocketTable[iCount])
			{
				continue;
			}

			if (FD_ISSET(pSocketTable[iCount], pReadSet))
			{
				if (iCount == 0)
				{
					netProc_Accept();
				}
				else
				{
					netProc_Recv(dwpTable[iCount]);
				}
			}

			if (FD_ISSET(pSocketTable[iCount], pWriteSet))
			{
				netProc_Send(dwpTable[iCount]);
			}
		}
	}
	else if (INVALID_SOCKET == iResult)
	{
		err_quit(L"SelectSocket()");
	}
}

int MakeRecvPacket(st_CLIENT * pClient)
{
	// return 1		패킷 미 완성
	// return 0		패킷 처리
	// return -1	오류

	st_PACKET_HEADER	stHeader;

	int iRecvQSize = pClient->RecvQ.GetUseSize();

	// 지금 recvQ에 들어있는게 헤더보다 작으면 처리할 게 없음
	if (sizeof(st_PACKET_HEADER) > iRecvQSize)
	{
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
		return 1;
	}

	// 헤더부분은 빼버리고
	// payload만 가져옴
	pClient->RecvQ.RemoveData(sizeof(stHeader));

	CProtocolBuffer clPacket;

	if (stHeader.wPayloadSize != pClient->RecvQ.Get(clPacket.GetBufferPtr(), stHeader.wPayloadSize))
	{
		return -1;
	}

	pClient->RecvQ.RemoveData(stHeader.wPayloadSize);

	// 체크섬 확인
	BYTE byChecksum = MakeCheckSum(&clPacket, stHeader.wMsgType);

	if (byChecksum != stHeader.wPayloadSize)
	{
		err_display(L"CheckSum");

		return -1;
	}

	// 실질적인 패킷 처리 함수 호출
	if (!PacketProc(pClient, stHeader.wMsgType, &clPacket))
	{
		return -1;
	}

	return 0;
}

BYTE MakeCheckSum(CProtocolBuffer * pPacket, WORD wMsgType)
{
	BYTE		byCheckSum = 0;
	int			iPacketSize = pPacket->GetDataSize();
	BYTE		*bypTemp = (BYTE*)pPacket->GetBufferPtr();

	//byCheckSum	= LOBYTE(wMsgType) + HIBYTE(wMsgType);
	byCheckSum = wMsgType;

	for (int iCount = 0; iCount < iPacketSize; iCount++)
	{
		byCheckSum += bypTemp[iCount];
	}

	byCheckSum %= 256;

	return byCheckSum;
}

BOOL PacketProc(st_CLIENT * pClient, WORD wMsgType, CProtocolBuffer * pPacket)
{
	wprintf(L"PacketRecv [UserNO:%d][Type:%d]\n", pClient->dwUserNO, wMsgType);

	switch (wMsgType)
	{
	case df_REQ_LOGIN:
		return netPacket_ReqLogin(pClient, pPacket);

	case df_REQ_ROOM_LIST:
		return netPacket_ReqRoomList(pClient, pPacket);

	case df_REQ_ROOM_CREATE:
		return netPacket_ReqRoomCreate(pClient, pPacket);

	case df_REQ_ROOM_ENTER:
		return netPacket_ReqRoomEnter(pClient, pPacket);
	case df_REQ_CHAT:
		return netPacket_ReqRoomEnter(pClient, pPacket);

	case df_REQ_ROOM_LEAVE:
		return netPacket_ReqRoomLeave(pClient, pPacket);
	}
	return TRUE;
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

void netProc_Recv(DWORD dwUserNO)
{
	st_CLIENT *pClient = g_ClientMap[dwUserNO];
	int		iResult = 0;


	if (nullptr == pClient)
	{
		return;
	}

	iResult = recv(pClient->sock, pClient->RecvQ.GetWriteBufferPtr(),
		pClient->RecvQ.GetFreeSize(), 0);

	if (SOCKET_ERROR == iResult || 0 == iResult)
	{
		closesocket(pClient->sock);
		DisconnectClient(dwUserNO);

		return;
	}

	if (0 < iResult)
	{
		// write한만큼 이동
		pClient->RecvQ.MoveWritePos(iResult);

		// 패킷이 하나 이상 있을 수 있으므로
		// 한 번에 전부 처리할 것
		while (1)
		{
			iResult = MakeRecvPacket(pClient);

			if (1 == iResult)
			{
				break;
			}

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
	st_CLIENT *pClient;

	int			iResult;
	int			iSendSize;

	pClient = g_ClientMap[dwUserNO];

	if (nullptr == pClient)
	{
		return;
	}

	iSendSize = pClient->SendQ.GetUseSize();

	if (0 >= iSendSize)
	{
		return;
	}

	iResult = send(pClient->sock, pClient->SendQ.GetReadBufferPtr(), iSendSize, 0);

	if (SOCKET_ERROR == iResult)
	{
		DWORD dwError = WSAGetLastError();

		if (WSAEWOULDBLOCK == dwError)
		{
			wprintf(L"Socket WOULDBLOCK - UserNO:%d\n", dwUserNO);
			return;
		}

		wprintf(L"Socket Error - UserNO:%d ErrorCode:%d\n", dwUserNO, dwError);

		closesocket(pClient->sock);
		DisconnectClient(dwUserNO);
		return;
	}
	else
	{
		if (iSendSize < iResult)
		{
			wprintf(L"Send size error - UserNO:%d SendSize:%d SendResult:%d\n",
				dwUserNO, iSendSize, iResult);
			return;
		}
		else
		{
			pClient->SendQ.RemoveData(iResult);
		}
	}
}

void netProc_Accept(void)
{
	st_CLIENT *pClient = new st_CLIENT;
	int	iAddrLen = 0;
	WCHAR szTemp[16] = { 0, };


	pClient->dwEnterRoomNO = -1;
	pClient->dwUserNO = g_dwKey_UserNO;
	g_dwKey_UserNO++;
	pClient->szNickName[0] = NULL;

	iAddrLen = sizeof(pClient->connectAddr);

	pClient->sock = accept(g_ListenSocket, (SOCKADDR*)&(pClient->connectAddr), &iAddrLen);

	if (INVALID_SOCKET == pClient->sock)
	{
		int ierr = WSAGetLastError();

		if (WSAEWOULDBLOCK != ierr)
		{
			err_display(L"accept()");
		}
	}
	else
	{
		g_ClientMap[g_dwKey_UserNO] = pClient;

		InetNtop(AF_INET, &pClient->connectAddr.sin_addr, szTemp, 16);

		wprintf(L"Accetp - %s:%d [UserNO:%d]\n",
			szTemp, ntohs(pClient->connectAddr.sin_port), pClient->dwUserNO);
	}
}

st_CLIENT * FindClient(DWORD dwUserNO)
{


}

BOOL DisconnectClient(DWORD dwUserNO)
{
	DWORD dwEnterRoomNO = g_ClientMap[dwUserNO]->dwEnterRoomNO;

	// 방에 들어가 있다면
	// 그 방 유저들에게 broadcast

	// 동적할당한 구조체 삭제
	delete g_ClientMap[dwUserNO];

	// 맵에서 삭제
	g_ClientMap.erase(dwUserNO);

	return TRUE;
}

BOOL netPacket_ReqLogin(st_CLIENT * pClient, CProtocolBuffer * pPacket)
{
	return 0;
}

BOOL netPacket_ReqRoomList(st_CLIENT * pClient, CProtocolBuffer * pPacket)
{
	return 0;
}

BOOL netPacket_ReqRoomCreate(st_CLIENT * pClient, CProtocolBuffer * pPacket)
{
	WCHAR	szRoomTitle[256] = { 0, };
	WORD	wTitleSize;

	(*pPacket) >> wTitleSize;

	if (256 < wTitleSize)
	{
		wTitleSize = 255;
	}

	pPacket->GetData((char*)szRoomTitle, wTitleSize);

	// 방 생성
	st_CHAT_ROOM *pRoom = new st_CHAT_ROOM;
	memset(pRoom->szTitle, 0, sizeof(WCHAR) * 256);

	pRoom->dwRoomNO = g_dwKey_RoomNO++;

	wcscpy_s(pRoom->szTitle, szRoomTitle);

	g_RoomMap.insert(pair<DWORD, st_CHAT_ROOM*>(pRoom->dwRoomNO, pRoom));

	// 방생성 로그
	wprintf(L"방 생성 [UserNO:%d][Room:%s][TotalRoom:%d]\n",
		pClient->dwUserNO, pRoom->szTitle, g_RoomMap.size());

	// 방 생성 결과를 클라로 전송
	Send_ResRoomCreate(pClient, df_RESULT_ROOM_CREATE_OK, pRoom);

	return TRUE;
}

BOOL netPacket_ReqRoomEnter(st_CLIENT * pClient, CProtocolBuffer * pPacket)
{

	return 0;
}

BOOL netPacket_ReqChat(st_CLIENT * pClient, CProtocolBuffer * pPacket)
{
	return 0;
}

BOOL netPacket_ReqRoomLeave(st_CLIENT * pClient, CProtocolBuffer * pPacket)
{
	return 0;
}

void Send_ResLogin(st_CLIENT * pClient, BYTE byResult)
{
}

void Send_ResRoomList(st_CLIENT * pClient, BYTE byResult)
{
}

void Send_ResRoomCreate(st_CLIENT * pClient, st_CHAT_ROOM * pRoom, BYTE byResult)
{
}

void Send_ResRoomEnter(st_CLIENT * pClient, st_CHAT_ROOM * pRoom, BYTE byResult)
{
}

void Send_ResChat(st_CLIENT * pClient, WORD wMsgSize, PWCHAR szMsg)
{
}

void Send_ResRoomLeave(st_CLIENT * pClient)
{
}

void Send_ResRoomDelete(st_CLIENT * pClient, st_CHAT_ROOM * pRoom)
{
}

void Send_ResUserEnter(st_CLIENT * pClient)
{
}

void Send_ResRoomCreate(st_CLIENT * pClient, BYTE byResult, st_CHAT_ROOM * pRoom)
{
	st_PACKET_HEADER stHeader;
	CProtocolBuffer	clPacket;

	MakePacket_ResRoomCreate(&stHeader, &clPacket, byResult, pRoom);

	// 채팅방 생성이 성공인 경우 모든 유저에게
	if (df_RESULT_ROOM_CREATE_OK == byResult)
	{
		SendBroadcast(&stHeader, &clPacket);
	}
	// 실패면 방 만들기 요청한 유저에게만
	else
	{
		SendUnicast(pClient, &stHeader, &clPacket);
	}
}

void MakePacket_ResLogin(st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket, BYTE byResult, st_CLIENT * pClient)
{
}

void MakePacket_ResRoomList(st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket, BYTE byResult)
{
}

void MakePacket_ResRoomCreate(st_PACKET_HEADER *pHeader, CProtocolBuffer * pPacket, BYTE byResult, st_CHAT_ROOM * pRoom)
{
	WORD	wTitleSize;

	pPacket->Clear();
	pPacket->PutData((char*)&byResult, sizeof(BYTE));

	if (byResult == df_RESULT_ROOM_CREATE_OK)
	{
		*pPacket << pRoom->dwRoomNO;
		wTitleSize = wcslen(pRoom->szTitle) * sizeof(WCHAR);
		*pPacket << wTitleSize;
		pPacket->PutData((char*)pRoom->szTitle, wTitleSize);
	}

	pHeader->byCode = dfPACKET_CODE;
	pHeader->byCheckSum = MakeCheckSum(pPacket, df_RES_ROOM_CREATE);
	pHeader->wMsgType = df_RES_ROOM_CREATE;
	pHeader->wPayloadSize = pPacket->GetDataSize();
}

void MakePacket_ResRoomEnter(st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket, BYTE byResult, st_CHAT_ROOM * pRoom)
{
}

void MakePacket_ResChat(st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket, st_CLIENT * pClient, BYTE byResult, WORD wMsgSize, PWCHAR szMsg)
{
}

void MakePacket_ResUserEnter(st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket, st_CLIENT * pClient)
{
}

void MakePacket_ResRoomLeave(st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket, st_CLIENT * pClient)
{
}

void MakePacket_ResRoomDelete(st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket, st_CLIENT * pClient, st_CHAT_ROOM * pRoom)
{
}

void SendUnicast(st_CLIENT * pClient, st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket)
{
	if (nullptr == pClient)
	{
		err_display(L"SendUnicast()");
		return;
	}

	pClient->SendQ.Put((char *)pHeader, sizeof(st_PACKET_HEADER));
	pClient->SendQ.Put((char *)pPacket->GetBufferPtr(), pPacket->GetDataSize());
}

void SendBroadcast(st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket)
{
	st_CLIENT *pClient;

	map<DWORD, st_CLIENT*>::iterator iter;

	for (iter = g_ClientMap.begin(); iter != g_ClientMap.end(); iter++)
	{
		pClient = iter->second;

		SendUnicast(pClient, pHeader, pPacket);
	}
}

void SendBroadcastRoom(st_CHAT_ROOM * pRoom, st_CLIENT * pClient, st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket)
{
	st_CLIENT *pDestClient;

	list<DWORD>::iterator iter;

	for (iter = pRoom->UserList.begin(); iter != pRoom->UserList.end(); iter++)
	{
		pDestClient = g_ClientMap[*iter];

		if (pDestClient != pClient)
		{
			SendUnicast(pDestClient, pHeader, pPacket);
		}
	}
}