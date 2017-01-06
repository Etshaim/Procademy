//----------------------------------------------------------------------
//160708
//별 움직이기 클라이언트
//----------------------------------------------------------------------

//패킷프로토콜 - 패킷은 16바이트 고정
//
//ID할당(0)		Type(4Byte) | ID(4Byte) | 안씀(4Byte) | 안씀(4Byte)
//신규접속(1)		Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//접속해제(2)		Type(4Byte) | ID(4Byte) | 안씀(4Byte) | 안씀(4Byte)
//이동(3)			Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//
//0 ~2번 패킷은 서버->클라패킷 이며, 3번 패킷은  클라 <->서버  쌍방 패킷임.
//
//
//
//클라이언트가 서버에 최초 접속시
//
//- ID할당(0) 패킷 수신
//- 신규접속(1) 패킷 수신(자기 자신꺼)
//- 신규접속(1) 패킷 수신(남들이 있을 경우에만)
//
//
//자기 자신이 이동할 때는 자기 좌표를 먼저 바꾸어주고, 서버에 이동 패킷을 보낸다.
//
//
//
//서버사용 포트 : 3000

#pragma comment(lib, "ws2_32")

//< 헤더 추가
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include "Console.h"

#define	dfMAX_STAR			20
#define dfTYPE_ID_ALLOC		0
#define	dfTYPE_CONNECT		1
#define	dfTYPE_DISCONNECT	2
#define dfTYPE_MOVE			3
#define dfST_PACKET_SIZE	16
#define dfSERVER_PORT		3000
#define dfSERVER_IP			"192.168.10.49"

//< 구조체
//패킷프로토콜 - 패킷은 16바이트 고정
//
//ID할당(0)			Type(4Byte) | ID(4Byte) | 안씀(4Byte) | 안씀(4Byte)
//신규접속(1)		Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//접속해제(2)		Type(4Byte) | ID(4Byte) | 안씀(4Byte) | 안씀(4Byte)
//이동(3)			Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//0 ~2번 패킷은 서버->클라패킷 이며, 3번 패킷은  클라 <->서버  쌍방 패킷임.

struct st_Packet
{
	int iType;
	int iID;
	int iPosX;
	int iPosY;
};

struct st_Star
{
	int		iPosX;
	int		iPosY;
	int		iID;
	BOOL	bVisible;
};

// 스크린 버퍼
char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// 패킷
st_Packet packet;

// 별 배열
st_Star starArr[dfMAX_STAR];

// 별 내꺼
st_Star myStar;

// 소켓
SOCKET sock;

// return value
int retval;

//sock fuc
// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg);

// 소켓 함수 오류 출력
void err_display(char *msg);

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags);

// func
// 초기화
void Init(void);

//< 버퍼의 내용을 화면으로 찍어주는 함수
void BufferFlip(void);

//< 화면 버퍼를 지워주는 함수
void BufferClear(void);

//< 버퍼의 특정 위치에 원하는 문자를 출력
void SpriteDraw(int x, int y, char sprite);

// Draw
void Draw(void);

//< 키보드 입력
bool KeyProcess(void);

//< 네트워크 프로세스
void NetworkProcess(void);

//< 패킷 배열 처리
void PacketProcess(void);



//< Main
void main(void)
{
	cs_Initial();

	// 윈속 초기화
	WSADATA wsa;
	
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	//serveraddr.sin_addr.s_addr = inet_addr(dfSERVER_IP);
	InetPton(AF_INET, L"127.0.0.1"/*(PCWSTR)dfSERVER_IP*/, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(dfSERVER_PORT);
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

	while (1)
	{
		// NetWorkProc()
		NetworkProcess();

		// KeyProcess()
		if (KeyProcess())
		{
			// 이동했는지 확인
			if ((myStar.iPosX != starArr[0].iPosX) || (myStar.iPosY != starArr[0].iPosY))
			{
				st_Packet tempPacket;

				tempPacket.iType	= dfTYPE_MOVE;
				tempPacket.iID		= myStar.iID;
				tempPacket.iPosX	= myStar.iPosX;
				tempPacket.iPosY	= myStar.iPosY;

				retval = send(sock, (char*)&tempPacket, sizeof(st_Packet), 0);

				if (retval == SOCKET_ERROR)
				{
					err_display("send()");
				}
			}
		}

		// Draw()
		BufferClear();
		Draw();
		BufferFlip();

		// Sleep()
		Sleep(50);
	}
}

//sock fuc
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
	char	*ptr		= buf;
	int		left		= len;

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

		left	-= received;
		ptr		+= received;
	}

	return (len - left);
}

//< 네트워크 프로세스
void NetworkProcess()
{
	//	FD_SET 세팅
	FD_SET rset, wset;

	while (1)
	{
		// 소켓 셋 초기화
		// FD_ZERO	셋을 비운다
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(sock, &rset);

		TIMEVAL tv;
		tv.tv_sec	= 0;
		tv.tv_usec	= 0;

		// select()
		retval = select(0, &rset, NULL, NULL, &tv);

		if (SOCKET_ERROR == retval)
		{
			err_quit("select()");
		}

		if (retval > 0)
		{
			if (FD_ISSET(sock, &rset))
			{
				recvn(sock, (char*)&packet, dfST_PACKET_SIZE, 0);
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

// 초기화
void Init(void)
{
	myStar.iPosX	= 10;
	myStar.iPosY	= 10;
	myStar.bVisible = TRUE;
}

//< 버퍼의 내용을 화면으로 찍어주는 함수
void BufferFlip(void)
{
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

//< 키보드 입력
bool KeyProcess(void)
{
	BOOL bFlag = FALSE;

	if (GetAsyncKeyState(VK_LEFT) & 0x8001)
	{
		myStar.iPosX -= 1;
		bFlag = TRUE;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8001)
	{
		myStar.iPosX += 1;
		bFlag = TRUE;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		myStar.iPosY -= 1;
		bFlag = TRUE;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		myStar.iPosY += 1;
		bFlag = TRUE;
	}

	// esc 키
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		exit(0);
	}

	// 플레이어 이동 반경 제한
	myStar.iPosX = max(myStar.iPosX, 0);
	myStar.iPosX = min(myStar.iPosX, dfSCREEN_WIDTH - 2);

	myStar.iPosY = max(myStar.iPosY, 0);
	myStar.iPosY = min(myStar.iPosY, dfSCREEN_HEIGHT - 1);

	if (bFlag)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//< 스크린 버퍼에 플레이어 그리기
void Draw(void)
{
	for (int iCount = 0; iCount < dfMAX_STAR; ++iCount)
	{
		if (starArr[iCount].bVisible)
		{
			SpriteDraw(starArr[iCount].iPosX, starArr[iCount].iPosY, '*');
		}
	}
	//SpriteDraw(myStar.iPosX, myStar.iPosY, '*');
}

//< 패킷 배열 처리
void PacketProcess(void)
{
	/*for (int iCount = 0; iCount < dfMAX_STAR; ++iCount)
	{*/
		switch (packet.iType)
		{
		//< 아이디 할당은 처음에만 받음
		case dfTYPE_ID_ALLOC:
		{
			// myStar 정보 세팅
			// ID 할당 받기
			myStar.iID = packet.iID;
			myStar.bVisible = TRUE;
		}
		break;

		case dfTYPE_CONNECT:
		{
			// 별 배열 돌면서 빈 자리에 세팅
			for (int iCount = 0; iCount < dfMAX_STAR; iCount++)
			{
				// 빈자리에 세팅
				if (starArr[iCount].bVisible == FALSE)
				{
					starArr[iCount].bVisible = TRUE;
					starArr[iCount].iID = packet.iID;
					starArr[iCount].iPosX = packet.iPosX;
					starArr[iCount].iPosY = packet.iPosY;

					// 만약 내 별이면 내 정보도 바꾸기
					if (myStar.iID == packet.iID)
					{
						myStar.iPosX = packet.iPosX;
						myStar.iPosY = packet.iPosY;

						break;
					}

					break;	// 빈 자리에 세팅했으면 break
				}
			}
		}
		break;
		
		case dfTYPE_DISCONNECT:
		{
			for (int iCount = 0; iCount < dfMAX_STAR; iCount++)
			{
				// 별 배열 돌면서 같은 아이디 정보 지우기
				if (starArr[iCount].iID == packet.iID)
				{
					starArr[iCount].iID = 0;
					starArr[iCount].iPosX = 0;
					starArr[iCount].iPosY = 0;
					starArr[iCount].bVisible = FALSE;

				// 만약 내 별이면 내 정보도 바꾸기
				if (myStar.iID == packet.iID)
				{
					myStar.iPosX = 0;
					myStar.iPosY = 0;
					myStar.iID = 0;
					myStar.bVisible = FALSE;
				}

					break;	// 찾았으면 break
				}
			}
		}
		break;
		
		case dfTYPE_MOVE:
		{
			for (int iCount = 0; iCount < dfMAX_STAR; iCount++)
			{
				if (starArr[iCount].iID == packet.iID)
				{
					starArr[iCount].iPosX = packet.iPosX;
					starArr[iCount].iPosY = packet.iPosY;

					// 만약 바꾼 별이 내 별이면 내 정보도 바꾸기
					if (myStar.iID == packet.iID)
					{
						myStar.iPosX = packet.iPosX;
						myStar.iPosY = packet.iPosY;
					}

					break;
				}
			}
		}
		break;

		default:
		{
			return;
		}
		break;
		}
}
