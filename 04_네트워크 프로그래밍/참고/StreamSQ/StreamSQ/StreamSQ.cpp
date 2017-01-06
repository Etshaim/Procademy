#include "StreamSQ.h"
#include <memory.h>
#include <stdio.h>

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
	return m_iBufferSize;
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
		freeSize = 0;
	}
	// 버퍼 사이즈 - 사용중인 용량 - 구분을 위한 빈공간
	//return m_iBufferSize - GetUseSize() - eBUFFER_BLANK;

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
	// 빈 상태일때
	if (m_iWritePos == m_iReadPos)
	{
		return m_iBufferSize - eBUFFER_BLANK;
	}
	// read > write
	else if ( m_iReadPos > m_iWritePos)
	{
		int iSize = m_iReadPos - m_iWritePos - eBUFFER_BLANK;

		if (iSize < 0)
		{
			printf_s("GetNotBrokenPutSize()");
		}
		
		return iSize;
	}
	// write > read
	else if (m_iWritePos > m_iReadPos)
	{
		// eBUFFER_BLANK가 최대 인덱스쪽에 위치할 수 있으므로
		if (m_iReadPos < eBUFFER_BLANK)
		{
			int blankSize = eBUFFER_BLANK - m_iReadPos;

			return m_iBufferSize - blankSize - m_iWritePos;
		}

		return m_iBufferSize - m_iWritePos;
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
	// 큐가 비었다면 0 return
	if (m_iReadPos == m_iWritePos)
	{
		ClearBuffer();
	}

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
	int notBrokenPutSize = GetNotBrokenPutSize();

	// 쪼개지지 않을 상황
	if (	notBrokenPutSize >= iSize	)
	{
		if (	0 == memcpy_s(&m_chpBuffer[m_iWritePos], notBrokenPutSize, chpData, iSize	)	)
		{
			m_iWritePos += iSize;

			// 만약 write가 버퍼 사이즈 초과하면 에러
			if (m_iWritePos > m_iBufferSize)
			{
				printf_s("m_iWritePos > m_iBufferSize");
				return 0;
			}
			// 만약 write가 끝에 도달하면
			if (m_iWritePos == m_iBufferSize)
			{
				m_iWritePos = 0;
			}

			return iSize;
		}
	}
	// 쪼개질 상황
	else        // 
	{
		if (0 == memcpy_s(&m_chpBuffer[m_iWritePos], notBrokenPutSize, chpData, notBrokenPutSize))
		{
			// 추가로 put할 사이즈
			int iTempSize = iSize - notBrokenPutSize;
			m_iWritePos = 0;

			if (0 == memcpy_s(&m_chpBuffer[m_iWritePos], GetNotBrokenPutSize(), chpData + notBrokenPutSize, iTempSize))
			{
				m_iWritePos += iTempSize;

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
		ClearBuffer();
		return 0;
	}

	// 만약 iSize가 사용중인 용량보다 크면
	if (iSize > GetUseSize())
	{
		iSize = GetUseSize();
	}

	int notBrokenGetSize = GetNotBrokenGetSize();
	
	// 쪼개지지 않을 상황
	if (notBrokenGetSize >= iSize)
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
		if (0 == memcpy_s(chpDest, notBrokenGetSize, &m_chpBuffer[m_iReadPos], notBrokenGetSize))
		{
			// 더 Get해야하는 사이즈
			int iTempSize = iSize - notBrokenGetSize;
			m_iReadPos = 0;

			if (0 == memcpy_s(chpDest + notBrokenGetSize, iTempSize, &m_chpBuffer[m_iReadPos], iTempSize))
			{
				m_iReadPos += iTempSize;
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
		ClearBuffer();
		return 0;
	}

	// 임시 ReadPos
	int iTempReadPos = m_iReadPos;

	// 만약 iSize가 사용중인 용량보다 크면
	if (iSize > GetUseSize())
	{
		iSize = GetUseSize();
	}

	int notBrokenGetSize = GetNotBrokenGetSize();

	// 쪼개지지 않을 상황
	if (notBrokenGetSize >= iSize)
	{
		if (0 == memcpy_s(chpDest, iSize, &m_chpBuffer[iTempReadPos], notBrokenGetSize))
		{
			iTempReadPos += notBrokenGetSize;

			if (iTempReadPos == m_iBufferSize)
			{
				iTempReadPos = 0;
			}

			return iSize;
		}
	}

	// 쪼개질 상황
	else
	{
		if (0 == memcpy_s(chpDest, notBrokenGetSize, &m_chpBuffer[iTempReadPos], notBrokenGetSize))
		{
			// 더 Get해야하는 사이즈
			int iTempSize = iSize - notBrokenGetSize;
			iTempReadPos = 0;

			if (0 == memcpy_s(chpDest + notBrokenGetSize, iTempSize, &m_chpBuffer[iTempReadPos], iTempSize))
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
	int useSize = GetUseSize();

	// 원하는 길이가 사용 중인 용량보다 크면 비우기
	if (iSize > useSize)
	{
		ClearBuffer();
	}
	else
	{
		int notBrokenGetSize = GetNotBrokenGetSize();

		// 최대 인덱스를 지나가지 않는 상황
		if (notBrokenGetSize >= iSize)
		{
			m_iWritePos += iSize;
		}

		// 최대 인덱스를 지나가는 상황
		else
		{
			m_iWritePos = iSize - m_iWritePos;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// Write 의 위치를 이동하는 함수.
//
// Parameters: 없음.
// Return: (int)Write 이동 사이즈
/////////////////////////////////////////////////////////////////////////
int	CStreamSQ::MoveWritePos(int iSize)
{
	return -1;
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