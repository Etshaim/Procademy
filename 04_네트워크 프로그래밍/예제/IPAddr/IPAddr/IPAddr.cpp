#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <locale.h>

void main(void)
{
	WSADATA wsa;

	if (NULL != WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		return;
	}

	// 원래의 IP 주소 출력
	_wsetlocale(LC_ALL, L"korean");

	WCHAR *ipAddr = L"147.46.114.70";
	IN_ADDR addr;

	wprintf(L"IP 주소 = %s\n", ipAddr);
	
	// inet_addr() 함수 연습
	InetPton(AF_INET, ipAddr, &addr.S_un.S_addr);
	wprintf(L"IP 주소(변환 후) = 0x%x\n", addr);
	
	// inet_ntoa() 함수 연습
	IN_ADDR temp;
	WCHAR szTemp[14];

	InetPton(AF_INET, ipAddr, &temp.S_un.S_addr);

	InetNtop(AF_INET, &temp.S_un.S_addr, szTemp, wcslen(szTemp) );

	wprintf(L"IP 주소(변환 후) = %s\n", szTemp);


	WSACleanup();


}