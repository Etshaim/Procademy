// DialogBox.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "DialogBox.h"
#include <WinSock2.h>

#define MAX_LOADSTRING 100

#define dfSERVERPORT	25000
#define dfBUFSIZE		512
#define dfWM_SOCKET		(WM_USER+1)

struct st_PACKET
{
	DWORD dwPacketLen;
	int	iStartPosX;
	int iStartPosY;
	int iEndPosX;
	int iEndPosY;
};

// ���� ����:
HINSTANCE	hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR		szTitle[MAX_LOADSTRING];			// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR		szWindowClass[MAX_LOADSTRING];		// �⺻ â Ŭ���� �̸��Դϴ�.

WCHAR		g_szIP[16];
BOOL		bIsConnected;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

BOOL	CALLBACK	DialogProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

// ���� ���� ������ �޽��� ó��
void				ProcessSocketMessage(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

// 
// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg);

// ���� �Լ� ���� ���
void err_display(char *msg);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���̾�α� ����
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_ADDR), NULL, DialogProc);

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DIALOGBOX, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// ���� ����

	// WSAAsyncSelect


	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIALOGBOX));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

BOOL	CALLBACK	DialogProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HWND	hEditBox;

	switch (iMsg)
	{
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

//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIALOGBOX));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_DIALOGBOX);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc;
	static int iX, iY;
	static BOOL bNowDraw = FALSE;
	static BOOL bNowKeyStatus = FALSE;
	static HPEN MyPen, OldPen;

	switch (message)
	{
	case dfWM_SOCKET:		// ���� ���� ������ �޽���
		ProcessSocketMessage(hWnd, message, wParam, lParam);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		
		case VK_CONTROL:
			if (bNowKeyStatus == FALSE)
			{
				MyPen = CreatePen(PS_SOLID, rand() % 10 + 1, RGB(rand() % 256, rand() % 256, rand() % 256));
				bNowKeyStatus = TRUE;
			}
		}
		break;

	case WM_KEYUP:
		switch (wParam)
		{
		case VK_CONTROL:
			SelectObject(hdc, OldPen);
			DeleteObject(MyPen);
			bNowKeyStatus = FALSE;
		}
		break;

	case WM_LBUTTONDOWN:
		iX = LOWORD(lParam);
		iY = HIWORD(lParam);
		bNowDraw = TRUE;
		break;

	case WM_MOUSEMOVE:
		if (bNowDraw == TRUE)
		{
			hdc = GetDC(hWnd);
			OldPen = (HPEN)SelectObject(hdc, MyPen);
			MoveToEx(hdc, iX, iY, NULL);
			iX = LOWORD(lParam);
			iY = HIWORD(lParam);
			LineTo(hdc, iX, iY);
			ReleaseDC(hWnd, hdc);
		}
		break;

	case WM_LBUTTONUP:
		bNowDraw = FALSE;
		break;
		// �ش� ���α׷��� ���� ��

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void	ProcessSocketMessage(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// ������ ��ſ� ����� ����
	st_PACKET *ptr;
	SOCKET client_sock;

	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;

	// ���� �߻� ���� Ȯ��
	if (WSAGETSELECTERROR(lParam))
	{
		err_display(	(char*)WSAGETSELECTERROR(lParam)	);
		return;
	}

	// �޽��� ó��
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(wParam, (SOCKADDR *)&clientaddr, &addrlen);

		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			return;
		}
	}
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