#include "BinaryTree.h"
#include <stdio.h>

BSearchTree::BSearchTree() 
{
	m_iNodeCount	= 0;
	m_pRoot			= nullptr;
}

BSearchTree::~BSearchTree()
{
	ReleaseTree();
}

bool BSearchTree::InsertNode(int iData)
{
	st_TNODE *pTemp = new st_TNODE;

	pTemp->iData	= iData;
	pTemp->pLeft	= nullptr;
	pTemp->pRight	= nullptr;

	if (nullptr == m_pRoot)
	{
		m_pRoot = pTemp;
		return true;
	}

	if (false == linkNode(m_pRoot, pTemp))
	{
		delete pTemp;

		return false;
	}
	
	return true;
}

bool BSearchTree::DeleteNode(int iData)
{
	st_TNODE *pDeleteNode = deleteNode(m_pRoot, nullptr, iData);

	if (nullptr != pDeleteNode)
	{
		delete pDeleteNode;
		return true;
	}
	else
	{
		return false;
	}
}

st_TNODE * BSearchTree::SearchNode(int iData)
{
	if (nullptr == m_pRoot)
	{
		return nullptr;
	}
	
	return findNode(m_pRoot, iData);
}

bool BSearchTree::ReleaseTree(void)
{
	if (nullptr != m_pRoot)
	{
		releaseNode(m_pRoot);
		return true;
	}
	else
	{
		return false;
	}
}

void BSearchTree::PreorderPrint(st_TNODE * pNode, int iDepth, int iLR)
{
	if (nullptr == pNode)
	{
		pNode = m_pRoot;
	}

	print(pNode, iDepth, iLR, enPreorder);
}

void BSearchTree::InorderPrint(st_TNODE * pNode, int iDepth, int iLR)
{
	if (nullptr == pNode)
	{
		pNode = m_pRoot;
	}

	print(pNode, iDepth, iLR, enInorder);
}

void BSearchTree::PostorderPrint(st_TNODE * pNode, int iDepth, int iLR)
{
	if (nullptr == pNode)
	{
		pNode = m_pRoot;
	}

	print(pNode, iDepth, iLR, enPostorder);
}


bool BSearchTree::linkNode(st_TNODE * pParent, st_TNODE * pChild)
{
	// ������ ��������
	if (pParent->iData > pChild->iData)
	{
		// �� ��Ʈ���� �����ߴٸ� �ֱ�
		if (nullptr == pParent->pLeft)
		{
			pParent->pLeft = pChild;
			return true;
		}
		else
		{
			return linkNode(pParent->pLeft, pChild);
		}
	}

	if (pParent->iData < pChild->iData)
	{
		if (nullptr == pParent->pRight)
		{
			pParent->pRight = pChild;
			return true;
		}
		else
		{
			return linkNode(pParent->pRight, pChild);
		}
	}

	return false;
}

st_TNODE * BSearchTree::findNode(st_TNODE * pNode, int iData)
{
	if (pNode->iData > iData)
	{
		if (nullptr == pNode->pLeft)
		{
			return nullptr;
		}

		findNode(pNode->pLeft, iData);
	}

	if (pNode->iData < iData)
	{
		if (nullptr == pNode->pRight)
		{
			return nullptr;
		}

		findNode(pNode->pRight, iData);
	}

	return pNode;
}

st_TNODE * BSearchTree::deleteNode(st_TNODE * pNode, st_TNODE * pParent, int iData)
{
	if (pNode->iData > iData)
	{
		if (nullptr == pNode->pLeft)
		{
			return nullptr;
		}
		else
		{
			return deleteNode(pNode->pLeft, pNode, iData);
		}
	}

	else if (pNode->iData < iData)
	{
		if (nullptr == pNode->pRight)
		{
			return nullptr;
		}
		else
		{
			return deleteNode(pNode->pRight, pNode, iData);
		}
	}
	else
	{
		// ������� �Դٴ� ���� ������ ��带 ã�Ҵٴ� ��
		// ������ ����� ���´� 3����

		// 1.�ڽ��� ���� ���
		if (nullptr == pNode->pLeft && nullptr == pNode->pRight)
		{
			// �θ� ��� null ó��
			if (pParent->pLeft == pNode)
			{
				pParent->pLeft = nullptr;
			}
			else
			{
				pParent->pRight = nullptr;
			}
			return pNode;
		}
		// 2. �ڽ��� �� �� �ִ� ���
		else if (nullptr != pNode->pLeft && nullptr != pNode->pRight)
		{
			// ��Ÿ
			// ����� ��带 ã�� ����
			// right �ڼ��߿� ���� ���� �༮����
			st_TNODE *pSubstitute = pNode->pRight;

			while (nullptr != pSubstitute->pLeft)
			{
				pSubstitute = pSubstitute->pLeft;
			}

			// pSubstitute left�� null������ right�� �ڽ��� ���� ���� �����Ƿ�
			pSubstitute = deleteNode(pNode, nullptr, pSubstitute->iData);
			
			pSubstitute->pRight = pNode->pRight;
			pSubstitute->pLeft = pNode->pLeft;
			
			if (pNode == m_pRoot)
			{
				m_pRoot = pSubstitute;
			}

			return pNode;
		}

		// 3. �ڽ��� �ϳ��� �ִ� ���
		else
		{
			// �ڽ��� �ϳ��� �ִ� ���� �׳� ��ü�ϸ� ��
			st_TNODE *pSubstitute = pNode;

			if (pNode->pLeft != nullptr)
			{
				pSubstitute = pNode->pLeft;
			}
			else
			{
				pSubstitute = pNode->pRight;
			}

			// �������ֱ�
			if (pParent->pLeft == pNode)
			{
				pParent->pLeft = pSubstitute;
			}
			else
			{
				pParent->pRight = pSubstitute;
			}

			if (pNode == m_pRoot)
			{
				m_pRoot = pSubstitute;
			}

			return pNode;
		}
		
	}
}

void BSearchTree::releaseNode(st_TNODE * pNode)
{
	if (nullptr != pNode->pLeft)
	{
		releaseNode(pNode->pLeft);
	}

	if (nullptr != pNode->pRight)
	{
		releaseNode(pNode->pRight);
	}

	delete pNode;
}

void BSearchTree::print(st_TNODE * pParent, int iDepth, int iLR, PRINT_ODER enOrder)
{
	st_TNODE *pFirstDir		= nullptr;
	st_TNODE *pSecondDir	= nullptr;

	if (iLR != 0)
	{
		pFirstDir	= pParent->pLeft;
		pSecondDir	= pParent->pRight;
	}
	else
	{
		pFirstDir = pParent->pRight;
		pSecondDir = pParent->pLeft;
	}

	if (enPreorder == enOrder)
	{
		for (int iCount = 0; iCount < iDepth; iCount++)
		{
			printf("  ");
		}

		printf("%d\n", pParent->iData);
	}


	if (nullptr != pFirstDir)
	{
		PrintLeftFirst(pFirstDir, iDepth + 1);
	}

	if (enInorder == enOrder)
	{
		for (int iCount = 0; iCount < iDepth; iCount++)
		{
			printf("  ");
		}

		printf("%d\n", pParent->iData);
	}


	if (nullptr != pSecondDir)
	{
		PrintLeftFirst(pSecondDir, iDepth + 1);
	}

	if (enPostorder == enOrder)
	{
		for (int iCount = 0; iCount < iDepth; iCount++)
		{
			printf("  ");
		}

		printf("%d\n", pParent->iData);
	}
}

void BSearchTree::PrintLeftFirst(st_TNODE * pParent, int iDepth)
{
	for (int iCount = 0; iCount < iDepth; iCount++)
	{
		printf("  ");
	}

	printf("%d\n", pParent->iData);


	if (nullptr != pParent->pLeft)
	{
		PrintLeftFirst(pParent->pLeft, iDepth + 1);
	}

	if (nullptr != pParent->pRight)
	{
		PrintLeftFirst(pParent->pRight, iDepth + 1);
	}
}

void BSearchTree::PrintRightFirst(st_TNODE * pParent, int iDepth)
{
	

	if (nullptr != pParent->pRight)
	{
		PrintRightFirst(pParent->pRight, iDepth + 1);
	}

	for (int iCount = 0; iCount < iDepth; iCount++)
	{
		printf("	");
	}

	printf("%d\n", pParent->iData);

	if (nullptr != pParent->pLeft)
	{
		PrintRightFirst(pParent->pLeft, iDepth + 1);
	}

	
}
