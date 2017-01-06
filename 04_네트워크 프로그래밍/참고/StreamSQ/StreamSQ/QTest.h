#ifndef __CQ_TEST__
#define __CQ_TEST__

#include "StreamSQ.h"

class CQTest
{
public:
	enum eCQTest
	{
		eBUF_SIZE	= 81,
		eMAX_INDEX	= 80,
		eNULL_INDEX = 80,
		eSQ_SIZE	= 200,
	};

public :
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
	CStreamSQ*	pSQ;

	// �׽�Ʈ�� ����
	char szTestBuf[eBUF_SIZE];

	// ���� �ε���
	int m_iCurIndex;
};

#endif // __CQ_TEST__