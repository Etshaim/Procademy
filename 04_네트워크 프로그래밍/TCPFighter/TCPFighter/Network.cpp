#include "stdafx.h"
#include "Protocol.h"
#include "Network.h"
#include "TCPFighter.h"
#include "StreamSQ.h"
#include "PacketProc.h"


#define WM_SOCKET		(WM_USER + 1)

extern	SOCKET		g_Socket;
extern	BOOL		g_bConnected;
extern	HWND		g_hWnd;
extern	BOOL		g_SendFlag;
extern	CStreamSQ	g_SendQ;
extern	CStreamSQ	g_RecvQ;
extern	WCHAR		g_szIP[16];

// 에러 및 종료 return FALSE
// 정상 처리	return TRUE
BOOL	NetworkProc(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int iRetval = 0;

	// 오류 발생 여부 확인
	if (0 != WSAGETSELECTERROR(lParam))
	{
		err_quit((WCHAR*)WSAGETSELECTERROR(lParam));
		closesocket(g_Socket);
		return FALSE;
	}

	// 메시지 처리
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
	{
		iRetval = WSAAsyncSelect(g_Socket, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);

		if (SOCKET_ERROR == iRetval)
		{
			err_quit(L"WSAAsyncSelect()");
			closesocket(g_Socket);

			return FALSE;
		}
	}
	break;

	case FD_CLOSE:
	{
		closesocket(g_Socket);
		return FALSE;
	}
	break;

	case FD_READ:
	{
		if (FALSE == RecvEvent())
		{
			return FALSE;
		}
	}
	break;

	case FD_WRITE:
	{
		g_SendFlag = TRUE;

		if (FALSE == SendEvent())
		{
			return FALSE;
		}
	}
	break;
	}

	return TRUE;
}

// RecvEvent
BOOL	RecvEvent(void)
{
	int iRetval = 0;

	// 임시	수신 버퍼 
	char recvBuff[1000];

	// recv 호출
	iRetval = recv(g_Socket, recvBuff, 1000, 0);

	// retval  종료(0) / 에러(SOCKET_ERROR)
	if (SOCKET_ERROR == iRetval)
	{
		if (WSAEWOULDBLOCK == WSAGetLastError())
		{
			return TRUE;
		}
		else
		{
			err_display(L"recv()");
			closesocket(g_Socket);
			return FALSE;
		}
	}

	if (0 == iRetval)
	{
		err_display(L"recv()");
		closesocket(g_Socket);
		return FALSE;
	}

	// RecvQ에 넣기
	g_RecvQ.Put(recvBuff, iRetval);

	// 완료 패킷 처리부
	// 완료 패킷 처리 부분은 큐에 있는 "완성" 패킷을 다 처리할 것
	while (1)
	{
		int iUseSize = g_RecvQ.GetUseSize();

		// 최소한의 사이즈 확인 ( 헤더 사이즈 )
		if (sizeof(st_PACKET_HEADER) > iUseSize)
		{
			break;
		}

		st_PACKET_HEADER header;
		ZeroMemory(&header, sizeof(st_PACKET_HEADER));

		// 헤더 Peek
		iRetval = g_RecvQ.Peek((char*)&header, sizeof(st_PACKET_HEADER));

		// code 확인
		if (header.byCode != dfPACKET_CODE)
		{
			MessageBox(NULL, L"Error : Packet Header", NULL, MB_ICONERROR);
			return FALSE;
		}

		int iPacketSize = sizeof(st_PACKET_HEADER) + header.bySize + sizeof(BYTE);

		// 완성 패킷 크기만큼 큐에 데이터가 있는지 확인
		if (iUseSize < iPacketSize)
		{
			// 완성 패킷만큼 큐에 없음
			return TRUE;
		}

		// RecvQ에서 헤더 지우기
		g_RecvQ.RemoveData(sizeof(st_PACKET_HEADER));

		// 페이로드 빼오기
		iRetval = g_RecvQ.Get(recvBuff, header.bySize);

		// End 코드 빼서 확인
		BYTE byEndCode = 0;

		iRetval = g_RecvQ.Get((char*)&byEndCode, sizeof(BYTE));

		if (dfNETWORK_PACKET_END != byEndCode)
		{
			MessageBox(NULL, L"Error : Packet Endcode", NULL, MB_ICONERROR);
			return FALSE;
		}

		// 헤더 타입에 따라 분기
		PacketProc(header.byType, recvBuff);
	}

	return TRUE;
}

void	PacketProc(BYTE byPacketType, char *pPacket)
{
	switch (byPacketType)
	{
	case dfPACKET_SC_CREATE_MY_CHARACTER:
		PacketProc_CreateMyCharacter(pPacket);
		break;

	case dfPACKET_SC_CREATE_OTHER_CHARACTER:
		PacketProc_CreateOtherCharacter(pPacket);
		break;

	case dfPACKET_SC_DELETE_CHARACTER:
		PacketProc_DeleteCharacter(pPacket);
		break;

	case dfPACKET_SC_MOVE_START:
		PacketProc_MoveStart(pPacket);
		break;

	case dfPACKET_SC_MOVE_STOP:
		PacketProc_MoveStop(pPacket);
		break;

	case dfPACKET_SC_ATTACK1:
		PacketProc_Attack1(pPacket);
		break;

	case dfPACKET_SC_ATTACK2:
		PacketProc_Attack2(pPacket);
		break;

	case dfPACKET_SC_ATTACK3:
		PacketProc_Attack3(pPacket);
		break;

	case dfPACKET_SC_DAMAGE:
		PacketProc_Damage(pPacket);
		break;

	default:
		break;
	}
}

BOOL	SendPacket(st_PACKET_HEADER *pHeader, char *pPacket)
{
	// 접속 상태 예외 처리
	if (FALSE == g_bConnected)
	{
		// 접속 중이 아니므로 나감
		return FALSE;
	}

	int iRetval = 0;

	// SendQ에 헤더 넣기
	iRetval = g_SendQ.Put((char*)pHeader, sizeof(st_PACKET_HEADER));

	// SendQ에 패킷 넣기
	iRetval = g_SendQ.Put(pPacket, pHeader->bySize);

	BYTE byEndCode = dfNETWORK_PACKET_END;

	// EndCode 넣기
	iRetval = g_SendQ.Put((char*)&byEndCode, sizeof(BYTE));

	// SendEvent
	if (FALSE == SendEvent())
	{
		return FALSE;
	}

	return TRUE;
}

BOOL	SendEvent(void)
{
	// 보내기용 임시 버퍼
	char sendBuff[1000];
	ZeroMemory(&sendBuff, 1000);

	// send 가능 여부 ( FD_WRITE)
	if (FALSE == g_SendFlag)
	{
		return TRUE;
	}

	// SendQ에 보낼 데이터가 있는지 확인
	int iUseSize = g_SendQ.GetUseSize();

	if (0 == iUseSize)
	{
		return TRUE;
	}

	// SendQ에 데이터가 없거나 실패할 때 까지 계속 보냄
	while (1)
	{
		// SendQ에 데이터가 있는지 확인
		iUseSize = g_SendQ.GetUseSize();

		if (0 == iUseSize)
		{
			break;
		}

		int iRetval = 0;
		int iSize = 0;
		// 임시버퍼로 Peek

		iSize = g_SendQ.Peek(sendBuff, iUseSize);

		iRetval = send(g_Socket, (char*)sendBuff, iSize, 0);

		g_SendQ.RemoveData(iRetval);

		if (SOCKET_ERROR == iRetval)
		{
			if (WSAEWOULDBLOCK == WSAGetLastError())
			{
				g_SendFlag = FALSE;
				break;
			}
			err_display(L"send()");
			closesocket(g_Socket);

			return FALSE;
		}
	}

	return TRUE;
}


// 소켓 함수 오류 출력 후 종료
void err_quit(LPWSTR msg)
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

// 소켓 함수 오류 출력
void err_display(LPWSTR msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);

	LocalFree(lpMsgBuf);
}


void Connect(HWND hWnd)
{
	int retval = 0;

	g_Socket = socket(AF_INET, SOCK_STREAM, 0);

	if (INVALID_SOCKET == g_Socket)
	{
		err_quit(L"socket()");
	}

	// WSAAsyncSelect()
	retval = WSAAsyncSelect(g_Socket, hWnd, WM_SOCKET, FD_CONNECT);
	//FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE

	if (SOCKET_ERROR == retval)
	{
		err_quit(L"WSAAsyncSelect()");
	}

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN serveraddr;

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(dfPORT);
	//inet_pton(AF_INET, dfIP, &serveraddr.sin_addr.S_un.S_addr);
	InetPton(AF_INET, g_szIP, &serveraddr.sin_addr);
	//serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	retval = connect(g_Socket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (SOCKET_ERROR == retval)
	{
		int Ret = WSAGetLastError();

		if (Ret != WSAEWOULDBLOCK)
		{
			err_quit(L"connect()");
			closesocket(g_Socket);
		}
	}
}