#pragma once

struct st_TNODE
{
	enum NODE_COLOR
	{
		enBLACK,
		enRED
	};

	st_TNODE *pParent;
	st_TNODE *pLeft;
	st_TNODE *pRight;

	NODE_COLOR enColor;

	int iData;
};

class CRedBlackTree
{
public:

	enum PRINT_ODER
	{
		enPreorder	= 0,
		enInorder	= 1,
		enPostorder = 2
	};

public:
	CRedBlackTree();
	~CRedBlackTree();

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


	// Ʈ�� ���
	void		DrawTree(HDC memDC, st_TNODE *pParent, int iDepth, POINT pParentPos);

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

	//------------------------------------------------------
	// Balance ���߱�
	//------------------------------------------------------
	void		balanceLink(st_TNODE *pNode);
	void		balanceDelete(st_TNODE *pNode);
	//------------------------------------------------------
	// �������� ȸ��
	//------------------------------------------------------
	void		rotationLeft(st_TNODE *pNode);

	//------------------------------------------------------
	// ���������� ȸ��
	//------------------------------------------------------
	void		rotationRight(st_TNODE *pNode);

private:
	st_TNODE	*m_pRoot;
	int			m_iNodeCount;
	st_TNODE	m_stNil;
};