#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#define dfIP			INADDR_BROADCAST
#define	dfPORT			19001
#define	dfBUFSIZE		32
#define dfPROTOCOL_LEN	10

// ���� ����
int		retval;
SOCKET	sock;

// ������ ��ſ� ����� ����
WCHAR buf[dfBUFSIZE + 1];
int len;

// �������ݿ�
BYTE szProtocol[dfPROTOCOL_LEN];

// ������ ��ſ� ����� ����
// ���̸�
WCHAR szBuf[dfBUFSIZE + 1];
// IP �ּ�
WCHAR szBufIP[dfBUFSIZE + 1];

void SetProtocol(void);

//sock fuc
// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg);

// ���� �Լ� ���� ���
void err_display(char *msg);

void main(void)
{
	SetProtocol();

	// ���� �ʱ�ȭ
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}

	// ��ε�ĳ��Ʈ ������ ������
	for (int iCount = dfPORT; iCount < dfPORT + 100; iCount++)
	{
		// socket()
		sock = socket(AF_INET, SOCK_DGRAM, 0);

		if (sock == INVALID_SOCKET)
		{
			err_quit("socket()");
		}

		// ��ε� ĳ���� Ȱ��ȭ
		BOOL bEnable = TRUE;

		retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST,
			(char*)&bEnable, sizeof(bEnable));

		if (retval == SOCKET_ERROR)
		{
			err_quit("setsockopt()");
		}

		// ���� �ּ� ����ü �ʱ�ȭ
		SOCKADDR_IN remoteaddr;

		ZeroMemory(&remoteaddr, sizeof(remoteaddr));

		remoteaddr.sin_family = AF_INET;
		remoteaddr.sin_port = htons(iCount);
		remoteaddr.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);

		retval = sendto(sock, (char*)szProtocol, sizeof(szProtocol), 0,
			(SOCKADDR*)&remoteaddr, sizeof(remoteaddr));

		if (SOCKET_ERROR == retval)
		{
			err_display("sendto()");
			continue;
		}

		// Select ����

		// FD_SET
		FD_SET rset;

		FD_ZERO(&rset);
		FD_SET(sock, &rset);

		TIMEVAL tv;
		tv.tv_sec = 0;
		tv.tv_usec = 200000;

		// ������ ��ſ� ����� ����
		SOCKADDR_IN peeraddr;

		int addlen = sizeof(peeraddr);

		retval = select(0, &rset, NULL, NULL, &tv);

		if (SOCKET_ERROR == retval)
		{
			err_quit("select()");
		}

		if (retval > 0)
		{
			if (FD_ISSET(sock, &rset))
			{
				memset(szBuf, 0, dfBUFSIZE);

				retval = recvfrom(sock, (char*)szBuf, dfBUFSIZE, 0,
					(SOCKADDR*)&peeraddr, &addlen);

				wprintf_s(L"[IP %ws] [PORT %d] %s\n",
					InetNtop(AF_INET, &peeraddr.sin_addr, szBufIP, dfBUFSIZE),
					ntohs(peeraddr.sin_port), szBuf);

				
				
			}

			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				exit(0);
			}
		}

		// closesocket()
		closesocket(sock);
	}

	// ���� ����
	WSACleanup();
}

void SetProtocol(void)
{
	szProtocol[0] = 0xff;
	szProtocol[1] = 0xee;
	szProtocol[2] = 0xdd;
	szProtocol[3] = 0xaa;
	szProtocol[4] = 0x00;
	szProtocol[5] = 0x99;
	szProtocol[6] = 0x77;
	szProtocol[7] = 0x55;
	szProtocol[8] = 0x33;
	szProtocol[9] = 0x11;
}
//sock fuc
// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);

	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	printf_s("[%s] %s", msg, (char*)lpMsgBuf);

	LocalFree(lpMsgBuf);
}