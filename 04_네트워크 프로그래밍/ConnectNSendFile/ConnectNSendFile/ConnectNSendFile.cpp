//---------------------------------------------------------------
//1. 서버접속
//
//2. 특정 파일오픈
//
//3. 사이즈체크
//
//4. 헤더생성
//
//5. 서버로 헤더 전송
//
//6. 서버로 파일 데이터 전송 1000단위 쪼개서 보내기
//
//7. 파일전송 성공시 서버에서 클라이언트로 0xdddddddd 를 보내줌.
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

// 헤더
struct st_PACKET_HEADER
{
	DWORD	dwPacketType;      // 0x11223344    우리의 패킷확인 고정값
	WCHAR	szName[32];        // 본인이름, 유니코드 NULL 문자 끝
	WCHAR	szFileName[128];   // 파일이름, 유니코드 NULL 문자 끝
	int		iFileSize;
};

// 전역 변수
st_PACKET_HEADER packetHeader;	// 패킷헤더
BYTE* pBuf;	// 버퍼 포인터


// 패킷헤더 세팅
void SetPacketHeader(void);

// 이미지 불러오기
BOOL LoadFile(WCHAR *szFileName);

// 도메인 -> IPv4 주소
//BOOL GetIPAddr(char* name, IN_ADDR *addr);

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

	printf_s("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
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

	// 헤더 초기화
	SetPacketHeader();


	// 도메인 주소 -> IPv4주소
	//IN_ADDR addr;
	//
	//if (GetIPAddr(DOMAIN, &addr))
	//{
	//	WCHAR szIP[16] = { 0, };
	//	printf_s("IP 주소(변환 후) = %s\n", inet_ntoa(addr));
	//	//printf_s("IP 주소(변환 후) = %s\n", InetNtop(AF_INET, &addr, szIP, 16));
	//}

	// 윈속 초기화
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

	// 파일 불러오기
	HANDLE	hFile;
	DWORD	dwRead;


	hFile = CreateFile(L"반격유희.bmp", GENERIC_READ, NULL, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return FALSE;
	}

	packetHeader.iFileSize = GetFileSize(hFile, NULL);





	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE + 1];
	//int len;

	// 서버와 데이터 통신

	retval = send(sock, (char*)&packetHeader, sizeof(st_PACKET_HEADER), 0);

	if (retval == SOCKET_ERROR)
	{
		err_display("send()");
		return 0;
	}

	printf_s("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

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

		printf_s("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		if (dwRead != 1000)
		{
			break;
		}
	}

	// 데이터 입력
	//printf_s("\n[보낼 데이터] ");

	//if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
	//{
	//	break;
	//}

	//// '\n' 문자 제거
	//len = strlen(buf);

	//if (buf[len - 1] == '\n')
	//{
	//	buf[len - 1] = '\0';
	//}

	//if (strlen(buf) == 0)
	//{
	//	break;
	//}

	// 데이터 보내기
	//retval = send(sock, byTemp, strlen(buf), 0);

	while (1)
	{
		DWORD iResult = 0;

		// 데이터 받기
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

		// 받은 데이터 출력
		buf[retval] = '\0';

		printf_s("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		//printf_s("[받은 데이터] %s\n", buf);
		printf_s("[받은 데이터] %x\n", iResult);
	}

	CloseHandle(hFile);

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();

	return 0;
}

// 패킷헤더 세팅
void SetPacketHeader(void)
{
	packetHeader.dwPacketType = 0x11223344;
	packetHeader.iFileSize = 0;
	wcscpy_s(packetHeader.szName, 32, L"김민규");
	wcscpy_s(packetHeader.szFileName, 128, L"반격유희");
}

// 파일 불러오기
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

//// 도메인 이름 -> IPv4 주소
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