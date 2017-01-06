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
	// 새로운 데이터를 추가한다.
	//------------------------------------------------------
	bool		InsertNode(int iData);

	//------------------------------------------------------
	// 특정 데이터를 삭제한다.
	//------------------------------------------------------
	bool		DeleteNode(int iData);

	//------------------------------------------------------
	// 특정 데이터를 검색하여 노드 리턴
	//------------------------------------------------------
	st_TNODE	*SearchNode(int iData);

	//------------------------------------------------------
	// 트리 전체 삭제
	//------------------------------------------------------
	bool		ReleaseTree(void);


	//------------------------------------------------------
	// 전위순회 방식 출력
	//------------------------------------------------------
	void		PreorderPrint(st_TNODE *pNode = nullptr, int iDepth = 0, int iLR = 0);

	//------------------------------------------------------
	// 중위순회 방식 출력
	//------------------------------------------------------
	void		InorderPrint(st_TNODE *pNode = nullptr, int iDepth = 0, int iLR = 0);

	//------------------------------------------------------
	// 후위순회 방식 출력
	//------------------------------------------------------
	void		PostorderPrint(st_TNODE *pNode = nullptr, int iDepth = 0, int iLR = 0);


	// 트리 출력
	void		DrawTree(HDC memDC, st_TNODE *pParent, int iDepth, POINT pParentPos);

private:
	//------------------------------------------------------
	// InsertNode 내부에서 호출되는 위치 찾아 추가 재귀 함수
	//------------------------------------------------------
	bool		linkNode(st_TNODE *pParent, st_TNODE *pChild);

	//------------------------------------------------------
	// SearchNode 내부에서 호출되는 노드찾기 재귀함수
	//------------------------------------------------------
	st_TNODE *	findNode(st_TNODE *pNode, int iData);

	//------------------------------------------------------
	// DeleteNode 내부에서 호출되는 노드찾기 & 삭제 & 후처리 재귀함수
	//------------------------------------------------------
	st_TNODE *	deleteNode(st_TNODE *pNode, st_TNODE *pParent, int iData);

	//------------------------------------------------------
	// ReleaseNode 내부에서 호출되는 노드찾기 재귀함수
	//------------------------------------------------------
	void		releaseNode(st_TNODE *pNode);

	//------------------------------------------------------
	// Print 내부에서 호출되는 노드찾기 재귀함수
	//------------------------------------------------------
	void		print(st_TNODE *pParent, int iDepth, int iLR, PRINT_ODER enOrder);

	//------------------------------------------------------
	// Balance 맞추기
	//------------------------------------------------------
	void		balanceLink(st_TNODE *pNode);
	void		balanceDelete(st_TNODE *pNode);
	//------------------------------------------------------
	// 왼쪽으로 회전
	//------------------------------------------------------
	void		rotationLeft(st_TNODE *pNode);

	//------------------------------------------------------
	// 오른쪽으로 회전
	//------------------------------------------------------
	void		rotationRight(st_TNODE *pNode);

private:
	st_TNODE	*m_pRoot;
	int			m_iNodeCount;
	st_TNODE	m_stNil;
};