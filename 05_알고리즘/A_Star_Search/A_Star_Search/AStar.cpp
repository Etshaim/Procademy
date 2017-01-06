#include "stdafx.h"
#include "AStar.h"

using namespace std;

CAStar::CAStar()
{
	InitTile();
	InitGDI();
}

CAStar::~CAStar()
{
	delete[] h_pMyPen;
	delete[] h_pBrush;
}

void CAStar::InitTile()
{
	for (int iCnt = 0; iCnt < df_ARRAYMAPY; iCnt++)
	{
		memset(m_bTileArr[iCnt], df_NONBLOCK, sizeof(char) * df_ARRAYMAPX);
	}

	m_bCheckStart = FALSE;
	m_iTempStartX = -1;
	m_iTempStartY = -1;
	m_iStartX = -1;
	m_iStartY = -1;

	m_bCheckEnd = FALSE;
	m_iTempEndX = -1;
	m_iTempEndY = -1;
	m_iEndX = -1;
	m_iEndY = -1;

	m_stpDest = NULL;
	m_mmOpenList.clear();
	m_mmCloseList.clear();
}

void CAStar::InitGDI()
{
	//--------------------------------------------------------------------------
	// 펜 초기화	
	//--------------------------------------------------------------------------
	h_pMyPen = new HPEN[2];
	h_pMyPen[eGRAYPEN]	= CreatePen(PS_SOLID, 1, RGB(190, 190, 190));
	h_pMyPen[eREDPEN]	= CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	//--------------------------------------------------------------------------
	// 브러쉬 초기화	
	//--------------------------------------------------------------------------
	h_pBrush			 = new HBRUSH[5];
	h_pBrush[eSTART]	 = CreateSolidBrush( RGB(255,   0,   0) );
	h_pBrush[eEND]		 = CreateSolidBrush( RGB(  0, 255,   0) );
	h_pBrush[eBLOCK]	 = CreateSolidBrush( RGB(153, 153, 153) );
	h_pBrush[eMOVE]		 = CreateSolidBrush( RGB(255, 255, 153) );
	h_pBrush[eMOVERANGE] = CreateSolidBrush( RGB(  0,   0, 255) );
}

void CAStar::DrawTile(HWND hWnd, HDC hdc)
{
	CRect cRect;

	GetClientRect(hWnd, &cRect);

	if (h_MemDC == NULL)
	{
		h_MemDC = CreateCompatibleDC(hdc);

		if (h_MyBitMap == NULL)
		{
			h_MyBitMap = CreateCompatibleBitmap(hdc, cRect.right, cRect.bottom);
			SelectObject(h_MemDC, h_MyBitMap);
		}
	}

	SelectObject(h_MemDC, h_pMyPen[eGRAYPEN]);

	h_OldBrush = (HBRUSH)SelectObject(h_MemDC, h_pBrush[eEND]);

	PatBlt(h_MemDC, 0, 0, cRect.right, cRect.bottom, WHITENESS);
			
	// R2_COPYPEN은 PEN으로 지정
	SetROP2(h_MemDC, R2_COPYPEN);

	//--------------------------------------------------------------------------
	// 바둑판 모양의 맵을 그린다.
	//--------------------------------------------------------------------------
	for (int iX = 0; iX < df_ARRAYMAPX; iX++)
	{
		for (int iY = 0; iY < df_ARRAYMAPY; iY++)
		{
			if (m_bTileArr[iY][iX] == df_NONBLOCK)
			{
				if (m_iTempStartX == iX && m_iTempStartY == iY)
					SelectObject(h_MemDC, h_pBrush[eSTART]);
				else if (m_iTempEndX == iX && m_iTempEndY == iY)
					SelectObject(h_MemDC, h_pBrush[eEND]);
				else
					SelectObject(h_MemDC, h_OldBrush);

				Rectangle(h_MemDC, iX * df_RECTINTERVAL + 1, iY * df_RECTINTERVAL + 1, (iX + 1) * df_RECTINTERVAL, (iY + 1) * df_RECTINTERVAL);
			}
			else
			{
				SelectObject(h_MemDC, h_pBrush[eBLOCK]);
				Rectangle(h_MemDC, iX * df_RECTINTERVAL + 1, iY * df_RECTINTERVAL + 1, (iX + 1) * df_RECTINTERVAL, (iY + 1) * df_RECTINTERVAL);
			}

		}
	}

	SelectObject(h_MemDC, h_OldBrush);
}

void CAStar::DrawProcessOfMovement(HWND hWnd, HDC hdc)
{
	int iX = 0;
	int iY = 0;
	CRect cRect;

	GetClientRect(hWnd, &cRect);

	if (h_MemDC == NULL)
	{
		h_MemDC = CreateCompatibleDC(hdc);

		if (h_MyBitMap == NULL)
		{
			h_MyBitMap = CreateCompatibleBitmap(hdc, cRect.right, cRect.bottom);
			SelectObject(h_MemDC, h_MyBitMap);
		}
	}

	h_OldBrush = (HBRUSH)SelectObject(h_MemDC, h_pBrush[eMOVE]);

	// R2_COPYPEN은 PEN으로 지정
	SetROP2(h_MemDC, R2_COPYPEN);

	multimap<float, ST_TILENODE *>::iterator iter;

	//--------------------------------------------------------------------------
	// 실제 이동 경로
	//--------------------------------------------------------------------------
	for (iter = m_mmCloseList.begin(); iter != m_mmCloseList.end(); iter++)
	{
		iX = iter->second->iX;
		iY = iter->second->iY;

		if (iX == m_iTempStartX && iY == m_iTempStartY)
			continue;

		if (iX == m_iTempEndX && iY == m_iTempEndY)
			continue;

		if (m_bTileArr[iY][iX] != df_BLOCK)
		{
			SelectObject(h_MemDC, h_pBrush[eMOVE]);
			Rectangle(h_MemDC, iX * df_RECTINTERVAL + 1, iY * df_RECTINTERVAL + 1, (iX + 1) * df_RECTINTERVAL, (iY + 1) * df_RECTINTERVAL);
		}

	}

	//--------------------------------------------------------------------------
	// 적합하지 않은 노드들임.
	//--------------------------------------------------------------------------
	for (iter = m_mmOpenList.begin(); iter != m_mmOpenList.end(); iter++)
	{
		iX = iter->second->iX;
		iY = iter->second->iY;

		if (iX == m_iTempStartX && iY == m_iTempStartY)
			continue;

		if (iX == m_iTempEndX && iY == m_iTempEndY)
			continue;

		if (m_bTileArr[iY][iX] != df_BLOCK)
		{
			SelectObject(h_MemDC, h_pBrush[eMOVERANGE]);
			Rectangle(h_MemDC, iX * df_RECTINTERVAL + 1, iY * df_RECTINTERVAL + 1, (iX + 1) * df_RECTINTERVAL, (iY + 1) * df_RECTINTERVAL);
		}

	}

	SelectObject(h_MemDC, h_OldBrush);
}

void CAStar::DrawBestRoute(HWND hWnd, HDC hdc, ST_TILENODE *stpDest)
{
	CRect cRect;

	GetClientRect(hWnd, &cRect);

	if (h_MemDC == NULL)
	{
		h_MemDC = CreateCompatibleDC(hdc);

		if (h_MyBitMap == NULL)
		{
			h_MyBitMap = CreateCompatibleBitmap(hdc, cRect.right, cRect.bottom);
			SelectObject(h_MemDC, h_MyBitMap);
		}
	}

	SelectObject(h_MemDC, h_pMyPen[eREDPEN]);

	// R2_COPYPEN은 PEN으로 지정
	SetROP2(h_MemDC, R2_COPYPEN);

	while (1)
	{
		//--------------------------------------------------------------------------
		// 검색 도중에 검색 초기화가 되면 while 문 빠져 나와야 함.
		//--------------------------------------------------------------------------
		if (NULL == stpDest)
			return;

		if (stpDest->iX == m_iStartX && stpDest->iY == m_iStartY)
			break;

		MoveToEx(h_MemDC, stpDest->iX * df_RECTINTERVAL + df_RECTINTERVAL / 2, stpDest->iY * df_RECTINTERVAL + df_RECTINTERVAL / 2, NULL);
		LineTo(h_MemDC, stpDest->stpParent->iX * df_RECTINTERVAL + df_RECTINTERVAL / 2, stpDest->stpParent->iY * df_RECTINTERVAL + df_RECTINTERVAL / 2);

		stpDest = stpDest->stpParent;
	}
}

void CAStar::DrawBitMap(HWND hWnd, HDC hdc)
{
	CRect cRect;

	GetClientRect(hWnd, &cRect);
	SelectObject(h_MemDC, h_MyBitMap);

	BitBlt(hdc, 0, 0, cRect.right, cRect.bottom, h_MemDC, 0, 0, SRCCOPY);
}

void CAStar::CheckBlockPoint(int iXPos, int iYPos, BOOL bButton)
{
	if (bButton == TRUE)
		m_bTileArr[iYPos][iXPos] = df_BLOCK;
	else
		m_bTileArr[iYPos][iXPos] = df_NONBLOCK;
}

void CAStar::MakeStartNode(int iStartX, int iStartY, int iEndX, int iEndY)
{
	ST_TILENODE *stpNew = new ST_TILENODE;

	m_iStartX = iStartX;
	m_iStartY = iStartY;
	m_iEndX = iEndX;
	m_iEndY = iEndY;

	stpNew->iX = iStartX;
	stpNew->iY = iStartY;
	stpNew->fH = abs(iStartX - iEndX) + abs(iStartY - iEndY);
	stpNew->fG = 0;
	stpNew->fF = stpNew->fH + stpNew->fG;
	stpNew->stpParent = NULL;

	//--------------------------------------------------------------------------
	// 오픈리스트 초기화 후, 오픈리스트에 삽입
	//--------------------------------------------------------------------------
	m_mmOpenList.clear();
	m_mmOpenList.insert(pair<float, ST_TILENODE *>(stpNew->fF, stpNew));
}

BOOL CAStar::FindTargetSpot(HWND hWnd)
{
	ST_TILENODE *stpTempNode;

	if (m_mmOpenList.empty() == TRUE)
		return FALSE;

	stpTempNode = m_mmOpenList.begin()->second;
	m_mmOpenList.erase(m_mmOpenList.begin());
	m_mmCloseList.insert(pair<float, ST_TILENODE *>(stpTempNode->fF, stpTempNode));

	if (stpTempNode->iX == m_iEndX && stpTempNode->iY == m_iEndY)
	{
		m_stpDest = stpTempNode;
		return TRUE;
	}

	MakeEightDirectionNode(stpTempNode, stpTempNode->iX, stpTempNode->iY);

	return FALSE;
}


void CAStar::MakeEightDirectionNode(ST_TILENODE *stpCurrent, int iCurrentX, int iCurrentY)
{
	// 위
	if ((df_NONBLOCK == m_bTileArr[iCurrentY - 1][iCurrentX]) && (iCurrentX >= 0 
		&& iCurrentX <= df_ARRAYMAPX - 1) && ((iCurrentY - 1) >= 0 && (iCurrentY - 1) <= df_ARRAYMAPY - 1))
		MakeNode(stpCurrent, iCurrentX, iCurrentY - 1, FALSE);


	// 오른쪽 위 대각선
	if (df_NONBLOCK == m_bTileArr[iCurrentY - 1][iCurrentX + 1] && ((iCurrentX + 1) >= 0 
		&& (iCurrentX + 1) <= df_ARRAYMAPX - 1) && ((iCurrentY - 1) >= 0 && (iCurrentY - 1) <= df_ARRAYMAPY - 1))
		MakeNode(stpCurrent, iCurrentX + 1, iCurrentY - 1, TRUE);

	// 오른쪽
	if (df_NONBLOCK == m_bTileArr[iCurrentY][iCurrentX + 1] && ((iCurrentX + 1) >= 0 
		&& (iCurrentX + 1) <= df_ARRAYMAPX - 1) && (iCurrentY >= 0 && iCurrentY <= df_ARRAYMAPY - 1))
		MakeNode(stpCurrent, iCurrentX + 1, iCurrentY, FALSE);

	// 오른쪽 아래 대각선
	if (df_NONBLOCK == m_bTileArr[iCurrentY + 1][iCurrentX + 1] && ((iCurrentX + 1) >= 0 
		&& (iCurrentX + 1) <= df_ARRAYMAPX - 1) && ((iCurrentY + 1) >= 0 && (iCurrentY + 1) <= df_ARRAYMAPY - 1))
		MakeNode(stpCurrent, iCurrentX + 1, iCurrentY + 1, TRUE);

	// 아래
	if (df_NONBLOCK == m_bTileArr[iCurrentY + 1][iCurrentX] && (iCurrentX >= 0 && iCurrentX <= df_ARRAYMAPX - 1) 
		&& ((iCurrentY + 1) >= 0 && (iCurrentY + 1) <= df_ARRAYMAPY - 1))
		MakeNode(stpCurrent, iCurrentX, iCurrentY + 1, FALSE);

	// 아래 왼쪽 대각선
	if (df_NONBLOCK == m_bTileArr[iCurrentY + 1][iCurrentX - 1] && ((iCurrentX - 1) >= 0 
		&& (iCurrentX - 1) <= df_ARRAYMAPX - 1) && ((iCurrentY + 1) >= 0 && (iCurrentY + 1) <= df_ARRAYMAPY - 1))
		MakeNode(stpCurrent, iCurrentX - 1, iCurrentY + 1, TRUE);

	// 왼쪽
	if (df_NONBLOCK == m_bTileArr[iCurrentY][iCurrentX - 1] && ((iCurrentX - 1) >= 0 
		&& (iCurrentX - 1) <= df_ARRAYMAPX - 1) && (iCurrentY >= 0 && iCurrentY <= df_ARRAYMAPY - 1))
		MakeNode(stpCurrent, iCurrentX - 1, iCurrentY, FALSE);

	// 왼쪽 위 대각선
	if (df_NONBLOCK == m_bTileArr[iCurrentY - 1][iCurrentX - 1] && ((iCurrentX - 1) >= 0 
		&& (iCurrentX - 1) <= df_ARRAYMAPX - 1) && ((iCurrentY - 1) >= 0 && (iCurrentY - 1) <= df_ARRAYMAPY - 1))
		MakeNode(stpCurrent, iCurrentX - 1, iCurrentY - 1, TRUE);
}

void CAStar::MakeNode(ST_TILENODE *stpCurrent, int iCurrentX, int iCurrentY, BOOL bDiagonalState)
{
	float fTempG = 0;

	if (bDiagonalState == TRUE)
		fTempG = (float)(stpCurrent->fG + 1.5);
	else
		fTempG = (float)(stpCurrent->fG + 1);


	auto aOpenNode = find_if(	m_mmOpenList.begin(), m_mmOpenList.end(), [iCurrentX, iCurrentY](const pair<float, ST_TILENODE *> &pOpenList) 
	{
		return pOpenList.second->iX == iCurrentX && pOpenList.second->iY == iCurrentY; 
	});


	if (aOpenNode != m_mmOpenList.end())
	{
		if (aOpenNode->second->fG > fTempG)
		{
			aOpenNode->second->fG = fTempG;
			aOpenNode->second->stpParent = stpCurrent;
		}

		return;
	}

	auto aCloseNode = find_if(m_mmCloseList.begin(), m_mmCloseList.end(), [iCurrentX, iCurrentY](const pair<float, ST_TILENODE *> &pCloseList) {return pCloseList.second->iX == iCurrentX && pCloseList.second->iY == iCurrentY; });

	if (aCloseNode != m_mmCloseList.end())
	{
		if (aCloseNode->second->fG > fTempG)
		{
			aCloseNode->second->fG = fTempG;
			aCloseNode->second->stpParent = stpCurrent;
		}

		return;
	}

	ST_TILENODE *stpNew = new ST_TILENODE;

	stpNew->iX = iCurrentX;
	stpNew->iY = iCurrentY;
	stpNew->fH = abs(iCurrentX - m_iEndX) + abs(iCurrentY - m_iEndY);
	stpNew->fG = fTempG;
	stpNew->fF = stpNew->fH + stpNew->fG;
	stpNew->stpParent = stpCurrent;

	m_mmOpenList.insert(pair<float, ST_TILENODE *>(stpNew->fF, stpNew));
}