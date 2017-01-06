// TCPFighter.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "TCPFighter.h"
#include "ScreenDIB.h"
#include "SpriteDIB.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "Common.h"
#include "LinkedList.h"
#include "FrameSkip.h"
#include "MemoryPool.h"

#define MAX_LOADSTRING 100

// 전역으로 스크린 DIB 생성		
HWND			g_hWnd;								// 전역 윈도우 핸들. 윈도우 생성 후 핸들 대입
CScreenDib		g_cScreenDib(640, 480, 32);			// 전역으로 ScreenDib 생성
CSpriteDib		g_cSpriteDib(100, 0x00ffffff);		// 100개의 스프라이트 사용, 흰색 칼라키

CBaseObject		*g_pPlayerObject;
CFrameSkip		g_FrameSkip(50);

CDoubleLinkedList<CBaseObject*> objectList;
CMemoryPool<CPlayerObject> playerPool(10, true);

BOOL			bPlayer;

// 전역 변수:
HINSTANCE	hInst;									// 현재 인스턴스입니다.
TCHAR		szTitle[MAX_LOADSTRING];				// 제목 표시줄 텍스트입니다.
TCHAR		szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

// 
void	Update(void);		// 실제 게임 메인 함수
void	UpdateGame(void);	//
void	InitialGame(void);	//
BOOL	SpriteLoad(void);	// sprite 로드
void	KeyProcess(void);
void	Action(void);
void	Draw(void);
void	Sort(void);
void	AllocPlayer(void);
void	FreePlayer(void);
void	MakeTitle(void);

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TCPFIGHTER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	RECT WindowRect;
	WindowRect.top = 0;
	WindowRect.left = 0;
	WindowRect.right = 640;
	WindowRect.bottom = 480;

	AdjustWindowRectEx(&WindowRect,
		GetWindowStyle(g_hWnd),
		GetMenu(g_hWnd) != NULL,
		GetWindowExStyle(g_hWnd));

	int iX = (GetSystemMetrics(SM_CXSCREEN) - 640) / 2;
	int iY = (GetSystemMetrics(SM_CYSCREEN) - 480) / 2;

	MoveWindow(g_hWnd,
		iX,
		iY,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top, TRUE);

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TCPFIGHTER));

	InitialGame();

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		else
		{
			MakeTitle();

			// 게임 처리 함수 호출
			Update();
		}
	}
	//// 기본 메시지 루프입니다.
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TCPFIGHTER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TCPFIGHTER);
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
   //HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
   {
      return FALSE;
   }

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);

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
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.
		EndPaint(hWnd, &ps);
		break;
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

void Update(void)
{
	UpdateGame();

	//BYTE	*bypDest	= g_cScreenDib.GetDibBuffer();
	//int		iDestWidth	= g_cScreenDib.GetWidth();
	//int		iDestHeight = g_cScreenDib.GetHeight();
	//int		iDestPitch	= g_cScreenDib.GetPitch();

	//// 0 ~ 4번 스프라이트를 각 좌표에 출력
	//g_cSpriteDib.DrawSprite(0, 100, 100, bypDest, iDestWidth, iDestHeight, iDestPitch);
	//g_cSpriteDib.DrawSprite(1, 150, 150, bypDest, iDestWidth, iDestHeight, iDestPitch);
	//g_cSpriteDib.DrawSprite(2, 150, 200, bypDest, iDestWidth, iDestHeight, iDestPitch);
	//g_cSpriteDib.DrawSprite(3, 150, 250, bypDest, iDestWidth, iDestHeight, iDestPitch);

	//////----------------------------------------------------
	////// 버퍼 포인터에 그림을 그린다.
	//////
	////// 스프라이트 출력부
	//////----------------------------------------------------
	////BYTE byGrayColor = 0;

	////for (int iCount = 0; iCount < 480; ++iCount)
	////{
	////	memset(bypDest, byGrayColor, 640 * 4);
	////	bypDest += iDestPitch;
	////	byGrayColor++;
	////}

	////----------------------------------------------------
	//// Dib 버퍼의 내용을 화면으로 출력
	////----------------------------------------------------
	//g_cScreenDib.DrawBuffer(g_hWnd);
}

void UpdateGame(void)
{
	FLOAT fFPS;

	KeyProcess();

	if (bPlayer)
	{
		Action();
	}
	

	//------------------------------
	// frame skip 기능
	//------------------------------
	if (!g_FrameSkip.FrameSkip())
		Draw();


	fFPS = g_FrameSkip.CheckFPS();


	g_cScreenDib.DrawBuffer(g_hWnd);
}

void InitialGame(void)
{
	// 스프라이트 로드
	SpriteLoad();

	// CPlayerObject 생성
	// ( int objectType, int iCurX, int iCurY )
	//g_pPlayerObject = new CPlayerObject(0, 200, 200, dfDIRECTION_RIGHT, 1);

	//objectList.PushBack(g_pPlayerObject);

	// 테스트용 객체들 생성
	/*objectList.PushBack(new CPlayerObject(0, 400, 150, dfDIRECTION_LEFT, 2));
	objectList.PushBack(new CPlayerObject(0, 400, 280, dfDIRECTION_LEFT, 3));
	objectList.PushBack(new CPlayerObject(0, 400, 410, dfDIRECTION_LEFT, 4));*/
}

void KeyProcess(void)
{

	DWORD dwAction = dfACTION_STAND;

	// 테스트 용
	// 스페이스 누르면 STOP상태로
	if (GetAsyncKeyState(0x41) & 0X8000)
	{
		AllocPlayer();
	}

	if (GetAsyncKeyState(0x46) & 0X8000)
	{
		FreePlayer();
	}

	if (GetAsyncKeyState(VK_ESCAPE) & 0X8000)
	{
		exit(1);
	}

	if (GetAsyncKeyState(VK_UP) & 0X8000)
		dwAction = dfACTION_MOVE_UU;
	
	if (GetAsyncKeyState(VK_UP) & 0X8000)
		dwAction = dfACTION_MOVE_UU;

	if (GetAsyncKeyState(VK_DOWN) & 0X8000)
		dwAction = dfACTION_MOVE_DD;

	if (GetAsyncKeyState(VK_LEFT) & 0X8000)
		dwAction = dfACTION_MOVE_LL;

	if (GetAsyncKeyState(VK_RIGHT) & 0X8000)
		dwAction = dfACTION_MOVE_RR;

	if (GetAsyncKeyState(VK_LEFT) & 0X8000 && GetAsyncKeyState(VK_UP) & 0X8000)
		dwAction = dfACTION_MOVE_LU;

	if (GetAsyncKeyState(VK_LEFT) & 0X8000 && GetAsyncKeyState(VK_DOWN) & 0X8000)
		dwAction = dfACTION_MOVE_LD;

	if (GetAsyncKeyState(VK_RIGHT) & 0X8000 && GetAsyncKeyState(VK_UP) & 0X8000)
		dwAction = dfACTION_MOVE_RU;

	if (GetAsyncKeyState(VK_RIGHT) & 0X8000 && GetAsyncKeyState(VK_DOWN) & 0X8000)
		dwAction = dfACTION_MOVE_RD;

	// Attack1	q
	if (GetAsyncKeyState(0x51) & 0X8000)
	{
		dwAction = dfACTION_ATTACK1;
	}

	// Attack2	w
	if (GetAsyncKeyState(0x57) & 0X8000)
	{
		dwAction = dfACTION_ATTACK2;
	}

	// Attack3	e
	if (GetAsyncKeyState(0x45) & 0X8000)
	{
		dwAction = dfACTION_ATTACK3;
	}

	CDoubleLinkedList<CBaseObject*>::Iterator iterator;

	for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
	{
		// 플레이어 객체에게 입력된 동작을 알려줌
		(*iterator)->ActionInput(dwAction);
	}
	
}

void Action(void)
{
	// 리스트 추가 해야 됨
	//g_pPlayerObject->Action();

	// iterator end가 잘못 된듯
	// head와 tail은 데이터 없는 포인터로 사용하면 될듯
	// 리스트 버전
	CDoubleLinkedList<CBaseObject*>::Iterator iterator;

	for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
	{
		// 삭제 기능 구현해야 함
		if (eEND_EFFECT == (*iterator)->GetObjectType())
		{
			//objectList.RemoveAt((++iterator).pNode);
		}


		(*iterator)->Action();
	}

	//(*iterator)->Action();
}

void Draw(void)
{
	BYTE *bypDest = g_cScreenDib.GetDibBuffer();
	int iDestWidth = g_cScreenDib.GetWidth();
	int iDestHeight = g_cScreenDib.GetHeight();
	int iDestPitch = g_cScreenDib.GetPitch();

	// 맵 출력
	// 제대로 해보고 싶으면 맵 관리 클래스 만들어서 전역이나 싱글톤으로 찍어주면 됨
	g_cSpriteDib.DrawSprite(e_SPRITE::eMAP, 0, 0, bypDest, iDestWidth, iDestHeight, iDestPitch);

	if (FALSE == bPlayer)
	{
		return;
	}

	///////
	// 리스트의 모든 객체들에게 Draw 함수를 호출
	// 리스트 추가해야 됨
	//g_pPlayerObject->Draw(bypDest, iDestWidth, iDestHeight, iDestPitch);

	// 리스트 버전
	CDoubleLinkedList<CBaseObject*>::Iterator iterator;

	// draw
	for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
	{
		(*iterator)->Draw(bypDest, iDestWidth, iDestHeight, iDestPitch);
	}
	//(*iterator)->Draw(bypDest, iDestWidth, iDestHeight, iDestPitch);
}

BOOL SpriteLoad(void)
{
	// 스프라이트 로드
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::eMAP, L"Sprite_Data\\_Map.bmp", 0, 0))							return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L01, L"Sprite_Data\\Stand_L_01.bmp", 71, 90))		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L02, L"Sprite_Data\\Stand_L_02.bmp", 71, 90))		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L03, L"Sprite_Data\\Stand_L_03.bmp", 71, 90))		return FALSE;
	//if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L04, L"Sprite_Data\\Stand_L_04.bmp", 71, 90))		return FALSE;
	//if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L05, L"Sprite_Data\\Stand_L_05.bmp", 71, 90))		return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R01, L"Sprite_Data\\Stand_R_01.bmp", 71, 90))		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R02, L"Sprite_Data\\Stand_R_02.bmp", 71, 90))		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R03, L"Sprite_Data\\Stand_R_03.bmp", 71, 90))		return FALSE;
	//if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R04, L"Sprite_Data\\Stand_R_04.bmp", 71, 90))		return FALSE;
	//if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R05, L"Sprite_Data\\Stand_R_05.bmp", 71, 90))		return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L01, L"Sprite_Data\\Move_L_01.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L02, L"Sprite_Data\\Move_L_02.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L03, L"Sprite_Data\\Move_L_03.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L04, L"Sprite_Data\\Move_L_04.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L05, L"Sprite_Data\\Move_L_05.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L06, L"Sprite_Data\\Move_L_06.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L07, L"Sprite_Data\\Move_L_07.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L08, L"Sprite_Data\\Move_L_08.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L09, L"Sprite_Data\\Move_L_09.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L10, L"Sprite_Data\\Move_L_10.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L11, L"Sprite_Data\\Move_L_11.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_L12, L"Sprite_Data\\Move_L_12.bmp", 71, 90))			return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R01, L"Sprite_Data\\Move_R_01.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R02, L"Sprite_Data\\Move_R_02.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R03, L"Sprite_Data\\Move_R_03.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R04, L"Sprite_Data\\Move_R_04.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R05, L"Sprite_Data\\Move_R_05.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R06, L"Sprite_Data\\Move_R_06.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R07, L"Sprite_Data\\Move_R_07.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R08, L"Sprite_Data\\Move_R_08.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R09, L"Sprite_Data\\Move_R_09.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R10, L"Sprite_Data\\Move_R_10.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R11, L"Sprite_Data\\Move_R_11.bmp", 71, 90))			return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_MOVE_R12, L"Sprite_Data\\Move_R_12.bmp", 71, 90))			return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_L01, L"Sprite_Data\\Attack1_L_01.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_L02, L"Sprite_Data\\Attack1_L_02.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_L03, L"Sprite_Data\\Attack1_L_03.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_L04, L"Sprite_Data\\Attack1_L_04.bmp", 71, 90))	return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_R01, L"Sprite_Data\\Attack1_R_01.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_R02, L"Sprite_Data\\Attack1_R_02.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_R03, L"Sprite_Data\\Attack1_R_03.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK1_R04, L"Sprite_Data\\Attack1_R_04.bmp", 71, 90))	return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_L01, L"Sprite_Data\\Attack2_L_01.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_L02, L"Sprite_Data\\Attack2_L_02.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_L03, L"Sprite_Data\\Attack2_L_03.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_L04, L"Sprite_Data\\Attack2_L_04.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_R01, L"Sprite_Data\\Attack2_R_01.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_R02, L"Sprite_Data\\Attack2_R_02.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_R03, L"Sprite_Data\\Attack2_R_03.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK2_R04, L"Sprite_Data\\Attack2_R_04.bmp", 71, 90))	return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L01, L"Sprite_Data\\Attack3_L_01.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L02, L"Sprite_Data\\Attack3_L_02.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L03, L"Sprite_Data\\Attack3_L_03.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L04, L"Sprite_Data\\Attack3_L_04.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L05, L"Sprite_Data\\Attack3_L_05.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_L06, L"Sprite_Data\\Attack3_L_06.bmp", 71, 90))	return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R01, L"Sprite_Data\\Attack3_R_01.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R02, L"Sprite_Data\\Attack3_R_02.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R03, L"Sprite_Data\\Attack3_R_03.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R04, L"Sprite_Data\\Attack3_R_04.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R05, L"Sprite_Data\\Attack3_R_05.bmp", 71, 90))	return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_ATTACK3_R06, L"Sprite_Data\\Attack3_R_06.bmp", 71, 90))	return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::eEFFECT_SPARK_01, L"Sprite_Data\\xSpark_1.bmp", 70, 70))		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::eEFFECT_SPARK_02, L"Sprite_Data\\xSpark_2.bmp", 70, 70))		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::eEFFECT_SPARK_03, L"Sprite_Data\\xSpark_3.bmp", 70, 70))		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::eEFFECT_SPARK_04, L"Sprite_Data\\xSpark_4.bmp", 70, 70))		return FALSE;


	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::eGUAGE_HP, L"Sprite_Data\\HPGuage.bmp", 0, 0))				return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::eSHADOW, L"Sprite_Data\\Shadow.bmp", 32, 4))					return FALSE;

	return TRUE;
}

void Sort(void)
{

}

void AllocPlayer(void)
{
	int iPosX = 0;
	int iPosY = 0;
	
	iPosX = rand() % 620 + 10;
	iPosY = rand() % 420 + 50;

	if (playerPool.GetAllocCount() == playerPool.GetUseCount())
	{
		//return;
	}

	CPlayerObject *pTemp = playerPool.Alloc();
	//g_pPlayerObject = playerPool.Alloc();
	memset(pTemp, 0, sizeof(pTemp));

	new (pTemp)CPlayerObject(0, iPosX, iPosY, dfDIRECTION_RIGHT, 1);

	objectList.PushBack(pTemp);

	bPlayer = TRUE;

}

void FreePlayer(void)
{
	int iUseCount = playerPool.GetUseCount();

	if (0 == iUseCount)
	{
		bPlayer = FALSE;
		return;
	}

	// 랜덤하게 해제하기 위해서 
	int iCount = 0;
	int iIndex = playerPool.GetUseCount();

	iIndex = rand() % iIndex;

	CDoubleLinkedList<CBaseObject*>::Iterator iterator;

	for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
	{
		if (iCount == iIndex)
		{
			CPlayerObject *pTemp = (CPlayerObject*)(objectList.PopFront(iterator));

			playerPool.Free(pTemp);

			break;
		}
		else
		{
			iCount++;
		}
	}
}

void MakeTitle(void)
{
	WCHAR szText[64];
	memset(szText, 0, 64);

	int iAllocCount = playerPool.GetAllocCount();
	int iUseCount = playerPool.GetUseCount();

	int iListCount = objectList.GetSize();

	wsprintf(szText, L"Alloc: %d / Use: %d / List : %d", iAllocCount, iUseCount, iListCount);

	SetWindowTextW(g_hWnd, szText);
}

