#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Console.h"

#define SERVERPORT			3000
#define dfST_PACKET_SIZE	16

#define dfTYPE_ID_ALLOC		0
#define	dfTYPE_CONNECT		1
#define	dfTYPE_DISCONNECT	2
#define dfTYPE_MOVE			3

struct CLIENT
{
	SOCKET sock;
	int iPosX;
	int iPosY;
	int iID;
};

struct st_Packet
{
	int iType;
	int iID;
	int iPosX;
	int iPosY;
};


CLIENT *g_pClient[FD_SETSIZE];

// listen_sock
SOCKET listen_sock;

// 패킷
st_Packet packet;

// 스크린 버퍼
char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// return value
int retval;

// 클라이언트 소켓 수
int nTotalSockets = 0;

// 소켓 관리 함수
BOOL AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(int nIndex);

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags);

// 오류 출력 함수
void err_quit(char *msg);
void err_display(char *msg);

// ID 발급
int IssueID(void);

// Network
void Network(void);

// Uni Cast
BOOL SendUnicast(SOCKET sock, int iIndex, st_Packet pack, int iSize);

// Broad Cast
BOOL SendBroadcast(st_Packet pack, int iSize);

// Draw
void Draw(void);
void SpriteDraw(int posX, int posY, char sprite);
void BufferFlip(void);
void BufferClear(void);

//int main(int argc, char *argv[])
void main(void)
{
	// 콘솔 초기화
	cs_Initial();

	// 윈속 초기화
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}

	// socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (listen_sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
	{
		err_quit("bind()");
	}

	while (1)
	{
		// Network()
		Network();

		// Draw()
		BufferClear();
		Draw();
		BufferFlip();

		//Sleep(50);
	}
	// 윈속 종료
	WSACleanup();

	return;
}

// ID 발급
int IssueID(void)
{
	srand((unsigned)time(NULL));

	int randNum = rand() % FD_SETSIZE + 1;

	for (int iCount = 0; iCount < nTotalSockets; iCount++)
	{
		//if (randNum == g_Client[iCount].iID)
		if (randNum == g_pClient[iCount]->iID)
		{
			continue;
		}
	}

	return randNum;
}

// Uni Cast
BOOL SendUnicast(SOCKET sock, int iIndex, st_Packet pack, int iSize)
{
	retval = send(sock, (char*)&pack, iSize, 0);

	if (SOCKET_ERROR == retval)
	{
		RemoveSocketInfo(iIndex);
		return FALSE;
	}

	return TRUE;
}

// Broad Cast
BOOL SendBroadcast(st_Packet pack, int iSize)
{
	for (int iCount = 0; iCount < nTotalSockets; iCount++)
	{
		// 접속 체크
		//SendUnicast(g_Client[iCount].sock, iCount, pack, iSize);
		if (FALSE == SendUnicast(g_pClient[iCount]->sock, iCount, pack, iSize))
		{
			return FALSE;
		}

	}

	return TRUE;
}

// Network
void Network(void)
{
	// listen()
	retval = listen(listen_sock, SOMAXCONN);

	if (retval == SOCKET_ERROR)
	{
		err_quit("listen()");
	}

	// 데이터 통신에 사용할 변수
	FD_SET		rset;
	FD_SET		wset;
	SOCKET		client_sock;
	SOCKADDR_IN clientaddr;

	TIMEVAL tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	int addrlen = 0;

	// 소켓 셋 초기화
	FD_ZERO(&rset);
	FD_ZERO(&wset);

	FD_SET(listen_sock, &rset);

	for (int iCount = 0; iCount < nTotalSockets; iCount++)
	{
		if (g_pClient[iCount]->sock == INVALID_SOCKET)
		{
			err_display("accept()");
		}
		else
		{
			FD_SET(g_pClient[iCount]->sock, &rset);
		}
	}

	// selecet()
	retval = select(0, &rset, NULL, NULL, &tv);

	if (retval == SOCKET_ERROR)
	{
		err_quit("select()");
	}

	// 소켓 셋 검사(1) : 클라이언트 접속 수용
	if (FD_ISSET(listen_sock, &rset))
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);

		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
		}
		else
		{
			WCHAR szBuf[16] = { 0, };

			printf_s("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트번호=%d\n",
				InetNtop(AF_INET, &clientaddr.sin_addr, szBuf, 16),
				ntohs(clientaddr.sin_port));

			// 소켓정보 추가
			if (AddSocketInfo(client_sock))
			{
				st_Packet temp;
				temp.iID = g_pClient[nTotalSockets - 1]->iID;
				temp.iPosX = g_pClient[nTotalSockets - 1]->iPosX;
				temp.iPosY = g_pClient[nTotalSockets - 1]->iPosY;
				temp.iType = dfTYPE_ID_ALLOC;

				// 아이디 할당
				SendUnicast(g_pClient[nTotalSockets - 1]->sock,
					nTotalSockets - 1, temp, dfST_PACKET_SIZE);

				// 전체에게 알리기
				// 신규접속(1) 패킷 발신
				temp.iType = dfTYPE_CONNECT;
				SendBroadcast(temp, dfST_PACKET_SIZE);

				// 다른 정보 보내기
				for (int iCount = 0; iCount < nTotalSockets - 1; iCount++)
				{
					st_Packet temp;

					temp.iID = g_pClient[iCount]->iID;
					temp.iPosX = g_pClient[iCount]->iPosX;
					temp.iPosY = g_pClient[iCount]->iPosY;
					temp.iType = dfTYPE_CONNECT;


					SendUnicast(g_pClient[nTotalSockets - 1]->sock, nTotalSockets - 1,
						temp, dfST_PACKET_SIZE);

				}
			}
		}
	}

	// 소켓 셋 검사(2) : 데이터 통신
	for (int iCount = 0; iCount < nTotalSockets; iCount++)
	{
		//SOCKETINFO *ptr = SocketInfoArray[iCount];

		CLIENT *ptr = g_pClient[iCount];

		//if (FD_ISSET(ptr->sock, ptr->buf, BUFSIZE, 0))
		if (FD_ISSET(ptr->sock, &rset))
		{

			//retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
			if (g_pClient[iCount]->sock != INVALID_SOCKET)
			{
				retval = recvn(ptr->sock, (char*)&packet, dfST_PACKET_SIZE, 0);

				if (retval == SOCKET_ERROR)
				{
					err_display("recv()");
					RemoveSocketInfo(iCount);
					continue;
				}
				else if (retval == 0)
				{
					RemoveSocketInfo(iCount);
					continue;
				}

				if (packet.iType == dfTYPE_MOVE)
				{
					packet.iPosX = max(packet.iPosX, 0);
					packet.iPosX = min(packet.iPosX, dfSCREEN_WIDTH - 2);
					packet.iPosY = max(packet.iPosY, 0);
					packet.iPosY = min(packet.iPosY, dfSCREEN_HEIGHT - 1);

					for (int iCount = 0; iCount < nTotalSockets; iCount++)
					{
						if (g_pClient[iCount]->iID == packet.iID)
						{
							g_pClient[iCount]->iPosX = packet.iPosX;
							g_pClient[iCount]->iPosY = packet.iPosY;

							SendBroadcast(packet, dfST_PACKET_SIZE);
							break;
						}
					}

					
				}
			}
		}
	}

}

// Draw
void Draw(void)
{
	for (int iCount = 0; iCount < nTotalSockets; ++iCount)
	{
		SpriteDraw(g_pClient[iCount]->iPosX, g_pClient[iCount]->iPosY, '*');
	}
}

void SpriteDraw(int posX, int posY, char sprite)
{
	if (posX < 0 || posY < 0 || posX >= dfSCREEN_WIDTH - 1 || posY >= dfSCREEN_HEIGHT)
	{
		return;
	}

	screenBuffer[posY][posX] = sprite;
}

void BufferFlip(void)
{
	for (int iCount = 0; iCount < dfSCREEN_HEIGHT; ++iCount)
	{
		cs_MoveCursor(0, iCount);
		printf_s(screenBuffer[iCount]);
	}
}

void BufferClear(void)
{
	for (int iCount = 0; iCount < dfSCREEN_HEIGHT; ++iCount)
	{
		memset(screenBuffer[iCount], ' ', dfSCREEN_WIDTH);
		screenBuffer[iCount][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}

// 소켓 정보 추가
BOOL AddSocketInfo(SOCKET sock)
{
	if (nTotalSockets >= FD_SETSIZE)
	{
		printf_s("[오류] 소켓 정보를 추가할 수 없습니다.\n");
		return FALSE;
	}

	//SOCKETINFO *ptr = new SOCKETINFO;
	CLIENT *ptr = new CLIENT;

	if (ptr == NULL)
	{
		printf_s("[오류] 메모리가 부족합니다!\n");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->iID = IssueID();
	ptr->iPosX = 40;
	ptr->iPosY = 12;

	g_pClient[nTotalSockets++] = ptr;

	return TRUE;
}

// 소켓 정보 삭제
void RemoveSocketInfo(int nIndex)
{
	CLIENT *ptr = g_pClient[nIndex];

	// 클라이언트 정보 얻기
	SOCKADDR_IN clientaddr;

	int addrlen = sizeof(clientaddr);

	getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);

	WCHAR szTemp[16] = { 0, };
	printf_s("[TCP 서버] 클라이언트 종료: IP 주소=%S, 포트 번호=%d\n",
		InetNtop(AF_INET, &clientaddr.sin_addr, szTemp, 16),
		ntohs(clientaddr.sin_port));

	closesocket(ptr->sock);

	delete ptr;

	if (nIndex != (nTotalSockets - 1))
	{
		g_pClient[nIndex] = g_pClient[nTotalSockets - 1];
	}

	--nTotalSockets;
}

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
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

	//printf_s("[%s] %s", msg, (char *)lpMsgBuf);
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