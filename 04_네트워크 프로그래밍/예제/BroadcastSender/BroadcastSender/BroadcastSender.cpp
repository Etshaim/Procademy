#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>

#define dfBUFSIZE	512
#define dfPORT		9000
#define	dfIP		INADDR_BROADCAST


// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg);

// ���� �Լ� ���� ���
void err_display(char *msg);

void main(void)
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		return;
	}

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

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
	remoteaddr.sin_port = htons(dfPORT);
	remoteaddr.sin_addr.S_un.S_addr = htonl(dfIP);

	// ������ ��ſ� ����� ����
	char buf[dfBUFSIZE + 1];
	int	len;

	// ��ε�ĳ��Ʈ ������ ������
	while (1)
	{
		// ������ �Է�
		printf("\n[���� ������] ");
		
		if (fgets(buf, dfBUFSIZE + 1, stdin) == NULL)
		{
			break;
		}

		// '\n' ���� ����
		len = strlen(buf);

		if (buf[len - 1] == '\n')
		{
			buf[len - 1] = '\0';
		}

		if (strlen(buf) == 0)
		{
			break;
		}

		// ������ ������
		retval = sendto(sock, buf, strlen(buf), 0,
			(SOCKADDR*)&remoteaddr, sizeof(remoteaddr));

		if (retval == SOCKET_ERROR)
		{
			err_display("sendto()");
			continue;
		}
		printf("%d����Ʈ�� ���½��ϴ�.\n", retval);
	}

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();

}


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