#pragma comment (lib, "ws2_32")

#include <WinSock2.h>
#include <stdlib.h>

// 소켓 함수 오류 출력 후 종료
void err_quit(WCHAR *msg);

void main(void)
{
	// 윈속 초기화
	// 윈속 라이브러리(WS2_32.DLL) 초기화하는 역할
	WSADATA wsa;

	if ( 0 != WSAStartup( MAKEWORD(2, 2), &wsa ) )
	{
		return;
	}

	SOCKET tcpSock = socket( AF_INET, SOCK_STREAM, 0 );

	if (INVALID_SOCKET == tcpSock)
	{
		err_quit( L"socket()" );
	}
	MessageBox(NULL, L"윈속 초기화", L"성공", MB_OK);

	// closesocket()
	closesocket(tcpSock);

	// 윈속 종료
	// 운영체제에 알리고 관련 리소스를 반환
	WSACleanup();
	
	return;
}

void err_quit(WCHAR *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONERROR);

	LocalFree(lpMsgBuf);

	exit(-1);
}