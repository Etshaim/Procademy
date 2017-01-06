#include "StreamDQ.h"

CStreamDQ::CStreamDQ()
	:m_iBufferSize( eSTREAM_DQ::eBUFFER_DEFAULT )
{
	Initial();
}

CStreamDQ::CStreamDQ(int iBufferSize)
	: m_iBufferSize( iBufferSize )
{
	Initial();
}

CStreamDQ::~CStreamDQ()
{
	delete[] m_stBuffer[0].chpBuffer;
	delete[] m_stBuffer[1].chpBuffer;
}


//////////////////////////////////////////////////////////////////////////
// 초기화.
//
// Parameters: 없음.
// Return: 없음.
//////////////////////////////////////////////////////////////////////////
void	CStreamDQ::Initial(void)
{
	m_iInputBuffer	= 0;
	m_iOutputBuffer = 1;

	for (int iCount = 0; iCount < 2; ++iCount)
	{
		m_stBuffer[iCount].chpBuffer = new char[m_iBufferSize];
		memset(m_stBuffer[iCount].chpBuffer, 0, m_iBufferSize);

		m_stBuffer[iCount].iReadPos = 0;
		m_stBuffer[iCount].iWritePos = 0;

		m_stUseSize[iCount] = 0;
	}
}


/////////////////////////////////////////////////////////////////////////
// 버퍼 전체의 용량 얻음.
//
// Parameters: 없음.
// Return: (int)버퍼용량.
/////////////////////////////////////////////////////////////////////////
int		CStreamDQ::GetBufferSize(void)
{
	return m_iBufferSize - eBUFFER_BLANK;
}

/////////////////////////////////////////////////////////////////////////
// 현재 사용중인 용량 얻기.
//
// Parameters: 없음.
// Return: (int)사용중인 용량.
/////////////////////////////////////////////////////////////////////////
int		CStreamDQ::GetUseSize_OUT(void)
{
	return m_stUseSize[m_iOutputBuffer];
}

int		CStreamDQ::GetUseSize_IN(void)
{
	return m_stUseSize[m_iInputBuffer];
}


/////////////////////////////////////////////////////////////////////////
// 실제로 버퍼의 포인터 위치를 계산하여 정확하게 계산된 사용사이즈.
// 외부에서는 사용하지 않고 내부용으로 사용한다.
// 
// Parameters: 없음.
// Return: (int)사용중 용량.
/////////////////////////////////////////////////////////////////////////
int		CStreamDQ::GetUseSize_Correct_OUT(void)
{
	return m_stBuffer[m_iOutputBuffer].iWritePos
		- m_stBuffer[m_iOutputBuffer].iReadPos;
}

int		CStreamDQ::GetUseSize_Correct_IN(void)
{
	return m_stBuffer[m_iInputBuffer].iWritePos
		- m_stBuffer[m_iInputBuffer].iReadPos;
}

/////////////////////////////////////////////////////////////////////////
// 현재 버퍼에 남은 용량 얻기.
//
// Parameters: 없음.
// Return: (int)남은용량.
/////////////////////////////////////////////////////////////////////////
int		CStreamDQ::GetFreeSize_OUT(void)
{
	return m_iBufferSize - m_stUseSize[m_iOutputBuffer];
}

int		CStreamDQ::GetFreeSize_IN(void)
{
	return m_iBufferSize - m_stUseSize[m_iInputBuffer];
}


/////////////////////////////////////////////////////////////////////////
// InputBuffer WritePos 에 데이타 넣음.
//
// 마지막 인자 bLock 은, Put 내부에서 Lock_IN 을 할 것인지 여부이다.
// 간혹 데이터를 여러번에 걸쳐서 Put 을 해야하는 경우 내부 Lock 이 아닌 외부에서 Lock 을 해주도록 한다.
//
// Parameters: (char *)데이타 포인터. (int)크기. (bool) Lock 여부.
// Return: (int)넣은 크기.
/////////////////////////////////////////////////////////////////////////
int		CStreamDQ::Put(char *chpData, int iSize, bool bLock)
{
	int iFreeSize	= GetFreeSize_IN();
	int iWritePos	= m_stBuffer[m_iInputBuffer].iWritePos;
	char *pBuff		= m_stBuffer[m_iInputBuffer].chpBuffer;

	if (iSize <= 0)
	{
		return 0;
	}

	if (iSize > iFreeSize)
	{
		iSize = iFreeSize;
	}

	
	if ( 0 == memcpy_s( &pBuff[iWritePos], iFreeSize, chpData, iSize ) )
	{
		iWritePos = iWritePos + iSize;

		m_stBuffer[m_iInputBuffer].iWritePos = iWritePos;

		m_stUseSize[m_iInputBuffer] += iSize;

		return iSize;
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////
// Output ReadPos 에서 데이타 가져옴. ReadPos 이동.
//
// Parameters: (BYTE *)데이타 포인터. (int)크기.
// Return: (int)가져온 크기.
/////////////////////////////////////////////////////////////////////////
int		CStreamDQ::Get(char *chpDest, int iSize)
{
	int iUseSize	= GetUseSize_OUT();
	int iReadPos	= m_stBuffer[m_iOutputBuffer].iReadPos;
	char *pBuff		= m_stBuffer[m_iOutputBuffer].chpBuffer;

	if (iSize <= 0)
	{
		return 0;
	}

	if (iSize > iUseSize)
	{
		iSize = iUseSize;
	}

	if ( 0 == memcpy_s( chpDest, iSize, &pBuff[iReadPos], iSize ) )
	{
		iReadPos = iReadPos + iSize;
		m_stBuffer[m_iOutputBuffer].iReadPos = iReadPos;

		m_stUseSize[m_iOutputBuffer] -= iSize;

		return iSize;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////
// Output ReadPos 에서 데이타 읽어옴. ReadPos 고정.
//
// Parameters: (char *)데이타 포인터. (int)크기.
// Return: (int)가져온 크기.
/////////////////////////////////////////////////////////////////////////
int		CStreamDQ::Peek(char *chpDest, int iSize)
{
	int iUseSize	= GetUseSize_OUT();
	int iReadPos	= m_stBuffer[m_iOutputBuffer].iReadPos;
	char *pBuff		= m_stBuffer[m_iOutputBuffer].chpBuffer;

	if (iSize > iUseSize)
	{
		iSize = iUseSize;
	}

	if (0 == memcpy_s(chpDest, iSize, &pBuff[iReadPos], iSize))
	{
		return iSize;
	}

	return 0;
}


/////////////////////////////////////////////////////////////////////////
// 원하는 길이만큼 읽기위치 에서 삭제.
//
// Parameters: 없음.
// Return: 없음.
/////////////////////////////////////////////////////////////////////////
void	CStreamDQ::RemoveData_OUT(int iSize)
{
	int iUseSize = GetUseSize_OUT();

	if (iSize >= iUseSize)
	{
		ClearBuffer_OUT();
	}

	m_stBuffer[m_iOutputBuffer].iReadPos += iSize;
}

void	CStreamDQ::RemoveData_IN(int iSize)
{
	int iUseSize = GetUseSize_IN();

	if (iSize >= iUseSize)
	{
		ClearBuffer_IN();
	}

	m_stBuffer[m_iInputBuffer].iReadPos += iSize;
}

/////////////////////////////////////////////////////////////////////////
// 버퍼의 모든 데이타 삭제.
//
// Parameters: 없음.
// Return: 없음.
/////////////////////////////////////////////////////////////////////////
void	CStreamDQ::ClearBuffer_OUT(void)
{
	m_stBuffer[m_iOutputBuffer].iReadPos	= 0;
	m_stBuffer[m_iOutputBuffer].iWritePos	= 0;

	m_stUseSize[m_iOutputBuffer]			= 0;
}

void	CStreamDQ::ClearBuffer_IN(void)
{
	m_stBuffer[m_iInputBuffer].iReadPos		= 0;
	m_stBuffer[m_iInputBuffer].iWritePos	= 0;

	m_stUseSize[m_iInputBuffer]				= 0;
}


//=============================================================================
// Flip.
// ============================================================================
/////////////////////////////////////////////////////////////////////////
// Output, Input 의 버퍼를 교환한다.
//
// Parameters: 없음.
// Return: (bool) FLIP YES. NO
/////////////////////////////////////////////////////////////////////////
bool	CStreamDQ::FlipBuffer(void)
{
	// OutputBuffer가 비었을 때만 flip해줌
	if (0 == m_stUseSize[m_iOutputBuffer])
	{
		// 일단 OutPutBuffer를 clear해준다
		ClearBuffer_OUT();
		
		int iTempIndex	= m_iInputBuffer;
		m_iInputBuffer	= m_iOutputBuffer;
		m_iOutputBuffer = iTempIndex;

		return true;
	}

	return false;

	
}