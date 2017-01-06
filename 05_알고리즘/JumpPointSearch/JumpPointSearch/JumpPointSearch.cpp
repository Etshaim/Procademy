#include "stdafx.h"
#include "JumpPointSearch.h"
#include "LinkedList.h"
#include <math.h>

JumpPointSearch::JumpPointSearch()
{
	m_clOpenList = new CDoubleLinkedList<st_NODE*>;
	m_clCloseList = new CDoubleLinkedList<st_NODE*>;

	m_bNewSearch = TRUE;

	// 맵 초기화
	// 맵 색상 초기화
	for (int iPosY = 0; iPosY < dfTILE_HEIGHT_COUNT; iPosY++)
	{
		for (int iPosX = 0; iPosX < dfTILE_WIDTH_COUNT; iPosX++)
		{
			m_byMap[iPosY][iPosX] = dfTILE_NONE;
		}
	}

	// 시작점
	SetStartPoint();

	// 도착점
	SetEndPoint();


	// 맵 색상 초기화
	for (int iPosY = 0; iPosY < dfTILE_HEIGHT_COUNT; iPosY++)
	{
		for (int iPosX = 0; iPosX < dfTILE_WIDTH_COUNT; iPosX++)
		{
			SetTileColor(iPosX, iPosY, enNONE);
		}
	}

	// 시작점
	SetTileColor(m_loStartX, m_loStartY, enSTART);

	// 도착점
	SetTileColor(m_loEndX, m_loEndY, enEND);

	MakeColorTable();
}

JumpPointSearch::~JumpPointSearch()
{
	ClearOpenList();
	ClearCloseList();

	delete m_clOpenList;
	delete m_clCloseList;
}

BOOL JumpPointSearch::CreateNode(LONG loPosX, LONG loPosY, st_NODE * pParent, LONG loDir)
{
	st_NODE *stpNode;

	//open에서 x, y 좌표 검색
	CDoubleLinkedList<st_NODE*>::Iterator iter;

	if (m_clOpenList != nullptr)
	{
		for (iter = m_clOpenList->Begin(); iter != m_clOpenList->End(); iter++)
		{
			if ((*iter)->loPosX == loPosX
				&& (*iter)->loPosY == loPosY)
			{
				// G 값이 나+1 보다 크면 나를 부모로 바꿔주고 끝
				if ((*iter)->fG > pParent->fG + 1)
				{
					(*iter)->pParent = pParent;
					(*iter)->fG = pParent->fG + 1;
				}

				// G 값이 나+1 보다 작으면 할게 없음 끝
				return FALSE;
			}
		}
	}

	//closelist x, y 좌표 검색
	if (m_clCloseList != nullptr)
	{
		for (iter = m_clCloseList->Begin(); iter != m_clCloseList->End(); iter++)
		{
			if ((*iter)->loPosX == loPosX
				&& (*iter)->loPosY == loPosY)
			{
				return FALSE;
			}
		}
	}

	// 여기까지 왔다는 것은 open close에 없는 좌표
	//신규노드 생성
	stpNode = new st_NODE;
	stpNode->loPosX = loPosX;
	stpNode->loPosY = loPosY;
	stpNode->pParent = pParent;
	stpNode->loDir = loDir;

	// 목적지이면 
	if (loPosX == m_loEndX && loPosY == m_loEndY)
	{
		m_stpCompleteNode = stpNode;
	}

	// G값 구하기

	float fDistance = 0.0f;
	st_NODE *stpChild = stpNode;
	st_NODE *stpParent = stpNode->pParent;

	while (stpChild->pParent != nullptr)
	{
		fDistance += GetDistance(stpChild->loPosX, stpChild->loPosY,
			stpParent->loPosX, stpParent->loPosY);

		stpChild = stpParent;
		stpParent = stpChild->pParent;
	}
	stpNode->fG = fDistance;

	stpNode->fH = (float)(abs(m_loEndX - loPosX) + abs(m_loEndY - loPosY));
	stpNode->fF = stpNode->fG + stpNode->fH;

	m_clOpenList->PushBack(stpNode);

	// 색칠하기
	// 만약 시작점이나 끝점이면 패스
	if (stpNode->loPosX == m_loStartX &&
		stpNode->loPosY == m_loStartY)
	{
		return TRUE;
	}

	if (stpNode->loPosX == m_loEndX &&
		stpNode->loPosY == m_loEndY)
	{
		return TRUE;
	}

	SetTileColor(stpNode->loPosX, stpNode->loPosY, enOPENLIST);
	return TRUE;
}

JumpPointSearch::st_NODE * JumpPointSearch::GetNodeMinF(void)
{
	CDoubleLinkedList<st_NODE*>::Iterator iter = m_clOpenList->Begin();
	st_NODE* stpMinFNode = (*iter);

	for (iter = m_clOpenList->Begin(); iter != m_clOpenList->End(); iter++)
	{
		if (stpMinFNode->fF > (*iter)->fF)
		{
			stpMinFNode = (*iter);
		}
	}
	return stpMinFNode;
}

BOOL JumpPointSearch::Jump_LL(LONG loPosX, LONG loPosY, st_NODE *pParent)
{
	while (loPosX > 0)
	{
		loPosX--;

		// 현재 위치가 장애물이면 그냥 끝
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// 목적지인지 확인
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			if (pParent != nullptr)
			{
				CreateNode(loPosX, loPosY, pParent, 0);
			}

			return TRUE;
		}

		m_loColorMap[loPosY][loPosX] = m_color;

		// 꺽이는 지점인지 확인
		if (loPosY + 1 < dfTILE_HEIGHT_COUNT)
		{
			// 왼쪽으로 진행 아래가 막힘
			if (m_byMap[loPosY + 1][loPosX] == dfTILE_OBSTACLE)
			{
				if (m_byMap[loPosY + 1][loPosX - 1] == dfTILE_NONE)
				{
					if (pParent != nullptr)
					{
						CreateNode(loPosX, loPosY, pParent, dfDIR_LL | dfDIR_LD);
						return TRUE;
					}

					return TRUE;
				}
			}
		}

		if (loPosY - 1 >= 0)
		{
			// 왼쪽으로 진행 위가 막힘
			if (m_byMap[loPosY - 1][loPosX] == dfTILE_OBSTACLE)
			{
				if (m_byMap[loPosY - 1][loPosX - 1] == dfTILE_NONE)
				{
					if (pParent != nullptr)
					{
						CreateNode(loPosX, loPosY, pParent, dfDIR_LL | dfDIR_LU);
						return TRUE;
					}

					return TRUE;
				}
			}
		}


	}

	// 여기까지 왔다면 벽 도달
	return FALSE;

}

BOOL JumpPointSearch::Jump_RR(LONG loPosX, LONG loPosY, st_NODE *pParent)
{

	while (loPosX + 1 < dfTILE_WIDTH_COUNT)
	{
		loPosX++;

		// 현재 위치가 장애물이면 그냥 끝
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// 목적지인지 확인
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			if (pParent != nullptr)
			{
				CreateNode(loPosX, loPosY, pParent, 0);
			}

			return TRUE;
		}

		m_loColorMap[loPosY][loPosX] = m_color;

		// 꺽이는 지점인지 확인
		if (loPosY + 1 < dfTILE_HEIGHT_COUNT)
		{
			// 오른쪽으로 진행 아래가 막힘
			if (m_byMap[loPosY + 1][loPosX] == dfTILE_OBSTACLE)
			{
				if (m_byMap[loPosY + 1][loPosX + 1] == dfTILE_NONE)
				{
					if (pParent != nullptr)
					{
						CreateNode(loPosX, loPosY, pParent, dfDIR_RR | dfDIR_RD);
						return TRUE;
					}

					return TRUE;
				}
			}
		}

		if (loPosY - 1 >= 0)
		{
			// 오른쪽으로 진행 위가 막힘
			if (m_byMap[loPosY - 1][loPosX] == dfTILE_OBSTACLE)
			{
				if (m_byMap[loPosY - 1][loPosX + 1] == dfTILE_NONE)
				{
					if (pParent != nullptr)
					{
						CreateNode(loPosX, loPosY, pParent, dfDIR_RR | dfDIR_RU);
						return TRUE;
					}

					return TRUE;
				}
			}
		}
	}

	// 여기까지 왔다면 벽 도달
	return FALSE;
}

BOOL JumpPointSearch::Jump_DD(LONG loPosX, LONG loPosY, st_NODE *pParent)
{
	while (loPosY + 1 < dfTILE_HEIGHT_COUNT)
	{
		loPosY++;

		// 현재 위치가 장애물이면 그냥 끝
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// 목적지인지 확인
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			if (pParent != nullptr)
			{
				CreateNode(loPosX, loPosY, pParent, 0);
			}

			return TRUE;
		}

		m_loColorMap[loPosY][loPosX] = m_color;

		// 맨 아래에 도달했다면 꺾임 확인하지 않아도 됨
		if (loPosY + 1 != dfTILE_HEIGHT_COUNT)
		{
			// 꺽이는 지점인지 확인
			if (loPosX + 1 < dfTILE_WIDTH_COUNT)
			{
				if (m_byMap[loPosY][loPosX + 1] == dfTILE_OBSTACLE)
				{
					// 아래쪽으로 진행 우측이 막힘
					// 한칸 아래 오른쪽이 뚫렸으면 꺾이는 지점임
					if (m_byMap[loPosY + 1][loPosX + 1] == dfTILE_NONE)
					{
						if (pParent != nullptr)
						{
							CreateNode(loPosX, loPosY, pParent, dfDIR_DD | dfDIR_RD);
							return TRUE;
						}

						return TRUE;
					}
				}
			}

			if (loPosX - 1 >= 0)
			{
				if (m_byMap[loPosY][loPosX - 1] == dfTILE_OBSTACLE)
				{
					// 아래쪽으로 진행 좌측이 막힘
					// 한칸 아래 왼쪽이 뚫렸으면 꺾이는 지점임
					if (m_byMap[loPosY + 1][loPosX - 1] == dfTILE_NONE)
					{
						if (pParent != nullptr)
						{
							CreateNode(loPosX, loPosY, pParent, dfDIR_DD | dfDIR_LD);
							return TRUE;

						}
						return TRUE;
					}
				}
			}
		}





	}

	// 여기까지 왔다면 벽 도달
	return FALSE;
}


BOOL JumpPointSearch::Jump_UU(LONG loPosX, LONG loPosY, st_NODE *pParent)
{

	while (loPosY > 0)
	{
		loPosY--;

		// 현재 위치가 장애물이면 그냥 끝
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// 목적지인지 확인
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			if (pParent != nullptr)
			{
				CreateNode(loPosX, loPosY, pParent, 0);

			}

			return TRUE;
		}

		m_loColorMap[loPosY][loPosX] = m_color;

		// 꺽이는 지점인지 확인
		// 마지막 지점이면 꺾임 확인 하지 않아도 됨
		if (loPosY != 0)
		{
			if (loPosX + 1 < dfTILE_WIDTH_COUNT)
			{
				if (m_byMap[loPosY][loPosX + 1] == dfTILE_OBSTACLE)
				{
					if (m_byMap[loPosY - 1][loPosX + 1] == dfTILE_NONE)
					{
						// 위쪽으로 진행 우측이 막힘
						if (pParent != nullptr)
						{
							CreateNode(loPosX, loPosY, pParent, dfDIR_UU | dfDIR_RU);
							return TRUE;

						}

						return TRUE;
					}
				}
			}

			if (loPosX - 1 >= 0)
			{
				if (m_byMap[loPosY][loPosX - 1] == dfTILE_OBSTACLE)
				{
					if (m_byMap[loPosY - 1][loPosX - 1] == dfTILE_NONE)
					{
						// 위쪽으로 진행 좌측이 막힘
						if (pParent != nullptr)
						{
							CreateNode(loPosX, loPosY, pParent, dfDIR_UU | dfDIR_LU);
							return TRUE;

						}

						return TRUE;
					}
				}
			}
		}




	}

	// 여기까지 왔다면 벽 도달
	return FALSE;
}

BOOL JumpPointSearch::Jump_LU(LONG loPosX, LONG loPosY, st_NODE *pParent)
{
	while (loPosX > 0 && loPosY > 0)
	{
		loPosX--;
		loPosY--;

		// 현재 위치가 장애물이면 그냥 끝
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// 목적지인지 확인
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_NONE);

			return TRUE;
		}



		// 꺾임 검사
		if (m_byMap[loPosY + 1][loPosX] == dfTILE_OBSTACLE
			&& m_byMap[loPosY + 1][loPosX - 1] == dfTILE_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_LL | dfDIR_UU | dfDIR_LU | dfDIR_LD);

			return TRUE;
		}

		if (m_byMap[loPosY][loPosX + 1] == dfTILE_OBSTACLE
			&& m_byMap[loPosY - 1][loPosX + 1] == dfTILE_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_LL | dfDIR_UU | dfDIR_LU | dfDIR_RU);

			return TRUE;
		}

		// 여기까지 왔다면 그냥 지나가는 타일이라는 뜻
		m_loColorMap[loPosY][loPosX] = m_color;

		BYTE byDir = dfDIR_NONE;

		// 왼쪽 탐색
		if (TRUE == Jump_LL(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_LL;
		}

		// 위쪽 탐색
		if (TRUE == Jump_UU(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_UU;
		}

		if (byDir != dfDIR_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, byDir);

			//return TRUE;
		}


	}

	// 여기까지 왔다면 벽 도달
	return FALSE;
}

BOOL JumpPointSearch::Jump_LD(LONG loPosX, LONG loPosY, st_NODE *pParent)
{

	while (loPosX > 0 && loPosY < dfTILE_HEIGHT_COUNT - 1)
	{
		loPosX--;
		loPosY++;

		// 현재 위치가 장애물이면 그냥 끝
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// 목적지인지 확인
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_NONE);

			return TRUE;
		}

		// 꺾임 검사
		if (m_byMap[loPosY][loPosX + 1] == dfTILE_OBSTACLE
			&& m_byMap[loPosY + 1][loPosX + 1] == dfTILE_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_LL | dfDIR_DD | dfDIR_LD | dfDIR_RD);

			return TRUE;
		}

		if (m_byMap[loPosY - 1][loPosX] == dfTILE_OBSTACLE
			&& m_byMap[loPosY - 1][loPosX - 1] == dfTILE_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_LL | dfDIR_DD | dfDIR_LU | dfDIR_LD);

			return TRUE;
		}

		// 여기까지 왔다면 그냥 지나가는 타일이라는 뜻
		m_loColorMap[loPosY][loPosX] = m_color;

		BYTE byDir = dfDIR_NONE;


		// 왼쪽 탐색
		if (TRUE == Jump_LL(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_LL;
		}

		// 아래쪽 탐색
		if (TRUE == Jump_DD(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_DD;
		}

		if (byDir != dfDIR_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, byDir);
			//return TRUE;
		}


	}

	// 여기까지 왔다면 벽 도달
	return FALSE;
}

BOOL JumpPointSearch::Jump_RU(LONG loPosX, LONG loPosY, st_NODE *pParent)
{

	while (loPosX < dfTILE_WIDTH_COUNT - 1 && loPosY > 0)
	{
		loPosX++;
		loPosY--;

		// 현재 위치가 장애물이면 그냥 끝
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// 목적지인지 확인
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_NONE);

			return TRUE;
		}

		// 꺾임 검사
		if (m_byMap[loPosY][loPosX - 1] == dfTILE_OBSTACLE
			&& m_byMap[loPosY - 1][loPosX - 1] == dfTILE_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_RR | dfDIR_UU | dfDIR_RU | dfDIR_LU);

			return TRUE;
		}

		if (m_byMap[loPosY + 1][loPosX] == dfTILE_OBSTACLE
			&& m_byMap[loPosY + 1][loPosX + 1] == dfTILE_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_RR | dfDIR_UU | dfDIR_RD | dfDIR_RU);

			return TRUE;
		}

		BYTE byDir = dfDIR_NONE;

		// 오른쪽 탐색
		if (TRUE == Jump_RR(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_RR;
		}

		// 위쪽 탐색
		if (TRUE == Jump_UU(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_UU;
		}

		if (byDir != dfDIR_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, byDir);
			//return TRUE;
		}

		// 여기까지 왔다면 그냥 지나가는 타일이라는 뜻
		m_loColorMap[loPosY][loPosX] = m_color;
	}

	// 여기까지 왔다면 벽 도달
	return FALSE;
}

BOOL JumpPointSearch::Jump_RD(LONG loPosX, LONG loPosY, st_NODE *pParent)
{
	while (loPosX < dfTILE_WIDTH_COUNT - 1 && loPosY < dfTILE_HEIGHT_COUNT - 1)
	{
		loPosX++;
		loPosY++;

		// 현재 위치가 장애물이면 그냥 끝
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// 목적지인지 확인
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_NONE);

			return TRUE;
		}

		// 꺾임 검사
		if (m_byMap[loPosY][loPosX - 1] == dfTILE_OBSTACLE
			&& m_byMap[loPosY + 1][loPosX - 1] == dfTILE_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_RR | dfDIR_DD | dfDIR_LD | dfDIR_RD);

			return TRUE;
		}

		if (m_byMap[loPosY - 1][loPosX] == dfTILE_OBSTACLE
			&& m_byMap[loPosY - 1][loPosX + 1] == dfTILE_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_RR | dfDIR_DD | dfDIR_RU | dfDIR_RD);

			return TRUE;
		}

		BYTE byDir = dfDIR_NONE;

		// 오른쪽 탐색
		if (TRUE == Jump_RR(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_RR;
		}

		// 아래쪽 탐색
		if (TRUE == Jump_DD(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_DD;
		}

		if (byDir != dfDIR_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, byDir);
			//return TRUE;
		}

		// 여기까지 왔다면 그냥 지나가는 타일이라는 뜻
		m_loColorMap[loPosY][loPosX] = m_color;
	}

	// 여기까지 왔다면 벽 도달
	return FALSE;
}

BOOL JumpPointSearch::PathFinder(void)
{
	st_NODE *stpNode;



	if (m_bNewSearch == TRUE)
	{
		m_bNewSearch = FALSE;

		ReStart();

		CreateNode(m_loStartX, m_loStartY, nullptr, dfDIR_ALL);
	}

	// Openlist에서 f값이 가장 작은 노드 찾기
	stpNode = GetNodeMinF();

	// 길이 막힌경우
	if (nullptr == stpNode)
	{
		return FALSE;
	}

	//1.찾은 node가 목적지인지 확인
	if (stpNode->loPosX == m_loEndX
		&& stpNode->loPosY == m_loEndY)
	{
		m_stpCompleteNode = stpNode;

		return FALSE;
	}

	// 목적지가 아니면 openlist에서 삭제
	CDoubleLinkedList<st_NODE*>::Iterator iter;

	for (iter = m_clOpenList->Begin(); iter != m_clOpenList->End(); iter++)
	{
		if ((*iter) == stpNode)
		{
			m_clOpenList->Erase(iter);
		}
	}

	//close에 넣기
	m_clCloseList->PushBack(stpNode);

	// 만약 시작점이나 끝점이면 패스
	if (!(stpNode->loPosX == m_loStartX &&
		stpNode->loPosY == m_loStartY))
	{
		SetTileColor(stpNode->loPosX, stpNode->loPosY, enCLOSELIST);
	}

	// 탐색하는 길 색칠해주기
	//m_enCurColor = enPASSED;
	m_color = GetColor();

	// RR
	if (dfDIR_RR & stpNode->loDir)
	{
		Jump_RR(stpNode->loPosX, stpNode->loPosY, stpNode);
	}
	// DD
	if (dfDIR_DD & stpNode->loDir)
	{
		Jump_DD(stpNode->loPosX, stpNode->loPosY, stpNode);
	}
	// UU
	if (dfDIR_UU & stpNode->loDir)
	{
		Jump_UU(stpNode->loPosX, stpNode->loPosY, stpNode);
	}

	// LL
	if (dfDIR_LL & stpNode->loDir)
	{
		Jump_LL(stpNode->loPosX, stpNode->loPosY, stpNode);
	}


	// LU
	if (dfDIR_LU & stpNode->loDir)
	{
		Jump_LU(stpNode->loPosX, stpNode->loPosY, stpNode);
	}

	// LD
	if (dfDIR_LD & stpNode->loDir)
	{
		Jump_LD(stpNode->loPosX, stpNode->loPosY, stpNode);
	}

	// RU
	if (dfDIR_RU & stpNode->loDir)
	{
		Jump_RU(stpNode->loPosX, stpNode->loPosY, stpNode);
	}

	// RD
	if (dfDIR_RD & stpNode->loDir)
	{
		Jump_RD(stpNode->loPosX, stpNode->loPosY, stpNode);
	}

	return TRUE;
}

float JumpPointSearch::GetDistance(LONG loPosX1, LONG loPosY1, LONG loPosX2, LONG loPosY2)
{
	float fDistance;

	// 피타고라스의 정리
	// pow(x,2) x의 2승,  sqrt() 제곱근
	fDistance = (float)sqrt(pow(loPosX1 - loPosX2, 2) + pow(loPosY1 - loPosY2, 2));

	return fDistance;
}

void JumpPointSearch::SetStartPoint(LONG loStartX, LONG loStartY)
{
	m_byMap[m_loStartY][m_loStartX] = dfTILE_NONE;
	SetTileColor(m_loStartX, m_loStartY, enNONE);

	m_loStartX = loStartX;
	m_loStartY = loStartY;

	m_byMap[m_loStartY][m_loStartX] = dfTILE_START;
	SetTileColor(m_loStartX, m_loStartY, enSTART);

}

void JumpPointSearch::SetEndPoint(LONG loEndX, LONG loEndY)
{
	m_byMap[m_loEndY][m_loEndX] = dfTILE_NONE;
	SetTileColor(m_loEndX, m_loEndY, enNONE);

	m_loEndX = loEndX;
	m_loEndY = loEndY;

	m_byMap[m_loEndY][m_loEndX] = dfTILE_END;
	SetTileColor(m_loEndX, m_loEndY, enEND);
}

void JumpPointSearch::SetTileColor(LONG loPosX, LONG loPosY, enCOLOR color)
{
	m_loColorMap[loPosY][loPosX] = color;
}

void JumpPointSearch::ClearOpenList(void)
{
	CDoubleLinkedList<st_NODE*>::Iterator iter;

	for (iter = m_clOpenList->Begin(); iter != m_clOpenList->End(); iter++)
	{
		delete (*iter);
	}

	m_clOpenList->Clear();
}

void JumpPointSearch::ClearCloseList(void)
{
	CDoubleLinkedList<st_NODE*>::Iterator iter;

	for (iter = m_clCloseList->Begin(); iter != m_clCloseList->End(); iter++)
	{
		delete (*iter);
	}

	m_clCloseList->Clear();
}

void JumpPointSearch::ReStart(void)
{
	m_stpCompleteNode = nullptr;
	ClearOpenList();
	ClearCloseList();

	for (int iPosY = 0; iPosY < dfTILE_HEIGHT_COUNT; iPosY++)
	{
		for (int iPosX = 0; iPosX < dfTILE_WIDTH_COUNT; iPosX++)
		{
			if (m_byMap[iPosY][iPosX] == dfTILE_START
				|| (m_byMap[iPosY][iPosX] == dfTILE_END))
			{
				continue;
			}

			if (m_byMap[iPosY][iPosX] == dfTILE_NONE)
			{
				m_loColorMap[iPosY][iPosX] = enNONE;
			}

			if (m_byMap[iPosY][iPosX] == dfTILE_OBSTACLE)
			{
				m_loColorMap[iPosY][iPosX] = enOBSTACLE;
			}
		}
	}

}

void JumpPointSearch::RemoveOBSTACLE(void)
{
	for (int iPosY = 0; iPosY < dfTILE_HEIGHT_COUNT; iPosY++)
	{
		for (int iPosX = 0; iPosX < dfTILE_WIDTH_COUNT; iPosX++)
		{
			if (m_byMap[iPosY][iPosX] == dfTILE_OBSTACLE)
			{
				m_byMap[iPosY][iPosX] = dfTILE_NONE;
				m_loColorMap[iPosY][iPosX] = enNONE;
			}
		}
	}
}

void JumpPointSearch::MakeColorTable(void)
{
	m_colorTable[0] = 0x908070; //slategray 슬레이트그레이
	m_colorTable[1] = 0x9afa00; //mediumspringgreen 미디움스프링그린
	m_colorTable[2] = 0x696969; //dimgray 딤그레이
	m_colorTable[3] = 0x808080; //gray 그레이
	m_colorTable[4] = 0xa9a9a9; //darkgray 다크그레이
	m_colorTable[5] = 0xb9daff; //peachpuff 피치퍼프
	m_colorTable[6] = 0xaae8ee; //palegoldenrod 팔레골덴로드
	m_colorTable[7] = 0xe1e4ff; //mistyrose 미스티로즈
	m_colorTable[8] = 0xb5e4ff; //moccasin 모카신
	m_colorTable[9] = 0xaddeff; //navajowhite 나바조화이트
	m_colorTable[10] = 0x8cb4d2; //tan 탄
	m_colorTable[11] = 0xb3def5; //wheat 위트
	m_colorTable[12] = 0x8ce6f0; //khaki 카키
	m_colorTable[13] = 0xfae6e6; //lavender 라벤더
	m_colorTable[14] = 0xb48246; //steelblue 스틸블루
	m_colorTable[15] = 0xed9564; //cornflowerblue 콘플라워블루
	m_colorTable[16] = 0xe22b8a; //blueviolet 블루바이올렛
	m_colorTable[17] = 0xc1b6ff; //lightpink 라이트핑크
	m_colorTable[18] = 0xb469ff; //hotpink 핫핑크
	m_colorTable[19] = 0xcbc0ff; //pink 핑크
	m_colorTable[20] = 0xdda0dd; //plum 플럼
	m_colorTable[22] = 0xd8bfd8; //thistle 디스틀
	m_colorTable[23] = 0xd670da; //orchid 오치드
	m_colorTable[24] = 0x7a96e9; //darksalmon 다크샐몬
	m_colorTable[25] = 0x8080f0; //lightcoral 라이트코랄
	m_colorTable[26] = 0x5c5ccd; //indianred 인디안레드
	m_colorTable[27] = 0x7aa0ff; //lightsalmon 라이트샐몬
	m_colorTable[28] = 0x9370db; //palevioletred 팔레바이올렛레드
	m_colorTable[29] = 0x60a4f4; //sandybrown 샌디브라운
	m_colorTable[30] = 0x7280fa; //salmon 샐몬
	m_colorTable[31] = 0x13458b; //saddlebrown 새들브라운
	m_colorTable[32] = 0x2d52a0; //sienna 시에나
	m_colorTable[33] = 0x8f8fbc; //rosybrown 로지브라운
	m_colorTable[34] = 0x507fff; //coral 코랄
	m_colorTable[35] = 0x008cff; //darkorange 다크오렌지
	m_colorTable[36] = 0x00a5ff; //orange 오렌지
	m_colorTable[37] = 0x0b86b8; //darkgoldenrod 다크골덴로드
	m_colorTable[38] = 0x00d7ff; //gold 골드
	m_colorTable[39] = 0x71b33c; //mediumseagreen 미디움씨그린
	m_colorTable[40] = 0x2fffad; //greenyellow 그린옐로우
	m_colorTable[41] = 0x8fbc8f; //darkseagreen 다크씨그린
	m_colorTable[42] = 0x32cd9a; //yellowgreen 옐로우그린
	m_colorTable[43] = 0x578b2e; //seagreen 씨그린
	m_colorTable[44] = 0x998877; //lightslategray 라이트슬레이트그레이
	m_colorTable[45] = 0xaab220; //lightseagreen 라이트씨그린
	
}

COLORREF JumpPointSearch::GetColor(void)
{
	int iIndex = 0;

	iIndex = rand() % 46;

	return m_colorTable[iIndex];
}
