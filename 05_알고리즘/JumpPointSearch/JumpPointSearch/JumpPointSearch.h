#pragma once

#define dfSTART_X				5			
#define dfSTART_Y				5
#define dfEND_X					10
#define dfEND_Y					5

#define	dfTILE_WIDTH			15
#define	dfTILE_HEIGHT			15

#define dfTILE_WIDTH_COUNT		50
#define	dfTILE_HEIGHT_COUNT		40

#define	dfTILE_NONE				0
#define dfTILE_OBSTACLE			1
#define dfTILE_START			2
#define dfTILE_END				3

#define dfDIR_NONE				0x00000000
#define dfDIR_LL				0x00000001
#define dfDIR_LU				0x00000002
#define dfDIR_LD				0x00000004

#define dfDIR_RR				0x00000008
#define dfDIR_RU				0x00000010
#define dfDIR_RD				0x00000020

#define dfDIR_UU				0x00000040
#define dfDIR_DD				0x00000080
#define dfDIR_ALL				0x000000ff


// LinkedList ���漱��
template<typename T> class CDoubleLinkedList;

class JumpPointSearch
{
public:
	enum enCOLOR
	{
		enNONE,
		enOBSTACLE,
		enSTART,
		enEND,
		enOPENLIST,
		enCLOSELIST,
		enPASSED
	};

	struct st_NODE
	{
		st_NODE()
		{
			loPosX	= 0;
			loPosY	= 0;
			fG		= 0.f;
			fH		= 0.f;
			fF		= 0.f;
			loDir	= 0;
			pParent = nullptr;
		}
		// ��ǥ
		LONG loPosX;
		LONG loPosY;

		// ����ġ
		float	fG;
		float	fH;
		float	fF;

		// Ž�� ����
		LONG	loDir;

		// �θ� ���
		st_NODE *pParent;
	};

	JumpPointSearch();
	~JumpPointSearch();

	BOOL CreateNode(LONG loPosX, LONG loPosY, st_NODE *pParent, LONG loDir);

	// F�� ���� ���� ��� ã��
	st_NODE *GetNodeMinF(void);

	BOOL Jump_LL(LONG loPosX, LONG loPosY, st_NODE *pParent);
	BOOL Jump_RR(LONG loPosX, LONG loPosY, st_NODE *pParent);
	BOOL Jump_DD(LONG loPosX, LONG loPosY, st_NODE *pParent);
	BOOL Jump_UU(LONG loPosX, LONG loPosY, st_NODE *pParent);
	
	BOOL Jump_LU(LONG loPosX, LONG loPosY, st_NODE *pParent);
	BOOL Jump_LD(LONG loPosX, LONG loPosY, st_NODE *pParent);
	BOOL Jump_RU(LONG loPosX, LONG loPosY, st_NODE *pParent);
	BOOL Jump_RD(LONG loPosX, LONG loPosY, st_NODE *pParent);

	BOOL PathFinder(void);

	// �� �� ������ �Ÿ� ���ϱ�
	float GetDistance(LONG loPosX1, LONG loPosY1, LONG loPosX2, LONG loPosY2);

	// ������ ����
	void SetStartPoint(LONG loStartX = dfSTART_X, LONG loStartY = dfSTART_Y);

	// ������ ����
	void SetEndPoint(LONG loEndX = dfEND_X, LONG loEndY = dfEND_Y);

	// ���� ����
	void SetTileColor(LONG loPosX, LONG loPosY, enCOLOR color);

	// OpendList ����
	void ClearOpenList(void);
	
	// CloseList ����
	void ClearCloseList(void);

	// �ٽ� �˻��ϱ�
	void ReStart(void);

	// ��ֹ� �����
	void RemoveOBSTACLE(void);

	// ���� ���̺� �����
	void MakeColorTable(void);

	// ���� ���
	COLORREF GetColor(void);

	CDoubleLinkedList<st_NODE*> *m_clOpenList;
	CDoubleLinkedList<st_NODE*> *m_clCloseList;

	st_NODE		*m_stpCompleteNode;

	LONG		m_loStartX;
	LONG		m_loStartY;
	LONG		m_loEndX;
	LONG		m_loEndY;
	BOOL		m_bNewSearch;		// ���� �˻��ؾ��ϴ��� �Ǵ��ϴ� ����
	enCOLOR		m_enCurColor;
	COLORREF	m_color;
	COLORREF	m_colorTable[64];

	DWORD		m_dwMilliSeconds;
	BYTE		m_byMap[dfTILE_HEIGHT_COUNT][dfTILE_WIDTH_COUNT];
	LONG		m_loColorMap[dfTILE_HEIGHT_COUNT][dfTILE_WIDTH_COUNT];
};

