
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

	// 그래프 정보
	_graphX			= dfSINGLE_GRAPH_LEFT_MARGIN;
	_graphY			= 0;
	_iDataNum		= iDataNum;

	// 그리드 정보
	_dataMax		= 0;
	_bottomLine		= 0;

	// RGB 테이블 초기화
	InitRGBTable();

	// GDI 초기화
	InitGDI();
}

CMonitorGraphUnit::~CMonitorGraphUnit()
{
	delete[] _hMyPen;
	delete[] _hMyPenForGraphLine;
	delete[] _hMyBrush;
}


	// 윈도우 프로시저
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

	// 데이터 넣기.
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

	// 윈도우 만들기
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

	// 윈도우 설정
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

	// 그림 그리는 영역
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

	// 윈도우의 y축 라인 간격
	// y축의 라인 간격 수치
	intervalY = (_nHeight - dfWND_TITLE_HEIGHT - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL;
	
	
	// 윈도우의 y축 라인 수치
	// y축에 입력될 data 수치
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
	// PatBlt는 직접적으로 비트맵을 출력하는 아니며 패턴을 출력
	// 메모리 DC의 화면색상을 흰색으로 변경
	PatBlt(_hMemDC, 0, 0, cRect.right, cRect.bottom, WHITENESS);

	// 드로잉 모드를 지정
	// r2 copypen은 pen으로 지정한 색을 가져옴

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
			// Queue 배열에서 front 위치에 저장된 Y좌표를 iY에 저장
			Queue[count].Peek(&_graphY, _frontPos);

			//////////////////////////////////////////////////////////////////////////////////
			// 최대 값에 따른 실제 그림에 사용되는 y 값 구하기									//
			// 1. "최대 수치 / 그리드 라인 갯수 - 1 / 그리드 Y축 간격" 은 y축 1pixel 당 data 값	//
			// 2. "큐에 저장된 Y 값 / y축 1pixel 당 data 값" 은 윈도우 상에서 이동해야 할 y 값	//
			// 3. "그리드 최하단 - 이동해야 할 pixel'은 실제 그려줄 y 값						//
			//////////////////////////////////////////////////////////////////////////////////
			fY = _bottomLine - _graphY / (_dataMax / (dfGRID_INTERVAL - 1) / ((_nHeight - dfWND_TITLE_HEIGHT - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL));

			//////////////////////////////////////////
			// Queue의 Y 값이 최대 값을 넘을 시,		//
			// 그려질 Y 값을 grid의 최대 값으로 보정	//
			//////////////////////////////////////////
			if (_graphY >= _dataMax)
			{
				fY = (float)(dfWND_TITLE_HEIGHT + (_nHeight - dfWND_TITLE_HEIGHT - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL);
			}

			//////////////////////////
			// 현재 X, Y좌표로 이동	//
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
			// Queue 배열에서 front+1 위치에 저장된 Y좌표를 iY에 저장	//
			//////////////////////////////////////////////////////////
			Queue[count].Peek(&_graphY, (_frontPos + 1) % dfQUEUE_ARRAY_MAX);

			fY = _bottomLine - _graphY / _dataMax / (dfGRID_INTERVAL -1) / ((_nHeight - dfWND_TITLE_HEIGHT - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL);

			//////////////////////////////////////////
			// Queue의 Y 값이 최대 값을 넘을 시,		//
			// 그려질 Y 값을 grid의 최대 값으로 보정	//
			//////////////////////////////////////////
			if (_graphY >= _dataMax)
			{
				fY = (float)(dfWND_TITLE_HEIGHT + (_nHeight - dfWND_TITLE_HEIGHT - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL);
			}

			//////////////////////////////////////////////////
			// MoveToEx의 X, Y부터 현재 X,Y좌표까지 선을 그림	//
			//////////////////////////////////////////////////
			LineTo(_hMemDC, (int)ceil(_graphX), (int)ceil(fY));

			_frontPos = (_frontPos + 1) % dfQUEUE_ARRAY_MAX;

			//////////////////////////////////////////////
			// front + 1이 rear와 동일 하면,				//
			// X좌표를 초기화하고, while문을 빠져나옴		//
			// front + 1이 rear와 동일하다는 것은,		//
			// 모든 큐 데이터를 그렸다는 의미				//
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
		// Queue의 Y 값이 최대 값을 넘을 시,	//
		// Alert함수 호출					//
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
		// Queue의 Y 값이 최대 값을 넘을 시,		//
		// 그려질 Y 값을 grid의 최대 값으로 보정	//
		//////////////////////////////////////////
		if (Queue[count]._lastData >= _dataMax)
		{
			iY = (float)(dfWND_TITLE_HEIGHT + (_nHeight - dfWND_BOTTOM_HEIGHT) / dfGRID_INTERVAL);
		}

		//////////////////
		// 컬럼 그리기	//
		//////////////////
		SetRect(cRectForColumn, (int)ceil(iX), (int)ceil(iY), (int)ceil(iX) + iColumnWidth, (int)ceil(_bottomLine));
		FillRect(_hMemDC, &cRectForColumn, _hMyBrush[BRUSH_TYPE::COLUMN_GRAPH_BRUSH]);

		//////////////////
		// 컬럼 값 출력	//
		//////////////////
		_itow_s(Queue[count]._lastData, tcBuffer, 10);

		TextOut(_hMemDC, (int)ceil(iX) + ((int)ceil(iX) + iColumnWidth - (int)ceil(iX)) / 2, (int)ceil(iY) - 20, tcBuffer, (int)wcslen(tcBuffer));

		iX += iColumnWidth;
		iX += dfCOLUMN_MARGIN;

		//////////////////////////////////////
		// Queue의 Y 값이 최대 값을 넘을 시,	//
		// Alert함수 호출					//
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

// 경고
void CMonitorGraphUnit::Alert(void)
{
	if (_bAlertMode)
	{
		return;
	}

	_bAlertMode = true;

	SendMessage(_hWndParent, WM_USER, 0, 0);
}

// RGB 테이블 초기화
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

// GDI 초기화
void CMonitorGraphUnit::InitGDI(void)
{
	int count = 0;

	// Grid 펜 초기화	//
	_hMyPen = new HPEN[1];
	_hMyPen[PEN_TYPE::LINE_GRAPH_PEN] = CreatePen(PS_SOLID, 1, RGB(190, 190, 190));

	// Graph 펜 초기화	//
	_hMyPenForGraphLine = new HPEN[_iDataNum];

	for (count = 0; count < _iDataNum; ++count)
	{
		_hMyPenForGraphLine[count] = CreatePen(PS_SOLID, 2, 
			RGB(stArrRGBTable[count].R, stArrRGBTable[count].G, stArrRGBTable[count].B));
	}

	// 폰트 초기화
	_hMyFont = new HFONT[4];

	_hMyFont[FONT_TYPE::GRID_SWITCH_FONT] = CreateFont(80, 80, 0, 0,
		FW_MEDIUM, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, L"굴림체");

	_hMyFont[FONT_TYPE::GRID_VALUE_FONT] = CreateFont(11, 0, 0, 0, 
		0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, L"굴림체");

	_hMyFont[FONT_TYPE::GRID_TITLE_FONT] = CreateFont(17, 0, 0, 0,
		0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, L"굴림체");

	_hMyFont[FONT_TYPE::GRID_COLUMN_VALUE_FONT] = CreateFont(13, 0, 0, 0,
		0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, L"굴림체");

	_hMyFont[FONT_TYPE::GRID_COLUMN_VALUE_FONT] = CreateFont(13, 0, 0, 0,
		0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, L"굴림체");

	// Column 브러쉬 초기화
	_hMyBrush = new HBRUSH[1];
	_hMyBrush[BRUSH_TYPE::COLUMN_GRAPH_BRUSH] = CreateSolidBrush(RGB(0, 191, 255));
}

	//------------------------------------------------------
	// 윈도우 핸들, this 포인터 매칭 테이블 관리.
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