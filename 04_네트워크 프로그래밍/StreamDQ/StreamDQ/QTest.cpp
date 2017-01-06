#include "QTest.h"
#include "StreamDQ.h"
#include <stdlib.h>
#include <memory.h>
#include <time.h>
#include <Windows.h>
#include <stdio.h>


CQTest::CQTest()
{

}

CQTest::~CQTest()
{
}

// 초기화
void CQTest::Init(void)
{
	pSQ = new CStreamDQ(eSQBUFF_SIZE);

	//szTestBuf = new char[eMAX_NUM + 1];
	memset(szTestBuf, 0, eBUF_SIZE);
	memset(szGetData, 0, eARRDATA_SIZE);
	memset(szPeekData, 0, eARRDATA_SIZE);
	//memcpy_s(szTestBuf, eBUF_SIZE, "123 abc ", eBUF_SIZE - 1);

	memcpy_s(szTestBuf, eBUF_SIZE,
		"1234567890 abcdefghij klmnopqrst uvwxyz 111 222 333 444 555 666 777 888 999 0000 ", eBUF_SIZE - 1);


	m_iCurIndex = 0;
}

// 프로세스
void CQTest::TestProcess(void)
{
	//srand((unsigned int)time(NULL));

	Init();

	while (1)
	{

		if (!m_bIsFull)
		{
			PutQueue();
		}

		if (true == pSQ->FlipBuffer())
		{
			m_bIsFull = false;
		}
		else
		{
			GetQueue();
		}
	}
}

// 큐로 넣기
void CQTest::PutQueue(void)
{

	int iPutSize = rand() % eBUF_SIZE;

	if (iPutSize > (eMAX_INDEX - m_iCurIndex))
	{
		iPutSize = eMAX_INDEX - m_iCurIndex;
	}

	// 넣은 크기
	int inputNum = pSQ->Put(szTestBuf + m_iCurIndex, iPutSize);

	if (inputNum != iPutSize)
	{
		m_bIsFull = TRUE;
		//Sleep(100);
	}

	m_iCurIndex += inputNum;

	if (m_iCurIndex == eMAX_INDEX)
	{
		m_iCurIndex = 0;
	}
}

// 큐에서 빼기
void CQTest::GetQueue(void)
{
	int randNum = rand() % eSQBUFF_SIZE + 1;

	//char szTemp[eBUF_SIZE + 1] = { 0, };

	//pSQ->Get(szTemp, randNum);

	memset(szGetData, 0, eARRDATA_SIZE);
	memset(szPeekData, 0, eARRDATA_SIZE);

	pSQ->Peek(szPeekData, randNum);
	pSQ->Get(szGetData, randNum);

	if (0 != memcmp(szGetData, szPeekData, randNum))
	{
		printf_s("                 ");
	}

	printf_s("%s", szGetData);
}