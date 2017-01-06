
#include "stdafx.h"
#include "MonitorGraphUnit.h"

CMonitorGraphUnit::CMonitorGraphUnit(HINSTANCE hInstance, HWND hWndParent, 
	COLORREF BackColor, GRAPH_TYPE enGRAPH_TYPE, int iPosX, int iPosY, int iWidth, int iHeight, int iDataNum)
{
	_hInstance		= hInstance;
	_hWndParent		= hWndParent;
	_colorData		= BackColor;
	_eGraphType		= enGRAPH_TYPE;
	_iWndX			= iPosX;
	_iWndY			= iPosY;
	_nWidth			= iWidth;
	_nHeight		= iHeight;

	// �׷��� ����
	_graphX			= dfSINGLE_GRAPH_LEFT_MARGIN;
	_graphY			= 0;
	_iDataNum		= iDataNum;

	// �׸��� ����
	_dataMax		= 0;
	_bottomLine		= 0;

	// RGB ���̺� �ʱ�ȭ
	InitRGBTable();

	// GDI �ʱ�ȭ
	InitGDI();
}

CMonitorGraphUnit::~CMonitorGraphUnit()
{
	delete[] _hMyPen;
	delete[] _hMyPenForGraphLine;
	delete[] _hMyBrush;
}


	// ������ ���ν���
LRESULT CALLBACK CMonitorGraphUnit::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	CMonitorGraphUnit* pThis;

	switch (message)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		pThis = GetThis(hWnd);

		pThis->DrawGrid(hWnd);

		if (pThis->_eGraphType == GRAPH_TYPE::SWITCH)
		{
			pThis->DrawSwitch(hWnd);
		}
		else if (pThis->_eGraphType == GRAPH_TYPE::BAR_COLUMN_HORIZON
			|| pThis->_eGraphType == GRAPH_TYPE::BAR_COLUMN_VERTICAL
			|| pThis->_eGraphType == GRAPH_TYPE::BAR_SINGLE_HORIZON
			|| pThis->_eGraphType == GRAPH_TYPE::BAR_SINGLE_VERTICAL)
		{
			pThis->DrawColumnGraph(hWnd);
		}

		pThis->DrawBitMap(hWnd, hdc);

		EndPaint(hWnd, &ps);

		break;
	}

	case WM_DESTROY:
	{
		pThis = GetThis(hWnd);
		DeleteDC(pThis->_hMemDC);
		DeleteObject(pThis->_hMYBitMap);

		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		break;
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

	// ������ �ֱ�.
BOOL	CMonitorGraphUnit::InsertData(void)
{
	int count = 0;

	if (_eGraphType != GRAPH_TYPE::SWITCH)
	{
		if (_bFirstInsert == FALSE)
		{
			for (count = 0; count < _iDataNum; ++count)
			{
				if (_bConnectStatus == TRUE)
				{
					_graphY = rand() % 600 + (count + 1) * 300;
				}
				else if (_bConnectStatus == FALSE)
				{
					_graphY = 0;
				}

				Queue[count].Enqueue(_graphY);

				if (_eGraphType == GRAPH_TYPE::BAR_COLUMN_HORIZON
					|| _eGraphType == GRAPH_TYPE::BAR_COLUMN_VERTICAL
					|| _eGraphType == GRAPH_TYPE::BAR_SINGLE_HORIZON
					|| _eGraphType == GRAPH_TYPE::BAR_SINGLE_VERTICAL)
				{
					Queue[count]._lastData = _graphY;
				}
				
			}
			_bFirstInsert = TRUE;
		}

		for (count = 0; count < _iDataNum; ++count)
		{
			if (_bConnectStatus == TRUE)
			{
				_graphY = rand() % 600 + (count + 1) * 300;
			}
			else if (_bConnectStatus == FALSE)
			{
				_graphY = 0;
			}

			Queue[count]._lastData = _graphY;
		}
	}

	InvalidateRect(_hWnd, NULL, FALSE);

	return TRUE;
}

	// ������ �����
ATOM CMonitorGraphUnit::MyRegisterClass(void)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = _hInstance;
	wcex.hIcon = LoadIcon(_hInstance, MAKEINTRESOURCE(IDC_MONITORINGTOOL));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);

	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_MONITORINGTOOL);
	wcex.lpszClassName = _szTitle;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
	
}

void CMonitorGraphUnit::MyCreateWindow(void)
{
	int count = 0;

	_hWnd = CreateWindow(	_szTitle,
							_szTitle,
							WS_CHILD | WS_VISIBLE,
							_iWndX,
							_iWndY,
							_nWidth,
							_nHeight,
							_hWndParent,
							NULL,
							_hInstance,
							NULL	);

	for (count = 0; count < dfHANDLE_ARRAY_MAX; ++count)
	{
		if (stArrHandle[count].hWnd == NULL)
		{
			stArrHandle[count].hWnd = _hWnd;
			stArrHandle[count].pThis = this;
			return;
		}
	}
}

	// ������ ����
void CMonitorGraphUnit::SetSwitchInfo(TCHAR* szTitle)
{
	wmemcpy_s(_szTitle, dfTITLE_ARRAY_MAX, szTitle, wcslen( szTitle ) * sizeof( TCHAR ) );
	MyRegisterClass();
	MyCreateWindow();

	_hMemDC		= NULL;
	_hMYBitMap	= NULL;
}

void CMonitorGraphUnit::SetSingleDataInfo(TCHAR* szTitle, float dataMax)
{
	wmemcpy_s(_szTitle, dfTITLE_ARRAY_MAX, szTitle, wcslen(szTitle) * sizeof(TCHAR));
	_dataMax = dataMax;

	MyRegisterClass();
	MyCreateWindow();

	_hMemDC = NULL;
	_hMYBitMap = NULL;

}
void CMonitorGraphUnit::SetMultiDataInfo(TCHAR* szTitle, float dataMax)
{
	wmemcpy_s(_szTitle, dfTITLE_ARRAY_MAX, szTitle, wcslen(szTitle) * sizeof( TCHAR ));
	_dataMax = dataMax;
	MyRegisterClass();
	MyCreateWindow();

	_hMemDC = NULL;
	_hMYBitMap = NULL;

	//
}

	// �׸� �׸��� ����
void CMonitorGraphUnit::DrawGrid(HWND hWnd)
{
	HDC		hdc;
	CRect	cRect;

	int count;
	int data;
	float y;
	int intervalY;
	int dataInterval;
	TCHAR tcBuffer[dfITOW_ARRAY_MAX] = { 0 };

	y = dfWND_TITLE_HEIGHT;
	data = 0;

	// �������� y�� ���� ����
	// y���� ���� ���� ��ġ
	intervalY = (_nHeight - dfWND_TITLE_HEIGHT - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL;
	
	
	// �������� y�� ���� ��ġ
	// y�࿡ �Էµ� data ��ġ
	dataInterval = (int)( _dataMax / (dfGRID_INTERVAL - 1) );

	hdc = GetDC(hWnd);

	GetClientRect(hWnd, &cRect);

	if (_hMemDC == NULL)
	{
		_hMemDC = CreateCompatibleDC(hdc);
	}

	if (_hMYBitMap == NULL)
	{
		_hMYBitMap = CreateCompatibleBitmap(hdc, cRect.right, cRect.bottom);
	}

	SelectObject(_hMemDC, _hMYBitMap);
	SelectObject(_hMemDC, _hMyPen[PEN_TYPE::LINE_GRAPH_PEN]);
	SelectObject(_hMemDC, _hMyFont[FONT_TYPE::GRID_VALUE_FONT]);
	
	SetBkMode( _hMemDC, TRANSPARENT );
	SetTextAlign( _hMemDC, TA_RIGHT );
	SetTextColor( _hMemDC, RGB(105, 105, 105) );

	// PatBlt
	// PatBlt�� ���������� ��Ʈ���� ����ϴ� �ƴϸ� ������ ���
	// �޸� DC�� ȭ������� ������� ����
	PatBlt(_hMemDC, 0, 0, cRect.right, cRect.bottom, WHITENESS);

	// ����� ��带 ����
	// r2 copypen�� pen���� ������ ���� ������

	SetROP2(_hMemDC, R2_COPYPEN);

	for (count = 1; count <= dfGRID_INTERVAL; ++count)
	{
		y += intervalY;
		data = ( int )( _dataMax - ( dataInterval * ( count - 1 ) ) );
		_itow_s(data, tcBuffer, 10);

		TextOut(_hMemDC, (int)(dfSINGLE_GRAPH_LEFT_MARGIN - 5), (int)y - 5,
			tcBuffer, (int)wcslen(tcBuffer));

		MoveToEx(_hMemDC, (int)(dfSINGLE_GRAPH_LEFT_MARGIN), (int)y, NULL);

		if (_eGraphType == GRAPH_TYPE::LINE_SINGLE)
		{
			LineTo(_hMemDC, _nWidth - dfSINGLE_GRAPH_RIGHT_MARGIN, (int)y);
		}
		else if (_eGraphType == GRAPH_TYPE::LINE_MULTI)
		{
			LineTo(_hMemDC, _nWidth - dfMULTI_LINE_GRAPH_RIGHT_MARGIN, (int)y);
		}
		else if (_eGraphType == GRAPH_TYPE::BAR_COLUMN_VERTICAL)
		{
			LineTo(_hMemDC, _nWidth - dfSINGLE_GRAPH_RIGHT_MARGIN, (int)y);
		}
	}

	SelectObject(_hMemDC, _hMyFont[FONT_TYPE::GRID_TITLE_FONT]);
	SetTextColor(_hMemDC, RGB(0, 0, 0));

}

void CMonitorGraphUnit::DrawSwitch(HWND hWnd)
{
	HDC hdc;
	CRect cRect;

	hdc = GetDC(hWnd);
	GetClientRect(hWnd, &cRect);

	if (_hMemDC == NULL)
		_hMemDC = CreateCompatibleDC(hdc);

	if (_hMYBitMap == NULL)
		_hMYBitMap = CreateCompatibleBitmap(hdc, cRect.right, cRect.bottom);

	SelectObject(_hMemDC, _hMYBitMap);
	SelectObject(_hMemDC, _hMyFont[FONT_TYPE::GRID_SWITCH_FONT]);
	SetBkMode(_hMemDC, TRANSPARENT);
	SetTextColor(_hMemDC, RGB(0, 0, 0));
	SetTextAlign(_hMemDC, TA_CENTER);

	if (_bConnectStatus == TRUE)
	{
		TextOut(_hMemDC, _nWidth / 2, (_nHeight - (_nHeight - dfWND_TITLE_HEIGHT) / 2) - 40, L"ON", (int)wcslen(L"ON"));
	}
	else if (_bConnectStatus == FALSE)
	{
		TextOut(_hMemDC, _nWidth / 2, (_nHeight - (_nHeight - dfWND_TITLE_HEIGHT) / 2) - 40, L"OFF", (int)wcslen(L"OFF"));
	}

	ReleaseDC(hWnd, hdc);
}

void CMonitorGraphUnit::DrawLineGraph(HWND hWnd)
{
	HDC hdc;
	CRect cRect;
	TCHAR tcBuffer[dfITOW_ARRAY_MAX] = { 0 };

	int count;
	float fY;
	float fQueueIntervalForSingle;
	float fQueueIntervalForMulti;

	fY = 0;
	fQueueIntervalForSingle = (float)(_nWidth - dfSINGLE_GRAPH_RIGHT_MARGIN) / (dfQUEUE_ARRAY_MAX - 1);
	fQueueIntervalForMulti = (float)(_nWidth - dfMULTI_LINE_GRAPH_LEFT_MARGIN  - dfMULTI_LINE_GRAPH_RIGHT_MARGIN) / (dfQUEUE_ARRAY_MAX - 1);

	hdc = GetDC(hWnd);
	GetClientRect(hWnd, &cRect);

	SelectObject(_hMemDC, _hMYBitMap);
	SelectObject(_hMemDC, _hMyFont[FONT_TYPE::GRID_SWITCH_FONT]);
	SetBkMode(_hMemDC, TRANSPARENT);
	SetTextColor(_hMemDC, RGB(0, 0, 0));
	SetTextAlign(_hMemDC, TA_CENTER);

	for (count = 0; count < _iDataNum; ++count)
	{
		SelectObject(_hMemDC, _hMyPenForGraphLine[(_iDataNum - 1) - count]);
		_frontPos = Queue[count].GetFront();
		_rearPos = Queue[count].GetRear();

		if (_rearPos == _frontPos)
			return;

		while (1)
		{
			// Queue �迭���� front ��ġ�� ����� Y��ǥ�� iY�� ����
			Queue[count].Peek(&_graphY, _frontPos);

			//////////////////////////////////////////////////////////////////////////////////
			// �ִ� ���� ���� ���� �׸��� ���Ǵ� y �� ���ϱ�									//
			// 1. "�ִ� ��ġ / �׸��� ���� ���� - 1 / �׸��� Y�� ����" �� y�� 1pixel �� data ��	//
			// 2. "ť�� ����� Y �� / y�� 1pixel �� data ��" �� ������ �󿡼� �̵��ؾ� �� y ��	//
			// 3. "�׸��� ���ϴ� - �̵��ؾ� �� pixel'�� ���� �׷��� y ��						//
			//////////////////////////////////////////////////////////////////////////////////
			fY = _bottomLine - _graphY / (_dataMax / (dfGRID_INTERVAL - 1) / ((_nHeight - dfWND_TITLE_HEIGHT - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL));

			//////////////////////////////////////////
			// Queue�� Y ���� �ִ� ���� ���� ��,		//
			// �׷��� Y ���� grid�� �ִ� ������ ����	//
			//////////////////////////////////////////
			if (_graphY >= _dataMax)
			{
				fY = (float)(dfWND_TITLE_HEIGHT + (_nHeight - dfWND_TITLE_HEIGHT - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL);
			}

			//////////////////////////
			// ���� X, Y��ǥ�� �̵�	//
			//////////////////////////
			MoveToEx(_hMemDC, (int)ceil(_graphX), (int)ceil(fY), NULL);

			if (_eGraphType == GRAPH_TYPE::LINE_SINGLE)
			{
				_graphX = _graphX + fQueueIntervalForSingle;
			}
			else if (_eGraphType == GRAPH_TYPE::LINE_MULTI
				|| _eGraphType == GRAPH_TYPE::BAR_COLUMN_VERTICAL)
			{
				_graphX = _graphX + fQueueIntervalForMulti;
			}

			//////////////////////////////////////////////////////////
			// Queue �迭���� front+1 ��ġ�� ����� Y��ǥ�� iY�� ����	//
			//////////////////////////////////////////////////////////
			Queue[count].Peek(&_graphY, (_frontPos + 1) % dfQUEUE_ARRAY_MAX);

			fY = _bottomLine - _graphY / _dataMax / (dfGRID_INTERVAL -1) / ((_nHeight - dfWND_TITLE_HEIGHT - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL);

			//////////////////////////////////////////
			// Queue�� Y ���� �ִ� ���� ���� ��,		//
			// �׷��� Y ���� grid�� �ִ� ������ ����	//
			//////////////////////////////////////////
			if (_graphY >= _dataMax)
			{
				fY = (float)(dfWND_TITLE_HEIGHT + (_nHeight - dfWND_TITLE_HEIGHT - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL);
			}

			//////////////////////////////////////////////////
			// MoveToEx�� X, Y���� ���� X,Y��ǥ���� ���� �׸�	//
			//////////////////////////////////////////////////
			LineTo(_hMemDC, (int)ceil(_graphX), (int)ceil(fY));

			_frontPos = (_frontPos + 1) % dfQUEUE_ARRAY_MAX;

			//////////////////////////////////////////////
			// front + 1�� rear�� ���� �ϸ�,				//
			// X��ǥ�� �ʱ�ȭ�ϰ�, while���� ��������		//
			// front + 1�� rear�� �����ϴٴ� ����,		//
			// ��� ť �����͸� �׷ȴٴ� �ǹ�				//
			//////////////////////////////////////////////
			if ((_frontPos + 1) % dfQUEUE_ARRAY_MAX == _rearPos)
			{
				//_itow_s((int)m_fGraphX, tcBuffer, 10);
				//TextOut(h_MemDC, 100, 100, tcBuffer, wcslen(tcBuffer));
				_graphX = dfSINGLE_GRAPH_LEFT_MARGIN;

				break;
			}
		}

		//////////////////////////////////////
		// Queue�� Y ���� �ִ� ���� ���� ��,	//
		// Alert�Լ� ȣ��					//
		//////////////////////////////////////
		if (_graphY >= _dataMax)
		{
			this->Alert();
		}
		
	}
	ReleaseDC(hWnd, hdc);
}
void CMonitorGraphUnit::DrawColumnGraph(HWND hWnd)
{
	HDC hdc;
	CRect cRect, cRectForColumn;

	int count;
	int iColumnLeftMargin;
	int iColumnWidth;
	float iX, iY;
	TCHAR tcBuffer[dfITOW_ARRAY_MAX] = { 0 };

	iColumnLeftMargin = (int)(_graphX + dfCOLUMN_LEFT_MARGIN);
	iColumnWidth = 0;
	iX = 0;
	iY = 0;

	hdc = GetDC(hWnd);
	GetClientRect(hWnd, &cRect);

	if (_hMemDC == NULL)
		_hMemDC = CreateCompatibleDC(hdc);

	if (_hMYBitMap == NULL)
		_hMYBitMap = CreateCompatibleBitmap(hdc, cRect.right, cRect.bottom);

	SelectObject(_hMemDC, _hMYBitMap);
	SelectObject(_hMemDC, _hMyFont[FONT_TYPE::GRID_COLUMN_VALUE_FONT]);
	SetTextAlign(_hMemDC, TA_CENTER);
	SetBkMode(_hMemDC, TRANSPARENT);
	iX = (float)iColumnLeftMargin;

	for (count = 0; count < _iDataNum; ++count)
	{
		iColumnWidth = (_nWidth - iColumnLeftMargin - dfCOLUMN_RIGHT_MARGIN - ((_iDataNum - 1) * dfCOLUMN_MARGIN)) / _iDataNum;

		iY = _bottomLine - Queue[count]._lastData / (_dataMax / (dfGRID_INTERVAL - 1) / ((_nHeight - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL));

		//////////////////////////////////////////
		// Queue�� Y ���� �ִ� ���� ���� ��,		//
		// �׷��� Y ���� grid�� �ִ� ������ ����	//
		//////////////////////////////////////////
		if (Queue[count]._lastData >= _dataMax)
		{
			iY = (float)(dfWND_TITLE_HEIGHT + (_nHeight - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL);
		}

		//////////////////
		// �÷� �׸���	//
		//////////////////
		SetRect(cRectForColumn, (int)ceil(iX), (int)ceil(iY), (int)ceil(iX) + iColumnWidth, (int)ceil(_bottomLine));
		FillRect(_hMemDC, &cRectForColumn, _hMyBrush[BRUSH_TYPE::COLUMN_GRAPH_BRUSH]);

		//////////////////
		// �÷� �� ���	//
		//////////////////
		_itow_s(Queue[count]._lastData, tcBuffer, 10);

		TextOut(_hMemDC, (int)ceil(iX) + ((int)ceil(iX) + iColumnWidth - (int)ceil(iX)) / 2, (int)ceil(iY) - 20, tcBuffer, (int)wcslen(tcBuffer));

		iX += iColumnWidth;
		iX += dfCOLUMN_MARGIN;

		//////////////////////////////////////
		// Queue�� Y ���� �ִ� ���� ���� ��,	//
		// Alert�Լ� ȣ��					//
		//////////////////////////////////////
		if (Queue[count]._lastData >= _dataMax)
		{
			this->Alert();
		}
	}

	ReleaseDC(hWnd, hdc);
}
void CMonitorGraphUnit::DrawBitMap(HWND hWnd, HDC hdc)
{
	//BITMAP bit;
	CRect cRect;

	GetClientRect(hWnd, &cRect);
	SelectObject(_hMemDC, _hMYBitMap);

	//GetObject(h_MyBitMap, sizeof(BITMAP), &bit);
	BitBlt(hdc, 0, 0, cRect.right, cRect.bottom, _hMemDC, 0, 0, SRCCOPY);
}

// ���
void CMonitorGraphUnit::Alert(void)
{
	if (_bAlertMode)
	{
		return;
	}

	_bAlertMode = true;

	SendMessage(_hWndParent, WM_USER, 0, 0);
}

// RGB ���̺� �ʱ�ȭ
void CMonitorGraphUnit::InitRGBTable(void)
{
	// Deep Sky Blue
	stArrRGBTable[0].R = 0;
	stArrRGBTable[0].G = 191;
	stArrRGBTable[0].B = 255;

	// Purple
	stArrRGBTable[1].R = 160;
	stArrRGBTable[1].G = 32;
	stArrRGBTable[1].B = 240;

	// Lime Green
	stArrRGBTable[2].R = 50;
	stArrRGBTable[2].G = 205;
	stArrRGBTable[2].B = 50;

	// Hot Pink
	stArrRGBTable[3].R = 255;
	stArrRGBTable[3].G = 105;
	stArrRGBTable[3].B = 180;

	// Goldenrod
	stArrRGBTable[4].R = 218;
	stArrRGBTable[4].G = 165;
	stArrRGBTable[4].B = 32;
}

// GDI �ʱ�ȭ
void CMonitorGraphUnit::InitGDI(void)
{
	int count = 0;

	// Grid �� �ʱ�ȭ	//
	_hMyPen = new HPEN[1];
	_hMyPen[PEN_TYPE::LINE_GRAPH_PEN] = CreatePen(PS_SOLID, 1, RGB(190, 190, 190));

	// Graph �� �ʱ�ȭ	//
	_hMyPenForGraphLine = new HPEN[_iDataNum];

	for (count = 0; count < _iDataNum; ++count)
	{
		_hMyPenForGraphLine[count] = CreatePen(PS_SOLID, 2, 
			RGB(stArrRGBTable[count].R, stArrRGBTable[count].G, stArrRGBTable[count].B));
	}

	// ��Ʈ �ʱ�ȭ
	_hMyFont = new HFONT[4];

	_hMyFont[FONT_TYPE::GRID_SWITCH_FONT] = CreateFont(80, 80, 0, 0,
		FW_MEDIUM, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, L"����ü");

	_hMyFont[FONT_TYPE::GRID_VALUE_FONT] = CreateFont(11, 0, 0, 0, 
		0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, L"����ü");

	_hMyFont[FONT_TYPE::GRID_TITLE_FONT] = CreateFont(17, 0, 0, 0,
		0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, L"����ü");

	_hMyFont[FONT_TYPE::GRID_COLUMN_VALUE_FONT] = CreateFont(13, 0, 0, 0,
		0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, L"����ü");

	_hMyFont[FONT_TYPE::GRID_COLUMN_VALUE_FONT] = CreateFont(13, 0, 0, 0,
		0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, L"����ü");

	// Column �귯�� �ʱ�ȭ
	_hMyBrush = new HBRUSH[1];
	_hMyBrush[BRUSH_TYPE::COLUMN_GRAPH_BRUSH] = CreateSolidBrush(RGB(0, 191, 255));
}

	//------------------------------------------------------
	// ������ �ڵ�, this ������ ��Ī ���̺� ����.
	//------------------------------------------------------
BOOL CMonitorGraphUnit::PutThis(void)
{
	return TRUE;
}
BOOL CMonitorGraphUnit::RemoveThis(HWND hWnd)
{
	return TRUE;
}
CMonitorGraphUnit* CMonitorGraphUnit::GetThis(HWND hWnd)
{
	int count = 0;

	for (count = 0; count < dfHANDLE_ARRAY_MAX; ++count)
	{
		return stArrHandle[count].pThis;
	}

	return stArrHandle[count].pThis;
}