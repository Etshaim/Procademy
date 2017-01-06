// MonitoringTool.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "MonitorGraphUnit.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE	g_hInst;											// 현재 인스턴스입니다.
TCHAR		szTitle[MAX_LOADSTRING] = L"MonitoringTool";		// 제목 표시줄 텍스트입니다.
TCHAR		szWindowClass[MAX_LOADSTRING] = L"MonitoringTool";	// 기본 창 클래스 이름입니다.


// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


// CMonitorGraphUnit의 Static 멤버 변수
CMonitorGraphUnit::ST_HWNDtoTHIS	CMonitorGraphUnit::stArrHandle[];
BOOL								CMonitorGraphUnit::_bAlertMode = FALSE;
BOOL								CMonitorGraphUnit::_bConnectStatus = FALSE;
BOOL								CMonitorGraphUnit::_bDrawStatus = FALSE;


// 부모 윈도우의 전역 핸들
HBRUSH	g_MyBrush	= NULL;
HBRUSH	g_OldBrush	= NULL;
CRect	cRect		= NULL;


// 자식 윈도우 포인터
CMonitorGraphUnit *p0;
CMonitorGraphUnit *p1;
CMonitorGraphUnit *p2;
CMonitorGraphUnit *p3;
CMonitorGraphUnit *p4;
CMonitorGraphUnit *p5;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
	int count;

 	// TODO: 여기에 코드를 입력합니다.

	g_hInst = hInstance;

	MSG msg;
	HACCEL hAccelTable;

	MyRegisterClass(hInstance);

	srand(	(unsigned)time(NULL)	);

	for (count = 0; count < dfHANDLE_ARRAY_MAX; ++count)
	{
		CMonitorGraphUnit::stArrHandle[count].hWnd = NULL;
		CMonitorGraphUnit::stArrHandle[count].pThis = NULL;
	}

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MONITORINGTOOL));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MONITORINGTOOL));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);

	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(220, 220, 220));
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MONITORINGTOOL);
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

   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, SW_MAXIMIZE);

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
	int wmId, wmEvent;

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
	{
		p0 = new CMonitorGraphUnit(g_hInst, hWnd, RGB(200, 200, 10), CMonitorGraphUnit::GRAPH_TYPE::SWITCH, 20, 20, 260, 250, 1);
		p1 = new CMonitorGraphUnit(g_hInst, hWnd, RGB(199, 10, 10), CMonitorGraphUnit::GRAPH_TYPE::LINE_SINGLE, 300, 20, 520, 250, 1);
		p2 = new CMonitorGraphUnit(g_hInst, hWnd, RGB(100, 100, 100), CMonitorGraphUnit::GRAPH_TYPE::LINE_SINGLE, 840, 20, 520, 250, 1);
		p3 = new CMonitorGraphUnit(g_hInst, hWnd, RGB(0, 200, 200), CMonitorGraphUnit::GRAPH_TYPE::LINE_SINGLE, 1380, 20, 520, 250, 1);
		p4 = new CMonitorGraphUnit(g_hInst, hWnd, RGB(200, 200, 10), CMonitorGraphUnit::GRAPH_TYPE::BAR_COLUMN_VERTICAL, 20, 290, 1240, 300, 5);
		p5 = new CMonitorGraphUnit(g_hInst, hWnd, RGB(200, 200, 10), CMonitorGraphUnit::GRAPH_TYPE::LINE_MULTI, 1280, 290, 620, 300, 5);

		p0->SetSwitchInfo(TEXT("접속 상태"));
		p1->SetSingleDataInfo(TEXT("TEST"), 2000);
		p2->SetSingleDataInfo(TEXT("AAAA"), 1000);
		p3->SetSingleDataInfo(TEXT("BBBB"), 5000);
		p4->SetMultiDataInfo(TEXT("ALL"), 2000);
		p5->SetMultiDataInfo(TEXT("사용률"), 2000);

		SetTimer(hWnd, 1, 100, NULL);
	}
	break;

	case WM_TIMER:
	{
		switch (wParam)
		{
		case 1:
			CMonitorGraphUnit::_bDrawStatus = TRUE;

			p0->InsertData();
			p1->InsertData();
			p2->InsertData();
			p3->InsertData();
			p4->InsertData();
			p5->InsertData();
		}
		break;

		case 1000:
		{
			InvalidateRect(hWnd, &cRect, TRUE);
			KillTimer(hWnd, 1000);
			SetTimer(hWnd, 1001, 500, NULL);
		}
		break;

		case 1001:
		{
			CMonitorGraphUnit::_bAlertMode = FALSE;
			KillTimer(hWnd, 1001);
		}
		break;
	}
	break;
		
	case WM_USER:
	{
		hdc = GetDC(hWnd);
		GetClientRect(hWnd, &cRect);
		g_MyBrush = CreateSolidBrush(	RGB(225, 1, 1)	);
		g_OldBrush = (HBRUSH)SelectObject(hdc, g_MyBrush);
		FillRect(hdc, &cRect, g_MyBrush);
		SelectObject(hdc, g_OldBrush);
		DeleteObject(g_MyBrush);

		ReleaseDC(hWnd, hdc);
		SetTimer(hWnd, 1000, 500, NULL);
	}
	break;
	
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_SPACE:
		{
			CMonitorGraphUnit::_bConnectStatus = !CMonitorGraphUnit::_bConnectStatus;
		}
		break;
		}
	}
	break;

	case WM_KEYUP:
	{
		switch (wParam)
		{
		case VK_SPACE:
			//////////////////////////////////////////////////
			// SPACECOLUMN가 눌렸다가 때어졌을 때 처리가능한 영역	//
			//////////////////////////////////////////////////
			break;
		}
	}
	break;

	case WM_COMMAND:
	{
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
	}
	break;

	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
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
