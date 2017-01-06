#ifndef MonitorGraphUnit
#define MonitorGraphUnit

#include "stdafx.h"
#include "Resource.h"
#include "Queue.h"
#include <math.h>

#define dfMAX_CHILD							100

// define
//< �迭 ũ�� ����
#define dfTITLE_ARRAY_MAX					100
//#define dfQUEUE_ARRAY_MAX					256
#define dfHANDLE_ARRAY_MAX					100
#define dfITOW_ARRAY_MAX					100
#define dfMULTI_INFO_ARRAY_MAX				100
#define dfRGB_ARRAY_MAX						5
#define dfQUEUE_CAPACITY					10

//< �׷��� ���� ũ�� ����
#define dfWND_TITLE_HEIGHT					10
#define dfWND_BOTTOM_HEIGHT					30
#define dfSINGLE_GRAPH_LEFT_MARGIN			50
#define dfSINGLE_GRAPH_RIGHT_MARGIN			50
#define dfMULTI_LINE_GRAPH_LEFT_MARGIN		50
#define dfMULTI_LINE_GRAPH_RIGHT_MARGIN		150
#define dfCOLUMN_LEFT_MARGIN				50
#define dfCOLUMN_RIGHT_MARGIN				70
#define dfCOLUMN_MARGIN						20

//< �׸��� ���� ����
#define dfGRID_INTERVAL						5
#define dfGRID_INTERVAL_HEIGHT				40
#define dfSINGLE_LINE_INTERVAL_HEIGHT		40
#define dfTITLE								50

class CMonitorGraphUnit
{
public:
	//< enum
	// �׷��� Ÿ��
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

	// �� Ÿ��
	enum PEN_TYPE
	{
		LINE_GRAPH_PEN
	};

	// ��Ʈ Ÿ��
	enum FONT_TYPE
	{
		GRID_SWITCH_FONT,
		GRID_VALUE_FONT,
		GRID_TITLE_FONT,
		GRID_COLUMN_VALUE_FONT
	};

	// �귯�� Ÿ��
	enum BRUSH_TYPE
	{
		COLUMN_GRAPH_BRUSH
	};

// ����ü
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
	// ������ ���ν���
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// ������ �ֱ�.
	BOOL	InsertData(void);

	// ������ �����
	ATOM MyRegisterClass(void);
	void MyCreateWindow(void);

	// ������ ����
	void SetSwitchInfo(TCHAR* szTitle);
	void SetSingleDataInfo(TCHAR* szTitle, float dataMax);
	void SetMultiDataInfo(TCHAR* szTitle, float dataMax);

	// �׸� �׸��� ����
	void DrawGrid(HWND hWnd);
	void DrawSwitch(HWND hWnd);
	void DrawLineGraph(HWND hWnd);
	void DrawColumnGraph(HWND hWnd);
	void DrawBitMap(HWND hWnd, HDC hdc);

	// ���
	void Alert(void);

	// RGB ���̺� �ʱ�ȭ
	void InitRGBTable(void);

	// GDI �ʱ�ȭ
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
	BOOL		_bFirstInsert;		// ù ��ǥ�� �� �� insert�Ǵ� �� ����
	static BOOL _bAlertMode;		// �˶���尡 true�� ���� �θ� �����쿡 �޽���
	static BOOL _bConnectStatus;	// ���� ���� ����
	static BOOL _bDrawStatus;		// ť�� �����Ͱ� ���� ���� WM_PAINT ������ ������� �ʵ���

	// queue
	CQueue<int> Queue[dfQUEUE_CAPACITY];

protected:
	// ������ �ڵ�, this ������ ��Ī ���̺� ����.
	BOOL						PutThis(void);
	static BOOL					RemoveThis(HWND hWnd);
	static CMonitorGraphUnit	*GetThis(HWND hWnd);

private:
	//< ������ ����
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

	//< �׸��� ����
	float						_dataMax;
	float						_bottomLine;

	//< �׷��� ����
	float						_graphX;
	int							_graphY;
	int							_frontPos;
	int							_rearPos;
};
#endif // !MonitorGraphUnit