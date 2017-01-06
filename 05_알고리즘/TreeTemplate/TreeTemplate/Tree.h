#pragma once

template <typename DATA>
struct stTREE_NODE
{
	stTREE_NODE *pLeftChild;
	stTREE_NODE *pRightSibling;
	DATA Data;
};

template <typename DATA>
class CTree
{
public:
	CTree();
	~CTree();

	bool CreateNode(DATA Data, DATA ParentData);

	stTREE_NODE<DATA> *SearchNode(DATA Data, stTREE_NODE<DATA> *pNode = NULL);
	
	void ReleaseTree(stTREE_NODE<DATA> *pRoot);
	void Print(stTREE_NODE<DATA> *pRoot = nullptr, int iDepth = 0);
	void PrintTree(stTREE_NODE<DATA> *pRoot, int iDepth);

protected:
	void AddChildNode(stTREE_NODE<DATA> *pParent, stTREE_NODE<DATA> *pChild);
	void DeleteNode(stTREE_NODE<DATA> *pNode);

	stTREE_NODE<DATA> *m_pRoot;
};

template <typename DATA>
CTree<DATA>::CTree()
{
	m_pRoot = nullptr;
}

template <typename DATA>
CTree<DATA>::~CTree()
{
	ReleaseTree(m_pRoot);
}

template <typename DATA>
bool CTree<DATA>::CreateNode(DATA Data, DATA ParentData)
{
	// 
	if (m_pRoot == nullptr)
	{
		m_pRoot = new stTREE_NODE<DATA>;

		m_pRoot->Data = Data;
		m_pRoot->pLeftChild = nullptr;
		m_pRoot->pRightSibling = nullptr;

		return true;
	}

	else
	{
		// ParentData�� ã�´�
		stTREE_NODE<DATA> *pParentNode = SearchNode(ParentData, m_pRoot);

		stTREE_NODE<DATA> *pNewNode = new stTREE_NODE<DATA>;

		pNewNode->Data = Data;
		pNewNode->pLeftChild = nullptr;
		pNewNode->pRightSibling = nullptr;

		// ��ã���� ��� ��Ʈ�� �ִ´�
		if (nullptr == pParentNode)
		{
			AddChildNode(m_pRoot, pNewNode);
		}
		else
		{
			AddChildNode(pParentNode, pNewNode);
		}

		return true;
	}
}


template <typename DATA>
typename  stTREE_NODE<DATA>* CTree<DATA>::SearchNode(DATA Data, typename stTREE_NODE<DATA> *pNode)
{
	stTREE_NODE<DATA> *pSearchNode = NULL;

	if (0 == memcmp(&(pNode->Data), &Data, sizeof(Data)))
	{
		return pNode;
	}

	// �������� ã�´�
	if (pNode->pRightSibling != NULL)
	{
		pSearchNode = SearchNode(Data, pNode->pRightSibling);

		if (NULL != pSearchNode)
		{
			// �̰��� ���Դٴ� ���� ��带 ã�Ҵٴ� ���� �ǹ�
			// ã�� ��带 return �ϰ� ������
			return pSearchNode;
		}
	}

	// �ڽ����� ã�´�
	if (pNode->pLeftChild != NULL)
	{
		pSearchNode = SearchNode(Data, pNode->pLeftChild);

		if (NULL != pSearchNode)
		{
			// �̰��� ���Դٴ� ���� ��带 ã�Ҵٴ� ���� �ǹ�
			// ã�� ��带 return �ϰ� ������
			return pSearchNode;
		}
	}
	
	// ������� �Դٴ� ���� ���� �ڽ� ��� ���ٴ� ��
	// pSearchNode 
	return pSearchNode;
	
}

template <typename DATA>
void CTree<DATA>::ReleaseTree(typename stTREE_NODE<DATA> *pRoot)
{
	// �ڽ������� Ÿ�� ���鼭 ����
	if (pRoot->pLeftChild != nullptr)
	{
		ReleaseTree(pRoot->pLeftChild);
	}

	// ���������� Ÿ�� ���鼭 ����

	if (pRoot->pRightSibling != nullptr)
	{
		ReleaseTree(pRoot->pRightSibling);
	}

	pRoot->pLeftChild = nullptr;
	pRoot->pRightSibling = nullptr;

	// �̰����� �Դٴ� ���� �ڽ� ���� ���ٴ� ��
	DeleteNode(pRoot);
}
template <typename DATA>
void CTree<DATA>::Print(typename stTREE_NODE<DATA> *pRoot, int iDepth)
{
	if (pRoot == nullptr)
	{
		pRoot = m_pRoot;
	}

	PrintTree(pRoot, iDepth);
}

template <typename DATA>
void CTree<DATA>::PrintTree(typename stTREE_NODE<DATA> *pRoot, int iDepth)
{
	// Depth��ŭ �鿩����
	for (int iCount = 0; iCount < iDepth; iCount++)
	{
		printf(" ");
	}

	printf("%d\n", pRoot->Data);
	

	if (pRoot->pLeftChild != nullptr)
	{
		PrintTree(pRoot->pLeftChild, iDepth + 1);
	}

	if (pRoot->pRightSibling != nullptr)
	{
		PrintTree(pRoot->pRightSibling, iDepth);
	}
}

template <typename DATA>
void CTree<DATA>::AddChildNode(typename stTREE_NODE<DATA> *pParent, typename stTREE_NODE<DATA> *pChild)
{
	// �θ� ����� �ڽ��� ���ٸ� �θ���� ���� ��Ű��
	// �θ� ����� �ڽ��� �ִٸ� �� �ڽ��� ���� ��带 Ÿ�� ���� �� ���� ���� ����
	if (pParent->pLeftChild == nullptr)
	{
		pParent->pLeftChild = pChild;
	}
	else
	{
		// ������ sibling ã�� �뵵
		stTREE_NODE<DATA> *pTemp = pParent->pLeftChild;

		while (pTemp->pRightSibling != nullptr)
		{
			pTemp = pTemp->pRightSibling;
		}

		pTemp->pRightSibling = pChild;
	}
}

template <typename DATA>
void CTree<DATA>::DeleteNode(typename stTREE_NODE<DATA> *pNode)
{
	delete pNode;
}

