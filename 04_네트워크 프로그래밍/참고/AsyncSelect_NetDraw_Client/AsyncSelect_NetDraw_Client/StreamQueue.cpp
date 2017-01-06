// StreamQueue.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "stdafx.h"
#include "StreamQueue.h"

CStreamQueue::CStreamQueue()
{
	m_iBufferSize = e_CSTREAMQUEUE::eBUFFER_DEFAULT;
	Initial(m_iBufferSize);
}

CStreamQueue::CStreamQueue(int iBufferSize)
{
	Initial(iBufferSize);
}

CStreamQueue::~CStreamQueue()
{
	delete[] m_chpBuffer;
	m_chpBuffer = NULL;
}

void CStreamQueue::Initial(int iBufferSize)
{
	if (m_chpBuffer != NULL)
		delete[] m_chpBuffer;

	m_chpBuffer = new char[iBufferSize];
	memset(m_chpBuffer, 0, iBufferSize);
	m_iBufferSize = iBufferSize - e_CSTREAMQUEUE::eBUFFER_BLANK;

	m_iFront = 0;
	m_iRear = 0;
}

int CStreamQueue::GetBufferSize(void)
{
	if (m_chpBuffer == NULL)
		return 0;
	else
		return m_iBufferSize;
}

int CStreamQueue::GetRemainingSize(void)
{
	if (m_chpBuffer == NULL)
		return 0;

	if (m_iRear >= m_iFront)
		return (m_iBufferSize - m_iRear) + m_iFront;
	else
		return m_iFront - m_iRear - 1;
}

int CStreamQueue::GetOccupyingSize(void)
{
	if (m_chpBuffer == NULL)
		return 0;

	if (m_iRear >= m_iFront)
		return m_iRear - m_iFront;
	else
		return (m_iBufferSize - m_iFront + 1) + m_iRear;
}

int CStreamQueue::GetNotCirculatedEnqueuingSize(void)
{
	if (m_chpBuffer == NULL)
		return 0;

	if (m_iRear >= m_iFront)
		return m_iBufferSize - m_iRear + 1;
	else
		return m_iFront - m_iRear - 1;
}

int CStreamQueue::GetNotCirculatedDequeuingSize(void)
{
	if (m_chpBuffer == NULL)
		return 0;

	if (m_iRear > m_iFront)
		return m_iRear - m_iFront;
	else
		return m_iBufferSize - m_iFront + 1;
}

int CStreamQueue::Enqueue(char *chpData, int iSize)
{
	if (m_chpBuffer == NULL)
		return 0;

	int iRemainingSize = GetRemainingSize();
	int iNotCirculatedEnqueuingSize = GetNotCirculatedEnqueuingSize();

	if (iSize > iRemainingSize)
		iSize = iRemainingSize;

	if (m_iRear >= m_iFront)
	{
		if (iSize > iNotCirculatedEnqueuingSize)
		{
			memcpy(m_chpBuffer + m_iRear, chpData, iNotCirculatedEnqueuingSize);
			memcpy(m_chpBuffer, chpData + iNotCirculatedEnqueuingSize, iSize - iNotCirculatedEnqueuingSize);
			m_iRear = iSize - iNotCirculatedEnqueuingSize;
			return iSize;
		}
		else
		{
			memcpy(m_chpBuffer + m_iRear, chpData, iSize);

			if (m_iRear + iSize - 1 == m_iBufferSize)
				m_iRear = 0;
			else
				m_iRear += iSize;

			return iSize;
		}
	}
	else
	{
		memcpy(m_chpBuffer + m_iRear, chpData, iSize);
		m_iRear += iSize;
		return iSize;
	}
}

int CStreamQueue::Dequeue(char *chpData, int iSize)
{
	if (m_chpBuffer == NULL)
		return 0;

	int iOccupyingSize = GetOccupyingSize();
	int iNotCirculatedDequeuingSize = GetNotCirculatedDequeuingSize();

	if (iSize > iOccupyingSize)
		iSize = iOccupyingSize;

	if (m_iRear >= m_iFront)
	{
		memcpy(chpData, m_chpBuffer + m_iFront, iSize);
		m_iFront += iSize;
		return iSize;
	}
	else
	{
		if (iSize > iNotCirculatedDequeuingSize)
		{
			memcpy(chpData, m_chpBuffer + m_iFront, iNotCirculatedDequeuingSize);
			memcpy(chpData + iNotCirculatedDequeuingSize, m_chpBuffer, iSize - iNotCirculatedDequeuingSize);
			m_iFront = iSize - iNotCirculatedDequeuingSize;
			return iSize;
		}
		else
		{
			memcpy(chpData, m_chpBuffer + m_iFront, iSize);

			if (m_iFront + iSize - 1 == m_iBufferSize)
				m_iFront = 0;
			else
				m_iFront += iSize;

			return iSize;
		}
	}
}

int CStreamQueue::Peek(char *chpDest, int iSize)
{
	if (m_chpBuffer == NULL)
		return 0;

	int iOccupyingSize = GetOccupyingSize();
	int iNotCirculatedDequeuingSize = GetNotCirculatedDequeuingSize();

	if (iSize > iOccupyingSize)
		iSize = iOccupyingSize;

	if (m_iRear >= m_iFront)
	{
		memcpy(chpDest, m_chpBuffer + m_iFront, iSize);
		return iSize;
	}
	else
	{
		if (iSize > iNotCirculatedDequeuingSize)
		{
			memcpy(chpDest, m_chpBuffer + m_iFront, iNotCirculatedDequeuingSize);
			memcpy(chpDest + iNotCirculatedDequeuingSize, m_chpBuffer, iSize - iNotCirculatedDequeuingSize);
			return iSize;
		}
		else
		{
			memcpy(chpDest, m_chpBuffer + m_iFront, iSize);
			return iSize;
		}
	}
}

int CStreamQueue::RemoveData(int iSize)
{
	if (m_chpBuffer == NULL)
		return 0;
	
	if (m_iFront + iSize - 1 >= m_iBufferSize)
		m_iFront = (m_iFront + iSize - 1) - m_iBufferSize;
	else
		m_iFront += iSize;

	return iSize;
}


int CStreamQueue::MoveRear(int iSize)
{
	if (m_chpBuffer == NULL)
		return 0;

	if (m_iRear + iSize - 1 >= m_iBufferSize)
		m_iRear = (m_iRear + iSize - 1) - m_iBufferSize;
	else
		m_iRear += iSize;

	return iSize;
}

void CStreamQueue::ClearBuffer(void)
{
	if (m_chpBuffer == NULL)
		return;

	m_iFront = 0;
	m_iRear = 0;
}

char* CStreamQueue::GetBufferPtr(void)
{
	if (m_chpBuffer == NULL)
		return nullptr;

	return (char *)&m_chpBuffer;
}

char* CStreamQueue::GetFrontBufferPtr(void)
{
	if (m_chpBuffer == NULL)
		return nullptr;

	return (char *)(m_chpBuffer + m_iFront);
}

char* CStreamQueue::GetRearBufferPtr(void)
{
	if (m_chpBuffer == NULL)
		return nullptr;

	return (char *)(m_chpBuffer + m_iRear);
}
