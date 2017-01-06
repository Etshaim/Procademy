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

	// 초기화
	void Init(void);

	// 프로세스
	void TestProcess(void);

	// 큐로 넣기
	void PutQueue(void);

	// 큐에서 빼기
	void GetQueue(void);

private:
	CStreamDQ*	pSQ;

	// 테스트용 버퍼
	char szTestBuf[eBUF_SIZE];

	char szGetData[eARRDATA_SIZE];

	char szPeekData[eARRDATA_SIZE];
	
	// 현재 인덱스
	int m_iCurIndex;

	bool m_bWrite;
	bool m_bIsFull;
};

#endif // __CQ_TEST__