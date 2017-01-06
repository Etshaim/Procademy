// AsyncSelectNetDrawClient.cpp : ���� ���α׷��� ���� �������� �����մϴ�.

#include "stdafx.h"
#include "AsyncSelectNetDrawClient.h"
#include  "StreamSQ.h"

#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>


#pragma comment (lib, "ws2_32")

#define	dfPORT			25000
#define dfIP			"127.0.0.1"
#define WM_SOCKET		(WM_USER + 1)
#define dfPACKET_SIZE	18


#pragma pack(push, 1)
struct st_DRAW_PACKET
{
	u_short	Len;
	int		iStartX;
	int		iStartY;
	int		iEndX;
	int		iEndY;
};
#pragma pack(pop)

struct st_SESSION
{
	SOCKET		sock;
	CStreamSQ	*recvQ;
	CStreamSQ	*sendQ;
	BOOL		sendFlag;
};

st_SESSION session;

HINSTANCE hInst;
//HWND	g_hWnd;

int		iSendVal;
int		iRecvVal;

//SOCKET	sock;
int		retval;
BOOL	bConnected;
WCHAR	g_szIP[16];



BOOL	CALLBACK	DialogProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


// ���� ���� ������ �޽��� ó��
void ProcessSocketMessage(HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam);

void SendPacket(st_DRAW_PACKET *packet);
void SendProc(void);

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg);

// wchar ����
void err_quit(WCHAR *msg);

// ���� �Լ� ���� ���
void err_display(char *msg);



void err_display(int errcode);

// ������ ������
void SetWindowSize(HWND hWnd);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	// ���� �ʱ�ȭ
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return -1;
	}

	MSG msg;

	WNDCLASSEX wcex;
	HWND hWnd;

	session.recvQ = new CStreamSQ(19);
	session.sendQ = new CStreamSQ(19);

	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASYNCSELECTNETDRAWCLIENT));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_ASYNCSELECTNETDRAWCLIENT);
	wcex.lpszClassName = L"AsyncSelectNetDrawClient";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);

	DialogBox(hInstance, MAKEINTRESOURCE(IDD_ADDR), NULL, (DLGPROC)&DialogProc);

	hWnd = CreateWindow(L"AsyncSelectNetDrawClient", NULL, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);

	UpdateWindow(hWnd);

	SetWindowSize(hWnd);

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}

	// ���� ����
	WSACleanup();

	return (int) msg.wParam;
}

BOOL	CALLBACK	DialogProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hEditBox;

	switch (iMsg)
	{
		// ���̾�α� ������ �߻�(�ʱ�ȭ��)
	case WM_INITDIALOG:
		memset(g_szIP, 0, sizeof(WCHAR) * 16);
		hEditBox = GetDlgItem(hWnd, IDC_EDIT);
		SetWindowText(hEditBox, L"127.0.0.1");
		return TRUE;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			GetDlgItemText(hWnd, IDC_EDIT, g_szIP, 16);
			EndDialog(hWnd, 99939);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	static BOOL bDraw = FALSE;
	static int iCurX;
	static int iCurY;
	static int iOldX;
	static int iOldY;

	switch (message)
	{
	case WM_LBUTTONDOWN:
		bDraw = TRUE;
		iCurX = LOWORD(lParam);
		iCurY = HIWORD(lParam);
		break;

	case WM_LBUTTONUP:
		bDraw = FALSE;
		break;

	case WM_MOUSEMOVE:
		if (TRUE == bConnected / TRUE == bDraw)
		{
			st_DRAW_PACKET pack;
			ZeroMemory(&pack, 0);

			iOldX = iCurX;
			iOldY = iCurY;
			iCurX = LOWORD(lParam);
			iCurY = HIWORD(lParam);
			
			pack.Len		= 16;
			pack.iStartX	= iOldX;
			pack.iStartY	= iOldY;
			pack.iEndX		= iCurX;
			pack.iEndY		= iCurY;

			SendPacket(&pack);
		}
		break;

	case WM_SOCKET:
		ProcessSocketMessage(hWnd, message, wParam, lParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		EndPaint(hWnd, &ps);
		break;

	case WM_CREATE:

		//AllocConsole();

		session.sock = socket(AF_INET, SOCK_STREAM, 0);

		if (INVALID_SOCKET == session.sock)
		{
			err_quit(L"socket()");
		}

		// WSAAsyncSelect()
		retval = WSAAsyncSelect(session.sock, hWnd, WM_SOCKET,
			FD_CONNECT);
		//FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE);

		//retval = WSAAsyncSelect(sock, hWnd, WM_SOCKET, FD_CONNECT);

		if (SOCKET_ERROR == retval)
		{
			err_quit("WSAAsyncSelect()");
		}

		// ������ ��ſ� ����� ����
		SOCKADDR_IN serveraddr;

		serveraddr.sin_family = AF_INET;
		serveraddr.sin_port = htons(dfPORT);
		//inet_pton(AF_INET, dfIP, &serveraddr.sin_addr.S_un.S_addr);
		InetPton(AF_INET, g_szIP, &serveraddr.sin_addr);
		//serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

		retval = connect(session.sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

		if (SOCKET_ERROR == retval)
		{
			err_quit(L"connect()");
			closesocket(session.sock);
		}
		break;

	case WM_DESTROY:
		//FreeConsole();

		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ���� ������ �޽��� ó��

void ProcessSocketMessage(HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	// ���� �߻� ���� Ȯ��
	if ( 0 != WSAGETSELECTERROR(lParam))
	{
		err_display( WSAGETSELECTERROR(lParam) );
		closesocket(session.sock);
		return;
	}

	// �޽��� ó��
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
	{
		st_DRAW_PACKET buf;
		ZeroMemory(&buf, sizeof(buf));
		
		int iNotBrokenPutSize	= session.recvQ->GetNotBrokenPutSize();
		int iUseSize			= session.recvQ->GetUseSize();

		// recv���� �������NotBrokenPutSize���� ũ�� �� ��

		// 18�� ó���� ���̹Ƿ� recvQ�� �߸� �����Ͱ� �ִٸ� 18�� �ǰԲ� ������ �κи� �޴´�
		int iRecvSize = dfPACKET_SIZE - iUseSize;

		// recv�� �ѹ��� �ִ� NotBrokenPutSize��ŭ ���� ����
		iRecvSize = min(iRecvSize, iNotBrokenPutSize);

		
		retval = recv(session.sock, session.recvQ->GetWriteBufferPtr(), iRecvSize, 0);

		iRecvVal += retval;

		// recvQ�� writepos ��ġ �̵�
		session.recvQ->MoveWritePos(retval);
		
		if (SOCKET_ERROR == retval)
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				break;
			}

			else
			{
				err_quit("recv()");
				closesocket(session.sock);
				return;
			}
		}

		if (0 == retval || iRecvSize != retval)
		{
			err_display("recv()");
			closesocket(session.sock);
			return;
		}

		if (dfPACKET_SIZE > session.recvQ->GetUseSize())
		{
			break;
		}

		if (dfPACKET_SIZE != session.recvQ->Get((char*)&buf, dfPACKET_SIZE))
		{
			err_display("Get()");
			return;
		}

		//session.recvQ.RemoveData(dfPACKET_SIZE);

		// �׸��� ó��
		HDC hdc = GetDC(hWnd);

		/*if (buf.Len != 16)
		{
			err_display("Get()");
		}
		
		if (buf.iStartX > 2000 || buf.iStartY > 2000 || buf.iEndX > 2000 || buf.iEndY > 2000)
		{
			err_display("Get()");
		}
*/
		

		MoveToEx(hdc, buf.iStartX, buf.iStartY, NULL);
		LineTo(hdc, buf.iEndX, buf.iEndY);

		//InvalidateRect(hWnd, NULL, NULL);

		ReleaseDC(hWnd, hdc);

		
	}
	break;

	case FD_WRITE:
	{
		session.sendFlag = TRUE;

		SendProc();
	}

	break;

	case FD_CONNECT:
	{
		bConnected = TRUE;
		retval = WSAAsyncSelect(session.sock, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);

		if (SOCKET_ERROR == retval)
		{
			err_display("WSAAsyncSelect()");
			closesocket(session.sock);
		}
	}
	break;

	case FD_CLOSE:
	{
		closesocket(session.sock);
	}
	break;

	}
}



void SendPacket(st_DRAW_PACKET *packet)
{
	int iFreeSize = session.sendQ->GetFreeSize();
	
	if (dfPACKET_SIZE <= iFreeSize)
	{
		if (dfPACKET_SIZE != session.sendQ->Put((char*)packet, dfPACKET_SIZE))
		{
			// ����
			closesocket(session.sock);
			err_quit("SendPacket()");
		}
	}
	SendProc();
}

void SendProc(void)
{
	int		iSize = 0;

	iSize	= session.sendQ->GetNotBrokenGetSize();

	retval	= send(session.sock, session.sendQ->GetReadBufferPtr(), iSize, 0);

	iSendVal += retval;

	session.sendQ->RemoveData(retval);

	if (SOCKET_ERROR == retval)
	{
		if (WSAEWOULDBLOCK == WSAGetLastError())
		{
			session.sendFlag = FALSE;
			return;
		}
		err_display("send()");
		closesocket(session.sock);
	}
}

// ���� �Լ� ���� ��� �� ����
// msg���ڷ� ���޵� ���ڿ��� ���Ҿ�
// ���� �߻��� ���� �޽����� ȭ�鿡 �޽��� ���ڷ� ǥ���ϰ� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, 
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);

	LocalFree(lpMsgBuf);
	exit(1);
}

// wchar ����
void err_quit(WCHAR *msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCWSTR)lpMsgBuf, msg, MB_ICONHAND /*MB_ICONERROR*/);

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

void err_display(int errcode)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	printf("[����] %s", (char*)lpMsgBuf);

	LocalFree(lpMsgBuf);
}

void SetWindowSize(HWND hWnd)
{
	RECT WindowRect;
	WindowRect.top = 0;
	WindowRect.left = 0;
	WindowRect.right = 640;
	WindowRect.bottom = 480;

	AdjustWindowRectEx(&WindowRect,
		GetWindowStyle(hWnd),
		GetMenu(hWnd) != NULL,
		GetWindowExStyle(hWnd));

	int iX = (GetSystemMetrics(SM_CXSCREEN) - 640) / 2;
	int iY = (GetSystemMetrics(SM_CYSCREEN) - 480) / 2;

	MoveWindow(hWnd,
		iX,
		iY,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top, TRUE);
}