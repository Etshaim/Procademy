#include "stdafx.h"
#include "Protocol.h"
#include "Network.h"
#include "TCPFighter.h"
#include "StreamSQ.h"
#include "PacketProc.h"
#include "ProtocolBuffer.h"


#define WM_SOCKET		(WM_USER + 1)

extern	SOCKET		g_Socket;
extern	BOOL		g_bConnected;
extern	HWND		g_hWnd;
extern	BOOL		g_SendFlag;
extern	CStreamSQ	g_SendQ;
extern	CStreamSQ	g_RecvQ;
extern	WCHAR		g_szIP[16];

// ���� �� ���� return FALSE
// ���� ó��	return TRUE
BOOL	NetworkProc(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int iRetval = 0;

	// ���� �߻� ���� Ȯ��
	if (0 != WSAGETSELECTERROR(lParam))
	{
		err_quit((WCHAR*)WSAGETSELECTERROR(lParam));
		closesocket(g_Socket);
		return FALSE;
	}

	// �޽��� ó��
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

	// �ӽ�	���� ���� 
	char recvBuff[1000];
	

	// recv ȣ��
	iRetval = recv(g_Socket, recvBuff, 1000, 0);

	// retval  ����(0) / ����(SOCKET_ERROR)
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

	// RecvQ�� �ֱ�
	g_RecvQ.Put(recvBuff, iRetval);

	// �Ϸ� ��Ŷ ó����
	// �Ϸ� ��Ŷ ó�� �κ��� ť�� �ִ� "�ϼ�" ��Ŷ�� �� ó���� ��
	while (1)
	{
		CProtocolBuffer cBuffer(16);

		int iUseSize = g_RecvQ.GetUseSize();

		// �ּ����� ������ Ȯ�� ( ��� ������ )
		if (sizeof(st_PACKET_HEADER) > iUseSize)
		{
			break;
		}

		st_PACKET_HEADER header;
		ZeroMemory(&header, sizeof(st_PACKET_HEADER));

		// ��� Peek
		iRetval = g_RecvQ.Peek((char*)&header, sizeof(st_PACKET_HEADER));

		// code Ȯ��
		if (header.byCode != dfPACKET_CODE)
		{
			MessageBox(NULL, L"Error : Packet Header", NULL, MB_ICONERROR);
			return FALSE;
		}

		int iPacketSize = sizeof(st_PACKET_HEADER) + header.bySize + sizeof(BYTE);

		// �ϼ� ��Ŷ ũ�⸸ŭ ť�� �����Ͱ� �ִ��� Ȯ��
		if (iUseSize < iPacketSize)
		{
			// �ϼ� ��Ŷ��ŭ ť�� ����
			return TRUE;
		}

		// RecvQ���� ��� �����
		g_RecvQ.RemoveData(sizeof(st_PACKET_HEADER));

		// ���̷ε� ������
		iRetval = g_RecvQ.Get(cBuffer.GetBufferPtr(), header.bySize);
		cBuffer.MoveWritePos(iRetval);


		// End �ڵ� ���� Ȯ��
		BYTE byEndCode = 0;

		iRetval = g_RecvQ.Get((char*)&byEndCode, sizeof(BYTE));

		if (dfNETWORK_PACKET_END != byEndCode)
		{
			MessageBox(NULL, L"Error : Packet Endcode", NULL, MB_ICONERROR);
			return FALSE;
		}

		// ��� Ÿ�Կ� ���� �б�
		PacketProc(header.byType, &cBuffer);
	}

	return TRUE;
}



void PacketProc(BYTE byPacketType, CProtocolBuffer * pPacket)
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

	case dfPacket_SC_SYNC:
		PacketProc_Sync(pPacket);

	default:
		break;
	}
}

BOOL	SendPacket(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket)
{
	// ���� ���� ���� ó��
	if (FALSE == g_bConnected)
	{
		// ���� ���� �ƴϹǷ� ����
		return FALSE;
	}

	int iRetval = 0;

	// SendQ�� ��� �ֱ�
	iRetval = g_SendQ.Put((char*)pHeader, sizeof(st_PACKET_HEADER));

	// SendQ�� ��Ŷ �ֱ�
	iRetval = g_SendQ.Put(pPacket->GetBufferPtr(), pHeader->bySize);
	pPacket->MoveWritePos(iRetval);

	BYTE byEndCode = dfNETWORK_PACKET_END;

	// EndCode �ֱ�
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
	// ������� �ӽ� ����
	char sendBuff[1000];
	ZeroMemory(&sendBuff, 1000);

	// send ���� ���� ( FD_WRITE)
	if (FALSE == g_SendFlag)
	{
		return TRUE;
	}

	// SendQ�� ���� �����Ͱ� �ִ��� Ȯ��
	int iUseSize = g_SendQ.GetUseSize();

	if (0 == iUseSize)
	{
		return TRUE;
	}

	// SendQ�� �����Ͱ� ���ų� ������ �� ���� ��� ����
	while (1)
	{
		// SendQ�� �����Ͱ� �ִ��� Ȯ��
		iUseSize = g_SendQ.GetUseSize();

		if (0 == iUseSize)
		{
			break;
		}

		int iRetval = 0;
		int iSize = 0;
		// �ӽù��۷� Peek

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


// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

	// ������ ��ſ� ����� ����
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