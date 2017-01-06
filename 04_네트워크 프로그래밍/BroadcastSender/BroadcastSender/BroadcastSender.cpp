//# 브로드 캐스팅을 통해서 현재 포트를 열고 기다리는 게임방 검색
//
//프로토콜 - 0xff 0xee 0xdd 0xaa 0x00 0x99 0x77 0x55 0x33 0x11   (10 Byte)
//
//포트범위 - 19001 ~19099
//
/////////////////////////////////////////////////////////////
//특정 포트에 대해 위 메시지를 브로드캐스팅으로 뿌린다.UDP
//해당 포트에 서버가 열려있다면 응답(게임 방 이름) 문자열이 되돌아 온다.
//로컬 네트워크 내부에 해당 포트의 서버가 없다면 응답이 오지 않음.
//
//조건 1. 포트범위 19001 ~19099 까지를 모두 검색해야 한다.
//
//조건 2. 데이터 응답까지는 200밀리세컨드를 대기한다. (마이크로세컨드, 밀리세컨드 필히 확인)
//
//조건 3. 서버의 방 이름, IP, Port 번호를 출력 한다.
//
//조건 4. 방은 총 10개 열려있음. 10개를 모두 찾아내면 성공
//
//
//= 브로드캐스팅은 같은 네트워크 내부에만 전송이 되므로 집에서는 테스트 불가
//
/////////////////////////////////////////////////////////////
//# 다 퍼주는 TIP
//1. UDP 소켓 생성
//2. 브로드캐스팅 활성화(소켓옵션 : 책참고, 옵션인자 : MSDN 참고)
//3. 브로드캐스팅 시 IP 는 INADDR_BROADCAST
//4. 프로토콜로 데이터 UDP 쏘기
//5. 200밀리세컨드 동안 데이터 수신 확인
//6. 데이터 오면 해당 송신자 확인, 데이터 확인 후 IP, Port, 방이름 출력
//
//위 방법으로 정해진 포트 범위를 모두 찾아봄.

#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#define dfREMOTE_IP		"255.255.255.255"
#define	dfREMOTE_PORT	19001
#define	dfBUFSIZE		32

// 전역 변수
int		retval;
SOCKET	sock;

// 데이터 통신에 사용할 변수
WCHAR buf[dfBUFSIZE + 1];
int len;

//sock fuc
// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg);

// 소켓 함수 오류 출력
void err_display(char *msg);

void main(void)
{
	BYTE szTemp[10];

	szTemp[0] = 0xff;
	szTemp[1] = 0xee;
	szTemp[2] = 0xdd;
	szTemp[3] = 0xaa;
	szTemp[4] = 0x00;
	szTemp[5] = 0x99;
	szTemp[6] = 0x77;
	szTemp[7] = 0x55;
	szTemp[8] = 0x33;
	szTemp[9] = 0x11;

	// 윈속 초기화
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}

	// 10 14 40 44 55 67 70 80 88 99
	

	// 브로드캐스트 데이터 보내기
	for (int iCount = dfREMOTE_PORT; iCount < dfREMOTE_PORT+99; iCount++)
	{
		// socket()
		sock = socket(AF_INET, SOCK_DGRAM, 0);

		if (sock == INVALID_SOCKET)
		{
			err_quit("socket()");
		}

		// 브로드 캐스팅 활성화
		BOOL bEnable = TRUE;

		retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST,
			(char*)&bEnable, sizeof(bEnable));

		if (retval == SOCKET_ERROR)
		{
			err_quit("setsockopt()");
		}

		// 소켓 주소 구조체 초기화
		SOCKADDR_IN remoteaddr;

		ZeroMemory(&remoteaddr, sizeof(remoteaddr));

		remoteaddr.sin_family = AF_INET;
		InetPton(AF_INET, L"255.255.255.255"/*(PCWSTR)SERVERIP*/, &remoteaddr.sin_addr);
		//remoteaddr.sin_addr.s_addr = inet_addr(dfREMOTE_IP);
		//remoteaddr.sin_port = htons(dfREMOTE_PORT);

		remoteaddr.sin_port = htons(iCount);

		retval = sendto(sock, (char*)(/*(WCHAR)*/szTemp), 10, 0,
			(SOCKADDR*)&remoteaddr, sizeof(remoteaddr));

		if (retval == SOCKET_ERROR)
		{
			err_display("sendto()");
			continue;
		}

		//	FD_SET 세팅
		FD_SET rset, wset;

		// 소켓 셋 초기화
		// FD_ZERO	셋을 비운다
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(sock, &rset);

		TIMEVAL tv;
		tv.tv_sec	= 0;
		tv.tv_usec	= 200000;

		// 데이터 통신에 사용할 변수
		SOCKADDR_IN peeraddr;
		int addlen = sizeof(peeraddr);

		// select()
		retval = select(0, &rset, NULL, NULL, &tv);

		if (SOCKET_ERROR == retval)
		{
			err_quit("select()");
		}

		if (retval > 0)
		{
			if (FD_ISSET(sock, &rset))
			{
				//retval = recv(sock, (char*)buf, dfBUFSIZE - 1, 0);
				//wprintf_s(L"[IP %s] [PORT %d] %s\n", ntohs(remoteaddr.sin_port), buf);
				
				memset(buf, 0, dfBUFSIZE);

				retval = recvfrom(sock, (char*)buf, dfBUFSIZE, 0, 
					(SOCKADDR*)&peeraddr, &addlen);

				WCHAR szIP[16] = { 0, };
				wprintf_s(L"[IP %s] [PORT %d] %s\n", 
					InetNtop(AF_INET, &peeraddr.sin_addr, szIP, 16), 
					ntohs(peeraddr.sin_port), buf);
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
	//프로토콜 - 0xff 0xee 0xdd 0xaa 0x00 0x99 0x77 0x55 0x33 0x11   ( 10 Byte )
}


//sock fuc
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