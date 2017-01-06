#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <stdio.h>

void main(void)
{
	WSADATA wsa;

	if ( NULL != WSAStartup(MAKEWORD(2, 2), &wsa ) )
	{
		return;
	}

	u_short x	= 0x1234;
	u_long	y	= 0x12345678;

	u_short x2;
	u_long	y2;

	// ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ
	printf_s("ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ\n");
	printf_s("0x%x -> 0x%x\n", x, x2 = htons(x));
	printf_s("0x%x -> 0x%x\n", y, y2 = htonl(y));

	// ��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ
	printf_s("��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ\n");
	printf_s("0x%x -> 0x%x\n", x2, ntohs(x2));
	printf_s("0x%x -> 0x%x\n", y2, ntohl(y2));

	// u_long�� u_short���� ����� ���� ������
	printf_s("�߸��� ��� ��\n");
	printf_s("0x%x -> 0x%x\n", x, htonl(x));
}
