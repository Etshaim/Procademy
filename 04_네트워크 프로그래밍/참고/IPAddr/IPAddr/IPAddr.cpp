#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	//------------------------------
	//	IPv4 ��ȯ ����
	//------------------------------

	// ������ IPv4 �ּ� ���
	char *ipv4test = "147.46.114.70";

	printf_s("IPv4 �ּ�(��ȯ ��) = %s\n", ipv4test);

	// inet_addr() �Լ� ����
	printf_s("IPv4 �ּ�(��ȯ ��) = 0x%x\n", inet_addr( ipv4test ) );

	// inet_ntoa() �Լ� ����
	IN_ADDR ipv4num;
	ipv4num.s_addr = inet_addr(ipv4test);

	printf_s("IPv4 �ּ�(�ٽ� ��ȯ ��) = %s\n", inet_ntoa( ipv4num ) );

	printf_s("\n");

	//------------------------------
	//	IPv6 ��ȯ ����
	//------------------------------

	// ������ IPv6 �ּ� ���
	char *ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";

	printf_s("IPv6 �ּ�(��ȯ ��) = %s\n", ipv6test);

	// WSAStringToAddress() �Լ� ����
	SOCKADDR_IN6 ipv6num;

	int addrlen = sizeof(ipv6num);
	WSAStringToAddress(ipv6test, AF_INET6, NULL,
		(SOCKADDR *)&ipv6num, &addrlen);

	printf_s("IPv6 �ּ�(��ȯ ��) = 0x");

	for (int i = 0; i < 16; ++i)
	{
		printf_s("%02x", ipv6num.sin6_addr.u.Byte[i]);
	}
	printf_s("\n");

	// WSAAddressToString() �Լ� ����
	char ipaddr[50];
	DWORD ipaddrlen = sizeof(ipaddr);

	WSAAddressToString((SOCKADDR *)&ipv6num, sizeof(ipv6num),
		NULL, ipaddr, &ipaddrlen);

	printf_s("IPv6 �ּ�(�ٽ� ��ȯ ��) = %s\n", ipaddr);

	WSACleanup();

	return 0;

}