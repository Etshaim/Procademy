// TCPFighter.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
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


#define dfIP			"192.168.10.49"
#define WM_SOCKET		(WM_USER + 1)


// 전역 변수:
HINSTANCE		hInst;								// 현재 인스턴스입니다.
BOOL			g_bActiveApp;						// 활성화 비활성화 여부

// 전역으로 스크린 DIB 생성		
HWND			g_hWnd;								// 전역 윈도우 핸들. 윈도우 생성 후 핸들 대입
CScreenDib		g_cScreenDib(640, 480, 32);			// 전역으로 ScreenDib 생성
CSpriteDib		g_cSpriteDib(100, 0x00ffffff);		// 100개의 스프라이트 사용, 흰색 칼라키

CPlayerObject	*g_pPlayerObject;
CFrameSkip		g_FrameSkip(50);

CDoubleLinkedList<CBaseObject*> objectList;
 
// 소켓 및 네트워크 처리
SOCKET			g_Socket;
CStreamSQ		g_RecvQ;
CStreamSQ		g_SendQ;
BOOL			g_SendFlag	 = FALSE;
BOOL			g_bConnected = FALSE;
int				g_iSendVal;
int				g_iRecvVal;
WCHAR			g_szIP[16];

int				retval;

void			Update(void);		// 실제 게임 메인 함수
void			UpdateGame(void);	
void			InitialGame(void);	
BOOL			SpriteLoad(void);	// sprite 로드
void			KeyProcess(void);
void			Action(void);
void			Draw(void);
void			SortYPos(void);		// Y 위치로 정렬
void			SortEffect(void);	// Effect 뒤로 빼기

// 윈도우 사이즈
void SetWindowSize();

// IP주소 입력용 다이얼로그
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

	//<	윈속 초기화 및 소켓 생성

	// 윈속 초기화
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return -1;
	}

	// 스프라이트 및 게임 데이터 로드
	InitialGame();

	// 아이피 입력용 다이얼로그 생성
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_ADDR), NULL, (DLGPROC)DialogProc);

	WNDCLASSEX wcex;

	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

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

	g_hWnd = CreateWindow(L"TCP FIGHTER", NULL, WS_OVERLAPPEDWINDOW,
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
			// 게임 처리 함수 호출
			if (TRUE == g_bConnected)
			{
				Update();
			}
		}
	}

	// 플레이어 정리
	//objectList.Clear();

	// 게임 리소스 정리
	//g_cSpriteDib.ReleaseAll();

	closesocket(g_Socket);

	// 윈속 정리
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
			MessageBox(hWnd, L"접속이 종료되었습니다.", NULL, MB_OK);
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
			MessageBox(hWnd, L"접속이 종료되었습니다.", NULL, MB_OK);
			g_bConnected = FALSE;
			PostQuitMessage(0);
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
		// 다이얼로그 생성시 발생(초기화용)
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

void Update(void)
{
	FLOAT fFPS = 0;

	// 키보드 입력처리
	if (TRUE == g_bActiveApp)
	{
		KeyProcess();
	}


	// 객체 액션 처리
	Action();

	//프레임 스킵 & 그리기
	// frame skip
	if (!g_FrameSkip.FrameSkip())
	{
		Draw();
	}

	fFPS = g_FrameSkip.CheckFPS();

	// 플립
	g_cScreenDib.DrawBuffer(g_hWnd);
}

void UpdateGame(void)
{
	FLOAT fFPS;

	KeyProcess();

	Action();

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

	

	// 테스트용 객체들 생성
	/*objectList.PushBack(new CPlayerObject(0, 400, 150, dfDIRECTION_LEFT, 2));
	objectList.PushBack(new CPlayerObject(0, 400, 280, dfDIRECTION_LEFT, 3));
	objectList.PushBack(new CPlayerObject(0, 400, 410, dfDIRECTION_LEFT, 4));*/
}

void KeyProcess(void)
{
	BYTE byAction = dfACTION_STAND;

	// 플레이어 오브젝트가 NULL인지 확인
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
	// 리스트 추가 해야 됨
	//g_pPlayerObject->Action();

	// iterator end가 잘못 된듯
	// head와 tail은 데이터 없는 포인터로 사용하면 될듯
	// 리스트 버전
	CDoubleLinkedList<CBaseObject*>::Iterator iterator;

	for(iterator = objectList.Begin(); iterator != objectList.End(); iterator++)
	{
		// 삭제 기능 구현해야 함
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

	// 맵 출력
	// 제대로 해보고 싶으면 맵 관리 클래스 만들어서 전역이나 싱글톤으로 찍어주면 됨
	g_cSpriteDib.DrawSprite(e_SPRITE::eMAP, 0, 0, bypDest, iDestWidth, iDestHeight, iDestPitch);

	///////
	// 리스트의 모든 객체들에게 Draw 함수를 호출
	
	// 리스트 정렬
	// Y 좌표로 정렬을 하고
	SortYPos();

	// effect는 뒤로 뺀다
	SortEffect();

	// 리스트 버전
	CDoubleLinkedList<CBaseObject*>::Iterator iterator;

	// draw
	for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
	{
		(*iterator)->Draw(bypDest, iDestWidth, iDestHeight, iDestPitch);
	}

	
}

BOOL SpriteLoad(void)
{
	// 스프라이트 로드
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
		// 현재 정렬 대상의 posY값을 기준값으로 함
		WORD wValue = (*i)->GetCurY();
		
		// 만약 현재 정렬 대상이 이전 대상보다 크다면
		// 정렬이 완료된 것이므로 다음 정렬 대상으로 넘어간다
		WORD wPrevValue = i.m_pNode->pPrev->data->GetCurY();

		
		if (wPrevValue <= wValue)
		{
			continue;
		}

		{
			// 처음부터 정렬된 범위내에서 정렬 대상이 들어갈 곳을 찾는다
			CDoubleLinkedList<CBaseObject*>::Iterator j;
			for (j = objectList.Begin(); j != i; ++j)
			{
				
				// 기준 값과 기존 정렬된 값들을 비교해서
				// 정렬 위치를 찾으면 이동하고 끝낸다
				if ( (*j)->GetCurY() > (*i)->GetCurY())
				{
					// i를 j왼쪽에 연결시킴
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
		// effect면 패스
		if ((*i)->GetObjectType() == e_OBJECT_TYPE::eEFFECT)
		{
			continue;
		}

		{
			// 처음부터 정렬된 범위내에서 정렬 대상이 들어갈 곳을 찾는다
			CDoubleLinkedList<CBaseObject*>::Iterator j;
			for (j = objectList.Begin(); j != i; ++j)
			{
				// 기존 정렬된 값 중에 effect를 발견하면 그 왼쪽에 넣는다
				if ((*j)->GetObjectType() == e_OBJECT_TYPE::eEFFECT)
				{
					// i를 j왼쪽에 연결시킴
					objectList.HangLeft(i, j);
					break;
				}
			}
		}
	}
}

