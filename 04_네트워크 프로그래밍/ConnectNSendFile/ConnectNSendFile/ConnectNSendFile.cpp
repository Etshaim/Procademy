//---------------------------------------------------------------
//1. ��������
//
//2. Ư�� ���Ͽ���
//
//3. ������üũ
//
//4. �������
//
//5. ������ ��� ����
//
//6. ������ ���� ������ ���� 1000���� �ɰ��� ������
//
//7. �������� ������ �������� Ŭ���̾�Ʈ�� 0xdddddddd �� ������.
//--------------------------------------------------------------


#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SERVERIP	"127.0.0.1"
#define SERVERPORT	9000
#define	BUFSIZE		512
#define DOMAIN		"procademyserver.iptime.org"

// ���
struct st_PACKET_HEADER
{
	DWORD	dwPacketType;      // 0x11223344    �츮�� ��ŶȮ�� ������
	WCHAR	szName[32];        // �����̸�, �����ڵ� NULL ���� ��
	WCHAR	szFileName[128];   // �����̸�, �����ڵ� NULL ���� ��
	int		iFileSize;
};

// ���� ����
st_PACKET_HEADER packetHeader;	// ��Ŷ���
BYTE* pBuf;	// ���� ������


// ��Ŷ��� ����
void SetPacketHeader(void);

// �̹��� �ҷ�����
BOOL LoadFile(WCHAR *szFileName);

// ������ -> IPv4 �ּ�
//BOOL GetIPAddr(char* name, IN_ADDR *addr);

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

	printf_s("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

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

int main(int argc, char *argv[])
{
	int retval;

	// ��� �ʱ�ȭ
	SetPacketHeader();


	// ������ �ּ� -> IPv4�ּ�
	//IN_ADDR addr;
	//
	//if (GetIPAddr(DOMAIN, &addr))
	//{
	//	WCHAR szIP[16] = { 0, };
	//	printf_s("IP �ּ�(��ȯ ��) = %s\n", inet_ntoa(addr));
	//	//printf_s("IP �ּ�(��ȯ ��) = %s\n", InetNtop(AF_INET, &addr, szIP, 16));
	//}

	// ���� �ʱ�ȭ
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
	{
		err_quit("socket()");
	}

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	//serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	InetPton(AF_INET, L"106.245.38.107", &serveraddr.sin_addr);
	serveraddr.sin_port = htons(19010);

	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
	{
		err_quit("connect()");
	}

	// ���� �ҷ�����
	HANDLE	hFile;
	DWORD	dwRead;


	hFile = CreateFile(L"�ݰ�����.bmp", GENERIC_READ, NULL, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return FALSE;
	}

	packetHeader.iFileSize = GetFileSize(hFile, NULL);





	// ������ ��ſ� ����� ����
	char buf[BUFSIZE + 1];
	//int len;

	// ������ ������ ���

	retval = send(sock, (char*)&packetHeader, sizeof(st_PACKET_HEADER), 0);

	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return 0;
	}

	printf_s("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

	while (1)
	{
		char byTemp[1000] = { 0, };

		ReadFile(hFile, byTemp, 1000, &dwRead, NULL);

		retval = send(sock, byTemp, dwRead, 0);

		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		printf_s("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);

		if (dwRead != 1000)
		{
			break;
		}
	}

	// ������ �Է�
	//printf_s("\n[���� ������] ");

	//if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
	//{
	//	break;
	//}

	//// '\n' ���� ����
	//len = strlen(buf);

	//if (buf[len - 1] == '\n')
	//{
	//	buf[len - 1] = '\0';
	//}

	//if (strlen(buf) == 0)
	//{
	//	break;
	//}

	// ������ ������
	//retval = send(sock, byTemp, strlen(buf), 0);

	while (1)
	{
		DWORD iResult = 0;

		// ������ �ޱ�
		retval = recv(sock, (char*)&iResult, retval, 0);

		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		// ���� ������ ���
		buf[retval] = '\0';

		printf_s("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		//printf_s("[���� ������] %s\n", buf);
		printf_s("[���� ������] %x\n", iResult);
	}

	CloseHandle(hFile);

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();

	return 0;
}

// ��Ŷ��� ����
void SetPacketHeader(void)
{
	packetHeader.dwPacketType = 0x11223344;
	packetHeader.iFileSize = 0;
	wcscpy_s(packetHeader.szName, 32, L"��α�");
	wcscpy_s(packetHeader.szFileName, 128, L"�ݰ�����");
}

// ���� �ҷ�����
BOOL LoadFile(WCHAR *szFileName)
{
	HANDLE	hFile;
	DWORD	dwRead;


	hFile = CreateFile((TCHAR*)szFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return FALSE;
	}

	packetHeader.iFileSize = GetFileSize(hFile, NULL);

	pBuf = new BYTE[packetHeader.iFileSize];

	ReadFile(hFile, pBuf, packetHeader.iFileSize, &dwRead, NULL);

	CloseHandle(hFile);

	return TRUE;
}

//// ������ �̸� -> IPv4 �ּ�
//BOOL GetIPAddr(char* name, IN_ADDR *addr)
//{
//	HOSTENT	*ptr = gethostbyname(name);
//	if (ptr == NULL)
//	{
//		err_display("gethostbyname()");
//		return FALSE;
//	}
//
//	if (ptr->h_addrtype != AF_INET)
//	{
//		return FALSE;
//	}
//
//	memcpy(addr, ptr->h_addr, ptr->h_length);
//
//	return TRUE;
//}