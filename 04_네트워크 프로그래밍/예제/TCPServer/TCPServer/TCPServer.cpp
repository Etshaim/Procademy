#pragma comment(lib, "ws2_32")


#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>

#define SERVERPORT	9000
#define	BUFSIZE		512

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

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (INVALID_SOCKET == listen_sock )
	{
		err_quit("socket()");
	}

	// bind()
	// ���� ������ ���� �����ϴ� �ñ�
	// ������ ���� IP�ּҿ� ���� ��Ʈ ��ȣ�� ����
	SOCKADDR_IN serveraddr;

	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	//memset(&serveraddr, 0, sizeof(SOCKADDR_IN));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	// INADDR_ANY : 0.0.0.0 ��� IP�󿡼��� ����(������ ��� ����)
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

	if (SOCKET_ERROR == retval )
	{
		err_quit("bind()");
	}

	// listen()
	// ������ TCP ��Ʈ ���¸� LISTENING���� �ٲ�
	// Ŭ���̾�Ʈ ������ �޾Ƶ��� �� �ִ� ����
	retval = listen(listen_sock, SOMAXCONN);

	if (SOCKET_ERROR == retval )
	{
		err_quit("listen()");
	}

	// ������ ��ſ� ����� ����
	SOCKET			client_sock;
	SOCKADDR_IN		clientaddr;

	int addrlen;
	char buf[BUFSIZE + 1];

	while (1)
	{
		// accept()
		addrlen = sizeof(SOCKADDR_IN);

		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen); // addrlen : ��-��� ����(_inout)
		// ��� ���·� �����
		// Ŭ���̾�Ʈ�� �����ϸ� ������ ����� accept()�Լ��� ������

		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf_s("\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ�=%s, ��Ʈ��ȣ=%d\n",
			inet_ntop(AF_INET, &clientaddr.sin_addr, buf, BUFSIZE + 1), ntohs(clientaddr.sin_port));

		// Ŭ���̾�Ʈ�� ������ ���
		while (1)
		{
			// ������ �ޱ�
			retval = recv(client_sock, buf, BUFSIZE, 0);

			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				break;
			}
			else if (retval == 0)
			{
				break;
			}

			// ���� ������ ���
			buf[retval] = '\0';

			printf_s("[TCP/%s:%d] %s\n", InetNtop(AF_INET, &clientaddr.sin_addr, (PWSTR)buf, BUFSIZE + 1)/*inet_ntoa(clientaddr.sin_addr)*/,
				ntohs(clientaddr.sin_port), buf);

			// ������ ������
			retval = send(client_sock, buf, retval, 0);

			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
		}

		// closesocket()
		closesocket(client_sock);
		printf_s("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			InetNtop(AF_INET, &clientaddr.sin_addr, (PWSTR)buf, BUFSIZE + 1)/*inet_ntoa(clientaddr.sin_addr)*/, ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(client_sock);

	// �޼� ����
	WSACleanup();

	return 0;


}