#include <stdio.h>

struct TREE_NODE
{
	struct TREE_NODE *pChild;
	struct TREE_NODE *pSibling;
	int iData;
};

// 트리 노드를 동적 생성하고 초기화 된 노드를 리턴 합니다.
TREE_NODE *CreateNode(int iData);  

// 인자로 입력 받은 노드를 delete 합니다. 
void DeleteNode(TREE_NODE *pNode); 

// 부모 노드를 인자로 받아 해당 노드의 자식으로 붙입니다.
void AddChildNode(TREE_NODE *pParent, TREE_NODE *pChild);  

// 트리를 출력 합니다.
void PrintTree(TREE_NODE *pNode, int iDepth);              

// 전체 노드를 삭제 (delete) 
void ReleaseTree(TREE_NODE *pRoot);			   


void main()
{
	TREE_NODE *pAddNode_R, *pAddNode_N;
	TREE_NODE *pRoot = CreateNode(0);

	pAddNode_R = CreateNode(1);
	AddChildNode(pRoot, pAddNode_R);

	pAddNode_N = CreateNode(11);
	AddChildNode(pAddNode_R, pAddNode_N);
	
	pAddNode_N = CreateNode(12);
	AddChildNode(pAddNode_R, pAddNode_N);
	
	pAddNode_N = CreateNode(13);
	AddChildNode(pAddNode_R, pAddNode_N);
	
	pAddNode_N = CreateNode(14);
	AddChildNode(pAddNode_R, pAddNode_N);
	
	pAddNode_N = CreateNode(15);
	AddChildNode(pAddNode_R, pAddNode_N);
	
	// pAddNode_R = CreateNode(2);
	// 이 부분이 주석처리 되면 sibling이 자기 자신이 되서 무한루프에 빠짐
	pAddNode_R = CreateNode(2);
	AddChildNode(pRoot, pAddNode_R);


	pAddNode_N = CreateNode(21);
	AddChildNode(pAddNode_R, pAddNode_N);

	pAddNode_N = CreateNode(22);
	AddChildNode(pAddNode_R, pAddNode_N);

	pAddNode_N = CreateNode(23);
	AddChildNode(pAddNode_R, pAddNode_N);

	pAddNode_N = CreateNode(24);
	AddChildNode(pAddNode_R, pAddNode_N);

	pAddNode_N = CreateNode(25);
	AddChildNode(pAddNode_R, pAddNode_N);

	pAddNode_R = CreateNode(3);
	AddChildNode(pRoot, pAddNode_R);

	pAddNode_N = CreateNode(31);
	AddChildNode(pAddNode_R, pAddNode_N);

	pAddNode_N = CreateNode(32);
	AddChildNode(pAddNode_R, pAddNode_N);

	pAddNode_N = CreateNode(33);
	AddChildNode(pAddNode_R, pAddNode_N);

	pAddNode_N = CreateNode(34);
	AddChildNode(pAddNode_R, pAddNode_N);

	pAddNode_N = CreateNode(35);

	AddChildNode(pAddNode_R, pAddNode_N);

	PrintTree(pRoot, 0);

	ReleaseTree(pRoot);
}

TREE_NODE *CreateNode(int iData)  // 트리 노드를 동적 생성하고 초기화 된 노드를 리턴 합니다.
{
	TREE_NODE *pNewNode = new TREE_NODE;

	pNewNode->iData		= iData;
	pNewNode->pChild	= nullptr;
	pNewNode->pSibling	= nullptr;

	return pNewNode;
}

void DeleteNode(TREE_NODE *pNode) // 인자로 입력 받은 노드를 delete 합니다. 
{
	delete pNode;
}

void AddChildNode(TREE_NODE *pParent, TREE_NODE *pChild)  // 부모 노드를 인자로 받아 해당 노드의 자식으로 붙입니다.
{
	if (pParent->pChild == nullptr)
	{
		pParent->pChild = pChild;
	}
	else
	{
		TREE_NODE *pTemp = pParent->pChild;
		
		while (pTemp->pSibling != nullptr )
		{
			pTemp = pTemp->pSibling;
		}

		pTemp->pSibling = pChild;
	}
}

void PrintTree(TREE_NODE *pNode, int iDepth)              // 트리를 출력 합니다.
{
	for (int iCount = 0; iCount < iDepth; iCount++)
	{
		printf(" ");
	}

	printf("%d\n", pNode->iData);

	if (pNode->pChild != nullptr)
	{
		PrintTree(pNode->pChild, iDepth + 1);
	}

	if (pNode->pSibling != nullptr)
	{
		PrintTree(pNode->pSibling, iDepth);
	}

	
}

void ReleaseTree(TREE_NODE *pRoot)			   // 전체 노드를 삭제 (delete) 
{
	if (pRoot->pSibling != nullptr)
	{
		ReleaseTree(pRoot->pSibling);
	}

	if (pRoot->pChild != nullptr)
	{
		ReleaseTree(pRoot->pChild);
	}

	// 여기까지 왔다는 것은 sibling과 child 모두 null이라는 것, 즉 루트
	pRoot->pSibling = nullptr;
	pRoot->pChild	= nullptr;

	// root 삭제
	DeleteNode(pRoot);
}