// TCPFighter.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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

// �������� ��ũ�� DIB ����		
HWND			g_hWnd;								// ���� ������ �ڵ�. ������ ���� �� �ڵ� ����
CScreenDib		g_cScreenDib(640, 480, 32);			// �������� ScreenDib ����
CSpriteDib		g_cSpriteDib(100, 0x00ffffff);		// 100���� ��������Ʈ ���, ��� Į��Ű

CBaseObject		*g_pPlayerObject;
CFrameSkip		g_FrameSkip(50);

CDoubleLinkedList<CBaseObject*> objectList;
CMemoryPool<CPlayerObject> playerPool(10, true);

BOOL			bPlayer;

// ���� ����:
HINSTANCE	hInst;									// ���� �ν��Ͻ��Դϴ�.
TCHAR		szTitle[MAX_LOADSTRING];				// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR		szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// 
void	Update(void);		// ���� ���� ���� �Լ�
void	UpdateGame(void);	//
void	InitialGame(void);	//
BOOL	SpriteLoad(void);	// sprite �ε�
void	KeyProcess(void);
void	Action(void);
void	Draw(void);
void	Sort(void);
void	AllocPlayer(void);
void	FreePlayer(void);
void	MakeTitle(void);

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TCPFIGHTER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
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

			// ���� ó�� �Լ� ȣ��
			Update();
		}
	}
	//// �⺻ �޽��� �����Դϴ�.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TCPFIGHTER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TCPFIGHTER);
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
   //HWND hWnd;

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
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
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
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

void Update(void)
{
	UpdateGame();

	//BYTE	*bypDest	= g_cScreenDib.GetDibBuffer();
	//int		iDestWidth	= g_cScreenDib.GetWidth();
	//int		iDestHeight = g_cScreenDib.GetHeight();
	//int		iDestPitch	= g_cScreenDib.GetPitch();

	//// 0 ~ 4�� ��������Ʈ�� �� ��ǥ�� ���
	//g_cSpriteDib.DrawSprite(0, 100, 100, bypDest, iDestWidth, iDestHeight, iDestPitch);
	//g_cSpriteDib.DrawSprite(1, 150, 150, bypDest, iDestWidth, iDestHeight, iDestPitch);
	//g_cSpriteDib.DrawSprite(2, 150, 200, bypDest, iDestWidth, iDestHeight, iDestPitch);
	//g_cSpriteDib.DrawSprite(3, 150, 250, bypDest, iDestWidth, iDestHeight, iDestPitch);

	//////----------------------------------------------------
	////// ���� �����Ϳ� �׸��� �׸���.
	//////
	////// ��������Ʈ ��º�
	//////----------------------------------------------------
	////BYTE byGrayColor = 0;

	////for (int iCount = 0; iCount < 480; ++iCount)
	////{
	////	memset(bypDest, byGrayColor, 640 * 4);
	////	bypDest += iDestPitch;
	////	byGrayColor++;
	////}

	////----------------------------------------------------
	//// Dib ������ ������ ȭ������ ���
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
	// frame skip ���
	//------------------------------
	if (!g_FrameSkip.FrameSkip())
		Draw();


	fFPS = g_FrameSkip.CheckFPS();


	g_cScreenDib.DrawBuffer(g_hWnd);
}

void InitialGame(void)
{
	// ��������Ʈ �ε�
	SpriteLoad();

	// CPlayerObject ����
	// ( int objectType, int iCurX, int iCurY )
	//g_pPlayerObject = new CPlayerObject(0, 200, 200, dfDIRECTION_RIGHT, 1);

	//objectList.PushBack(g_pPlayerObject);

	// �׽�Ʈ�� ��ü�� ����
	/*objectList.PushBack(new CPlayerObject(0, 400, 150, dfDIRECTION_LEFT, 2));
	objectList.PushBack(new CPlayerObject(0, 400, 280, dfDIRECTION_LEFT, 3));
	objectList.PushBack(new CPlayerObject(0, 400, 410, dfDIRECTION_LEFT, 4));*/
}

void KeyProcess(void)
{

	DWORD dwAction = dfACTION_STAND;

	// �׽�Ʈ ��
	// �����̽� ������ STOP���·�
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
		// �÷��̾� ��ü���� �Էµ� ������ �˷���
		(*iterator)->ActionInput(dwAction);
	}
	
}

void Action(void)
{
	// ����Ʈ �߰� �ؾ� ��
	//g_pPlayerObject->Action();

	// iterator end�� �߸� �ȵ�
	// head�� tail�� ������ ���� �����ͷ� ����ϸ� �ɵ�
	// ����Ʈ ����
	CDoubleLinkedList<CBaseObject*>::Iterator iterator;

	for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
	{
		// ���� ��� �����ؾ� ��
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

	// �� ���
	// ����� �غ��� ������ �� ���� Ŭ���� ���� �����̳� �̱������� ����ָ� ��
	g_cSpriteDib.DrawSprite(e_SPRITE::eMAP, 0, 0, bypDest, iDestWidth, iDestHeight, iDestPitch);

	if (FALSE == bPlayer)
	{
		return;
	}

	///////
	// ����Ʈ�� ��� ��ü�鿡�� Draw �Լ��� ȣ��
	// ����Ʈ �߰��ؾ� ��
	//g_pPlayerObject->Draw(bypDest, iDestWidth, iDestHeight, iDestPitch);

	// ����Ʈ ����
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
	// ��������Ʈ �ε�
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

	// �����ϰ� �����ϱ� ���ؼ� 
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

