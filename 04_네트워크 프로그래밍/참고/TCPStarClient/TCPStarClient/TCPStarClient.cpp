//----------------------------------------------------------------------
//160708
//�� �����̱� Ŭ���̾�Ʈ
//----------------------------------------------------------------------

//��Ŷ�������� - ��Ŷ�� 16����Ʈ ����
//
//ID�Ҵ�(0)		Type(4Byte) | ID(4Byte) | �Ⱦ�(4Byte) | �Ⱦ�(4Byte)
//�ű�����(1)		Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//��������(2)		Type(4Byte) | ID(4Byte) | �Ⱦ�(4Byte) | �Ⱦ�(4Byte)
//�̵�(3)			Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//
//0 ~2�� ��Ŷ�� ����->Ŭ����Ŷ �̸�, 3�� ��Ŷ��  Ŭ�� <->����  �ֹ� ��Ŷ��.
//
//
//
//Ŭ���̾�Ʈ�� ������ ���� ���ӽ�
//
//- ID�Ҵ�(0) ��Ŷ ����
//- �ű�����(1) ��Ŷ ����(�ڱ� �ڽŲ�)
//- �ű�����(1) ��Ŷ ����(������ ���� ��쿡��)
//
//
//�ڱ� �ڽ��� �̵��� ���� �ڱ� ��ǥ�� ���� �ٲپ��ְ�, ������ �̵� ��Ŷ�� ������.
//
//
//
//������� ��Ʈ : 3000

#pragma comment(lib, "ws2_32")

//< ��� �߰�
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

//< ����ü
//��Ŷ�������� - ��Ŷ�� 16����Ʈ ����
//
//ID�Ҵ�(0)			Type(4Byte) | ID(4Byte) | �Ⱦ�(4Byte) | �Ⱦ�(4Byte)
//�ű�����(1)		Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//��������(2)		Type(4Byte) | ID(4Byte) | �Ⱦ�(4Byte) | �Ⱦ�(4Byte)
//�̵�(3)			Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//0 ~2�� ��Ŷ�� ����->Ŭ����Ŷ �̸�, 3�� ��Ŷ��  Ŭ�� <->����  �ֹ� ��Ŷ��.

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

// ��ũ�� ����
char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// ��Ŷ
st_Packet packet;

// �� �迭
st_Star starArr[dfMAX_STAR];

// �� ����
st_Star myStar;

// ����
SOCKET sock;

// return value
int retval;

//sock fuc
// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg);

// ���� �Լ� ���� ���
void err_display(char *msg);

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags);

// func
// �ʱ�ȭ
void Init(void);

//< ������ ������ ȭ������ ����ִ� �Լ�
void BufferFlip(void);

//< ȭ�� ���۸� �����ִ� �Լ�
void BufferClear(void);

//< ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���
void SpriteDraw(int x, int y, char sprite);

// Draw
void Draw(void);

//< Ű���� �Է�
bool KeyProcess(void);

//< ��Ʈ��ũ ���μ���
void NetworkProcess(void);

//< ��Ŷ �迭 ó��
void PacketProcess(void);



//< Main
void main(void)
{
	cs_Initial();

	// ���� �ʱ�ȭ
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
			// �̵��ߴ��� Ȯ��
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

//< ��Ʈ��ũ ���μ���
void NetworkProcess()
{
	//	FD_SET ����
	FD_SET rset, wset;

	while (1)
	{
		// ���� �� �ʱ�ȭ
		// FD_ZERO	���� ����
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

			// ���� ó��
			PacketProcess();
		}
		else
		{
			break;
		}
	}
}

// �ʱ�ȭ
void Init(void)
{
	myStar.iPosX	= 10;
	myStar.iPosY	= 10;
	myStar.bVisible = TRUE;
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

//< Ű���� �Է�
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

	// esc Ű
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		exit(0);
	}

	// �÷��̾� �̵� �ݰ� ����
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

//< ��ũ�� ���ۿ� �÷��̾� �׸���
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

//< ��Ŷ �迭 ó��
void PacketProcess(void)
{
	/*for (int iCount = 0; iCount < dfMAX_STAR; ++iCount)
	{*/
		switch (packet.iType)
		{
		//< ���̵� �Ҵ��� ó������ ����
		case dfTYPE_ID_ALLOC:
		{
			// myStar ���� ����
			// ID �Ҵ� �ޱ�
			myStar.iID = packet.iID;
			myStar.bVisible = TRUE;
		}
		break;

		case dfTYPE_CONNECT:
		{
			// �� �迭 ���鼭 �� �ڸ��� ����
			for (int iCount = 0; iCount < dfMAX_STAR; iCount++)
			{
				// ���ڸ��� ����
				if (starArr[iCount].bVisible == FALSE)
				{
					starArr[iCount].bVisible = TRUE;
					starArr[iCount].iID = packet.iID;
					starArr[iCount].iPosX = packet.iPosX;
					starArr[iCount].iPosY = packet.iPosY;

					// ���� �� ���̸� �� ������ �ٲٱ�
					if (myStar.iID == packet.iID)
					{
						myStar.iPosX = packet.iPosX;
						myStar.iPosY = packet.iPosY;

						break;
					}

					break;	// �� �ڸ��� ���������� break
				}
			}
		}
		break;
		
		case dfTYPE_DISCONNECT:
		{
			for (int iCount = 0; iCount < dfMAX_STAR; iCount++)
			{
				// �� �迭 ���鼭 ���� ���̵� ���� �����
				if (starArr[iCount].iID == packet.iID)
				{
					starArr[iCount].iID = 0;
					starArr[iCount].iPosX = 0;
					starArr[iCount].iPosY = 0;
					starArr[iCount].bVisible = FALSE;

				// ���� �� ���̸� �� ������ �ٲٱ�
				if (myStar.iID == packet.iID)
				{
					myStar.iPosX = 0;
					myStar.iPosY = 0;
					myStar.iID = 0;
					myStar.bVisible = FALSE;
				}

					break;	// ã������ break
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

					// ���� �ٲ� ���� �� ���̸� �� ������ �ٲٱ�
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
