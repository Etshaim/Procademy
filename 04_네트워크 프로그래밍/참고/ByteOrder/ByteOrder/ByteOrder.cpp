#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	u_short	x1 = 0x1234;
	u_long	y1 = 0x12345678;
	u_short x2;
	u_long	y2;

	// ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ
	printf_s("[ȣ��Ʈ ����Ʈ -> ��Ʈ��ũ ����Ʈ]\n");
	printf_s("0x%x -> 0x%x\n", x1, x2 = htons(x1));
	printf_s("0x%x -> 0x%x\n", y1, y2 = htonl(y1));

	// ��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ
	printf_s("\n[��Ʈ��ũ ����Ʈ -> ȣ��Ʈ ����Ʈ]\n");
	printf_s("0x%x -> 0x%x\n", x2, x1 = htons(x2));
	printf_s("0x%x -> 0x%x\n", y2, y1 = htonl(y2));

	// �߸��� ��� ��
	printf_s("\n[�߸��� ��� ��]\n");
	printf_s("0x%x -> 0x%x\n", x1, htonl(x1));

	WSACleanup();

	return 0;

}