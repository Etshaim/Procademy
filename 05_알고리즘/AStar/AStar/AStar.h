#pragma once

// LinkedList 전방선언
template<typename T> class CDoubleLinkedList;

class AStar
{
public:
	struct st_NODE
	{
		// 좌표
		LONG loPosX;
		LONG loPosY;

		// 가중치
		float	fG;
		float	fH;
		float	fF;

		// 부모 노드
		st_NODE *pParent;
	};

	AStar();
	~AStar();

	// 노드 생성
	BOOL CreateNode(LONG loPosX, LONG loPosY, st_NODE *pParent, BOOL bDiagonal = FALSE );

	// F가 가장 작은 노드 찾기
	st_NODE *GetNodeMinF(void);


	CDoubleLinkedList<st_NODE*> *m_clOpenList;
	CDoubleLinkedList<st_NODE*> *m_clCloseList;

	LONG m_loStartX;
	LONG m_loStartY;
	LONG m_loEndX;
	LONG m_loEndY;

	st_NODE *m_stpCompleteNode;

	BOOL m_bSearch;
};

