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

	// 호스트 바이트 -> 네트워크 바이트
	printf_s("호스트 바이트 -> 네트워크 바이트\n");
	printf_s("0x%x -> 0x%x\n", x, x2 = htons(x));
	printf_s("0x%x -> 0x%x\n", y, y2 = htonl(y));

	// 네트워크 바이트 -> 호스트 바이트
	printf_s("네트워크 바이트 -> 호스트 바이트\n");
	printf_s("0x%x -> 0x%x\n", x2, ntohs(x2));
	printf_s("0x%x -> 0x%x\n", y2, ntohl(y2));

	// u_long에 u_short형을 사용할 때의 문제점
	printf_s("잘못된 사용 예\n");
	printf_s("0x%x -> 0x%x\n", x, htonl(x));
}
