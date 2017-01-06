// DialogBox.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
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

// 전역 변수:
HINSTANCE	hInst;								// 현재 인스턴스입니다.
TCHAR		szTitle[MAX_LOADSTRING];			// 제목 표시줄 텍스트입니다.
TCHAR		szWindowClass[MAX_LOADSTRING];		// 기본 창 클래스 이름입니다.

WCHAR		g_szIP[16];
BOOL		bIsConnected;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

BOOL	CALLBACK	DialogProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

// 소켓 관련 윈도우 메시지 처리
void				ProcessSocketMessage(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

// 
// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg);

// 소켓 함수 오류 출력
void err_display(char *msg);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 다이얼로그 생성
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_ADDR), NULL, DialogProc);

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DIALOGBOX, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	// 소켓 생성

	// WSAAsyncSelect


	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIALOGBOX));

	// 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
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
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
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
	case dfWM_SOCKET:		// 소켓 관련 윈도우 메시지
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
		// 해당 프로그램이 꺼질 때

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
	// 데이터 통신에 사용할 변수
	st_PACKET *ptr;
	SOCKET client_sock;

	SOCKADDR_IN clientaddr;
	int addrlen;
	int retval;

	// 오류 발생 여부 확인
	if (WSAGETSELECTERROR(lParam))
	{
		err_display(	(char*)WSAGETSELECTERROR(lParam)	);
		return;
	}

	// 메시지 처리
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