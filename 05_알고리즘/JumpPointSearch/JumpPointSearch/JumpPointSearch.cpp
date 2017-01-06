#include "stdafx.h"
#include "JumpPointSearch.h"
#include "LinkedList.h"
#include <math.h>

JumpPointSearch::JumpPointSearch()
{
	m_clOpenList = new CDoubleLinkedList<st_NODE*>;
	m_clCloseList = new CDoubleLinkedList<st_NODE*>;

	m_bNewSearch = TRUE;

	// �� �ʱ�ȭ
	// �� ���� �ʱ�ȭ
	for (int iPosY = 0; iPosY < dfTILE_HEIGHT_COUNT; iPosY++)
	{
		for (int iPosX = 0; iPosX < dfTILE_WIDTH_COUNT; iPosX++)
		{
			m_byMap[iPosY][iPosX] = dfTILE_NONE;
		}
	}

	// ������
	SetStartPoint();

	// ������
	SetEndPoint();


	// �� ���� �ʱ�ȭ
	for (int iPosY = 0; iPosY < dfTILE_HEIGHT_COUNT; iPosY++)
	{
		for (int iPosX = 0; iPosX < dfTILE_WIDTH_COUNT; iPosX++)
		{
			SetTileColor(iPosX, iPosY, enNONE);
		}
	}

	// ������
	SetTileColor(m_loStartX, m_loStartY, enSTART);

	// ������
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

	//open���� x, y ��ǥ �˻�
	CDoubleLinkedList<st_NODE*>::Iterator iter;

	if (m_clOpenList != nullptr)
	{
		for (iter = m_clOpenList->Begin(); iter != m_clOpenList->End(); iter++)
		{
			if ((*iter)->loPosX == loPosX
				&& (*iter)->loPosY == loPosY)
			{
				// G ���� ��+1 ���� ũ�� ���� �θ�� �ٲ��ְ� ��
				if ((*iter)->fG > pParent->fG + 1)
				{
					(*iter)->pParent = pParent;
					(*iter)->fG = pParent->fG + 1;
				}

				// G ���� ��+1 ���� ������ �Ұ� ���� ��
				return FALSE;
			}
		}
	}

	//closelist x, y ��ǥ �˻�
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

	// ������� �Դٴ� ���� open close�� ���� ��ǥ
	//�űԳ�� ����
	stpNode = new st_NODE;
	stpNode->loPosX = loPosX;
	stpNode->loPosY = loPosY;
	stpNode->pParent = pParent;
	stpNode->loDir = loDir;

	// �������̸� 
	if (loPosX == m_loEndX && loPosY == m_loEndY)
	{
		m_stpCompleteNode = stpNode;
	}

	// G�� ���ϱ�

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

	// ��ĥ�ϱ�
	// ���� �������̳� �����̸� �н�
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

		// ���� ��ġ�� ��ֹ��̸� �׳� ��
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// ���������� Ȯ��
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			if (pParent != nullptr)
			{
				CreateNode(loPosX, loPosY, pParent, 0);
			}

			return TRUE;
		}

		m_loColorMap[loPosY][loPosX] = m_color;

		// ���̴� �������� Ȯ��
		if (loPosY + 1 < dfTILE_HEIGHT_COUNT)
		{
			// �������� ���� �Ʒ��� ����
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
			// �������� ���� ���� ����
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

	// ������� �Դٸ� �� ����
	return FALSE;

}

BOOL JumpPointSearch::Jump_RR(LONG loPosX, LONG loPosY, st_NODE *pParent)
{

	while (loPosX + 1 < dfTILE_WIDTH_COUNT)
	{
		loPosX++;

		// ���� ��ġ�� ��ֹ��̸� �׳� ��
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// ���������� Ȯ��
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			if (pParent != nullptr)
			{
				CreateNode(loPosX, loPosY, pParent, 0);
			}

			return TRUE;
		}

		m_loColorMap[loPosY][loPosX] = m_color;

		// ���̴� �������� Ȯ��
		if (loPosY + 1 < dfTILE_HEIGHT_COUNT)
		{
			// ���������� ���� �Ʒ��� ����
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
			// ���������� ���� ���� ����
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

	// ������� �Դٸ� �� ����
	return FALSE;
}

BOOL JumpPointSearch::Jump_DD(LONG loPosX, LONG loPosY, st_NODE *pParent)
{
	while (loPosY + 1 < dfTILE_HEIGHT_COUNT)
	{
		loPosY++;

		// ���� ��ġ�� ��ֹ��̸� �׳� ��
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// ���������� Ȯ��
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			if (pParent != nullptr)
			{
				CreateNode(loPosX, loPosY, pParent, 0);
			}

			return TRUE;
		}

		m_loColorMap[loPosY][loPosX] = m_color;

		// �� �Ʒ��� �����ߴٸ� ���� Ȯ������ �ʾƵ� ��
		if (loPosY + 1 != dfTILE_HEIGHT_COUNT)
		{
			// ���̴� �������� Ȯ��
			if (loPosX + 1 < dfTILE_WIDTH_COUNT)
			{
				if (m_byMap[loPosY][loPosX + 1] == dfTILE_OBSTACLE)
				{
					// �Ʒ������� ���� ������ ����
					// ��ĭ �Ʒ� �������� �շ����� ���̴� ������
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
					// �Ʒ������� ���� ������ ����
					// ��ĭ �Ʒ� ������ �շ����� ���̴� ������
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

	// ������� �Դٸ� �� ����
	return FALSE;
}


BOOL JumpPointSearch::Jump_UU(LONG loPosX, LONG loPosY, st_NODE *pParent)
{

	while (loPosY > 0)
	{
		loPosY--;

		// ���� ��ġ�� ��ֹ��̸� �׳� ��
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// ���������� Ȯ��
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			if (pParent != nullptr)
			{
				CreateNode(loPosX, loPosY, pParent, 0);

			}

			return TRUE;
		}

		m_loColorMap[loPosY][loPosX] = m_color;

		// ���̴� �������� Ȯ��
		// ������ �����̸� ���� Ȯ�� ���� �ʾƵ� ��
		if (loPosY != 0)
		{
			if (loPosX + 1 < dfTILE_WIDTH_COUNT)
			{
				if (m_byMap[loPosY][loPosX + 1] == dfTILE_OBSTACLE)
				{
					if (m_byMap[loPosY - 1][loPosX + 1] == dfTILE_NONE)
					{
						// �������� ���� ������ ����
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
						// �������� ���� ������ ����
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

	// ������� �Դٸ� �� ����
	return FALSE;
}

BOOL JumpPointSearch::Jump_LU(LONG loPosX, LONG loPosY, st_NODE *pParent)
{
	while (loPosX > 0 && loPosY > 0)
	{
		loPosX--;
		loPosY--;

		// ���� ��ġ�� ��ֹ��̸� �׳� ��
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// ���������� Ȯ��
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_NONE);

			return TRUE;
		}



		// ���� �˻�
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

		// ������� �Դٸ� �׳� �������� Ÿ���̶�� ��
		m_loColorMap[loPosY][loPosX] = m_color;

		BYTE byDir = dfDIR_NONE;

		// ���� Ž��
		if (TRUE == Jump_LL(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_LL;
		}

		// ���� Ž��
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

	// ������� �Դٸ� �� ����
	return FALSE;
}

BOOL JumpPointSearch::Jump_LD(LONG loPosX, LONG loPosY, st_NODE *pParent)
{

	while (loPosX > 0 && loPosY < dfTILE_HEIGHT_COUNT - 1)
	{
		loPosX--;
		loPosY++;

		// ���� ��ġ�� ��ֹ��̸� �׳� ��
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// ���������� Ȯ��
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_NONE);

			return TRUE;
		}

		// ���� �˻�
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

		// ������� �Դٸ� �׳� �������� Ÿ���̶�� ��
		m_loColorMap[loPosY][loPosX] = m_color;

		BYTE byDir = dfDIR_NONE;


		// ���� Ž��
		if (TRUE == Jump_LL(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_LL;
		}

		// �Ʒ��� Ž��
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

	// ������� �Դٸ� �� ����
	return FALSE;
}

BOOL JumpPointSearch::Jump_RU(LONG loPosX, LONG loPosY, st_NODE *pParent)
{

	while (loPosX < dfTILE_WIDTH_COUNT - 1 && loPosY > 0)
	{
		loPosX++;
		loPosY--;

		// ���� ��ġ�� ��ֹ��̸� �׳� ��
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// ���������� Ȯ��
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_NONE);

			return TRUE;
		}

		// ���� �˻�
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

		// ������ Ž��
		if (TRUE == Jump_RR(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_RR;
		}

		// ���� Ž��
		if (TRUE == Jump_UU(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_UU;
		}

		if (byDir != dfDIR_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, byDir);
			//return TRUE;
		}

		// ������� �Դٸ� �׳� �������� Ÿ���̶�� ��
		m_loColorMap[loPosY][loPosX] = m_color;
	}

	// ������� �Դٸ� �� ����
	return FALSE;
}

BOOL JumpPointSearch::Jump_RD(LONG loPosX, LONG loPosY, st_NODE *pParent)
{
	while (loPosX < dfTILE_WIDTH_COUNT - 1 && loPosY < dfTILE_HEIGHT_COUNT - 1)
	{
		loPosX++;
		loPosY++;

		// ���� ��ġ�� ��ֹ��̸� �׳� ��
		if (m_byMap[loPosY][loPosX] == dfTILE_OBSTACLE)
		{
			return FALSE;
		}

		// ���������� Ȯ��
		if (m_loEndX == loPosX && m_loEndY == loPosY)
		{
			CreateNode(loPosX, loPosY, pParent, dfDIR_NONE);

			return TRUE;
		}

		// ���� �˻�
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

		// ������ Ž��
		if (TRUE == Jump_RR(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_RR;
		}

		// �Ʒ��� Ž��
		if (TRUE == Jump_DD(loPosX, loPosY, nullptr))
		{
			byDir = byDir | dfDIR_DD;
		}

		if (byDir != dfDIR_NONE)
		{
			CreateNode(loPosX, loPosY, pParent, byDir);
			//return TRUE;
		}

		// ������� �Դٸ� �׳� �������� Ÿ���̶�� ��
		m_loColorMap[loPosY][loPosX] = m_color;
	}

	// ������� �Դٸ� �� ����
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

	// Openlist���� f���� ���� ���� ��� ã��
	stpNode = GetNodeMinF();

	// ���� �������
	if (nullptr == stpNode)
	{
		return FALSE;
	}

	//1.ã�� node�� ���������� Ȯ��
	if (stpNode->loPosX == m_loEndX
		&& stpNode->loPosY == m_loEndY)
	{
		m_stpCompleteNode = stpNode;

		return FALSE;
	}

	// �������� �ƴϸ� openlist���� ����
	CDoubleLinkedList<st_NODE*>::Iterator iter;

	for (iter = m_clOpenList->Begin(); iter != m_clOpenList->End(); iter++)
	{
		if ((*iter) == stpNode)
		{
			m_clOpenList->Erase(iter);
		}
	}

	//close�� �ֱ�
	m_clCloseList->PushBack(stpNode);

	// ���� �������̳� �����̸� �н�
	if (!(stpNode->loPosX == m_loStartX &&
		stpNode->loPosY == m_loStartY))
	{
		SetTileColor(stpNode->loPosX, stpNode->loPosY, enCLOSELIST);
	}

	// Ž���ϴ� �� ��ĥ���ֱ�
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

	// ��Ÿ����� ����
	// pow(x,2) x�� 2��,  sqrt() ������
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
	m_colorTable[0] = 0x908070; //slategray ������Ʈ�׷���
	m_colorTable[1] = 0x9afa00; //mediumspringgreen �̵�������׸�
	m_colorTable[2] = 0x696969; //dimgray ���׷���
	m_colorTable[3] = 0x808080; //gray �׷���
	m_colorTable[4] = 0xa9a9a9; //darkgray ��ũ�׷���
	m_colorTable[5] = 0xb9daff; //peachpuff ��ġ����
	m_colorTable[6] = 0xaae8ee; //palegoldenrod �ȷ��񵧷ε�
	m_colorTable[7] = 0xe1e4ff; //mistyrose �̽�Ƽ����
	m_colorTable[8] = 0xb5e4ff; //moccasin ��ī��
	m_colorTable[9] = 0xaddeff; //navajowhite ������ȭ��Ʈ
	m_colorTable[10] = 0x8cb4d2; //tan ź
	m_colorTable[11] = 0xb3def5; //wheat ��Ʈ
	m_colorTable[12] = 0x8ce6f0; //khaki īŰ
	m_colorTable[13] = 0xfae6e6; //lavender �󺥴�
	m_colorTable[14] = 0xb48246; //steelblue ��ƿ���
	m_colorTable[15] = 0xed9564; //cornflowerblue ���ö�����
	m_colorTable[16] = 0xe22b8a; //blueviolet �����̿÷�
	m_colorTable[17] = 0xc1b6ff; //lightpink ����Ʈ��ũ
	m_colorTable[18] = 0xb469ff; //hotpink ����ũ
	m_colorTable[19] = 0xcbc0ff; //pink ��ũ
	m_colorTable[20] = 0xdda0dd; //plum �÷�
	m_colorTable[22] = 0xd8bfd8; //thistle ��Ʋ
	m_colorTable[23] = 0xd670da; //orchid ��ġ��
	m_colorTable[24] = 0x7a96e9; //darksalmon ��ũ����
	m_colorTable[25] = 0x8080f0; //lightcoral ����Ʈ�ڶ�
	m_colorTable[26] = 0x5c5ccd; //indianred �ε�ȷ���
	m_colorTable[27] = 0x7aa0ff; //lightsalmon ����Ʈ����
	m_colorTable[28] = 0x9370db; //palevioletred �ȷ����̿÷�����
	m_colorTable[29] = 0x60a4f4; //sandybrown �������
	m_colorTable[30] = 0x7280fa; //salmon ����
	m_colorTable[31] = 0x13458b; //saddlebrown �������
	m_colorTable[32] = 0x2d52a0; //sienna �ÿ���
	m_colorTable[33] = 0x8f8fbc; //rosybrown ��������
	m_colorTable[34] = 0x507fff; //coral �ڶ�
	m_colorTable[35] = 0x008cff; //darkorange ��ũ������
	m_colorTable[36] = 0x00a5ff; //orange ������
	m_colorTable[37] = 0x0b86b8; //darkgoldenrod ��ũ�񵧷ε�
	m_colorTable[38] = 0x00d7ff; //gold ���
	m_colorTable[39] = 0x71b33c; //mediumseagreen �̵�򾾱׸�
	m_colorTable[40] = 0x2fffad; //greenyellow �׸����ο�
	m_colorTable[41] = 0x8fbc8f; //darkseagreen ��ũ���׸�
	m_colorTable[42] = 0x32cd9a; //yellowgreen ���ο�׸�
	m_colorTable[43] = 0x578b2e; //seagreen ���׸�
	m_colorTable[44] = 0x998877; //lightslategray ����Ʈ������Ʈ�׷���
	m_colorTable[45] = 0xaab220; //lightseagreen ����Ʈ���׸�
	
}

COLORREF JumpPointSearch::GetColor(void)
{
	int iIndex = 0;

	iIndex = rand() % 46;

	return m_colorTable[iIndex];
}
