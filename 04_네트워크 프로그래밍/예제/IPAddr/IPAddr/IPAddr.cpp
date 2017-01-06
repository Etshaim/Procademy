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

	// ������ IP �ּ� ���
	_wsetlocale(LC_ALL, L"korean");

	WCHAR *ipAddr = L"147.46.114.70";
	IN_ADDR addr;

	wprintf(L"IP �ּ� = %s\n", ipAddr);
	
	// inet_addr() �Լ� ����
	InetPton(AF_INET, ipAddr, &addr.S_un.S_addr);
	wprintf(L"IP �ּ�(��ȯ ��) = 0x%x\n", addr);
	
	// inet_ntoa() �Լ� ����
	IN_ADDR temp;
	WCHAR szTemp[14];

	InetPton(AF_INET, ipAddr, &temp.S_un.S_addr);

	InetNtop(AF_INET, &temp.S_un.S_addr, szTemp, wcslen(szTemp) );

	wprintf(L"IP �ּ�(��ȯ ��) = %s\n", szTemp);


	WSACleanup();


}