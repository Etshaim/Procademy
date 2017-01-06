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

	// 호스트 바이트 -> 네트워크 바이트
	printf_s("[호스트 바이트 -> 네트워크 바이트]\n");
	printf_s("0x%x -> 0x%x\n", x1, x2 = htons(x1));
	printf_s("0x%x -> 0x%x\n", y1, y2 = htonl(y1));

	// 네트워크 바이트 -> 호스트 바이트
	printf_s("\n[네트워크 바이트 -> 호스트 바이트]\n");
	printf_s("0x%x -> 0x%x\n", x2, x1 = htons(x2));
	printf_s("0x%x -> 0x%x\n", y2, y1 = htonl(y2));

	// 잘못된 사용 예
	printf_s("\n[잘못된 사용 예]\n");
	printf_s("0x%x -> 0x%x\n", x1, htonl(x1));

	WSACleanup();

	return 0;

}