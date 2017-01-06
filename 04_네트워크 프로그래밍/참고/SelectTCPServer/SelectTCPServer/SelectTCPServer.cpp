#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Console.h"

#define SERVERPORT			3000
#define BUFSIZE				512
#define dfST_PACKET_SIZE	16

#define dfTYPE_ID_ALLOC		0
#define	dfTYPE_CONNECT		1
#define	dfTYPE_DISCONNECT	2
#define dfTYPE_MOVE			3

// ���� ���� ������ ���� ����ü�� ����
//struct SOCKETINFO
//{
//	SOCKET sock;
//	char buf[BUFSIZE + 1];
//	int recvbytes;
//	int sendbytes;
//};

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

// ����
//CLIENT g_Client[FD_SETSIZE];

CLIENT *gp_Client[FD_SETSIZE];

// listen_sock
SOCKET listen_sock;

// ��Ŷ
st_Packet packet;

// ��ũ�� ����
char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// return value
int retval;
int nTotalSockets = 0;

// id ����
int arrID[FD_SETSIZE];

//SOCKETINFO *SocketInfoArray[FD_SETSIZE];

// ���� ���� �Լ�
BOOL AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(int nIndex);

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags);

// ���� ��� �Լ�
void err_quit(char *msg);
void err_display(char *msg);

// ID �߱�
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

int main(int argc, char *argv[])
{
	// �ܼ� �ʱ�ȭ
	cs_Initial();

	// ���� �ʱ�ȭ
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
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

		// Sleep()
		//Sleep(50);
	}
	// ���� ����
	WSACleanup();

	return 0;
}

// ID �߱�
int IssueID(void)
{
	srand(	(unsigned)time(NULL)	);

	int randNum = rand() % FD_SETSIZE + 1;

	for (int iCount = 0; iCount < nTotalSockets; iCount++)
	{
		//if (randNum == g_Client[iCount].iID)
		if (randNum == gp_Client[iCount]->iID)
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
	}

	return TRUE;
}

// Broad Cast
BOOL SendBroadcast(st_Packet pack, int iSize)
{
	for (int iCount = 0; iCount < nTotalSockets; iCount++)
	{
		// ���� üũ
		//SendUnicast(g_Client[iCount].sock, iCount, pack, iSize);
		SendUnicast(gp_Client[iCount]->sock, iCount, pack, iSize);
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

	// ������ ��ſ� ����� ����
	FD_SET		rset;
	FD_SET		wset;
	SOCKET		client_sock;
	SOCKADDR_IN clientaddr;

	TIMEVAL tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	int addrlen = 0;

	// ���� �� �ʱ�ȭ
	FD_ZERO(&rset);
	FD_ZERO(&wset);
	FD_SET(listen_sock, &rset);

	/*for (int iCount = 0; iCount < nTotalSockets; iCount++)
	{
		if (SocketInfoArray[iCount]->recvbytes > SocketInfoArray[iCount]->sendbytes)
		{
			FD_SET(SocketInfoArray[iCount]->sock, &wset);
		}
		else
		{
			FD_SET(SocketInfoArray[iCount]->sock, &rset);
		}
	}*/
	for (int iCount = 0; iCount < nTotalSockets; iCount++)
	{
		if (gp_Client[iCount]->sock == INVALID_SOCKET)
		{
			err_display("accept()");
		}
		else
		{
			//FD_SET(g_Client[iCount].sock, &wset);
			//FD_SET(g_Client[iCount].sock, &rset);
			FD_SET(gp_Client[iCount]->sock, &rset);
		}
	}
			
	// selecet()
	retval = select(0, &rset, NULL, NULL, &tv);

	if (retval == SOCKET_ERROR)
	{
		err_quit("select()");
	}

	// ���� �� �˻�(1) : Ŭ���̾�Ʈ ���� ����
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
			TCHAR szBuf[16] = { 0, };

			printf_s("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ��ȣ=%d\n", 
				InetNtop(AF_INET, &clientaddr.sin_addr, (PWSTR)szBuf, 16), 
				ntohs(clientaddr.sin_port));

			// �������� �߰�
			if (	AddSocketInfo(client_sock)	)
			{
				st_Packet temp;
				temp.iID = gp_Client[nTotalSockets - 1]->iID;
				temp.iPosX = gp_Client[nTotalSockets - 1]->iPosX;
				temp.iPosY = gp_Client[nTotalSockets - 1]->iPosY;
				temp.iType = dfTYPE_ID_ALLOC;

				// ���̵� �Ҵ�
				SendUnicast(gp_Client[nTotalSockets - 1]->sock,
					nTotalSockets - 1, temp, dfST_PACKET_SIZE);

				// ��ü���� �˸���
				// �ű�����(1) ��Ŷ �߽�
				temp.iType = dfTYPE_CONNECT;
				SendBroadcast(temp, dfST_PACKET_SIZE);

				// �ٸ� ���� ������
				for (int iCount = 0; iCount < nTotalSockets-1; iCount++)
				{
					st_Packet temp;

					temp.iID	= gp_Client[iCount]->iID;
					temp.iPosX	= gp_Client[iCount]->iPosX;
					temp.iPosY	= gp_Client[iCount]->iPosY;
					temp.iType	= dfTYPE_CONNECT;

					
						SendUnicast(gp_Client[nTotalSockets - 1]->sock, nTotalSockets - 1,
							temp, dfST_PACKET_SIZE);
					
				}
			}
		}
	}

	// ���� �� �˻�(2) : ������ ���
	for (int iCount = 0; iCount < nTotalSockets; iCount++)
	{
		//SOCKETINFO *ptr = SocketInfoArray[iCount];

		CLIENT *ptr = gp_Client[iCount];

		//if (FD_ISSET(ptr->sock, ptr->buf, BUFSIZE, 0))
		if (FD_ISSET(ptr->sock, &rset))
		{
			
			//retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
			if (gp_Client[iCount]->sock != INVALID_SOCKET)
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
					for (int iCount = 0; iCount < nTotalSockets; iCount++)
					{
						if (gp_Client[iCount]->iID == packet.iID)
						{
							gp_Client[iCount]->iPosX = packet.iPosX;
							gp_Client[iCount]->iPosY = packet.iPosY;
						}
					}
					
					SendBroadcast(packet, dfST_PACKET_SIZE);
				}
			}
			//ptr->recvbytes = retval;

			//// ���� ������ ���
			//addrlen = sizeof(clientaddr);

			//getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);
			//ptr->buf[retval] = '\0';

			//char szTemp[16] = { 0, };

			//printf_s("[TCP/%s:%d] %s\n", InetNtop(AF_INET, &clientaddr.sin_addr, (PWSTR)szTemp, 16), ntohs(clientaddr.sin_port), ptr->buf);
		}

		//if (FD_ISSET(ptr->sock, &wset))
		//{
		//	// ������ ������
		//	retval = send(ptr->sock, ptr->buf + ptr->sendbytes,
		//		ptr->recvbytes - ptr->sendbytes, 0);

		//	if (retval == SOCKET_ERROR)
		//	{
		//		err_display("send()");
		//		RemoveSocketInfo(iCount);
		//		continue;
		//	}

		//	ptr->sendbytes += retval;

		//	if (ptr->recvbytes == ptr->sendbytes)
		//	{
		//		ptr->recvbytes = ptr->sendbytes = 0;
		//	}
		//}
	}
	
}

// Draw
void Draw(void)
{
	for (int iCount = 0; iCount < nTotalSockets; ++iCount)
	{
		SpriteDraw(gp_Client[iCount]->iPosX, gp_Client[iCount]->iPosY, '*');
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

// ���� ���� �߰�
BOOL AddSocketInfo(SOCKET sock)
{
	if (nTotalSockets >= FD_SETSIZE)
	{
		printf_s("[����] ���� ������ �߰��� �� �����ϴ�.\n");
		return FALSE;
	}

	//SOCKETINFO *ptr = new SOCKETINFO;
	CLIENT *ptr = new CLIENT;

	if (ptr == NULL)
	{
		printf_s("[����] �޸𸮰� �����մϴ�!\n");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->iID = IssueID();
	ptr->iPosX = 40;
	ptr->iPosY = 12;
	//ptr->sock = sock;
	//ptr->recvbytes = 0;
	//ptr->sendbytes = 0;

	//SocketInfoArray[nTotalSockets++] = ptr;
	gp_Client[nTotalSockets++] = ptr;

	return TRUE;
}

// ���� ���� ����
void RemoveSocketInfo(int nIndex)
{
	//SOCKETINFO *ptr = SocketInfoArray[nIndex];
	CLIENT *ptr = gp_Client[nIndex];

	// Ŭ���̾�Ʈ ���� ���
	SOCKADDR_IN clientaddr;

	int addrlen = sizeof(clientaddr);

	getpeername(ptr->sock, (SOCKADDR *)&clientaddr, &addrlen);

	TCHAR szTemp[16] = { 0, };
	printf_s("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%S, ��Ʈ ��ȣ=%d\n",
		InetNtop(AF_INET, &clientaddr.sin_addr, (LPWSTR)szTemp, 16), 
		ntohs(clientaddr.sin_port));

	closesocket(ptr->sock);

	delete ptr;

	if (	nIndex != (nTotalSockets - 1)	)
	{
		//SocketInfoArray[nIndex] = SocketInfoArray[nTotalSockets - 1];
		gp_Client[nIndex] = gp_Client[nTotalSockets - 1];
	}

	--nTotalSockets;
}

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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