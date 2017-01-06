#include "QTest.h"
#include "StreamSQ.h"
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
	pSQ = new CStreamSQ(eSQ_SIZE);

	//szTestBuf = new char[eMAX_NUM + 1];
	memset(szTestBuf, 0, eBUF_SIZE);

	//memcpy_s(szTestBuf, eBUF_SIZE, "123 abc ", eBUF_SIZE - 1);
	
	memcpy_s(szTestBuf, eBUF_SIZE,
		"1234567890 abcdefg hijklmn opqrstu vwxyz 111 222 333 444 555 666 777 888 999 000 ", eBUF_SIZE - 1);
	

	m_iCurIndex = 0;
}

// 프로세스
void CQTest::TestProcess(void)
{
	srand((unsigned int)time(NULL));
	
	Init();

	while (1)
	{
		int randNum = rand() % 4;

		if (randNum == 0)
		{
			GetQueue();
		}
		else
		{
			PutQueue();
		}
		/*switch (randNum)
		{
		case 0:
		{
			PutQueue();
		}
		break;

		case 1:
		{
			GetQueue();
		}
		break;

		default:
			break;
		}*/

		Sleep(10);

	}
}

// 큐로 넣기
void CQTest::PutQueue(void)
{
	int randNum = rand() % eMAX_INDEX + 1;
	
	if (randNum > (eMAX_INDEX - m_iCurIndex))
	{
		randNum = eMAX_INDEX - m_iCurIndex;
	}

	// 넣은 크기
	int inputNum = pSQ->Put(szTestBuf + m_iCurIndex, randNum);

	m_iCurIndex += inputNum;

	if (m_iCurIndex >= eMAX_INDEX)
	{
		m_iCurIndex = 0;
	}
}

// 큐에서 빼기
void CQTest::GetQueue(void)
{
	int randNum = rand() % eBUF_SIZE + 1;
	
	char szTemp[eBUF_SIZE + 1] = { 0, };

	pSQ->Get(szTemp, randNum);

	printf_s("%s", szTemp);
}