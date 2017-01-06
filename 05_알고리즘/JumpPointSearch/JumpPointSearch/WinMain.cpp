#include "stdafx.h"
#include "Resource.h"
#include "JumpPointSearch.h"
#include "LinkedList.h"

#define	dfTILE_WIDTH			15
#define	dfTILE_HEIGHT			15

#define dfTILE_WIDTH_COUNT		50
#define	dfTILE_HEIGHT_COUNT		40

#define	dfTILE_NONE				0
#define dfTILE_OBSTACLE			1
#define dfTILE_START			2
#define dfTILE_END				3

// ���� ����:
HINSTANCE	hInst;
HWND		g_hWnd;

JumpPointSearch		g_clJPS;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

// Ÿ�� �׸���
void DrawTile(HDC memDC);

// �� �׸���
void DrawPath(HDC memDC);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_JUMPPOINTSEARCH));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_JUMPPOINTSEARCH);
	wcex.lpszClassName = L"Jump Point Search";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	HWND hWnd = CreateWindowW(L"Jump Point Search", NULL, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_JUMPPOINTSEARCH));

	MSG msg;

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC				hdc;
	static	HDC		memDC;
	static	HBITMAP	hBitMap;
	PAINTSTRUCT		ps;
	static RECT		rect;

	static BOOL		bClick;
	static POINT	pntMouse;
	static POINT	pntTilePos;
	static BYTE		byTileType;
	static BYTE		byDrawMode;
	static BOOL		bSearching;

	switch (message)
	{
	case WM_CHAR:
	{
		switch (wParam)
		{
		case VK_RETURN:
			
			if (FALSE == g_clJPS.PathFinder())
			{
				g_clJPS.m_bNewSearch = TRUE;
			}
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case VK_SPACE:
			g_clJPS.ReStart();
			g_clJPS.m_bNewSearch = TRUE;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		}
	}
	break;

	case WM_TIMER:
	{
		switch (wParam)
		{
		case 1:
			if (FALSE == g_clJPS.PathFinder())
			{
				KillTimer(hWnd, 1);
				g_clJPS.m_bNewSearch = TRUE;
			}
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
	}
	break;

	// ���콺 ��Ŭ���ϸ� 
	case WM_LBUTTONDOWN:
	{
		// ��ǥ ���ϱ�
		pntMouse.x = LOWORD(lParam);
		pntMouse.y = HIWORD(lParam);

		if (pntMouse.x < dfTILE_WIDTH_COUNT * dfTILE_WIDTH
			&& pntMouse.y < dfTILE_HEIGHT_COUNT * dfTILE_HEIGHT)
		{
			if (bClick == FALSE)
			{
				// Ŭ�� ���·�
				bClick = TRUE;

				// Ŭ���� Ÿ�� �ľ�
				pntTilePos.x = (LONG)(pntMouse.x / dfTILE_WIDTH);
				pntTilePos.y = (LONG)(pntMouse.y / dfTILE_HEIGHT);

				byTileType = g_clJPS.m_byMap[pntTilePos.y][pntTilePos.x];

				switch (byTileType)
				{
				case dfTILE_NONE:
				{
					byDrawMode = dfTILE_OBSTACLE;
					g_clJPS.m_byMap[pntTilePos.y][pntTilePos.x] = dfTILE_OBSTACLE;
					g_clJPS.SetTileColor(pntTilePos.x, pntTilePos.y, JumpPointSearch::enOBSTACLE);
				}
				break;

				case dfTILE_OBSTACLE:
				{
					byDrawMode = dfTILE_NONE;
					g_clJPS.m_byMap[pntTilePos.y][pntTilePos.x] = dfTILE_NONE;
					g_clJPS.SetTileColor(pntTilePos.x, pntTilePos.y, JumpPointSearch::enNONE);
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
		g_clJPS.RemoveOBSTACLE();
		InvalidateRect(hWnd, NULL, FALSE);
	}
	break;

	case WM_MOUSEMOVE:
	{
		// ��ǥ ���ϱ�
		pntMouse.x = LOWORD(lParam);
		pntMouse.y = HIWORD(lParam);

		if (pntMouse.x < dfTILE_WIDTH_COUNT * dfTILE_WIDTH
			&& pntMouse.y < dfTILE_HEIGHT_COUNT * dfTILE_HEIGHT)
		{
			if (TRUE == bClick)
			{
				// Ŭ���� Ÿ�� �ľ�
				pntTilePos.x = (LONG)(pntMouse.x / dfTILE_WIDTH);
				pntTilePos.y = (LONG)(pntMouse.y / dfTILE_HEIGHT);

				switch (byDrawMode)
				{
				case dfTILE_NONE:
				{
					if (dfTILE_OBSTACLE == g_clJPS.m_byMap[pntTilePos.y][pntTilePos.x])
					{
						g_clJPS.m_byMap[pntTilePos.y][pntTilePos.x] = dfTILE_NONE;
						g_clJPS.SetTileColor(pntTilePos.x, pntTilePos.y, JumpPointSearch::enNONE);
					}

				}
				break;

				case dfTILE_OBSTACLE:
				{
					if (dfTILE_NONE == g_clJPS.m_byMap[pntTilePos.y][pntTilePos.x])
					{
						g_clJPS.m_byMap[pntTilePos.y][pntTilePos.x] = dfTILE_OBSTACLE;
						g_clJPS.SetTileColor(pntTilePos.x, pntTilePos.y, JumpPointSearch::enOBSTACLE);
					}
				}
				break;

				case dfTILE_START:
				{
					if (dfTILE_NONE == g_clJPS.m_byMap[pntTilePos.y][pntTilePos.x])
					{
						g_clJPS.SetStartPoint(pntTilePos.x, pntTilePos.y);
						
						g_clJPS.m_bNewSearch = TRUE;
					}
				}
				break;

				case dfTILE_END:
				{
					if (dfTILE_NONE == g_clJPS.m_byMap[pntTilePos.y][pntTilePos.x])
					{
						g_clJPS.SetEndPoint(pntTilePos.x, pntTilePos.y);

						g_clJPS.m_bNewSearch = TRUE;
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

		// Ÿ�� �׸���
		DrawTile(memDC);

		// �� �׸���
		DrawPath(memDC);

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

void DrawTile(HDC memDC)
{
	// �迭 ��ǥ
	POINT pntTile;

	// �� ��ǥ
	POINT pntWindow;

	// tile
	RECT rectTile;

	//< DC pen �̿�
	SetDCPenColor(memDC, RGB(222, 222, 222));

	SelectObject(memDC, (HPEN)GetStockObject(DC_PEN));

	// Ÿ�� ���¸� �ϴ� �� �׸���
	for (pntTile.y = 0; pntTile.y < dfTILE_HEIGHT_COUNT; (pntTile.y)++)
	{
		for (pntTile.x = 0; pntTile.x < dfTILE_WIDTH_COUNT; (pntTile.x)++)
		{
			LONG loTileColor = g_clJPS.m_loColorMap[pntTile.y][pntTile.x];

			// Ÿ���� ���� ������ ��ǥ(���� ��� ������) ���ϱ�
			pntWindow.x = dfTILE_WIDTH * pntTile.x;
			pntWindow.y = dfTILE_HEIGHT * pntTile.y;

			// Ÿ�� rect ��ǥ ���ϱ�
			rectTile.top = pntWindow.y;
			rectTile.bottom = pntWindow.y + dfTILE_HEIGHT;
			rectTile.left = pntWindow.x;
			rectTile.right = pntWindow.x + dfTILE_WIDTH;

			switch (loTileColor)
			{
			case JumpPointSearch::enNONE:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;

				hMyBrush = CreateSolidBrush(RGB(255, 255, 255));
				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);

				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);


				Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				SelectObject(memDC, (HBRUSH)hOldBrush);

				DeleteObject(hMyBrush);

			}
			break;

			case JumpPointSearch::enOBSTACLE:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;
				hMyBrush = CreateSolidBrush(RGB(128, 128, 128));
				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);

				Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				SelectObject(memDC, (HBRUSH)hOldBrush);

				DeleteObject(hMyBrush);
			}
			break;

			case JumpPointSearch::enSTART:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;
				hMyBrush = CreateSolidBrush(RGB(0, 221, 0));
				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);

				//SelectObject(memDC, (HPEN)hOldBrush);
				Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				SelectObject(memDC, (HBRUSH)hOldBrush);

				DeleteObject(hMyBrush);


				//< DC Brush �̿�
				//SetDCBrushColor(memDC, RGB(0, 255, 0));

				//FillRect(memDC, &rectTile, (HBRUSH)GetStockObject(DC_BRUSH));

				//SelectObject(hdc, (HBRUSH)hOldBrush);

				//DeleteObject(hMyBrush);
			}
			break;

			case JumpPointSearch::enEND:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;
				hMyBrush = CreateSolidBrush(RGB(238, 68, 0));
				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);

				Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				SelectObject(memDC, (HBRUSH)hOldBrush);

				DeleteObject(hMyBrush);
				//< DC Brush �̿�
				//SetDCBrushColor(memDC, RGB(255, 0, 0));

				//FillRect(memDC, &rectTile, (HBRUSH)GetStockObject(DC_BRUSH));
			}
			break;

			case JumpPointSearch::enOPENLIST:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;
				hMyBrush = CreateSolidBrush(RGB(152, 251, 152));
				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);

				Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				SelectObject(memDC, (HBRUSH)hOldBrush);

				DeleteObject(hMyBrush);
			}
			break;

			case JumpPointSearch::enCLOSELIST:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;
				hMyBrush = CreateSolidBrush(RGB(175, 238, 238));
				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);

				Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				SelectObject(memDC, (HBRUSH)hOldBrush);

				DeleteObject(hMyBrush);
			}
			break;

			case JumpPointSearch::enPASSED:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;
				hMyBrush = CreateSolidBrush(RGB(255, 206, 68));
				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);

				Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				SelectObject(memDC, (HBRUSH)hOldBrush);

				DeleteObject(hMyBrush);
			}
			break;

			default:
			{
				HBRUSH hMyBrush;
				HBRUSH hOldBrush;
				hMyBrush = CreateSolidBrush(loTileColor);
				hOldBrush = (HBRUSH)SelectObject(memDC, (HBRUSH)hMyBrush);

				Rectangle(memDC, rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);

				SelectObject(memDC, (HBRUSH)hOldBrush);

				DeleteObject(hMyBrush);
			}
			break;
			}

			
		}
	}
}

void DrawPath(HDC memDC)
{
	if (nullptr == g_clJPS.m_stpCompleteNode)
	{
		return;
	}

	POINT pntWindow;
	POINT pntTile;

	//< CreatePen �̿�
	HPEN hMyPen;
	HPEN hOldPen;
	hMyPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));

	hOldPen = (HPEN)SelectObject(memDC, (HPEN)hMyPen);

	JumpPointSearch::st_NODE* stpNode = g_clJPS.m_stpCompleteNode;

	pntTile.x = stpNode->loPosX;
	pntTile.y = stpNode->loPosY;

	pntWindow.x = (pntTile.x * dfTILE_WIDTH) + dfTILE_WIDTH / 2;
	pntWindow.y = (pntTile.y * dfTILE_HEIGHT) + dfTILE_HEIGHT / 2;
	MoveToEx(memDC, pntWindow.x, pntWindow.y, NULL);

	while (stpNode->pParent != nullptr)
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