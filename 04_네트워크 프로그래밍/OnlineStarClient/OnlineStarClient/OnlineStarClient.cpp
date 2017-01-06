#include "Console.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32")

#define dfMAX_STAR			20
#define dfTYPE_ID_ALLOC		0
#define dfTYPE_CONNECT		1
#define dfTYPE_DISCONNECT	2
#define dfTYPE_MOVE			3
#define dfPACKET_SIZE		16
#define dfSERVER_PORT		3000
#define dfSERVER_IP			"127.0.0.1"

//< 구조체
//패킷프로토콜 - 패킷은 16바이트 고정
//
//ID할당(0)			Type(4Byte) | ID(4Byte) | 안씀(4Byte) | 안씀(4Byte)
//신규접속(1)		Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//접속해제(2)		Type(4Byte) | ID(4Byte) | 안씀(4Byte) | 안씀(4Byte)
//이동(3)			Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//0 ~2번 패킷은 서버->클라패킷 이며, 3번 패킷은  클라 <->서버  쌍방 패킷임.

struct st_PACKET
{
	int	iType;
	int iID;
	int iPosX;
	int iPosY;
};

struct st_STAR
{
	int iPosX;
	int iPosY;
	int iID;
	BOOL bVisible;
};

// 스크린 버퍼
char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// 패킷
st_PACKET packet;

// 별 배열
st_STAR starArr[dfMAX_STAR];

// 내 별
//st_STAR starArr[0];

// 소켓
SOCKET sock;

// retval
int retval;

//< 소켓 함수
// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg);

// 소켓 함수 오류 출력
void err_display(char *msg);

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags);

// 초기화
void Init(void);

//< 버퍼의 내용을 화면으로 찍어주는 함수
void BufferFlip(void);

//< 화면 버퍼를 지워주는 함수
void BufferClear(void);

//< 버퍼의 특정 위치에 원하는 문자를 출력
void SpriteDraw(int x, int y, char sprite);

//< Draw 관련
// Draw
void Draw(void);

//< 키보드 입력
bool KeyProcess(void);

//< 네트워크 프로세스
void NetworkProcess(void);

//< 패킷 배열 처리
void PacketProcess(void);

void main(void)
{
	
	Init();
	// 윈속 초기화
	WSADATA wsa;

	if( 0 != WSAStartup( MAKEWORD(2, 2), &wsa ) )
	{
		return;
	}

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (INVALID_SOCKET == sock)
	{
		err_quit("socket()");
	}

	// connet()
	SOCKADDR_IN serveraddr;
	memset( &serveraddr, 0, sizeof(serveraddr) );
	
	inet_pton(AF_INET, dfSERVER_IP, &serveraddr.sin_addr.S_un.S_addr);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(dfSERVER_PORT);

	retval = connect( sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr) );

	if (SOCKET_ERROR == retval)
	{
		err_quit("connect()");
	}

	while (1)
	{
		// networkprocess();
		NetworkProcess();

		// keyprocess();
		if (KeyProcess())
		{
			// 이동했는지 확인
			if ((starArr[0].iPosX != starArr[0].iPosX) || (starArr[0].iPosY != starArr[0].iPosY))
			{
				st_PACKET tempPacket;

				tempPacket.iType	= dfTYPE_MOVE;
				tempPacket.iID		= starArr[0].iID;
				tempPacket.iPosX	= starArr[0].iPosX;
				tempPacket.iPosY	= starArr[0].iPosY;

				retval = send(sock, (char*)&tempPacket, sizeof(tempPacket), 0);

				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
				}
			}
			//// send()
			//packet.iType	= dfTYPE_MOVE;
			//packet.iID		= starArr[0].iID;
			//packet.iPosX	= starArr[0].iPosX;
			//packet.iPosY	= starArr[0].iPosY;
			
			/*retval = send(sock, (char*)&packet, sizeof(packet), 0);

			if (SOCKET_ERROR == retval)
			{
				err_display("send()");
			}*/
		}

		// draw();
		BufferClear();
		Draw();
		BufferFlip();

		// sleep();
		Sleep(50);
	}
	
}

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);

	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	printf_s("[%s] %s", msg, (char*)lpMsgBuf);

	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int		received;
	char	*ptr = buf;
	int		left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);

		if (received == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (received == 0)
		{
			break;
		}

		left -= received;
		ptr += received;
	}

	return (len - left);
}


// 초기화
void Init(void)
{
	cs_Initial();


	starArr[0].bVisible = FALSE;
	starArr[0].iID = 0;
	starArr[0].iPosX = 25;
	starArr[0].iPosY = 25;

	ZeroMemory(&packet, sizeof(packet));
}


//< 네트워크 프로세스
void NetworkProcess()
{
	//	FD_SET 세팅
	FD_SET rset;
	FD_SET wset;

	while (1)
	{
		// 소켓 셋 초기화
		// FD_ZERO	셋을 비운다
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(sock, &rset);
		
		// timeval
		// NULL : 적어도 한 소켓이 조건을 만족할 때까지 무한히 기다림
		TIMEVAL tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		// select()
		// 리턴값 : 조건을 만족하는 소켓 개수, 0(타임아웃), SOCKET_ERROR(실패)
		retval = select(0, &rset, NULL, NULL, &tv);

		if (SOCKET_ERROR == retval)
		{
			err_quit("select()");
		}

		if (retval > 0)
		{
			if (FD_ISSET(sock, &rset))
			{
				recvn(sock, (char*)&packet, dfPACKET_SIZE, 0);
			}

			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				exit(0);
			}

			// 로직 처리
			PacketProcess();
		}
		else
		{
			break;
		}
	}
}

//< 패킷 배열 처리
void PacketProcess(void)
{
	switch (packet.iType)
	{
		// ID 할당 (0)
	case dfTYPE_ID_ALLOC:
	{
		starArr[0].bVisible = TRUE;
		starArr[0].iID = packet.iID;
		// ID할당 시 x,y 좌표 안 보내주므로 주석 처리
		//starArr[0].iPosX = packet.iPosX;
		//starArr[0].iPosY = packet.iPosY;
	}
	break;

	// 신규 접속 (1)
	case dfTYPE_CONNECT:
	{
		// 별 배열에 추가
		for (int iCount = 0; iCount < dfMAX_STAR; iCount++)
		{
			// visible 이 FALSE면 빈 공간으로 간주
			if (FALSE == starArr[iCount].bVisible)
			{
				// 추가
				starArr[iCount].bVisible = TRUE;
				starArr[iCount].iID = packet.iID;
				starArr[iCount].iPosX = packet.iPosX;
				starArr[iCount].iPosY = packet.iPosY;

				// 내 별 정보도 바꾸기
				if (starArr[0].iID == packet.iID)
				{
					starArr[0].iPosX = packet.iPosX;
					starArr[0].iPosY = packet.iPosY;
				}

				break;
			}
		}
	}
	break;

	// 접속 해제 (2)
	case dfTYPE_DISCONNECT:
	{
		for (int iCount = 0; iCount < dfMAX_STAR; iCount++)
		{
			if (packet.iID == starArr[iCount].iID)
			{
				starArr[iCount].bVisible	= FALSE;
				starArr[iCount].iID			= 0;
				starArr[iCount].iPosX		= 0;
				starArr[iCount].iPosY		= 0;

				break;
			}
		}
	}
	break;

		// 이동 (3)
	case dfTYPE_MOVE:
	{
		for (int iCount = 0; iCount < dfMAX_STAR; iCount++)
		{
			if (packet.iID == starArr[iCount].iID)
			{
				starArr[iCount].iPosX = packet.iPosX;
				starArr[iCount].iPosY = packet.iPosY;

				break;
			}
		}
	}
	break;

	default:
	break;
	}
}

//< 키보드 입력
bool KeyProcess(void)
{
	BOOL bFlag = FALSE;

	if (GetAsyncKeyState(VK_LEFT) & 0x8001)
	{
		starArr[0].iPosX -= 1;
		bFlag = TRUE;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8001)
	{
		starArr[0].iPosX += 1;
		bFlag = TRUE;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		starArr[0].iPosY -= 1;
		bFlag = TRUE;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		starArr[0].iPosY += 1;
		bFlag = TRUE;
	}

	// esc 키
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		exit(0);
	}

	// 플레이어 이동 반경 제한
	starArr[0].iPosX = max(starArr[0].iPosX, 0);
	starArr[0].iPosX = min(starArr[0].iPosX, dfSCREEN_WIDTH - 2);

	starArr[0].iPosY = max(starArr[0].iPosY, 0);
	starArr[0].iPosY = min(starArr[0].iPosY, dfSCREEN_HEIGHT - 1);

	if (bFlag)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//< 버퍼의 내용을 화면으로 찍어주는 함수
void BufferFlip(void)
{
	// Connect Client

	

	for (int iCount = 0; iCount < dfSCREEN_HEIGHT; ++iCount)
	{
		cs_MoveCursor(0, iCount);
		printf_s(screenBuffer[iCount]);
	}

	
}

//< 화면 버퍼를 지워주는 함수
void BufferClear(void)
{
	for (int iCount = 0; iCount < dfSCREEN_HEIGHT; ++iCount)
	{
		memset(screenBuffer[iCount], ' ', dfSCREEN_WIDTH);
		screenBuffer[iCount][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}

//< 버퍼의 특정 위치에 원하는 문자를 출력
void SpriteDraw(int x, int y, char sprite)
{
	if (x < 0 || y < 0 || x >= dfSCREEN_WIDTH - 1 || y >= dfSCREEN_HEIGHT)
	{
		return;
	}

	screenBuffer[y][x] = sprite;
}

//< draw
void Draw(void)
{
	// Connect Client : x 출력

	/*int clientNum = 0;
	char szArr[18] = "Connect Cleint : ";

	for (int iCount = 0; iCount < dfMAX_STAR; iCount++)
	{
		if (TRUE == starArr[iCount].bVisible)
		{
			clientNum++;
		}
	}
*/
	
	for (int iCount = 0; iCount < dfMAX_STAR; ++iCount)
	{
		//// 내 별은 따로 찍을 것임
		//if (starArr[iCount].iID == starArr[0].iID)
		//{
		//	continue;
		//}

		if ( TRUE == starArr[iCount].bVisible)
		{
			SpriteDraw(starArr[iCount].iPosX, starArr[iCount].iPosY, '*');
		}
	}
	
	//SpriteDraw(starArr[0].iPosX, starArr[0].iPosY, '*');
}