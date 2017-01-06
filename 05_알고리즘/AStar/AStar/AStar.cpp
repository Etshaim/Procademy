#include "stdafx.h"
#include "AStar.h"
#include "LinkedList.h"

#define dfSTART_POS_X	5
#define dfSTART_POS_Y	5
#define dfEND_POS_X		10	
#define dfEND_POS_Y		5

AStar::AStar()
{
	m_clOpenList = new CDoubleLinkedList<st_NODE*>;
	m_clCloseList	= new CDoubleLinkedList<st_NODE*>;

	m_stpCompleteNode = nullptr;
}

AStar::~AStar()
{
	if (nullptr != m_clCloseList)
	{
		m_clCloseList->Clear();
	}

	if (nullptr != m_clOpenList)
	{
		m_clOpenList->Clear();
	}
	

	m_clOpenList	= nullptr;
	m_clCloseList	= nullptr;
}

BOOL AStar::CreateNode(LONG loPosX, LONG loPosY, st_NODE * pParent, BOOL bDiagonal)
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
	stpNode				= new st_NODE;
	stpNode->loPosX		= loPosX;
	stpNode->loPosY		= loPosY;
	stpNode->pParent	= pParent;

	
	if (loPosX == m_loEndX && loPosY == m_loEndY)
	{
		m_stpCompleteNode = stpNode;
	}

	if( stpNode->pParent != nullptr)
	{ 
		if (bDiagonal = TRUE)
		{
			stpNode->fG = pParent->fG + 1.5f;
		}
		else
		{
			stpNode->fG = pParent->fG + 1.0f;
		}
	}
	else
	{
		stpNode->fG = 0;
	}
	
	stpNode->fH			= (float)(abs(m_loEndX - loPosX) + abs(m_loEndY - loPosY));
	stpNode->fF			= stpNode->fG + stpNode->fH ;
	
	m_clOpenList->PushBack(stpNode);

	return TRUE;
}

AStar::st_NODE* AStar::GetNodeMinF(void)
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
