#include <stdio.h>

struct TREE_NODE
{
	struct TREE_NODE *pChild;
	struct TREE_NODE *pSibling;
	int iData;
};

// Ʈ�� ��带 ���� �����ϰ� �ʱ�ȭ �� ��带 ���� �մϴ�.
TREE_NODE *CreateNode(int iData);  

// ���ڷ� �Է� ���� ��带 delete �մϴ�. 
void DeleteNode(TREE_NODE *pNode); 

// �θ� ��带 ���ڷ� �޾� �ش� ����� �ڽ����� ���Դϴ�.
void AddChildNode(TREE_NODE *pParent, TREE_NODE *pChild);  

// Ʈ���� ��� �մϴ�.
void PrintTree(TREE_NODE *pNode, int iDepth);              

// ��ü ��带 ���� (delete) 
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
	// �� �κ��� �ּ�ó�� �Ǹ� sibling�� �ڱ� �ڽ��� �Ǽ� ���ѷ����� ����
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

TREE_NODE *CreateNode(int iData)  // Ʈ�� ��带 ���� �����ϰ� �ʱ�ȭ �� ��带 ���� �մϴ�.
{
	TREE_NODE *pNewNode = new TREE_NODE;

	pNewNode->iData		= iData;
	pNewNode->pChild	= nullptr;
	pNewNode->pSibling	= nullptr;

	return pNewNode;
}

void DeleteNode(TREE_NODE *pNode) // ���ڷ� �Է� ���� ��带 delete �մϴ�. 
{
	delete pNode;
}

void AddChildNode(TREE_NODE *pParent, TREE_NODE *pChild)  // �θ� ��带 ���ڷ� �޾� �ش� ����� �ڽ����� ���Դϴ�.
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

void PrintTree(TREE_NODE *pNode, int iDepth)              // Ʈ���� ��� �մϴ�.
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

void ReleaseTree(TREE_NODE *pRoot)			   // ��ü ��带 ���� (delete) 
{
	if (pRoot->pSibling != nullptr)
	{
		ReleaseTree(pRoot->pSibling);
	}

	if (pRoot->pChild != nullptr)
	{
		ReleaseTree(pRoot->pChild);
	}

	// ������� �Դٴ� ���� sibling�� child ��� null�̶�� ��, �� ��Ʈ
	pRoot->pSibling = nullptr;
	pRoot->pChild	= nullptr;

	// root ����
	DeleteNode(pRoot);
}