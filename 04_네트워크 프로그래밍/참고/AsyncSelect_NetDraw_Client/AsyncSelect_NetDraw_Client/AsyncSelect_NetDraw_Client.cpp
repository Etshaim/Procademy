// AsyncSelect_NetDraw_Client.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "AsyncSelect_NetDraw_Client.h"
#include "resource.h"
#include "StreamQueue.h"

#define MAX_LOADSTRING		100
#define WM_SOCKET			(WM_USER+1)
#pragma warning(disable:4996)

// created by NOH
#define dfPACKETSIZE		16


// ���� ����:
HINSTANCE			hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR				szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR				szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// ���� ���� created by NOH.
HWND				g_hWnd = NULL;
HBITMAP				g_MyBitmap = NULL;
HDC					g_MemDC = NULL;
CRect				g_Rect = NULL;

WCHAR				g_szIP[16] = { NULL };
SOCKET				g_Sock = NULL;

WCHAR				g_ErrTitle[50] = { NULL };
WCHAR				g_Error[100] = { NULL };

CStreamQueue		*SendQ = new CStreamQueue(512);
CStreamQueue		*RecvQ = new CStreamQueue(512);



// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

// �ʱ�ȭ �� ���� created by NOH.
void CloseProc(void);

// ��Ʈ��ũ ó�� created by NOH.
BOOL				NetWorkProc(void);
BOOL				GetIPAddr(WCHAR *wcpName, WCHAR *wcpPort, IN_ADDR *addr, ADDRINFOW hints, ADDRINFOW **result);
void				SocketMessageProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

// ���� ó�� created by NOH.
//void				err_log(TCHAR *tcpMsg, DWORD iError);
void				ErrorMessageBox(TCHAR *tcpMsg, DWORD iError);
INT_PTR CALLBACK	DialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DialogErrorProc(HWND, UINT, WPARAM, LPARAM);

// �׸� ó�� created by NOH.
void				EnqueueProc(int iStartX, int iStartY, int iEndX, int iEndY);
void				DrawProc();
void				DrawBitMap(HDC hdc, HBITMAP MyBitmap);

// ��Ŷ ó�� created by NOH.
void				SendProc(void);
void				RecvProc(void);
BOOL				CheckPacket(void);

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

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ASYNCSELECT_NETDRAW_CLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASYNCSELECT_NETDRAW_CLIENT));

	if (!NetWorkProc())
		return 0;


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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASYNCSELECT_NETDRAW_CLIENT));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_ASYNCSELECT_NETDRAW_CLIENT);
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
   
   g_hWnd = hWnd;

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	static int iStartX, iStartY, iEndX, iEndY;
	static BOOL bNowDraw = FALSE;

	switch (uiMsg)
	{
	case WM_CREATE:
		GetClientRect(hWnd, &g_Rect);
		break;
	case WM_SOCKET:
		SocketMessageProc(hWnd, uiMsg, wParam, lParam);
		return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, uiMsg, wParam, lParam);
		}
		break;
	case WM_LBUTTONDOWN:
		iStartX = LOWORD(lParam);
		iStartY = HIWORD(lParam);
		bNowDraw = TRUE;
		break;
	case WM_MOUSEMOVE:
		if (bNowDraw == TRUE)
		{
			//////////////////////////////////////////////////
			// InvalidateRect �� ���� ��ȿȭ �� ������			//
			// WM_PAINT �޽����� ���� �ٽ� �׷����� ��			//
			// 3��° ���ڰ�, TRUE���̸� ��ȿȭ ������ ������	//
			// FALSE�� ��ȿȭ ������ �״�� ����				//
			//////////////////////////////////////////////////
			InvalidateRect(g_hWnd, NULL, FALSE);
			iEndX = LOWORD(lParam);
			iEndY = HIWORD(lParam);
			EnqueueProc(iStartX, iStartY, iEndX, iEndY);

			iStartX = iEndX;
			iStartY = iEndY;
		}
		break;
	case WM_LBUTTONUP:
		bNowDraw = FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.

		DrawBitMap(hdc, g_MyBitmap);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteDC(g_MemDC);
		DeleteObject(g_MyBitmap);
		CloseProc();
		SendQ->~CStreamQueue();
		RecvQ->~CStreamQueue();
		delete SendQ;
		SendQ = NULL;
		delete RecvQ;
		RecvQ = NULL;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uiMsg, wParam, lParam);
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

INT_PTR CALLBACK DialogProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hEditBox;

	switch (uiMsg)
	{
	case WM_INITDIALOG:
		memset(g_szIP, 0, sizeof(g_szIP) * 2);
		hEditBox = GetDlgItem(hWnd, IDC_IPADDRESS1);
		SetWindowText(hEditBox, L"127.0.0.1");
		return TRUE;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			GetDlgItemText(hWnd, IDC_IPADDRESS1, g_szIP, 16);
			EndDialog(hWnd, 99999);
			return TRUE;
		case IDCANCEL:
			EndDialog(hWnd, 99999);
			return TRUE;
		}
		break;
	}
	return FALSE;
}


BOOL NetWorkProc(void)
{
	int iRetval;
	DWORD dwError;

	// ���� �ʱ�ȭ
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		dwError = WSAGetLastError();
		ErrorMessageBox(L"wsastartup() error:", dwError);
		return FALSE;
	}
		
	// socket()
	g_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_Sock == INVALID_SOCKET)
	{
		dwError = WSAGetLastError();
		ErrorMessageBox(L"socket() error:", dwError);
		return FALSE;
	}

	// asyncselect()
	iRetval = WSAAsyncSelect(g_Sock, g_hWnd, WM_SOCKET, FD_CONNECT|FD_CLOSE|FD_READ|FD_WRITE);
	if (iRetval == SOCKET_ERROR)
	{
		dwError = WSAGetLastError();
		ErrorMessageBox(L"asyncselect() error:", dwError);
		return FALSE;
	}

	// getip()
	IN_ADDR addr;
	ADDRINFOW *result = NULL;
	ADDRINFOW hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if (!GetIPAddr(g_szIP, L"25000", &addr, hints, &result))
	{
		dwError = WSAGetLastError();
		ErrorMessageBox(L"getip() error:", dwError);
		return FALSE;
	}

	// connect()
	iRetval = connect(g_Sock, result->ai_addr, (int)result->ai_addrlen);
	if (iRetval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			dwError = WSAGetLastError();
			ErrorMessageBox(L"connect() error:", dwError);
			CloseProc();
			return FALSE;
		}
		else
			return TRUE;
	}

	return TRUE;
}

BOOL GetIPAddr(WCHAR *wcpName, WCHAR *wcpPort, IN_ADDR *addr, ADDRINFOW hints, ADDRINFOW **result)
{
	DWORD dwRetval = GetAddrInfoW(wcpName, wcpPort, &hints, result);
	if (dwRetval != 0)
		return FALSE;

	return TRUE;
}

void ErrorMessageBox(TCHAR *tcpMsg, DWORD iError)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, iError, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPTSTR)lpMsgBuf, tcpMsg, MB_ICONEXCLAMATION);
	LocalFree(lpMsgBuf);
}

// ���� ���� �ε��� �޽��� ó��
void SocketMessageProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	DWORD dwError = NULL;

	if (WSAGETSELECTERROR(lParam))
	{
		dwError = WSAGetLastError();
		ErrorMessageBox(L"socketMessageproc() error:", WSAGETSELECTERROR(lParam));
		CloseProc();
	}	

	// �޽��� ó��
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		if (0 != WSAGETSELECTERROR(lParam))
		{
			dwError = WSAGetLastError();
			ErrorMessageBox(L"FD_CONNECT error:", dwError);
			CloseProc();
		}
		break;
	case FD_CLOSE:
		if (0 != WSAGETSELECTERROR(lParam))
		{
			dwError = WSAGetLastError();
			ErrorMessageBox(L"FD_CLOSE error:", dwError);
			CloseProc();
		}
		break;
	case FD_READ:
		if (0 != WSAGETSELECTERROR(lParam))
		{
			dwError = WSAGetLastError();
			ErrorMessageBox(L"FD_READ error:", dwError);
			CloseProc();
		}
		RecvProc();
		break;
	case FD_WRITE:
		if (0 != WSAGETSELECTERROR(lParam))
		{
			dwError = WSAGetLastError();
			ErrorMessageBox(L"FD_WRITE error:", dwError);
			CloseProc();
		}
		SendProc();
		break;
	}
}

void DrawProc()
{
	HDC hdc;
	HBITMAP OldBitmap;
	int iStartX, iStartY, iEndX, iEndY;
	static int i = 0;
	hdc = GetDC(g_hWnd);

	if (g_MemDC == NULL)
		g_MemDC = CreateCompatibleDC(hdc);

	if (g_MyBitmap == NULL)
		g_MyBitmap = CreateCompatibleBitmap(hdc, g_Rect.right, g_Rect.bottom);

	OldBitmap = (HBITMAP)SelectObject(g_MemDC, g_MyBitmap);

	//////////////////////////////////////////////////////////////////
	// PatBlt�� ���������� ��Ʈ���� ����ϴ� �Լ��� �ƴϸ� ������ ���	//
	// �޸� DC�� ȭ�� ������ � ������ �𸣱� ������					//
	// �޸� DC�� ȭ�� ������ ������� �������ش�.						//
	//////////////////////////////////////////////////////////////////
	if (i == 0)
		PatBlt(g_MemDC, 0, 0, g_Rect.right, g_Rect.bottom, WHITENESS);
	i = 1;
	while (1)
	{
		if (RecvQ->GetOccupyingSize() / 18 == 0)
			break;

		RecvQ->RemoveData(sizeof(short));
		RecvQ->Dequeue((char *)&iStartX, sizeof(iStartX));
		RecvQ->Dequeue((char *)&iStartY, sizeof(iStartY));
		RecvQ->Dequeue((char *)&iEndX, sizeof(iEndX));
		RecvQ->Dequeue((char *)&iEndY, sizeof(iEndY));

		MoveToEx(g_MemDC, iStartX, iStartY, NULL);
		LineTo(g_MemDC, iEndX, iEndY);
	}
	

	SelectObject(g_MemDC, OldBitmap);
	ReleaseDC(g_hWnd, hdc);
}

void DrawBitMap(HDC hdc, HBITMAP MyBitmap)
{
	BITMAP bit;
	HBITMAP OldBitmap;
	CRect rect;

	SetMapMode(hdc, MM_ISOTROPIC);
	GetClientRect(g_hWnd, &rect);
	SetWindowExtEx(hdc, rect.right, rect.bottom, NULL);
	SetViewportExtEx(hdc, rect.right, rect.bottom, NULL);

	OldBitmap = (HBITMAP)SelectObject(g_MemDC, g_MyBitmap);
	GetObject(MyBitmap, sizeof(BITMAP), &bit);

	BitBlt(hdc, 0, 0, bit.bmWidth, bit.bmHeight, g_MemDC, 0, 0, SRCCOPY);

	SelectObject(g_MemDC, OldBitmap);
}

void EnqueueProc(int iStartX, int iStartY, int iEndX, int iEndY)
{
	short sLen = 16;

	SendQ->Enqueue((char *)&sLen, sizeof(sLen));
	SendQ->Enqueue((char *)&iStartX, sizeof(iStartX));
	SendQ->Enqueue((char *)&iStartY, sizeof(iStartY));
	SendQ->Enqueue((char *)&iEndX, sizeof(iEndX));
	SendQ->Enqueue((char *)&iEndY, sizeof(iEndY));

	SendProc();
}

void SendProc(void)
{
	int iRetval;
	DWORD dwError;

	if (SendQ->GetOccupyingSize() == 0)
		return;

	iRetval = send(g_Sock, SendQ->GetFrontBufferPtr(), SendQ->GetNotCirculatedDequeuingSize(), 0);
	if (iRetval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			dwError = WSAGetLastError();
			ErrorMessageBox(L"send() error:", dwError);
			CloseProc();
			return;
		}
		else
			return;
	}
	SendQ->RemoveData(iRetval);
}

void RecvProc(void)
{
	if (!CheckPacket())
		return;

	DrawProc();
	//////////////////////////////////////////////////
	// InvalidateRect �� ���� ��ȿȭ �� ������			//
	// WM_PAINT �޽����� ���� �ٽ� �׷����� ��			//
	// 3��° ���ڰ�, TRUE���̸� ��ȿȭ ������ ������	//
	// FALSE�� ��ȿȭ ������ �״�� ����				//
	//////////////////////////////////////////////////
	InvalidateRect(g_hWnd, NULL, FALSE);
}

BOOL CheckPacket(void)
{
	short shPacketSize;
	int iRetval;
	DWORD dwError;
	int iOccupyingSize;

	iRetval = recv(g_Sock, RecvQ->GetRearBufferPtr(), RecvQ->GetNotCirculatedDequeuingSize(), 0);
	if (iRetval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			dwError = WSAGetLastError();
			ErrorMessageBox(L"recv() error:", dwError);
			CloseProc();
			return FALSE;
		}
		else
			return FALSE;
	}
	RecvQ->MoveRear(iRetval);

	RecvQ->Peek((char *)&shPacketSize, 2);
	iOccupyingSize = RecvQ->GetOccupyingSize();

	if (shPacketSize == dfPACKETSIZE)
	{
		if (iOccupyingSize >= dfPACKETSIZE)
		{
			return TRUE;
		}
	}		
	else
		return FALSE;	

	return TRUE;
}

void CloseProc(void)
{
	closesocket(g_Sock);
	g_Sock = INVALID_SOCKET;
	WSACleanup();
}

