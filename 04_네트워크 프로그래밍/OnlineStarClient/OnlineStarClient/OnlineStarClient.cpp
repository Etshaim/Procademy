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

//< ����ü
//��Ŷ�������� - ��Ŷ�� 16����Ʈ ����
//
//ID�Ҵ�(0)			Type(4Byte) | ID(4Byte) | �Ⱦ�(4Byte) | �Ⱦ�(4Byte)
//�ű�����(1)		Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//��������(2)		Type(4Byte) | ID(4Byte) | �Ⱦ�(4Byte) | �Ⱦ�(4Byte)
//�̵�(3)			Type(4Byte) | ID(4Byte) | X(4Byte) | Y(4Byte)
//0 ~2�� ��Ŷ�� ����->Ŭ����Ŷ �̸�, 3�� ��Ŷ��  Ŭ�� <->����  �ֹ� ��Ŷ��.

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

// ��ũ�� ����
char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// ��Ŷ
st_PACKET packet;

// �� �迭
st_STAR starArr[dfMAX_STAR];

// �� ��
//st_STAR starArr[0];

// ����
SOCKET sock;

// retval
int retval;

//< ���� �Լ�
// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg);

// ���� �Լ� ���� ���
void err_display(char *msg);

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags);

// �ʱ�ȭ
void Init(void);

//< ������ ������ ȭ������ ����ִ� �Լ�
void BufferFlip(void);

//< ȭ�� ���۸� �����ִ� �Լ�
void BufferClear(void);

//< ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���
void SpriteDraw(int x, int y, char sprite);

//< Draw ����
// Draw
void Draw(void);

//< Ű���� �Է�
bool KeyProcess(void);

//< ��Ʈ��ũ ���μ���
void NetworkProcess(void);

//< ��Ŷ �迭 ó��
void PacketProcess(void);

void main(void)
{
	
	Init();
	// ���� �ʱ�ȭ
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
			// �̵��ߴ��� Ȯ��
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


// �ʱ�ȭ
void Init(void)
{
	cs_Initial();


	starArr[0].bVisible = FALSE;
	starArr[0].iID = 0;
	starArr[0].iPosX = 25;
	starArr[0].iPosY = 25;

	ZeroMemory(&packet, sizeof(packet));
}


//< ��Ʈ��ũ ���μ���
void NetworkProcess()
{
	//	FD_SET ����
	FD_SET rset;
	FD_SET wset;

	while (1)
	{
		// ���� �� �ʱ�ȭ
		// FD_ZERO	���� ����
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(sock, &rset);
		
		// timeval
		// NULL : ��� �� ������ ������ ������ ������ ������ ��ٸ�
		TIMEVAL tv;
		tv.tv_sec = 0;
		tv.tv_usec = 0;

		// select()
		// ���ϰ� : ������ �����ϴ� ���� ����, 0(Ÿ�Ӿƿ�), SOCKET_ERROR(����)
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

			// ���� ó��
			PacketProcess();
		}
		else
		{
			break;
		}
	}
}

//< ��Ŷ �迭 ó��
void PacketProcess(void)
{
	switch (packet.iType)
	{
		// ID �Ҵ� (0)
	case dfTYPE_ID_ALLOC:
	{
		starArr[0].bVisible = TRUE;
		starArr[0].iID = packet.iID;
		// ID�Ҵ� �� x,y ��ǥ �� �����ֹǷ� �ּ� ó��
		//starArr[0].iPosX = packet.iPosX;
		//starArr[0].iPosY = packet.iPosY;
	}
	break;

	// �ű� ���� (1)
	case dfTYPE_CONNECT:
	{
		// �� �迭�� �߰�
		for (int iCount = 0; iCount < dfMAX_STAR; iCount++)
		{
			// visible �� FALSE�� �� �������� ����
			if (FALSE == starArr[iCount].bVisible)
			{
				// �߰�
				starArr[iCount].bVisible = TRUE;
				starArr[iCount].iID = packet.iID;
				starArr[iCount].iPosX = packet.iPosX;
				starArr[iCount].iPosY = packet.iPosY;

				// �� �� ������ �ٲٱ�
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

	// ���� ���� (2)
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

		// �̵� (3)
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

//< Ű���� �Է�
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

	// esc Ű
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		exit(0);
	}

	// �÷��̾� �̵� �ݰ� ����
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

//< ������ ������ ȭ������ ����ִ� �Լ�
void BufferFlip(void)
{
	// Connect Client

	

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
	
	for (int iCount = 0; iCount < dfMAX_STAR; ++iCount)
	{
		//// �� ���� ���� ���� ����
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