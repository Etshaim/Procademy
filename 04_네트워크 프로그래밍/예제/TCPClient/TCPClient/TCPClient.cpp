#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>

#define dfSERVERIP		"127.0.0.1"
#define dfSERVERPORT	9000
#define	dfBUFSIZE		512

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg);

// ���� �Լ� ���� ���
void err_display(char *msg);

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags);

void main(void)
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;

	if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		return;
	}

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	if (INVALID_SOCKET == sock)
	{
		err_quit("socket()");
	}

	// connect()
	SOCKADDR_IN serveraddr;

	memset(&serveraddr, 0, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(dfSERVERPORT);
	inet_pton(AF_INET, dfSERVERIP, &serveraddr.sin_addr.S_un.S_addr);

	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (SOCKET_ERROR == retval)
	{
		err_quit("conncet()");
	}

	// ������ ��ſ� ����� ����
	char	buf[dfBUFSIZE + 1];
	int		len;

	// ������ ������ ���
	while (1)
	{
		// ������ �Է�
		printf("\n[����������] ");

		if (NULL == fgets(buf, dfBUFSIZE + 1, stdin))
		{
			break;
		}

		// '\n' ���� ����
		len = strlen(buf);

		if (buf[len - 1] == '\n')
		{
			buf[len - 1] = '\0';
		}

		if (0 == strlen(buf))
		{
			break;
		}

		// ������ ������
		retval = send(sock, buf, strlen(buf), 0);

		if (SOCKET_ERROR == retval)
		{
			err_display("send()");
			break;
		}

		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

		// ������ �ޱ�
		retval = recvn(sock, buf, retval, 0);

		if (SOCKET_ERROR == retval)
		{
			err_display("recv()");
			break;
		}
		else if (0 == retval)
		{
			break;
		}

		// ���� ������ ���
		buf[retval] = '0';

		printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[���� ������] %s\n", buf);
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
		(LPWSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
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

	printf_s("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

//����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);

		if (SOCKET_ERROR == received)
		{
			return SOCKET_ERROR;
		}

		else if (0 == received)
		{
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}