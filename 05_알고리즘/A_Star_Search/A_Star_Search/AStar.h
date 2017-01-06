#pragma once 

#include <map>
#include <algorithm> 
#include <iterator>

using namespace std;

#define		df_RECTINTERVAL		20
#define		df_ARRAYMAPX		63		
#define		df_ARRAYMAPY		33
#define		df_NONBLOCK			0
#define		df_BLOCK			1

class CAStar
{
public:
	enum e_BRUSH
	{
		eSTART = 0,
		eEND,
		eBLOCK,
		eMOVE,
		eMOVERANGE
	};

	enum e_PEN
	{
		eGRAYPEN = 0,
		eREDPEN
	};

public:
	struct ST_TILENODE
	{
		int		iX;
		int		iY;	
		int		fH;		// ���������� �ܼ��� �Ÿ�. �����Ÿ� �ƴ�. 
		float	fG;		// ��������� �� ��ġ���� �̵��� Ÿ���� ����
		float	fF;		// ���������� �̵��ϱ⿡ ������ Ÿ�������� ���� �� (fH + fG)

		ST_TILENODE *stpParent;
	};

	//--------------------------------------------------------------
	// ������ ��
	//--------------------------------------------------------------
	multimap <float, ST_TILENODE *> m_mmOpenList;
	
	//--------------------------------------------------------------
	// ������ ��
	//--------------------------------------------------------------
	multimap <float, ST_TILENODE *> m_mmCloseList;

public:
	BOOL						m_bCheckStart;
	BOOL						m_bCheckEnd;

	BYTE						m_bTileArr[df_ARRAYMAPY][df_ARRAYMAPX];
	int							m_iTempStartX;
	int							m_iTempStartY;
	int							m_iStartX;
	int							m_iStartY;
	int							m_iTempEndX;
	int							m_iTempEndY;
	int							m_iEndX;
	int							m_iEndY;

	ST_TILENODE					*m_stpDest;

public:
	HDC							h_MemDC;
	HBITMAP						h_MyBitMap;
	HPEN						*h_pMyPen;
	HBRUSH						*h_pBrush;
	HBRUSH						h_OldBrush;

public: 
	CAStar();
	~CAStar();
	void InitTile();
	void CheckBlockPoint(int iXPos, int iYPos, BOOL bButton);
	void MakeStartNode(int iStarX, int iStartY, int iEndX, int iEndY);
	BOOL FindTargetSpot(HWND hWnd);

private:
	void InitGDI();
	void MakeEightDirectionNode(ST_TILENODE *stpCurrent, int iCurrentX, int iCurrentY);
	void MakeNode(ST_TILENODE *stpCurrent, int iCurrentX, int iCurrentY, BOOL bDiagonalState);

public:
	void DrawTile(HWND hWnd, HDC hdc);
	void DrawProcessOfMovement(HWND hWnd, HDC hdc);
	void DrawBestRoute(HWND hWnd, HDC hdc, ST_TILENODE *stpDest);
	void DrawBitMap(HWND hWnd, HDC hdc);
};