#include "StreamSQ.h"
#include <stdio.h>
#include <memory.h>

CStreamSQ::CStreamSQ(void)
	: m_iReadPos(0), m_iWritePos(0), m_iBufferSize(eBUFFER_DEFAULT)
{
	Initial(m_iBufferSize);
}

CStreamSQ::CStreamSQ(int iBufferSize)
	: m_iReadPos(0), m_iWritePos(0), m_iBufferSize(iBufferSize)
{
	Initial(m_iBufferSize);
}

CStreamSQ::~CStreamSQ(void)
{
	delete[] m_chpBuffer;
}

/////////////////////////////////////////////////////////////////////////
// 버퍼 생성 & 큐의 초기화.
//
// Parameters: (int)버퍼용량.
// Return: 없음.
/////////////////////////////////////////////////////////////////////////
void	CStreamSQ::Initial(int iBufferSize)
{
	if (m_iBufferSize < eBUFFER_BLANK)
	{
		m_iBufferSize = eBUFFER_BLANK;
	}
	m_chpBuffer = new char[iBufferSize];
}

/////////////////////////////////////////////////////////////////////////
// 버퍼 전체의 용량 얻음.
//
// Parameters: 없음.
// Return: (int)버퍼용량.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::GetBufferSize(void)
{
	return m_iBufferSize - eBUFFER_BLANK;
}
/////////////////////////////////////////////////////////////////////////
// 현재 사용중인 용량 얻기.
//
// Parameters: 없음.
// Return: (int)사용중인 용량.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::GetUseSize(void)
{
	// 큐가 빈 상태일 때
	if (m_iReadPos == m_iWritePos)
	{
		return 0;
	}
	// write > read
	else if (m_iWritePos > m_iReadPos)
	{
		return (m_iWritePos - m_iReadPos);
	}

	else if (m_iReadPos > m_iWritePos)
	{
		return m_iWritePos + (m_iBufferSize - m_iReadPos);
	}

	else
	{
		printf_s("GetUseSize()\n");
		return -1;
	}

}

/////////////////////////////////////////////////////////////////////////
// 현재 버퍼에 남은 용량 얻기.
//
// Parameters: 없음.
// Return: (int)남은용량.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::GetFreeSize(void)
{
	int freeSize = m_iBufferSize - GetUseSize() - eBUFFER_BLANK;

	if (freeSize < 0)
	{
		printf_s("GetFreeSize()\n");
		freeSize = 0;
	}

	return freeSize;
}

/////////////////////////////////////////////////////////////////////////
// 버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이.
// (끊기지 않은 길이)
//
// Parameters: 없음.
// Return: (int)사용가능 용량.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::GetNotBrokenGetSize(void)
{
	// 빈 상태일 때
	if (m_iReadPos == m_iWritePos)
	{
		return 0;
	}

	// write > read
	else if (m_iWritePos > m_iReadPos)
	{
		return m_iWritePos - m_iReadPos;
	}

	// read > write
	else if (m_iReadPos > m_iWritePos)
	{
		return m_iBufferSize - m_iReadPos;
	}

	printf_s("GetNotBrokenGetSize()\n");

	return -1;
}

int		CStreamSQ::GetNotBrokenPutSize(void)
{
	// read > write
	if (m_iReadPos > m_iWritePos)
	{
		int iSize = m_iReadPos - m_iWritePos - eBUFFER_BLANK;

		if (iSize < 0)
		{
			printf_s("GetNotBrokenPutSize()");
		}

		return iSize;
	}

	// write >= read
	else if (m_iWritePos >= m_iReadPos)
	{
		// eBUFFER_BLANK가 최대 인덱스쪽에 위치할 수 있으므로
		if (m_iReadPos < eBUFFER_BLANK)
		{
			int blankSize = eBUFFER_BLANK - m_iReadPos;

			return m_iBufferSize - blankSize - m_iWritePos;
		}
		else
		{
			return m_iBufferSize - m_iWritePos;
		}
	}

	printf_s("GetNotBrokenPutSize()\n");
	return -1;
}


/////////////////////////////////////////////////////////////////////////
// WritePos 에 데이타 넣음.
//
// Parameters: (char *)데이타 포인터. (int)크기. 
// Return: (int)넣은 크기.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::Put(char *chpData, int iSize)
{
	// m_iWritePos를 임시 변수 만들어서 사용하고,
	// 나눠서 m_iWritePos를 각각 변경하는게 아니고
	// 한번에 바꿔야함
	// 스레드 부분 가면 그러함
	// 또한, m_iWritePos나 m_iReadPos는 항상 맨 마지막에 바꿔줘야함

	// 꽉 차면 패스
	if (GetFreeSize() == 0)
	{
		return 0;
	}

	// iSize가 남은 용량보다 크면 iSize크기 바꾸기
	if (iSize > GetFreeSize())
	{
		iSize = GetFreeSize();
	}

	// 한번에 넣을 수 있는 사이즈
	int iNotBrokenPutSize = GetNotBrokenPutSize();

	// 쪼개지지 않을 상황
	if (iNotBrokenPutSize >= iSize)
	{
		if (0 == memcpy_s(&m_chpBuffer[m_iWritePos], iNotBrokenPutSize, chpData, iSize))
		{
			m_iWritePos += iSize;

			if (m_iWritePos == m_iBufferSize)
			{
				m_iWritePos = 0;
			}
			// 나중에 지울 부분
			// 만약 write가 버퍼 사이즈 초과하면 에러
			if (m_iWritePos > m_iBufferSize)
			{
				printf_s("m_iWritePos > m_iBufferSize");
				return 0;
			}

			return iSize;
		}
	}
	// 쪼개질 상황
	else        // 
	{
		if (0 == memcpy_s(&m_chpBuffer[m_iWritePos], iNotBrokenPutSize, chpData, iNotBrokenPutSize))
		{
			// 추가로 put할 사이즈
			int iTempSize = iSize - iNotBrokenPutSize;

			int iTempNotBrokenPutSize = GetNotBrokenPutSize();

			if (0 == memcpy_s(&m_chpBuffer[0], iTempSize, chpData + iNotBrokenPutSize, iTempSize))
			{
				m_iWritePos = iTempSize;

				return iSize;
			}
		}
	}

	printf_s("Put()\n");
	return -1;
}

/////////////////////////////////////////////////////////////////////////
// ReadPos 에서 데이타 가져옴. ReadPos 이동.
//
// Parameters: (char *)데이타 포인터. (int)크기.
// Return: (int)가져온 크기.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::Get(char *chpDest, int iSize)
{

	// 큐가 비었다면 0 return
	if (m_iReadPos == m_iWritePos)
	{
		return 0;
	}

	// 만약 iSize가 사용중인 용량보다 크면
	if (iSize > GetUseSize())
	{
		iSize = GetUseSize();
	}

	int iNotBrokenGetSize = GetNotBrokenGetSize();

	// 쪼개지지 않을 상황
	if (iNotBrokenGetSize >= iSize)
	{
		if (0 == memcpy_s(chpDest, iSize, &m_chpBuffer[m_iReadPos], iSize))
		{
			m_iReadPos += iSize;

			if (m_iReadPos == m_iBufferSize)
			{
				m_iReadPos = 0;
			}

			return iSize;
		}
	}

	// 쪼개질 상황
	else
	{
		int iTempReadPos = m_iReadPos;

		if (0 == memcpy_s(chpDest, iNotBrokenGetSize, &m_chpBuffer[iTempReadPos], iNotBrokenGetSize))
		{
			// 더 Get해야하는 사이즈
			int iTempSize = iSize - iNotBrokenGetSize;

			iTempReadPos = 0;

			if (0 == memcpy_s(chpDest + iNotBrokenGetSize, iTempSize, &m_chpBuffer[iTempReadPos], iTempSize))
			{
				m_iReadPos = iTempSize;

				return iSize;
			}
		}
	}

	printf_s("Get()\n");
	return -1;
}

/////////////////////////////////////////////////////////////////////////
// ReadPos 에서 데이타 읽어옴. ReadPos 고정.
//
// Parameters: (char *)데이타 포인터. (int)크기.
// Return: (int)가져온 크기.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::Peek(char *chpDest, int iSize)
{
	// 큐가 비었다면 0 return
	if (m_iReadPos == m_iWritePos)
	{
		return 0;
	}

	// 임시 ReadPos
	int iTempReadPos = m_iReadPos;

	// 만약 iSize가 사용중인 용량보다 크면
	if (iSize > GetUseSize())
	{
		iSize = GetUseSize();
	}

	int iNotBrokenGetSize = GetNotBrokenGetSize();

	// 쪼개지지 않을 상황
	if (iNotBrokenGetSize >= iSize)
	{
		if (0 == memcpy_s(chpDest, iSize, &m_chpBuffer[iTempReadPos], iSize))
		{
			return iSize;
		}
	}

	// 쪼개질 상황
	else
	{
		if (0 == memcpy_s(chpDest, iNotBrokenGetSize, &m_chpBuffer[iTempReadPos], iNotBrokenGetSize))
		{
			// 더 Get해야하는 사이즈
			int iTempSize = iSize - iNotBrokenGetSize;

			iTempReadPos = 0;

			if (0 == memcpy_s(chpDest + iNotBrokenGetSize, iTempSize, &m_chpBuffer[iTempReadPos], iTempSize))
			{
				return iSize;
			}
		}
	}
	printf_s("Peek()\n");
	return -1;
}



/////////////////////////////////////////////////////////////////////////
// 원하는 길이만큼 읽기위치 에서 삭제.
//
// Parameters: 없음.
// Return: 없음.
/////////////////////////////////////////////////////////////////////////
void	CStreamSQ::RemoveData(int iSize)
{
	// size가 사용 중인 용량과 같거나 크면 비우기
	if (iSize > GetUseSize())
	{
		printf_s("RemoveData()");
		return;
	}
	// size가 사용 중인 용량보다 작을 때
	else
	{
		int iNotBrokenGetSize = GetNotBrokenGetSize();

		// 최대 인덱스를 지나가지 않는 상황
		if (iNotBrokenGetSize >= iSize)
		{
			m_iReadPos += iSize;

			if (m_iReadPos == m_iBufferSize)
			{
				m_iReadPos = 0;
			}
		}
		// 최대 인덱스를 지나가는 상황
		else
		{
			m_iReadPos = iSize - iNotBrokenGetSize;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// Write 의 위치를 이동하는 함수.
// 외부에서 데이터를 넣고 강제로 Write의 위치를 옮겨야하는 경우 사용
//
// Parameters: 없음.
// Return: (int)Write 이동 사이즈
/////////////////////////////////////////////////////////////////////////
int	CStreamSQ::MoveWritePos(int iSize)
{
	// 남은 공간
	int iFreeSize = GetFreeSize();

	if (iSize > iFreeSize)
	{
		printf_s("MoveWritePos()");
		return -1;
		//iSize = iFreeSize;
	}

	// 한번에 안 쪼개지고 넣을 수 있는 공간
	int iNotBrokenPutSize = GetNotBrokenPutSize();

	// 최대 인덱스를 지나가지 않는 상황
	if (iNotBrokenPutSize >= iSize)
	{
		m_iWritePos += iSize;

		if (m_iWritePos == m_iBufferSize)
		{
			m_iWritePos = 0;
		}
	}

	// 최대 인덱스를 지나가는 상황
	else
	{
		m_iWritePos = iSize - iNotBrokenPutSize;
	}

	return iSize;
}


/////////////////////////////////////////////////////////////////////////
// 버퍼의 모든 데이타 삭제.
//
// Parameters: 없음.
// Return: 없음.
/////////////////////////////////////////////////////////////////////////
void	CStreamSQ::ClearBuffer(void)
{
	m_iReadPos = 0;
	m_iWritePos = 0;
}

/////////////////////////////////////////////////////////////////////////
// 버퍼의 포인터 얻음.
//
// Parameters: 없음.
// Return: (char *) 버퍼 포인터.
/////////////////////////////////////////////////////////////////////////
char*	CStreamSQ::GetBufferPtr(void)
{
	return m_chpBuffer;
}

/////////////////////////////////////////////////////////////////////////
// 버퍼의 ReadPos 포인터 얻음.
//
// Parameters: 없음.
// Return: (char *) 버퍼 포인터.
/////////////////////////////////////////////////////////////////////////
char*	CStreamSQ::GetReadBufferPtr(void)
{
	return &m_chpBuffer[m_iReadPos];
}
/////////////////////////////////////////////////////////////////////////
// 버퍼의 WritePos 포인터 얻음.
//
// Parameters: 없음.
// Return: (char *) 버퍼 포인터.
/////////////////////////////////////////////////////////////////////////
char*	CStreamSQ::GetWriteBufferPtr(void)
{
	return &m_chpBuffer[m_iWritePos];
}