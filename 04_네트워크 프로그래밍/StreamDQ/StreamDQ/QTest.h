#ifndef __CQ_TEST__
#define __CQ_TEST__

#include "StreamDQ.h"

class CQTest
{
public:
	enum eCQTest
	{
		eBUF_SIZE		= 82,
		eMAX_INDEX		= 81,
		eSQBUFF_SIZE	= 30,
		eARRDATA_SIZE	= 100,
	};

public:
	CQTest();
	~CQTest();

	// �ʱ�ȭ
	void Init(void);

	// ���μ���
	void TestProcess(void);

	// ť�� �ֱ�
	void PutQueue(void);

	// ť���� ����
	void GetQueue(void);

private:
	CStreamDQ*	pSQ;

	// �׽�Ʈ�� ����
	char szTestBuf[eBUF_SIZE];

	char szGetData[eARRDATA_SIZE];

	char szPeekData[eARRDATA_SIZE];
	
	// ���� �ε���
	int m_iCurIndex;

	bool m_bWrite;
	bool m_bIsFull;
};

#endif // __CQ_TEST__