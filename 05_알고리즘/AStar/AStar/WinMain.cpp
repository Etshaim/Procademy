// AStar.cpp : 응용 프로그램에 대한 진입점을 정의합니다.

#include "stdafx.h"
#include "AStar.h"
#include "Resource.h"
#include "LinkedList.h"

#define	dfTILE_WIDTH			15
#define	dfTILE_HEIGHT			15

#define dfTILE_WIDTH_COUNT		50
#define	dfTILE_HEIGHT_COUNT		40

#define	dfTILE_NONE				0
#define dfTILE_OBSTACLE			1
#define dfTILE_START			2
#define dfTILE_END				3

// 전역 변수:
HINSTANCE	hInst;
HWND		g_hWnd;

AStar		g_clAStar;
BOOL		g_bSearching;

// 타일로 이루어진 맵, 0 갈 수 있는길 1 장애물
BYTE		g_byTile[dfTILE_HEIGHT_COUNT][dfTILE_WIDTH_COUNT];
LONG		g_loStartX;
LONG		g_loStartY;
LONG		g_loEndX;
LONG		g_loEndY;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// 그리드 그리기
void DrawGrid(HDC memDC, RECT rect);

// 타일 그리기
void DrawTile(HDC memDC);

// 초기화
void Init(void);

// 찾기
BOOL PathFinder(LONG loStartX, LONG loStartY, LONG loEndX, LONG loEndY, BOOL bNewserch);

// 갈 수 있는 길인지 체크
BOOL CheckTileAttribute(LONG loPosX, LONG loPosY);

// 찾은 길 그리기
void DrawPath(HDC memDC);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	MSG				msg;
	WNDCLASSEXW		wcex;

	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTAR));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCEW(IDC_ASTAR);
	wcex.lpszClassName	= L"AStar";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	g_hWnd = CreateWindowW(L"AStar", NULL, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd)
	{
return FALSE;
	}

	Init();

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);



	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

void DrawGrid(HDC hdc, RECT rect)
{
	POINT pnt;
	pnt.x = rect.left;
	pnt.y = rect.top;

	//< CreatePen 이용
	//HPEN hMyPen;
	//HPEN hOldPen;
	//hMyPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	//hOldPen = (HPEN)SelectObject(hdc, (HPEN)hMyPen);
	//SetDCBrushColor(hdc, RGB(255, 0, 0));
	//SelectObject(hdc, (HPEN)hOldPen);
	//DeleteObject(hMyPen);

	//< DC pen 이용
	SetDCPenColor(hdc, RGB(222, 222, 222));

	SelectObject(hdc, (HPEN)GetStockObject(DC_PEN));

	// 세로로 그리기
	for (pnt.x = rect.left; pnt.x < rect.right; pnt.x += dfTILE_WIDTH)
	{
		MoveToEx(hdc, pnt.x, rect.top, NULL);
		LineTo(hdc, pnt.x, rect.bottom);
	}

	// 가로로 그리기
	for (pnt.y = rect.top; pnt.y < rect.bottom; pnt.y += dfTILE_HEIGHT)
	{
		MoveToEx(hdc, rect.left, pnt.y, NULL);
		LineTo(hdc, rect.right, pnt.y);
	}
}

// 전체 타일 그리기
void DrawTile(HDC memDC)
{
	// 배열 좌표
	POINT pntTile;

	// 맵 좌표
	POINT pntWindow;

	// tile
	RECT rectTile;

	//< DC pen 이용
	SetDCPenColor(memDC, RGB(222, 222, 222));

	SelectObject(memDC, (HPEN)GetStockObject(DC_PEN));

	// 타일 상태를 일단 쭉 그리고
	for (pntTile.y = 0; pntTile.y < dfTILE_HEIGHT_COUNT; (pntTile.y)++)
	{
		for (pntTile.x = 0; pntTile.x < dfTILE_WIDTH_COUNT; (pntTile.x)++)
		{
			BYTE byTileType = g_byTile[pntTile.y][pntTile.x];

			// 타일의 실제 윈도우 좌표(왼쪽 상단 꼭지점) 구하기
			pntWindow.x = dfTILE_WIDTH * pntTile.x;
			pntWindow.y = dfTILE_HEIGHT * pntTile.y;

			// 타일 rect 좌표 구하기
			rectTile.top	= pntWindow.y;
			rectTile.bottom = pntWindow.y + dfTILE_HEIGHT;
			rectTile.left	= pntWindow.x;
			rectTile.right	= pntWindow.x + dfTILE_WIDTH;

			switch (byTileType)
			{
			case dfTILE_NONE:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;

				hMyBrush	= CreateSolidBrush(RGB(255, 255, 255));
				hOldBrush	= (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);

				if (g_clAStar.m_clOpenList != nullptr)
				{
					// closelist도 그림
					CDoubleLinkedList<AStar::st_NODE*>::Iterator iter;


					for (iter = g_clAStar.m_clOpenList->Begin(); iter != g_clAStar.m_clOpenList->End(); iter++)
					{
						if ((*iter)->loPosX == pntTile.x
							&& (*iter)->loPosY == pntTile.y)
						{
							SelectObject(memDC, (HBRUSH)hOldBrush);

							DeleteObject(hMyBrush);

							hMyBrush = CreateSolidBrush(RGB(152, 251, 152));
							hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);
						}
					}
				}

				if (g_clAStar.m_clCloseList != nullptr)
				{
					// closelist도 그림
					CDoubleLinkedList<AStar::st_NODE*>::Iterator iter;


					for (iter = g_clAStar.m_clCloseList->Begin(); iter != g_clAStar.m_clCloseList->End(); iter++)
					{
						if ((*iter)->loPosX == pntTile.x
							&& (*iter)->loPosY == pntTile.y)
						{
							SelectObject(memDC, (HBRUSH)hOldBrush);

							DeleteObject(hMyBrush);

							hMyBrush = CreateSolidBrush(RGB(175, 238, 238));
							hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);
						}
					}
				}
				 
				
				
				 hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);


				 Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				 SelectObject(memDC, (HBRUSH)hOldBrush);

				 DeleteObject(hMyBrush);
				//< DC Brush 이용
				//SetDCBrushColor(memDC, RGB(0, 255, 0));
				//Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);
				//FillRect(memDC, &rectTile, (HBRUSH)GetStockObject(DC_BRUSH));

			}
			break;

			case dfTILE_OBSTACLE:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;
				hMyBrush = CreateSolidBrush(RGB(128, 128, 128));
				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);


				//PatBlt(memDC, 0, 0, dfTILE_WIDTH_COUNT* dfTILE_WIDTH, dfTILE_LENGTH_COUNT*dfTILE_HEIGHT, WHITENESS);
				//SetROP2(memDC, R2_COPYPEN);

				Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				SelectObject(memDC, (HBRUSH)hOldBrush);

				DeleteObject(hMyBrush);
				//< DC Brush 이용
				//SetDCBrushColor(memDC, RGB(100, 100, 100));
				//FillRect(memDC, &rectTile, (HBRUSH)GetStockObject(DC_BRUSH));

			}
			break;

			case dfTILE_START:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;
				hMyBrush = CreateSolidBrush(RGB(0, 221, 0));
				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);

				//SelectObject(memDC, (HPEN)hOldBrush);
				Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				SelectObject(memDC, (HBRUSH)hOldBrush);

				DeleteObject(hMyBrush);


				//< DC Brush 이용
				//SetDCBrushColor(memDC, RGB(0, 255, 0));

				//FillRect(memDC, &rectTile, (HBRUSH)GetStockObject(DC_BRUSH));

				//SelectObject(hdc, (HBRUSH)hOldBrush);

				//DeleteObject(hMyBrush);
			}
			break;
			case dfTILE_END:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;
				hMyBrush = CreateSolidBrush(RGB(238, 68, 0));
				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);

				Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				SelectObject(memDC, (HBRUSH)hOldBrush);

				DeleteObject(hMyBrush);
				//< DC Brush 이용
				//SetDCBrushColor(memDC, RGB(255, 0, 0));

				//FillRect(memDC, &rectTile, (HBRUSH)GetStockObject(DC_BRUSH));
			}
			break;
			}
		}
	}

	
}

void Init(void)
{
	POINT pntTile;

	for (pntTile.y = 0; pntTile.y < dfTILE_HEIGHT_COUNT; (pntTile.y)++)
	{
		for (pntTile.x = 0; pntTile.x < dfTILE_WIDTH_COUNT; (pntTile.x)++)
		{
			g_byTile[pntTile.y][pntTile.x] = dfTILE_NONE;
		}
	}

	// 시작점과 도착점이 같은 경우(초기 상태)의 경우만 지정
	
	if (g_loStartX == g_loEndX && g_loStartY == g_loEndY)
	{
		g_byTile[10][10] = dfTILE_START;
		g_byTile[10][20] = dfTILE_END;

		g_loStartX = 10;
		g_loStartY = 10;
		g_loEndX = 20;
		g_loEndY = 10;

		g_clAStar.m_loStartX = g_loStartX;
		g_clAStar.m_loStartY = g_loStartY;

		g_clAStar.m_loEndX = g_loEndX;
		g_clAStar.m_loEndY = g_loEndY;
	}

	// 아니면
	else
	{
		g_byTile[g_loStartY][g_loStartX] = dfTILE_START;
		g_byTile[g_loEndY][g_loEndX]	 = dfTILE_END;
	}

	
	
	
}

BOOL PathFinder(LONG loStartX, LONG loStartY, LONG loEndX, LONG loEndY, BOOL bNewserch)
{
	AStar::st_NODE *stpNode;

	if (false == bNewserch)
	{
		g_bSearching = TRUE;
		g_clAStar.m_clOpenList->Clear();
		g_clAStar.m_clCloseList->Clear();

		g_clAStar.CreateNode(loStartX, loStartY, nullptr);
	}

	if (g_clAStar.m_stpCompleteNode != nullptr)
	{
		stpNode = g_clAStar.m_stpCompleteNode;
	}

	else
	{
		// Openlist에서 f값이 가장 작은 노드 찾기
		stpNode = g_clAStar.GetNodeMinF();
		if (nullptr == stpNode)
		{
			return true;
		}
	}

	//1.찾은 node가 목적지인지 확인
	if (stpNode->loPosX == g_loEndX
		&& stpNode->loPosY == g_loEndY)
	{
		g_clAStar.m_stpCompleteNode = stpNode;

		return true;
	}

	// 목적지가 아니면 openlist에서 삭제
	CDoubleLinkedList<AStar::st_NODE*>::Iterator iter;

	for (iter = g_clAStar.m_clOpenList->Begin(); iter != g_clAStar.m_clOpenList->End(); iter++)
	{
		if ((*iter) == stpNode)
		{
			g_clAStar.m_clOpenList->Erase(iter);
		}
	}

	//2. close에 넣기
	g_clAStar.m_clCloseList->PushBack(stpNode);

	//3. 8방향으로 방향별 속성 체크 후 노드 생성
	//맵 영역을 벗어나거나 노드를 만들 수 없으면 못가는 지역임
	LONG loPosX = stpNode->loPosX;
	LONG loPosY = stpNode->loPosY;

	// LL
	if ( TRUE == CheckTileAttribute(loPosX - 1, loPosY))
	{
		g_clAStar.CreateNode(loPosX - 1, loPosY, stpNode);
	}

	// LU
	if (TRUE == CheckTileAttribute(loPosX - 1, loPosY - 1))
	{
		g_clAStar.CreateNode(loPosX - 1, loPosY - 1, stpNode, true);
	}

	// UU
	if (TRUE == CheckTileAttribute(loPosX, loPosY - 1))
	{
		g_clAStar.CreateNode(loPosX, loPosY - 1, stpNode);
	}

	// RU
	if (TRUE == CheckTileAttribute(loPosX + 1, loPosY - 1))
	{
		g_clAStar.CreateNode(loPosX + 1, loPosY - 1, stpNode, TRUE);
	}

	// RR
	if (TRUE == CheckTileAttribute(loPosX + 1, loPosY))
	{
		g_clAStar.CreateNode(loPosX + 1, loPosY, stpNode);
	}
	// RD
	if (TRUE == CheckTileAttribute(loPosX + 1, loPosY + 1))
	{
		g_clAStar.CreateNode(loPosX + 1, loPosY + 1, stpNode, TRUE);
	}
	// DD
	if (TRUE == CheckTileAttribute(loPosX, loPosY + 1))
	{
		g_clAStar.CreateNode(loPosX, loPosY + 1, stpNode);
	}
	// LD
	if (TRUE == CheckTileAttribute(loPosX - 1, loPosY + 1))
	{
		g_clAStar.CreateNode(loPosX - 1, loPosY + 1, stpNode, TRUE);
	}

	return FALSE;
}

BOOL CheckTileAttribute(LONG loPosX, LONG loPosY)
{
	// 맵 영역 안에 들어있고
	if ( 0 <= loPosX && loPosX < dfTILE_WIDTH_COUNT
		&&	0 <= loPosY  && loPosY < dfTILE_HEIGHT_COUNT )
	{
		// 장애물이 아니라면 
		if (g_byTile[loPosY][loPosX] != dfTILE_OBSTACLE)
		{
			return true;
		}
	}
	return false;
}

void DrawPath(HDC memDC)
{
	if (nullptr == g_clAStar.m_stpCompleteNode)
	{
		return;
	}

	POINT pntWindow;
	POINT pntTile;

	//< CreatePen 이용
	HPEN hMyPen;
	HPEN hOldPen;
	hMyPen	= CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
	
	hOldPen = (HPEN)SelectObject(memDC, (HPEN)hMyPen);

	AStar::st_NODE* stpNode = g_clAStar.m_stpCompleteNode;

	pntTile.x = stpNode->loPosX;
	pntTile.y = stpNode->loPosY;

	pntWindow.x = (pntTile.x * dfTILE_WIDTH) + dfTILE_WIDTH / 2;
	pntWindow.y = (pntTile.y * dfTILE_HEIGHT) + dfTILE_HEIGHT / 2;
	MoveToEx(memDC, pntWindow.x, pntWindow.y, NULL);

	while( stpNode->pParent != nullptr)
	{
		stpNode = stpNode->pParent;

		pntTile.x = stpNode->loPosX;
		pntTile.y = stpNode->loPosY;

		pntWindow.x = (pntTile.x * dfTILE_WIDTH) + dfTILE_WIDTH / 2;
		pntWindow.y = (pntTile.y * dfTILE_HEIGHT) + dfTILE_HEIGHT / 2;

		LineTo(memDC, pntWindow.x, pntWindow.y);
	}

	hOldPen = (HPEN)SelectObject(memDC, (HPEN)hMyPen);

	DeleteObject(hMyPen);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC				hdc;
	static	HDC		memDC;
	static	HBITMAP	hBitMap;
	PAINTSTRUCT		ps;
	static RECT			rect;

	static BOOL		bClick;
	static POINT	pntMouse;
	static POINT	pntTilePos;
	static BYTE		byTileType;
	static BYTE		byDrawMode;


	switch (message)
	{
	case WM_CHAR:
		switch (wParam)
		{
			// backspace
		case VK_RETURN:
			g_clAStar.m_bSearch = false;
			g_clAStar.m_clOpenList->Clear();
			g_clAStar.m_clCloseList->Clear();
			g_clAStar.m_stpCompleteNode = nullptr;
			SetTimer(hWnd, 1, NULL, NULL);
			break;
		}

	case WM_TIMER:
	{
		switch ((wParam))
		{
		case 1:
			
			if (true == PathFinder(g_loStartX, g_loStartY, g_loEndX, g_loEndY, g_bSearching))
			{
				
				KillTimer(hWnd, 1);
			}
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;
	}
	break;

		// 마우스 좌클릭하면 
	case WM_LBUTTONDOWN:
	{
		//InvalidateRect(hWnd, &rect, FALSE);

		// 좌표 구하기
		pntMouse.x = LOWORD(lParam);
		pntMouse.y = HIWORD(lParam);

		if (pntMouse.x < dfTILE_WIDTH_COUNT * dfTILE_WIDTH
			&& pntMouse.y < dfTILE_HEIGHT_COUNT * dfTILE_HEIGHT)
		{
			if (bClick == FALSE)
			{
				// 클릭 상태로
				bClick = TRUE;

				// 클릭한 타일 파악
				pntTilePos.x = (LONG)(pntMouse.x / dfTILE_WIDTH);
				pntTilePos.y = (LONG)(pntMouse.y / dfTILE_HEIGHT);

				byTileType = g_byTile[pntTilePos.y][pntTilePos.x];

				switch (byTileType)
				{
				case dfTILE_NONE:
				{
					byDrawMode = dfTILE_OBSTACLE;
					g_byTile[pntTilePos.y][pntTilePos.x] = dfTILE_OBSTACLE;
				}
				break;

				case dfTILE_OBSTACLE:
				{
					byDrawMode = dfTILE_NONE;
					g_byTile[pntTilePos.y][pntTilePos.x] = dfTILE_NONE;
				}
				break;

				case dfTILE_START:
				{
					byDrawMode = dfTILE_START;
				}
				break;

				case dfTILE_END:
				{
					byDrawMode = dfTILE_END;
				}
				break;
				}

				InvalidateRect(hWnd, &rect, FALSE);

				//	// 1. 시작점이 경우
				//	// 시작점을 움직임
				//	if (g_clAStar.stpStart->byPosX == pntTilePos.x &&
				//		g_clAStar.stpStart->byPosY == pntTilePos.y)
				//	{
				//		
				//		break;
				//	}
				//	// 2. 목적지인 경우
				//	// 목적지를 움직임
				//	else if (g_clAStar.stpEnd->byPosX == pntTilePos.x &&
				//		g_clAStar.stpEnd->byPosY == pntTilePos.y)
				//	{
				//		byDrawMode = dfTILE_END;
				//		break;
				//	}
				//	// 3. 갈 수 있는 길
				//	// 장애물로 바꿈
				//	else if (byTileType == dfTILE_NONE)
				//	{
				//		byDrawMode = dfTILE_OBSTACLE;
				//		g_byTile[pntTilePos.y][pntTilePos.x] = dfTILE_OBSTACLE;
				//	}

				//	// 4. 장애물인 경우
				//	// 갈 수 있는 길로 바꿈
				//	else if (byTileType == dfTILE_OBSTACLE)
				//	{
				//		byDrawMode = dfTILE_NONE;
				//		g_byTile[pntTilePos.y][pntTilePos.x] = dfTILE_NONE;
				//	}
				//	InvalidateRect(hWnd, &rect, TRUE);
			}
		}
	}
	break;


	case WM_LBUTTONUP:
	{
		bClick = FALSE;
	}
	break;

	case WM_RBUTTONDOWN:
	{
		Init();

		InvalidateRect(hWnd, &rect, FALSE);
	}
	break;

	case WM_MOUSEMOVE:
	{
		// 좌표 구하기
		pntMouse.x = LOWORD(lParam);
		pntMouse.y = HIWORD(lParam);

		if (pntMouse.x < dfTILE_WIDTH_COUNT * dfTILE_WIDTH
			&& pntMouse.y < dfTILE_HEIGHT_COUNT * dfTILE_HEIGHT)
		{
			if (TRUE == bClick)
			{
				// 클릭한 타일 파악
				pntTilePos.x = (LONG)(pntMouse.x / dfTILE_WIDTH);
				pntTilePos.y = (LONG)(pntMouse.y / dfTILE_HEIGHT);

				switch (byDrawMode)
				{
				case dfTILE_NONE:
				{
					if (dfTILE_OBSTACLE == g_byTile[pntTilePos.y][pntTilePos.x])
					{
						g_byTile[pntTilePos.y][pntTilePos.x] = dfTILE_NONE;
					}

				}
				break;

				case dfTILE_OBSTACLE:
				{
					if (dfTILE_NONE == g_byTile[pntTilePos.y][pntTilePos.x])
					{
						g_byTile[pntTilePos.y][pntTilePos.x] = dfTILE_OBSTACLE;
					}
				}
				break;

				case dfTILE_START:
				{
					if (dfTILE_NONE == g_byTile[pntTilePos.y][pntTilePos.x])
					{
						// 기존의 start는 none으로
						g_byTile[g_loStartY][g_loStartX] = dfTILE_NONE;

						// 새로 start 지정
						g_loStartX = pntTilePos.x;
						g_loStartY = pntTilePos.y;

						g_byTile[g_loStartY][g_loStartX] = dfTILE_START;
						//g_byTile[pntTilePos.y][pntTilePos.x] = dfTILE_START;

						// 새로 탐색하게끔
						g_bSearching = FALSE;
					}
				}
				break;

				case dfTILE_END:
				{
					if (dfTILE_NONE == g_byTile[pntTilePos.y][pntTilePos.x])
					{
						// 기존의 end는 none으로
						g_byTile[g_loEndY][g_loEndX] = dfTILE_NONE;

						// 새로 end지정
						g_loEndX = pntTilePos.x;
						g_loEndY = pntTilePos.y;

						g_clAStar.m_loEndX = g_loEndX;
						g_clAStar.m_loEndY = g_loEndY;

						g_byTile[pntTilePos.y][pntTilePos.x] = dfTILE_END;

						//새로 탐색하게끔
						g_bSearching = FALSE;
					}
				}
				break;
				}

				InvalidateRect(hWnd, &rect, FALSE);
			}
		}
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rect);

		if (NULL == memDC)
		{
			memDC = CreateCompatibleDC(hdc);
		}
		if (NULL == hBitMap)
		{
			hBitMap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			SelectObject(memDC, hBitMap);
		}

		SetBkMode(memDC, TRANSPARENT);
		SetTextColor(memDC, RGB(255, 255, 255));

		FillRect(memDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		// 타일 그리기
		DrawTile(memDC);

		// 길 그리기
		DrawPath(memDC);
		// 그리드 그리기
		//DrawGrid(memDC, rect);

		BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
