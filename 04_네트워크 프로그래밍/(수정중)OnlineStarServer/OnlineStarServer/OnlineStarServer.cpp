#include "Console.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <time.h>
#include <locale.h>

#pragma comment(lib, "ws2_32")

#define dfTYPE_ID_ALLOC		0
#define dfTYPE_CONNECT		1
#define dfTYPE_DISCONNECT	2
#define dfTYPE_MOVE			3

#define dfPACKET_SIZE		16
#define dfSERVER_PORT		3000
#define dfSERVER_IP			"127.0.0.1"

struct st_PACKET
{
	int		iType;
	int		iID;
	int		iPosX;
	int		iPosY;
};

struct st_SOCKETINFO
{
	SOCKET		socket;
	st_PACKET	packet;
};

//------------------------------------------------
// 전역 변수
//------------------------------------------------

// 스크린 버퍼
char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// 포인터 배열로
st_SOCKETINFO	*pSocketArray[FD_SETSIZE];
st_PACKET		g_packet;

int			iSocketCount;
int			retval;
SOCKET		listen_sock;


#pragma region
#pragma endregion

#pragma region Console 관련
// 버퍼의 내용을 화면으로 찍어주는 함수
void BufferFlip(void);

// 화면 버퍼를 지워주는 함수
void BufferClear(void);

// 버퍼의 특정 위치에 원하는 문자를 출력
void SpriteDraw(int x, int y, char sprite);

// Draw
void Draw(void);

#pragma endregion

#pragma region 소켓함수
// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg);

// 소켓 함수 오류 출력
void err_display(char *msg);

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags);

// AddSocketInfo
BOOL AddSocketInfo(SOCKET sock);

// RemoveSocketInfo
void RemoveSocketInfo(int index);

// 네트워크 프로세스
void NetworkProcess(void);

// RecvProc()
void RecvProc(int iIndex);

// SendUnicast
BOOL SendUnicast(int iIndex, st_PACKET packet, int iSize);

// SendBroadcast
BOOL SendBroadcast(st_PACKET packet, int iSize);

// #pragma endregion 소켓함수
#pragma endregion

void Init(void);

// ID 발급
int IssueID(void);

void main(void)
{
	Init();

	// 윈속 초기화
	WSADATA wsa;

	if ( 0 != WSAStartup( MAKEWORD(2, 2), &wsa ) )
	{
		return;
	}

	// socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (INVALID_SOCKET == listen_sock)
	{
		err_quit("socket()");
	}

	// bind()
	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(dfSERVER_PORT);
	//serveraddr.sin_addr.S_un.S_addr = htons(INADDR_ANY);
	inet_pton(AF_INET, dfSERVER_IP, &serveraddr.sin_addr.S_un.S_addr);

	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (SOCKET_ERROR == retval)
	{
		err_quit("bind()");
	}

	// listen()
	retval = listen(listen_sock, SOMAXCONN);

	if (SOCKET_ERROR == retval)
	{
		err_quit("listen()");
	}

	while (1)
	{
		
		// Network()
		NetworkProcess();

		// Draw()
		BufferClear();
		Draw();
		BufferFlip();

		//Sleep(50);
	}
}

void Init(void)
{
	cs_Initial();

	_wsetlocale(LC_ALL, L"korean");

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

	for (int iCount = 0; iCount < iSocketCount; ++iCount)
	{
		//if (NULL == pSocketArray[iCount])
		//{
		//	// continue 와 break 중에 고민
		//	break;
		//}
		int iTempPosX = pSocketArray[iCount]->packet.iPosX;
		int iTempPosY = pSocketArray[iCount]->packet.iPosY;
		
		SpriteDraw(iTempPosX, iTempPosY, '*');
		
	}

}
//Console 관련
#pragma endregion 

#pragma region 소켓함수
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

// AddSocketInfo
BOOL AddSocketInfo(SOCKET sock)
{
	if (iSocketCount >= FD_SETSIZE - 1)
	{
		printf_s("소켓을 추가할 수 없음\n");
		return FALSE;
	}

	st_SOCKETINFO *pTemp = new st_SOCKETINFO;

	if (NULL == pTemp)
	{
		printf("메모리 부족\n");
		return FALSE;
	}
	pTemp->socket = sock;
	pTemp->packet.iID = IssueID();
	pTemp->packet.iPosX = 25;
	pTemp->packet.iPosY = 10;
	pTemp->packet.iType = dfTYPE_ID_ALLOC;

	pSocketArray[iSocketCount] = pTemp;

	iSocketCount++;

	return TRUE;
}

// RemoveSocketInfo
void RemoveSocketInfo(int index)
{
	st_SOCKETINFO *pTemp = pSocketArray[index];

	closesocket(pTemp->socket);
	pTemp->socket = INVALID_SOCKET;

	g_packet.iID = pTemp->packet.iID;
	g_packet.iType = dfTYPE_DISCONNECT;

	delete pTemp;

	for (int iCount = index; iCount < iSocketCount; iCount++)
	{
		pSocketArray[iCount] = pSocketArray[iCount + 1];
	}

	//pSocketArray[index] = pSocketArray[iSocketCount-1];
	//pSocketArray[iSocketCount - 1] = nullptr;

	iSocketCount--;

	SendBroadcast(g_packet, dfPACKET_SIZE);
}

void NetworkProcess(void)
{

	// FD_SET 세팅
	FD_SET		rset;
	FD_SET		wset;

	SOCKET		client_sock;
	SOCKADDR_IN clientaddr;

	// timeval
	TIMEVAL tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	int addrlen = 0;

	FD_ZERO(&rset);
	FD_ZERO(&wset);

	FD_SET(listen_sock, &rset);

	for (int iCount = 0; iCount < iSocketCount; iCount++)
	{
		if (INVALID_SOCKET == pSocketArray[iCount]->socket)
		{
			err_display("accept()");
		}
		else
		{
			FD_SET(pSocketArray[iCount]->socket, &rset);
		}
	}

	// select()
	retval = select(0, &rset, NULL, NULL, &tv);

	if (SOCKET_ERROR == retval)
	{
		err_quit("select()");
	}

	// 소켓 셋 검사 : 클라이언트 접속 
	if (retval > 0)
	{
		if (FD_ISSET(listen_sock, &rset))
		{
			addrlen = sizeof(clientaddr);
			client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);

			// 에러 검사
			if (INVALID_SOCKET == client_sock)
			{
				// WSAEWOULDBLOCK이 아닐 때만 오류
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					err_display("accept()");
				}
			}
			else
			{
				WCHAR szBuf[16] = { 0, };

				wprintf_s(L"\n[TCP 서버] 클라이언트 접속: IP 주소=%ws, 포트번호=%d\n",
					InetNtop( AF_INET, &clientaddr.sin_addr, szBuf, dfPACKET_SIZE ),
					ntohs( clientaddr.sin_port) );

				// 소켓 정보 추가
				if (TRUE == AddSocketInfo(client_sock))
				{
					// 패킷 세팅
					g_packet.iID	= pSocketArray[iSocketCount - 1]->packet.iID;
					g_packet.iPosX	= pSocketArray[iSocketCount - 1]->packet.iPosX;
					g_packet.iPosY	= pSocketArray[iSocketCount - 1]->packet.iPosY;
					g_packet.iType	= dfTYPE_ID_ALLOC;

					// 아이디 할당
					SendUnicast( iSocketCount - 1, g_packet, dfPACKET_SIZE);

					// 신규 클라이언트에 다른 별 정보 보내기
					for (int iCount = 0; iCount < iSocketCount; iCount++)
					{
						if (g_packet.iID == pSocketArray[iCount]->packet.iID)
						{
							continue;
						}
						// 패킷 세팅
						g_packet.iID = pSocketArray[iCount]->packet.iID;
						g_packet.iPosX = pSocketArray[iCount]->packet.iPosX;
						g_packet.iPosY = pSocketArray[iCount]->packet.iPosY;
						g_packet.iType = dfTYPE_CONNECT;

						SendUnicast(iSocketCount - 1, g_packet, dfPACKET_SIZE);
					}

					// 다른 클라이언트에 신규접속 정보 보내기
					// 패킷 세팅
					g_packet.iID = pSocketArray[iSocketCount - 1]->packet.iID;
					g_packet.iPosX = pSocketArray[iSocketCount - 1]->packet.iPosX;
					g_packet.iPosY = pSocketArray[iSocketCount - 1]->packet.iPosY;
					g_packet.iType = dfTYPE_CONNECT;


					SendBroadcast(g_packet, dfPACKET_SIZE);

				}
				else
				{
					closesocket(client_sock);
				}
			}
		}

		// 소켓 셋 검사 : 데이터 통신
		for (int iCount = 0; iCount < iSocketCount; iCount++)
		{
			if (FD_ISSET(pSocketArray[iCount]->socket, &rset))
			{
				//RecvProc(iCount);
				// 나중에 멀티 스레드 가면 있을 수 있는 일
				if (INVALID_SOCKET != pSocketArray[iCount]->socket)
				{
					// 데이터 받기
					retval = recv(pSocketArray[iCount]->socket, (char*)&g_packet, dfPACKET_SIZE, 0);

					//retval = recvn(pSocketArray[iIndex]->socket, (char*)&g_packet, dfPACKET_SIZE, 0);

					if (SOCKET_ERROR == retval)
					{
						// WSAEWOULDBLOCK이 아닐 때만 오류
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							err_display("recv()");
							RemoveSocketInfo(iCount);
						}
						continue;
					}
					else if (0 == retval)
					{
						RemoveSocketInfo(iCount);
						continue;
					}

					switch (g_packet.iType)
					{
					case dfTYPE_MOVE:
					{
						// ID x좌표, y좌표 (braodcast)
						for (int iCount = 0; iCount < iSocketCount; iCount++)
						{
							if (g_packet.iID == pSocketArray[iCount]->packet.iID)
							{
								// 좌표 바꿔주기
								pSocketArray[iCount]->packet.iPosX = g_packet.iPosX;
								pSocketArray[iCount]->packet.iPosY = g_packet.iPosY;

								g_packet.iType = dfTYPE_MOVE;

								SendBroadcast(g_packet, dfPACKET_SIZE);

								break;
							}
						}
					}
					break;

					}
				}
			}
		}
	}
}

// RecvProc()
void RecvProc(int iIndex)
{
	// 나중에 멀티 스레드 가면 있을 수 있는 일
	if (INVALID_SOCKET != pSocketArray[iIndex]->socket)
	{
		// 데이터 받기
		retval = recv(pSocketArray[iIndex]->socket, (char*)&g_packet, dfPACKET_SIZE, 0);

		//retval = recvn(pSocketArray[iIndex]->socket, (char*)&g_packet, dfPACKET_SIZE, 0);

		if (SOCKET_ERROR == retval)
		{
			// WSAEWOULDBLOCK이 아닐 때만 오류
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				err_display("recv()");
				RemoveSocketInfo(iIndex);
			}
			return;
		}
		else if (0 == retval)
		{
			RemoveSocketInfo(iIndex);
			return;
		}

		switch (g_packet.iType)
		{
		case dfTYPE_MOVE:
		{
			// ID x좌표, y좌표 (braodcast)
			for (int iCount = 0; iCount < iSocketCount; iCount++)
			{
				if (g_packet.iID == pSocketArray[iCount]->packet.iID)
				{
					// 좌표 바꿔주기
					pSocketArray[iCount]->packet.iPosX = g_packet.iPosX;
					pSocketArray[iCount]->packet.iPosY = g_packet.iPosY;

					g_packet.iType = dfTYPE_MOVE;

					SendBroadcast(g_packet, dfPACKET_SIZE);

					break;
				}
			}
		}
		break;

		}
	}
}

// SendUnicast
BOOL SendUnicast(int iIndex, st_PACKET packet, int iSize)
{
	// 나중에 멀티 스레드 가면 있을 수 있는 일
	if (INVALID_SOCKET == pSocketArray[iIndex]->socket)
	{
		err_quit("SendUnicast()");
		return FALSE;
	}

	retval = send(pSocketArray[iIndex]->socket, (char*)&packet, iSize, 0);

	if (SOCKET_ERROR == retval)
	{
		// WSAEWOULDBLOCK이 아닐 때만 오류
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			err_display("send()");
			RemoveSocketInfo(iIndex);

			return FALSE;
		}
	}

	return TRUE;
}

// SendBroadcast
BOOL SendBroadcast(st_PACKET packet, int iSize)
{
	for (int iCount = 0; iCount < iSocketCount; iCount++)
	{
		if (FALSE == SendUnicast(iCount, packet, iSize))
		{
			return FALSE;
		}
		
	}
	return TRUE;
}

// #pragma endregion 소켓함수
#pragma endregion

// ID 발급
int IssueID(void)
{
	static int iIDCount = 0;

	iIDCount++;

	return iIDCount;
}