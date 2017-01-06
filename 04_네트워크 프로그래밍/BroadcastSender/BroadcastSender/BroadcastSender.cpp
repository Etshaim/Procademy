//# ��ε� ĳ������ ���ؼ� ���� ��Ʈ�� ���� ��ٸ��� ���ӹ� �˻�
//
//�������� - 0xff 0xee 0xdd 0xaa 0x00 0x99 0x77 0x55 0x33 0x11   (10 Byte)
//
//��Ʈ���� - 19001 ~19099
//
/////////////////////////////////////////////////////////////
//Ư�� ��Ʈ�� ���� �� �޽����� ��ε�ĳ�������� �Ѹ���.UDP
//�ش� ��Ʈ�� ������ �����ִٸ� ����(���� �� �̸�) ���ڿ��� �ǵ��� �´�.
//���� ��Ʈ��ũ ���ο� �ش� ��Ʈ�� ������ ���ٸ� ������ ���� ����.
//
//���� 1. ��Ʈ���� 19001 ~19099 ������ ��� �˻��ؾ� �Ѵ�.
//
//���� 2. ������ ��������� 200�и������带 ����Ѵ�. (����ũ�μ�����, �и������� ���� Ȯ��)
//
//���� 3. ������ �� �̸�, IP, Port ��ȣ�� ��� �Ѵ�.
//
//���� 4. ���� �� 10�� ��������. 10���� ��� ã�Ƴ��� ����
//
//
//= ��ε�ĳ������ ���� ��Ʈ��ũ ���ο��� ������ �ǹǷ� �������� �׽�Ʈ �Ұ�
//
/////////////////////////////////////////////////////////////
//# �� ���ִ� TIP
//1. UDP ���� ����
//2. ��ε�ĳ���� Ȱ��ȭ(���Ͽɼ� : å����, �ɼ����� : MSDN ����)
//3. ��ε�ĳ���� �� IP �� INADDR_BROADCAST
//4. �������ݷ� ������ UDP ���
//5. 200�и������� ���� ������ ���� Ȯ��
//6. ������ ���� �ش� �۽��� Ȯ��, ������ Ȯ�� �� IP, Port, ���̸� ���
//
//�� ������� ������ ��Ʈ ������ ��� ã�ƺ�.

#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

#define dfREMOTE_IP		"255.255.255.255"
#define	dfREMOTE_PORT	19001
#define	dfBUFSIZE		32

// ���� ����
int		retval;
SOCKET	sock;

// ������ ��ſ� ����� ����
WCHAR buf[dfBUFSIZE + 1];
int len;

//sock fuc
// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg);

// ���� �Լ� ���� ���
void err_display(char *msg);

void main(void)
{
	BYTE szTemp[10];

	szTemp[0] = 0xff;
	szTemp[1] = 0xee;
	szTemp[2] = 0xdd;
	szTemp[3] = 0xaa;
	szTemp[4] = 0x00;
	szTemp[5] = 0x99;
	szTemp[6] = 0x77;
	szTemp[7] = 0x55;
	szTemp[8] = 0x33;
	szTemp[9] = 0x11;

	// ���� �ʱ�ȭ
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}

	// 10 14 40 44 55 67 70 80 88 99
	

	// ��ε�ĳ��Ʈ ������ ������
	for (int iCount = dfREMOTE_PORT; iCount < dfREMOTE_PORT+99; iCount++)
	{
		// socket()
		sock = socket(AF_INET, SOCK_DGRAM, 0);

		if (sock == INVALID_SOCKET)
		{
			err_quit("socket()");
		}

		// ��ε� ĳ���� Ȱ��ȭ
		BOOL bEnable = TRUE;

		retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST,
			(char*)&bEnable, sizeof(bEnable));

		if (retval == SOCKET_ERROR)
		{
			err_quit("setsockopt()");
		}

		// ���� �ּ� ����ü �ʱ�ȭ
		SOCKADDR_IN remoteaddr;

		ZeroMemory(&remoteaddr, sizeof(remoteaddr));

		remoteaddr.sin_family = AF_INET;
		InetPton(AF_INET, L"255.255.255.255"/*(PCWSTR)SERVERIP*/, &remoteaddr.sin_addr);
		//remoteaddr.sin_addr.s_addr = inet_addr(dfREMOTE_IP);
		//remoteaddr.sin_port = htons(dfREMOTE_PORT);

		remoteaddr.sin_port = htons(iCount);

		retval = sendto(sock, (char*)(/*(WCHAR)*/szTemp), 10, 0,
			(SOCKADDR*)&remoteaddr, sizeof(remoteaddr));

		if (retval == SOCKET_ERROR)
		{
			err_display("sendto()");
			continue;
		}

		//	FD_SET ����
		FD_SET rset, wset;

		// ���� �� �ʱ�ȭ
		// FD_ZERO	���� ����
		FD_ZERO(&rset);
		FD_ZERO(&wset);
		FD_SET(sock, &rset);

		TIMEVAL tv;
		tv.tv_sec	= 0;
		tv.tv_usec	= 200000;

		// ������ ��ſ� ����� ����
		SOCKADDR_IN peeraddr;
		int addlen = sizeof(peeraddr);

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
				//retval = recv(sock, (char*)buf, dfBUFSIZE - 1, 0);
				//wprintf_s(L"[IP %s] [PORT %d] %s\n", ntohs(remoteaddr.sin_port), buf);
				
				memset(buf, 0, dfBUFSIZE);

				retval = recvfrom(sock, (char*)buf, dfBUFSIZE, 0, 
					(SOCKADDR*)&peeraddr, &addlen);

				WCHAR szIP[16] = { 0, };
				wprintf_s(L"[IP %s] [PORT %d] %s\n", 
					InetNtop(AF_INET, &peeraddr.sin_addr, szIP, 16), 
					ntohs(peeraddr.sin_port), buf);
			}

			if (retval == SOCKET_ERROR)
			{
				err_display("recv()");
				exit(0);
			}
		}

		// closesocket()
		closesocket(sock);
		
	}
	//�������� - 0xff 0xee 0xdd 0xaa 0x00 0x99 0x77 0x55 0x33 0x11   ( 10 Byte )
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