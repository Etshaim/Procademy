#pragma comment (lib, "ws2_32")

#include <WinSock2.h>
#include <stdlib.h>

// ���� �Լ� ���� ��� �� ����
void err_quit(WCHAR *msg);

void main(void)
{
	// ���� �ʱ�ȭ
	// ���� ���̺귯��(WS2_32.DLL) �ʱ�ȭ�ϴ� ����
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
	MessageBox(NULL, L"���� �ʱ�ȭ", L"����", MB_OK);

	// closesocket()
	closesocket(tcpSock);

	// ���� ����
	// �ü���� �˸��� ���� ���ҽ��� ��ȯ
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