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

	// 초기화
	void Init(void);

	// 프로세스
	void TestProcess(void);

	// 큐로 넣기
	void PutQueue(void);

	// 큐에서 빼기
	void GetQueue(void);

private:
	CStreamSQ*	pSQ;

	// 테스트용 버퍼
	char szTestBuf[eBUF_SIZE];

	// 현재 인덱스
	int m_iCurIndex;
};

#endif // __CQ_TEST__