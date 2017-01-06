#pragma once

// LinkedList ���漱��
template<typename T> class CDoubleLinkedList;

class AStar
{
public:
	struct st_NODE
	{
		// ��ǥ
		LONG loPosX;
		LONG loPosY;

		// ����ġ
		float	fG;
		float	fH;
		float	fF;

		// �θ� ���
		st_NODE *pParent;
	};

	AStar();
	~AStar();

	// ��� ����
	BOOL CreateNode(LONG loPosX, LONG loPosY, st_NODE *pParent, BOOL bDiagonal = FALSE );

	// F�� ���� ���� ��� ã��
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

