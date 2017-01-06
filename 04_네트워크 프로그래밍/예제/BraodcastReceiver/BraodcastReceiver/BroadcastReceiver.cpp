#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#define dfBUFSIZE	512
#define dfPORT		9000
#define dfIP		INADDR_ANY


// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg);

// 소켓 함수 오류 출력
void err_display(char *msg);


void main(void)
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// bind()
	SOCKADDR_IN localaddr;
	ZeroMemory(&localaddr, sizeof(localaddr));
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(dfPORT);
	localaddr.sin_addr.S_un.S_addr = htonl(dfIP);

	retval = bind(sock, (SOCKADDR*)&localaddr, sizeof(localaddr));

	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN peeraddr;
	int addrlen;
	char buf[dfBUFSIZE + 1];

	// 브로드캐스트 데이터 받기
	while (1)
	{
		// 데이터 받기
		addrlen = sizeof(peeraddr);

		retval = recvfrom(sock, buf, dfBUFSIZE, 0,
			(SOCKADDR*)&peeraddr, &addrlen);

		if (retval == SOCKET_ERROR)
		{
			err_display("recvfrom()");
			continue;
		}

		// 받은 데이터 출력
		buf[retval] = '\0';

		char szBuf[16];

		inet_ntop(AF_INET, &peeraddr.sin_addr, szBuf, 16);

		
		printf("[UDP/%s:%d] %s\n", szBuf,ntohs(peeraddr.sin_port), buf);
	}

	// closesocket
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
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);

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

	printf_s("[%s] %s", msg, (char*)lpMsgBuf);

	LocalFree(lpMsgBuf);
}