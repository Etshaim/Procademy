#ifndef MonitorGraphUnit
#define MonitorGraphUnit

#include "stdafx.h"
#include "Resource.h"
#include "Queue.h"
#include <math.h>

#define dfMAX_CHILD							100

// define
//< 배열 크기 정보
#define dfTITLE_ARRAY_MAX					100
//#define dfQUEUE_ARRAY_MAX					256
#define dfHANDLE_ARRAY_MAX					100
#define dfITOW_ARRAY_MAX					100
#define dfMULTI_INFO_ARRAY_MAX				100
#define dfRGB_ARRAY_MAX						5
#define dfQUEUE_CAPACITY					10

//< 그래프 여백 크기 정보
#define dfWND_TITLE_HEIGHT					10
#define dfWND_BOTTOM_HEIGHT					30
#define dfSINGLE_GRAPH_LEFT_MARGIN			50
#define dfSINGLE_GRAPH_RIGHT_MARGIN			50
#define dfMULTI_LINE_GRAPH_LEFT_MARGIN		50
#define dfMULTI_LINE_GRAPH_RIGHT_MARGIN		150
#define dfCOLUMN_LEFT_MARGIN				50
#define dfCOLUMN_RIGHT_MARGIN				70
#define dfCOLUMN_MARGIN						20

//< 그리드 관련 정보
#define dfGRID_INTERVAL						5
#define dfGRID_INTERVAL_HEIGHT				40
#define dfSINGLE_LINE_INTERVAL_HEIGHT		40
#define dfTITLE								50

class CMonitorGraphUnit
{
public:
	//< enum
	// 그래프 타입
	enum GRAPH_TYPE
	{
		BAR_SINGLE_VERTICAL,
		BAR_SINGLE_HORIZON,
		BAR_COLUMN_VERTICAL,
		BAR_COLUMN_HORIZON,
		LINE_SINGLE,
		LINE_MULTI,
		PIE,
		SWITCH
	};

	// 펜 타입
	enum PEN_TYPE
	{
		LINE_GRAPH_PEN
	};

	// 폰트 타입
	enum FONT_TYPE
	{
		GRID_SWITCH_FONT,
		GRID_VALUE_FONT,
		GRID_TITLE_FONT,
		GRID_COLUMN_VALUE_FONT
	};

	// 브러쉬 타입
	enum BRUSH_TYPE
	{
		COLUMN_GRAPH_BRUSH
	};

// 구조체
public :
	typedef struct ST_HWNDtoTHIS
	{
		HWND				hWnd;
		CMonitorGraphUnit	*pThis;

	} stHWNDtoTHIS;

	static stHWNDtoTHIS stArrHandle[dfMAX_CHILD];

	typedef struct ST_RGBTABLE
	{
		int	R;
		int G;
		int B;
	}stRGBTable;

	stRGBTable stArrRGBTable[dfRGB_ARRAY_MAX];

public:
	CMonitorGraphUnit(HINSTANCE hInstance, HWND hWndParent, COLORREF BackColor, 
		GRAPH_TYPE enGRAPH_TYPE, int iPosX, int iPosY, int iWidth, int iHeight, int iDataNum);

	~CMonitorGraphUnit();


public:
	// 윈도우 프로시저
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// 데이터 넣기.
	BOOL	InsertData(void);

	// 윈도우 만들기
	ATOM MyRegisterClass(void);
	void MyCreateWindow(void);

	// 윈도우 설정
	void SetSwitchInfo(TCHAR* szTitle);
	void SetSingleDataInfo(TCHAR* szTitle, float dataMax);
	void SetMultiDataInfo(TCHAR* szTitle, float dataMax);

	// 그림 그리는 영역
	void DrawGrid(HWND hWnd);
	void DrawSwitch(HWND hWnd);
	void DrawLineGraph(HWND hWnd);
	void DrawColumnGraph(HWND hWnd);
	void DrawBitMap(HWND hWnd, HDC hdc);

	// 경고
	void Alert(void);

	// RGB 테이블 초기화
	void InitRGBTable(void);

	// GDI 초기화
	void InitGDI(void);



public:
	// GDI Resource
	HDC			_hMemDC;
	HBITMAP		_hMYBitMap;
	HBRUSH*		_hMyBrush;
	HFONT*		_hMyFont;
	HPEN*		_hMyPen;
	HPEN*		_hMyPenForGraphLine;

	// bool
	BOOL		_bFirstInsert;		// 첫 좌표가 두 번 insert되는 것 방지
	static BOOL _bAlertMode;		// 알람모드가 true일 때만 부모 윈도우에 메시지
	static BOOL _bConnectStatus;	// 서버 접속 상태
	static BOOL _bDrawStatus;		// 큐에 데이터가 들어가기 전에 WM_PAINT 로직이 수행되지 않도록

	// queue
	CQueue<int> Queue[dfQUEUE_CAPACITY];

protected:
	// 윈도우 핸들, this 포인터 매칭 테이블 관리.
	BOOL						PutThis(void);
	static BOOL					RemoveThis(HWND hWnd);
	static CMonitorGraphUnit	*GetThis(HWND hWnd);

private:
	//< 윈도우 정보
	HINSTANCE					_hInstance;
	HWND						_hWndParent;
	HWND						_hWnd;
	COLORREF					_colorData;
	int							_nCmdShow;
	int							_iWndX;
	int							_iWndY;
	int							_nWidth;
	int							_nHeight;
	int							_iDataNum;
	GRAPH_TYPE					_eGraphType;
	TCHAR						_szTitle[dfTITLE_ARRAY_MAX];

	//< 그리드 정보
	float						_dataMax;
	float						_bottomLine;

	//< 그래프 정보
	float						_graphX;
	int							_graphY;
	int							_frontPos;
	int							_rearPos;
};
#endif // !MonitorGraphUnit