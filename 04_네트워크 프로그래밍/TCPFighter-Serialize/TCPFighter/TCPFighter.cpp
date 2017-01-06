// TCPFighter.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//
#include "stdafx.h"

#include "resource.h"

#include "ScreenDIB.h"
#include "SpriteDIB.h"
#include "BaseObject.h"
#include "PlayerObject.h"
#include "Common.h"
#include "LinkedList.h"
#include "FrameSkip.h"

#include "Protocol.h"
#include "Network.h"
#include "MakePacket.h"

#include "StreamSQ.h"


#define dfIP			L"192.168.10.49"
#define WM_SOCKET		(WM_USER + 1)


// ���� ����:
HINSTANCE		hInst;								// ���� �ν��Ͻ��Դϴ�.
BOOL			g_bActiveApp;						// Ȱ��ȭ ��Ȱ��ȭ ����

// �������� ��ũ�� DIB ����		
HWND			g_hWnd;								// ���� ������ �ڵ�. ������ ���� �� �ڵ� ����
CScreenDib		g_cScreenDib(640, 480, 32);			// �������� ScreenDib ����
CSpriteDib		g_cSpriteDib(100, 0x00ffffff);		// 100���� ��������Ʈ ���, ��� Į��Ű

CPlayerObject	*g_pPlayerObject;
CFrameSkip		g_FrameSkip(50);

CDoubleLinkedList<CBaseObject*> objectList;
 
// ���� �� ��Ʈ��ũ ó��
SOCKET			g_Socket;
CStreamSQ		g_RecvQ;
CStreamSQ		g_SendQ;
BOOL			g_SendFlag	 = FALSE;
BOOL			g_bConnected = FALSE;
int				g_iSendVal;
int				g_iRecvVal;
WCHAR			g_szIP[16];

int				retval;

void			Update(void);		// ���� ���� ���� �Լ�
void			UpdateGame(void);	
void			InitialGame(void);	
BOOL			SpriteLoad(void);	// sprite �ε�
void			KeyProcess(void);
void			Action(void);
void			Draw(void);
void			SortYPos(void);		// Y ��ġ�� ����
void			SortEffect(void);	// Effect �ڷ� ����

// ������ ������
void SetWindowSize();

// IP�ּ� �Է¿� ���̾�α�
BOOL	CALLBACK	DialogProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

// WndProc
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	MSG msg;
	timeBeginPeriod(1);

	//<	���� �ʱ�ȭ �� ���� ����

	// ���� �ʱ�ȭ
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return -1;
	}

	// ��������Ʈ �� ���� ������ �ε�
	InitialGame();

	// ������ �Է¿� ���̾�α� ����
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_ADDR), NULL, (DLGPROC)DialogProc);

	WNDCLASSEX wcex;

	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TCPFIGHTER));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TCPFIGHTER);
	wcex.lpszClassName = L"TCP FIGHTER";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);
	g_hWnd = CreateWindow(L"TCP FIGHTER", L"TCP FIGHTER : Serialized Buffer", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);

	UpdateWindow(g_hWnd);
	
	SetWindowSize();
	
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
			// ���� ó�� �Լ� ȣ��
			if (TRUE == g_bConnected)
			{
				Update();
			}
		}
	}

	// �÷��̾� ����
	//objectList.Clear();

	// ���� ���ҽ� ����
	//g_cSpriteDib.ReleaseAll();

	closesocket(g_Socket);

	// ���� ����
	WSACleanup();

	timeEndPeriod(1);
	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		Connect(hWnd);
		break;

	case WM_SOCKET:
		if (FALSE == NetworkProc(hWnd, wParam, lParam))
		{
			MessageBox(hWnd, L"������ ����Ǿ����ϴ�.", NULL, MB_OK);
			g_bConnected = FALSE;
			exit(1);
		}
		break;
	case WM_ACTIVATEAPP:
		g_bActiveApp = (BOOL)wParam;
		break;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			MessageBox(hWnd, L"������ ����Ǿ����ϴ�.", NULL, MB_OK);
			g_bConnected = FALSE;
			PostQuitMessage(0);
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

void SetWindowSize()
{
	RECT WindowRect;
	WindowRect.top		= 0;
	WindowRect.left		= 0;
	WindowRect.right	= 640;
	WindowRect.bottom	= 480;

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
		SetWindowText(hEditBox, dfIP);
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

void Update(void)
{
	FLOAT fFPS = 0;

	// Ű���� �Է�ó��
	if (TRUE == g_bActiveApp)
	{
		KeyProcess();
	}


	// ��ü �׼� ó��
	Action();

	//������ ��ŵ & �׸���
	// frame skip
	if (!g_FrameSkip.FrameSkip())
	{
		Draw();
	}

	fFPS = g_FrameSkip.CheckFPS();

	// �ø�
	g_cScreenDib.DrawBuffer(g_hWnd);
}

void UpdateGame(void)
{
	FLOAT fFPS;

	KeyProcess();

	Action();

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

	

	// �׽�Ʈ�� ��ü�� ����
	/*objectList.PushBack(new CPlayerObject(0, 400, 150, dfDIRECTION_LEFT, 2));
	objectList.PushBack(new CPlayerObject(0, 400, 280, dfDIRECTION_LEFT, 3));
	objectList.PushBack(new CPlayerObject(0, 400, 410, dfDIRECTION_LEFT, 4));*/
}

void KeyProcess(void)
{
	BYTE byAction = dfACTION_STAND;

	// �÷��̾� ������Ʈ�� NULL���� Ȯ��
	if (NULL == g_pPlayerObject)
	{
		return;
	}

	if (GetAsyncKeyState(VK_UP) & 0X8000)
	{
		byAction = dfMOVE_DIR_UU;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0X8000)
	{
		byAction = dfMOVE_DIR_DD;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0X8000)
	{
		byAction = dfMOVE_DIR_LL;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0X8000)
	{
		byAction = dfMOVE_DIR_RR;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0X8000 && GetAsyncKeyState(VK_UP) & 0X8000)
	{
		byAction = dfMOVE_DIR_LU;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0X8000 && GetAsyncKeyState(VK_DOWN) & 0X8000)
	{
		byAction = dfMOVE_DIR_LD;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0X8000 && GetAsyncKeyState(VK_UP) & 0X8000)
	{
		byAction = dfMOVE_DIR_RU;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0X8000 && GetAsyncKeyState(VK_DOWN) & 0X8000)
	{
		byAction = dfMOVE_DIR_RD;
	}

	// Attack1	q
	if (GetAsyncKeyState(0x51) & 0X8000)
	{
		byAction = dfACTION_ATTACK1;
	}

	// Attack2	w
	if (GetAsyncKeyState(0x57) & 0X8000)
	{
		byAction = dfACTION_ATTACK2;
	}

	// Attack3	e
	if (GetAsyncKeyState(0x45) & 0X8000)
	{
		byAction = dfACTION_ATTACK3;
	}

	g_pPlayerObject->InputAction(byAction);

}

void Action(void)
{
	// ����Ʈ �߰� �ؾ� ��
	//g_pPlayerObject->Action();

	// iterator end�� �߸� �ȵ�
	// head�� tail�� ������ ���� �����ͷ� ����ϸ� �ɵ�
	// ����Ʈ ����
	CDoubleLinkedList<CBaseObject*>::Iterator iterator;

	for(iterator = objectList.Begin(); iterator != objectList.End(); iterator++)
	{
		// ���� ��� �����ؾ� ��
		if (eEND_EFFECT == (*iterator)->GetObjectType())
		{
			iterator = objectList.Erase(iterator);
			continue;
		}
		
		(*iterator)->Action();
	}

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

	///////
	// ����Ʈ�� ��� ��ü�鿡�� Draw �Լ��� ȣ��
	
	// ����Ʈ ����
	// Y ��ǥ�� ������ �ϰ�
	SortYPos();

	// effect�� �ڷ� ����
	SortEffect();

	// ����Ʈ ����
	CDoubleLinkedList<CBaseObject*>::Iterator iterator;

	// draw
	for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
	{
		(*iterator)->Draw(bypDest, iDestWidth, iDestHeight, iDestPitch);
	}

	
}

BOOL SpriteLoad(void)
{
	// ��������Ʈ �ε�
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::eMAP, L"Sprite_Data\\_Map.bmp", 0, 0))							return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L01, L"Sprite_Data\\Stand_L_01.bmp", 71, 90))		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L02, L"Sprite_Data\\Stand_L_02.bmp", 71, 90))		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_L03, L"Sprite_Data\\Stand_L_03.bmp", 71, 90))		return FALSE;

	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R01, L"Sprite_Data\\Stand_R_01.bmp", 71, 90))		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R02, L"Sprite_Data\\Stand_R_02.bmp", 71, 90))		return FALSE;
	if (!g_cSpriteDib.LoadDibSprite(e_SPRITE::ePLAYER_STAND_R03, L"Sprite_Data\\Stand_R_03.bmp", 71, 90))		return FALSE;

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

void SortYPos(void)
{
	CDoubleLinkedList<CBaseObject*>::Iterator i = objectList.Begin();
	
	i++;
	
	for ( ; i != objectList.End(); ++i)
	{
		// ���� ���� ����� posY���� ���ذ����� ��
		WORD wValue = (*i)->GetCurY();
		
		// ���� ���� ���� ����� ���� ��󺸴� ũ�ٸ�
		// ������ �Ϸ�� ���̹Ƿ� ���� ���� ������� �Ѿ��
		WORD wPrevValue = i.m_pNode->pPrev->data->GetCurY();

		
		if (wPrevValue <= wValue)
		{
			continue;
		}

		{
			// ó������ ���ĵ� ���������� ���� ����� �� ���� ã�´�
			CDoubleLinkedList<CBaseObject*>::Iterator j;
			for (j = objectList.Begin(); j != i; ++j)
			{
				
				// ���� ���� ���� ���ĵ� ������ ���ؼ�
				// ���� ��ġ�� ã���� �̵��ϰ� ������
				if ( (*j)->GetCurY() > (*i)->GetCurY())
				{
					// i�� j���ʿ� �����Ŵ
					objectList.HangLeft(i, j);
					break;
				}
			}
		}
	}
	
	
}

void SortEffect(void)
{
	CDoubleLinkedList<CBaseObject*>::Iterator i = objectList.Begin();

	i++;

	for (; i != objectList.End(); ++i)
	{
		// effect�� �н�
		if ((*i)->GetObjectType() == e_OBJECT_TYPE::eEFFECT)
		{
			continue;
		}

		{
			// ó������ ���ĵ� ���������� ���� ����� �� ���� ã�´�
			CDoubleLinkedList<CBaseObject*>::Iterator j;
			for (j = objectList.Begin(); j != i; ++j)
			{
				// ���� ���ĵ� �� �߿� effect�� �߰��ϸ� �� ���ʿ� �ִ´�
				if ((*j)->GetObjectType() == e_OBJECT_TYPE::eEFFECT)
				{
					// i�� j���ʿ� �����Ŵ
					objectList.HangLeft(i, j);
					break;
				}
			}
		}
	}
}

