#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>

#define dfSERVERIP		"127.0.0.1"
#define dfSERVERPORT	9000
#define	dfBUFSIZE		512

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg);

// 소켓 함수 오류 출력
void err_display(char *msg);

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags);

void main(void)
{
	int retval;

	// 윈속 초기화
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

	// 데이터 통신에 사용할 변수
	char	buf[dfBUFSIZE + 1];
	int		len;

	// 서버와 데이터 통신
	while (1)
	{
		// 데이터 입력
		printf("\n[보낼데이터] ");

		if (NULL == fgets(buf, dfBUFSIZE + 1, stdin))
		{
			break;
		}

		// '\n' 문자 제거
		len = strlen(buf);

		if (buf[len - 1] == '\n')
		{
			buf[len - 1] = '\0';
		}

		if (0 == strlen(buf))
		{
			break;
		}

		// 데이터 보내기
		retval = send(sock, buf, strlen(buf), 0);

		if (SOCKET_ERROR == retval)
		{
			err_display("send()");
			break;
		}

		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
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

		// 받은 데이터 출력
		buf[retval] = '0';

		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
}

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

//사용자 정의 데이터 수신 함수
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