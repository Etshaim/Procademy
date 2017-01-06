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
		// ParentData를 찾는다
		stTREE_NODE<DATA> *pParentNode = SearchNode(ParentData, m_pRoot);

		stTREE_NODE<DATA> *pNewNode = new stTREE_NODE<DATA>;

		pNewNode->Data = Data;
		pNewNode->pLeftChild = nullptr;
		pNewNode->pRightSibling = nullptr;

		// 못찾았을 경우 루트에 넣는다
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

	// 형제쪽을 찾는다
	if (pNode->pRightSibling != NULL)
	{
		pSearchNode = SearchNode(Data, pNode->pRightSibling);

		if (NULL != pSearchNode)
		{
			// 이곳에 들어왔다는 것은 노드를 찾았다는 것을 의미
			// 찾은 노드를 return 하고 끝낸다
			return pSearchNode;
		}
	}

	// 자식쪽을 찾는다
	if (pNode->pLeftChild != NULL)
	{
		pSearchNode = SearchNode(Data, pNode->pLeftChild);

		if (NULL != pSearchNode)
		{
			// 이곳에 들어왔다는 것은 노드를 찾았다는 것을 의미
			// 찾은 노드를 return 하고 끝낸다
			return pSearchNode;
		}
	}
	
	// 여기까지 왔다는 것은 형제 자식 모두 없다는 것
	// pSearchNode 
	return pSearchNode;
	
}

template <typename DATA>
void CTree<DATA>::ReleaseTree(typename stTREE_NODE<DATA> *pRoot)
{
	// 자식쪽으로 타고 들어가면서 삭제
	if (pRoot->pLeftChild != nullptr)
	{
		ReleaseTree(pRoot->pLeftChild);
	}

	// 형제쪽으로 타고 들어가면서 삭제

	if (pRoot->pRightSibling != nullptr)
	{
		ReleaseTree(pRoot->pRightSibling);
	}

	pRoot->pLeftChild = nullptr;
	pRoot->pRightSibling = nullptr;

	// 이곳까지 왔다는 것은 자식 형제 없다는 것
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
	// Depth만큼 들여쓰기
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
	// 부모 노드의 자식이 없다면 부모노드와 연결 시키고
	// 부모 노드의 자식이 있다면 그 자식의 형제 노드를 타고 가서 맨 끝에 연결 해줌
	if (pParent->pLeftChild == nullptr)
	{
		pParent->pLeftChild = pChild;
	}
	else
	{
		// 마지막 sibling 찾는 용도
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

