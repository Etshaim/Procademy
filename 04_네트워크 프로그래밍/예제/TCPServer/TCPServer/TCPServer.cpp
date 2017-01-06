#pragma comment(lib, "ws2_32")


#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <WS2tcpip.h>

#define SERVERPORT	9000
#define	BUFSIZE		512

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

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
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
	// 내가 오픈할 소켓 결정하는 시기
	// 소켓의 지역 IP주소와 지역 포트 번호를 결정
	SOCKADDR_IN serveraddr;

	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	//memset(&serveraddr, 0, sizeof(SOCKADDR_IN));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);	// INADDR_ANY : 0.0.0.0 어느 IP상에서도 오픈(서버만 사용 가능)
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

	if (SOCKET_ERROR == retval )
	{
		err_quit("bind()");
	}

	// listen()
	// 소켓의 TCP 포트 상태를 LISTENING으로 바꿈
	// 클라이언트 접속을 받아들일 수 있는 상태
	retval = listen(listen_sock, SOMAXCONN);

	if (SOCKET_ERROR == retval )
	{
		err_quit("listen()");
	}

	// 데이터 통신에 사용할 변수
	SOCKET			client_sock;
	SOCKADDR_IN		clientaddr;

	int addrlen;
	char buf[BUFSIZE + 1];

	while (1)
	{
		// accept()
		addrlen = sizeof(SOCKADDR_IN);

		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen); // addrlen : 값-결과 인자(_inout)
		// 대기 상태로 만든다
		// 클라이언트가 접속하면 서버가 깨어나고 accept()함수는 리턴함

		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		printf_s("\n[TCP 서버] 클라이언트 접속 : IP 주소=%s, 포트번호=%d\n",
			inet_ntop(AF_INET, &clientaddr.sin_addr, buf, BUFSIZE + 1), ntohs(clientaddr.sin_port));

		// 클라이언트와 데이터 통신
		while (1)
		{
			// 데이터 받기
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

			// 받은 데이터 출력
			buf[retval] = '\0';

			printf_s("[TCP/%s:%d] %s\n", InetNtop(AF_INET, &clientaddr.sin_addr, (PWSTR)buf, BUFSIZE + 1)/*inet_ntoa(clientaddr.sin_addr)*/,
				ntohs(clientaddr.sin_port), buf);

			// 데이터 보내기
			retval = send(client_sock, buf, retval, 0);

			if (retval == SOCKET_ERROR)
			{
				err_display("send()");
				break;
			}
		}

		// closesocket()
		closesocket(client_sock);
		printf_s("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
			InetNtop(AF_INET, &clientaddr.sin_addr, (PWSTR)buf, BUFSIZE + 1)/*inet_ntoa(clientaddr.sin_addr)*/, ntohs(clientaddr.sin_port));
	}

	// closesocket()
	closesocket(client_sock);

	// 왼속 종료
	WSACleanup();

	return 0;


}