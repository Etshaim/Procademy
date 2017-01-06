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
// �ʱ�ȭ.
//
// Parameters: ����.
// Return: ����.
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
// ���� ��ü�� �뷮 ����.
//
// Parameters: ����.
// Return: (int)���ۿ뷮.
/////////////////////////////////////////////////////////////////////////
int		CStreamDQ::GetBufferSize(void)
{
	return m_iBufferSize - eBUFFER_BLANK;
}

/////////////////////////////////////////////////////////////////////////
// ���� ������� �뷮 ���.
//
// Parameters: ����.
// Return: (int)������� �뷮.
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
// ������ ������ ������ ��ġ�� ����Ͽ� ��Ȯ�ϰ� ���� ��������.
// �ܺο����� ������� �ʰ� ���ο����� ����Ѵ�.
// 
// Parameters: ����.
// Return: (int)����� �뷮.
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
// ���� ���ۿ� ���� �뷮 ���.
//
// Parameters: ����.
// Return: (int)�����뷮.
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
// InputBuffer WritePos �� ����Ÿ ����.
//
// ������ ���� bLock ��, Put ���ο��� Lock_IN �� �� ������ �����̴�.
// ��Ȥ �����͸� �������� ���ļ� Put �� �ؾ��ϴ� ��� ���� Lock �� �ƴ� �ܺο��� Lock �� ���ֵ��� �Ѵ�.
//
// Parameters: (char *)����Ÿ ������. (int)ũ��. (bool) Lock ����.
// Return: (int)���� ũ��.
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
// Output ReadPos ���� ����Ÿ ������. ReadPos �̵�.
//
// Parameters: (BYTE *)����Ÿ ������. (int)ũ��.
// Return: (int)������ ũ��.
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
// Output ReadPos ���� ����Ÿ �о��. ReadPos ����.
//
// Parameters: (char *)����Ÿ ������. (int)ũ��.
// Return: (int)������ ũ��.
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
// ���ϴ� ���̸�ŭ �б���ġ ���� ����.
//
// Parameters: ����.
// Return: ����.
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
// ������ ��� ����Ÿ ����.
//
// Parameters: ����.
// Return: ����.
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
// Output, Input �� ���۸� ��ȯ�Ѵ�.
//
// Parameters: ����.
// Return: (bool) FLIP YES. NO
/////////////////////////////////////////////////////////////////////////
bool	CStreamDQ::FlipBuffer(void)
{
	// OutputBuffer�� ����� ���� flip����
	if (0 == m_stUseSize[m_iOutputBuffer])
	{
		// �ϴ� OutPutBuffer�� clear���ش�
		ClearBuffer_OUT();
		
		int iTempIndex	= m_iInputBuffer;
		m_iInputBuffer	= m_iOutputBuffer;
		m_iOutputBuffer = iTempIndex;

		return true;
	}

	return false;

	
}