#pragma once

struct st_TNODE
{
	struct st_TNODE *pLeft;
	struct st_TNODE *pRight;

	int iData;
};

class BSearchTree
{
	enum PRINT_ODER
	{
		enPreorder	= 0,
		enInorder	= 1,
		enPostorder	= 2
	};

public:
	BSearchTree();
	~BSearchTree();

	//------------------------------------------------------
	// ���ο� �����͸� �߰��Ѵ�.
	//------------------------------------------------------
	bool		InsertNode(int iData);

	//------------------------------------------------------
	// Ư�� �����͸� �����Ѵ�.
	//------------------------------------------------------
	bool		DeleteNode(int iData);

	//------------------------------------------------------
	// Ư�� �����͸� �˻��Ͽ� ��� ����
	//------------------------------------------------------
	st_TNODE	*SearchNode(int iData);

	//------------------------------------------------------
	// Ʈ�� ��ü ����
	//------------------------------------------------------
	bool		ReleaseTree(void);

	
	//------------------------------------------------------
	// ������ȸ ��� ���
	//------------------------------------------------------
	void		PreorderPrint(st_TNODE *pNode = nullptr, int iDepth = 0, int iLR = 0);

	//------------------------------------------------------
	// ������ȸ ��� ���
	//------------------------------------------------------
	void		InorderPrint(st_TNODE *pNode = nullptr, int iDepth = 0, int iLR = 0);

	//------------------------------------------------------
	// ������ȸ ��� ���
	//------------------------------------------------------
	void		PostorderPrint(st_TNODE *pNode = nullptr, int iDepth = 0, int iLR = 0);

private:
	//------------------------------------------------------
	// InsertNode ���ο��� ȣ��Ǵ� ��ġ ã�� �߰� ��� �Լ�
	//------------------------------------------------------
	bool		linkNode(st_TNODE *pParent, st_TNODE *pChild);

	//------------------------------------------------------
	// SearchNode ���ο��� ȣ��Ǵ� ���ã�� ����Լ�
	//------------------------------------------------------
	st_TNODE *	findNode(st_TNODE *pNode, int iData);

	//------------------------------------------------------
	// DeleteNode ���ο��� ȣ��Ǵ� ���ã�� & ���� & ��ó�� ����Լ�
	//------------------------------------------------------
	st_TNODE *	deleteNode(st_TNODE *pNode, st_TNODE *pParent, int iData);

	//------------------------------------------------------
	// ReleaseNode ���ο��� ȣ��Ǵ� ���ã�� ����Լ�
	//------------------------------------------------------
	void		releaseNode(st_TNODE *pNode);

	//------------------------------------------------------
	// Print ���ο��� ȣ��Ǵ� ���ã�� ����Լ�
	//------------------------------------------------------
	void		print(st_TNODE *pParent, int iDepth, int iLR, PRINT_ODER enOrder);

	void		PrintLeftFirst(st_TNODE *pParent, int iDepth);

	void		PrintRightFirst(st_TNODE *pParent, int iDepth);

	st_TNODE	*m_pRoot;

	int			m_iNodeCount;
};
