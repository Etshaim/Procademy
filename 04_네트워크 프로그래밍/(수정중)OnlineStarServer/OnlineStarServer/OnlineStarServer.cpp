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
// ���� ����
//------------------------------------------------

// ��ũ�� ����
char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// ������ �迭��
st_SOCKETINFO	*pSocketArray[FD_SETSIZE];
st_PACKET		g_packet;

int			iSocketCount;
int			retval;
SOCKET		listen_sock;


#pragma region
#pragma endregion

#pragma region Console ����
// ������ ������ ȭ������ ����ִ� �Լ�
void BufferFlip(void);

// ȭ�� ���۸� �����ִ� �Լ�
void BufferClear(void);

// ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���
void SpriteDraw(int x, int y, char sprite);

// Draw
void Draw(void);

#pragma endregion

#pragma region �����Լ�
// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg);

// ���� �Լ� ���� ���
void err_display(char *msg);

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags);

// AddSocketInfo
BOOL AddSocketInfo(SOCKET sock);

// RemoveSocketInfo
void RemoveSocketInfo(int index);

// ��Ʈ��ũ ���μ���
void NetworkProcess(void);

// RecvProc()
void RecvProc(int iIndex);

// SendUnicast
BOOL SendUnicast(int iIndex, st_PACKET packet, int iSize);

// SendBroadcast
BOOL SendBroadcast(st_PACKET packet, int iSize);

// #pragma endregion �����Լ�
#pragma endregion

void Init(void);

// ID �߱�
int IssueID(void);

void main(void)
{
	Init();

	// ���� �ʱ�ȭ
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


//< ������ ������ ȭ������ ����ִ� �Լ�
void BufferFlip(void)
{
	for (int iCount = 0; iCount < dfSCREEN_HEIGHT; ++iCount)
	{
		cs_MoveCursor(0, iCount);
		printf_s(screenBuffer[iCount]);
	}
}

//< ȭ�� ���۸� �����ִ� �Լ�
void BufferClear(void)
{
	for (int iCount = 0; iCount < dfSCREEN_HEIGHT; ++iCount)
	{
		memset(screenBuffer[iCount], ' ', dfSCREEN_WIDTH);
		screenBuffer[iCount][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}

//< ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���
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
	// Connect Client : x ���

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
		//	// continue �� break �߿� ���
		//	break;
		//}
		int iTempPosX = pSocketArray[iCount]->packet.iPosX;
		int iTempPosY = pSocketArray[iCount]->packet.iPosY;
		
		SpriteDraw(iTempPosX, iTempPosY, '*');
		
	}

}
//Console ����
#pragma endregion 

#pragma region �����Լ�
// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

// ����� ���� ������ ���� �Լ�
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
		printf_s("������ �߰��� �� ����\n");
		return FALSE;
	}

	st_SOCKETINFO *pTemp = new st_SOCKETINFO;

	if (NULL == pTemp)
	{
		printf("�޸� ����\n");
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

	// FD_SET ����
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

	// ���� �� �˻� : Ŭ���̾�Ʈ ���� 
	if (retval > 0)
	{
		if (FD_ISSET(listen_sock, &rset))
		{
			addrlen = sizeof(clientaddr);
			client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);

			// ���� �˻�
			if (INVALID_SOCKET == client_sock)
			{
				// WSAEWOULDBLOCK�� �ƴ� ���� ����
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					err_display("accept()");
				}
			}
			else
			{
				WCHAR szBuf[16] = { 0, };

				wprintf_s(L"\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%ws, ��Ʈ��ȣ=%d\n",
					InetNtop( AF_INET, &clientaddr.sin_addr, szBuf, dfPACKET_SIZE ),
					ntohs( clientaddr.sin_port) );

				// ���� ���� �߰�
				if (TRUE == AddSocketInfo(client_sock))
				{
					// ��Ŷ ����
					g_packet.iID	= pSocketArray[iSocketCount - 1]->packet.iID;
					g_packet.iPosX	= pSocketArray[iSocketCount - 1]->packet.iPosX;
					g_packet.iPosY	= pSocketArray[iSocketCount - 1]->packet.iPosY;
					g_packet.iType	= dfTYPE_ID_ALLOC;

					// ���̵� �Ҵ�
					SendUnicast( iSocketCount - 1, g_packet, dfPACKET_SIZE);

					// �ű� Ŭ���̾�Ʈ�� �ٸ� �� ���� ������
					for (int iCount = 0; iCount < iSocketCount; iCount++)
					{
						if (g_packet.iID == pSocketArray[iCount]->packet.iID)
						{
							continue;
						}
						// ��Ŷ ����
						g_packet.iID = pSocketArray[iCount]->packet.iID;
						g_packet.iPosX = pSocketArray[iCount]->packet.iPosX;
						g_packet.iPosY = pSocketArray[iCount]->packet.iPosY;
						g_packet.iType = dfTYPE_CONNECT;

						SendUnicast(iSocketCount - 1, g_packet, dfPACKET_SIZE);
					}

					// �ٸ� Ŭ���̾�Ʈ�� �ű����� ���� ������
					// ��Ŷ ����
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

		// ���� �� �˻� : ������ ���
		for (int iCount = 0; iCount < iSocketCount; iCount++)
		{
			if (FD_ISSET(pSocketArray[iCount]->socket, &rset))
			{
				//RecvProc(iCount);
				// ���߿� ��Ƽ ������ ���� ���� �� �ִ� ��
				if (INVALID_SOCKET != pSocketArray[iCount]->socket)
				{
					// ������ �ޱ�
					retval = recv(pSocketArray[iCount]->socket, (char*)&g_packet, dfPACKET_SIZE, 0);

					//retval = recvn(pSocketArray[iIndex]->socket, (char*)&g_packet, dfPACKET_SIZE, 0);

					if (SOCKET_ERROR == retval)
					{
						// WSAEWOULDBLOCK�� �ƴ� ���� ����
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
						// ID x��ǥ, y��ǥ (braodcast)
						for (int iCount = 0; iCount < iSocketCount; iCount++)
						{
							if (g_packet.iID == pSocketArray[iCount]->packet.iID)
							{
								// ��ǥ �ٲ��ֱ�
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
	// ���߿� ��Ƽ ������ ���� ���� �� �ִ� ��
	if (INVALID_SOCKET != pSocketArray[iIndex]->socket)
	{
		// ������ �ޱ�
		retval = recv(pSocketArray[iIndex]->socket, (char*)&g_packet, dfPACKET_SIZE, 0);

		//retval = recvn(pSocketArray[iIndex]->socket, (char*)&g_packet, dfPACKET_SIZE, 0);

		if (SOCKET_ERROR == retval)
		{
			// WSAEWOULDBLOCK�� �ƴ� ���� ����
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
			// ID x��ǥ, y��ǥ (braodcast)
			for (int iCount = 0; iCount < iSocketCount; iCount++)
			{
				if (g_packet.iID == pSocketArray[iCount]->packet.iID)
				{
					// ��ǥ �ٲ��ֱ�
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
	// ���߿� ��Ƽ ������ ���� ���� �� �ִ� ��
	if (INVALID_SOCKET == pSocketArray[iIndex]->socket)
	{
		err_quit("SendUnicast()");
		return FALSE;
	}

	retval = send(pSocketArray[iIndex]->socket, (char*)&packet, iSize, 0);

	if (SOCKET_ERROR == retval)
	{
		// WSAEWOULDBLOCK�� �ƴ� ���� ����
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

// #pragma endregion �����Լ�
#pragma endregion

// ID �߱�
int IssueID(void)
{
	static int iIDCount = 0;

	iIDCount++;

	return iIDCount;
}