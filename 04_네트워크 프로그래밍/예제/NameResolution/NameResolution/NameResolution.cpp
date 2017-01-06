#pragma comment (lib, "ws2_32")

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>


// 소켓 함수 오류 출력
void err_display(char *msg);

// 도메인 이름 -> IP 주소
BOOL GetIPAddr(WCHAR *name, IN_ADDR *addr);

// IP 주소 -> 도메인 이름
BOOL GetDomainName(IN_ADDR addr, WCHAR *name);

void main(void)
{
	WSADATA wsa;

	if (NULL != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		return;
	}

	_wsetlocale(LC_ALL, L"korean");

	// 도메인 이름 -> ip 주소
	IN_ADDR addr;

	WCHAR hostname[NI_MAXHOST];

	if (GetIPAddr(L"www.naver.com", &addr) )
	{
		wprintf(L"IP 주소 = %ws\n", InetNtopW(AF_INET, &addr, hostname, NI_MAXHOST));
	}

	ZeroMemory(hostname, NI_MAXHOST);

	// IP 주소 -> 도메인 이름
	
	if (GetDomainName(addr, hostname))
	{
		wprintf(L"도메인 이름 = %ws\n", hostname);
	}

	
	WSACleanup();
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf, 0, NULL);

	wprintf( L"[%s] %s", msg, (LPCWSTR)lpMsgBuf );
	
	LocalFree(lpMsgBuf);
}

// 도메인 이름 -> IP 주소
BOOL GetIPAddr(WCHAR *name, IN_ADDR *addr)
{
	PCWSTR pNodeName = name;
	PCWSTR pServiceName = L"0";

	DWORD	dwRetval;

	SOCKADDR_IN *pSockAddr;

	ADDRINFOW *result = NULL;
	ADDRINFOW hints;

	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	dwRetval = GetAddrInfoW(pNodeName, pServiceName, &hints, &result);

	if (0 != dwRetval)
	{
		err_display("GetIPAddr()");

		return FALSE;
	}

	pSockAddr = (SOCKADDR_IN*)result->ai_addr;

	*addr = pSockAddr->sin_addr;

	FreeAddrInfoW(result);

	return TRUE;
}

// IP 주소 -> 도메인 이름
BOOL GetDomainName(IN_ADDR addr, WCHAR *name)
{
	WCHAR hostname[NI_MAXHOST];
	WCHAR servInfo[NI_MAXSERV];
	
	DWORD dwRetval;

	SOCKADDR_IN sockaddr;
	
	ZeroMemory(&sockaddr, sizeof(SOCKADDR_IN));

	sockaddr.sin_addr.S_un.S_addr = addr.S_un.S_addr;
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = 0;

	dwRetval = GetNameInfoW((SOCKADDR*)&sockaddr,
							sizeof(SOCKADDR),
							hostname,
							NI_MAXHOST, servInfo, NI_MAXSERV, NI_NUMERICSERV);
	
	if (0 != dwRetval)
	{
		err_display("GetDomainName()");

		return FALSE;
	}
	
	wcscpy_s(name, NI_MAXHOST,hostname);

	return TRUE;
}
