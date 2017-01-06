#include "stdafx.h"
#include "StreamSQ.h"


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
// ���� ���� & ť�� �ʱ�ȭ.
//
// Parameters: (int)���ۿ뷮.
// Return: ����.
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
// ���� ��ü�� �뷮 ����.
//
// Parameters: ����.
// Return: (int)���ۿ뷮.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::GetBufferSize(void)
{
	return m_iBufferSize - eBUFFER_BLANK;
}
/////////////////////////////////////////////////////////////////////////
// ���� ������� �뷮 ���.
//
// Parameters: ����.
// Return: (int)������� �뷮.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::GetUseSize(void)
{
	// ť�� �� ������ ��
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
// ���� ���ۿ� ���� �뷮 ���.
//
// Parameters: ����.
// Return: (int)�����뷮.
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
// ���� �����ͷ� �ܺο��� �ѹ濡 �а�, �� �� �ִ� ����.
// (������ ���� ����)
//
// Parameters: ����.
// Return: (int)��밡�� �뷮.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::GetNotBrokenGetSize(void)
{
	// �� ������ ��
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
		// eBUFFER_BLANK�� �ִ� �ε����ʿ� ��ġ�� �� �����Ƿ�
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
// WritePos �� ����Ÿ ����.
//
// Parameters: (char *)����Ÿ ������. (int)ũ��. 
// Return: (int)���� ũ��.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::Put(char *chpData, int iSize)
{
	// m_iWritePos�� �ӽ� ���� ���� ����ϰ�,
	// ������ m_iWritePos�� ���� �����ϴ°� �ƴϰ�
	// �ѹ��� �ٲ����
	// ������ �κ� ���� �׷���
	// ����, m_iWritePos�� m_iReadPos�� �׻� �� �������� �ٲ������

	// �� ���� �н�
	if (GetFreeSize() == 0)
	{
		return 0;
	}

	// iSize�� ���� �뷮���� ũ�� iSizeũ�� �ٲٱ�
	if (iSize > GetFreeSize())
	{
		iSize = GetFreeSize();
	}

	// �ѹ��� ���� �� �ִ� ������
	int iNotBrokenPutSize = GetNotBrokenPutSize();

	// �ɰ����� ���� ��Ȳ
	if (iNotBrokenPutSize >= iSize)
	{
		if (0 == memcpy_s(&m_chpBuffer[m_iWritePos], iNotBrokenPutSize, chpData, iSize))
		{
			m_iWritePos += iSize;

			if (m_iWritePos == m_iBufferSize)
			{
				m_iWritePos = 0;
			}
			// ���߿� ���� �κ�
			// ���� write�� ���� ������ �ʰ��ϸ� ����
			if (m_iWritePos > m_iBufferSize)
			{
				printf_s("m_iWritePos > m_iBufferSize");
				return 0;
			}

			return iSize;
		}
	}
	// �ɰ��� ��Ȳ
	else        // 
	{
		if (0 == memcpy_s(&m_chpBuffer[m_iWritePos], iNotBrokenPutSize, chpData, iNotBrokenPutSize))
		{
			// �߰��� put�� ������
			int iTempSize = iSize - iNotBrokenPutSize;

			int iTempNotBrokenPutSize = GetNotBrokenPutSize();

			if (0 == memcpy_s(	&m_chpBuffer[0], iTempSize, chpData + iNotBrokenPutSize, iTempSize)	)
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
// ReadPos ���� ����Ÿ ������. ReadPos �̵�.
//
// Parameters: (char *)����Ÿ ������. (int)ũ��.
// Return: (int)������ ũ��.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::Get(char *chpDest, int iSize)
{

	// ť�� ����ٸ� 0 return
	if (m_iReadPos == m_iWritePos)
	{
		return 0;
	}

	// ���� iSize�� ������� �뷮���� ũ��
	if (iSize > GetUseSize())
	{
		iSize = GetUseSize();
	}

	int iNotBrokenGetSize = GetNotBrokenGetSize();

	// �ɰ����� ���� ��Ȳ
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

	// �ɰ��� ��Ȳ
	else
	{
		int iTempReadPos = m_iReadPos;

		if (0 == memcpy_s(chpDest, iNotBrokenGetSize, &m_chpBuffer[iTempReadPos], iNotBrokenGetSize))
		{
			// �� Get�ؾ��ϴ� ������
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
// ReadPos ���� ����Ÿ �о��. ReadPos ����.
//
// Parameters: (char *)����Ÿ ������. (int)ũ��.
// Return: (int)������ ũ��.
/////////////////////////////////////////////////////////////////////////
int		CStreamSQ::Peek(char *chpDest, int iSize)
{
	// ť�� ����ٸ� 0 return
	if (m_iReadPos == m_iWritePos)
	{
		return 0;
	}

	// �ӽ� ReadPos
	int iTempReadPos = m_iReadPos;

	// ���� iSize�� ������� �뷮���� ũ��
	if (iSize > GetUseSize())
	{
		iSize = GetUseSize();
	}

	int iNotBrokenGetSize = GetNotBrokenGetSize();

	// �ɰ����� ���� ��Ȳ
	if (iNotBrokenGetSize >= iSize)
	{
		if (0 == memcpy_s(chpDest, iSize, &m_chpBuffer[iTempReadPos], iSize))
		{
			return iSize;
		}
	}

	// �ɰ��� ��Ȳ
	else
	{
		if (0 == memcpy_s(chpDest, iNotBrokenGetSize, &m_chpBuffer[iTempReadPos], iNotBrokenGetSize))
		{
			// �� Get�ؾ��ϴ� ������
			int iTempSize	= iSize - iNotBrokenGetSize;

			iTempReadPos	= 0;

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
// ���ϴ� ���̸�ŭ �б���ġ ���� ����.
//
// Parameters: ����.
// Return: ����.
/////////////////////////////////////////////////////////////////////////
void	CStreamSQ::RemoveData(int iSize)
{
	// size�� ��� ���� �뷮�� ���ų� ũ�� ����
	if ( iSize > GetUseSize() )
	{
		printf_s("RemoveData()");
		return;
	}
	// size�� ��� ���� �뷮���� ���� ��
	else
	{
		int iNotBrokenGetSize = GetNotBrokenGetSize();

		// �ִ� �ε����� �������� �ʴ� ��Ȳ
		if (iNotBrokenGetSize >= iSize)
		{
			m_iReadPos += iSize;

			if (m_iReadPos == m_iBufferSize)
			{
				m_iReadPos = 0;
			}
		}
		// �ִ� �ε����� �������� ��Ȳ
		else
		{
			m_iReadPos = iSize - iNotBrokenGetSize;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
// Write �� ��ġ�� �̵��ϴ� �Լ�.
// �ܺο��� �����͸� �ְ� ������ Write�� ��ġ�� �Űܾ��ϴ� ��� ���
//
// Parameters: ����.
// Return: (int)Write �̵� ������
/////////////////////////////////////////////////////////////////////////
int	CStreamSQ::MoveWritePos(int iSize)
{
	// ���� ����
	int iFreeSize = GetFreeSize();

	if (iSize > iFreeSize)
	{
		printf_s("MoveWritePos()");
		return -1;
		//iSize = iFreeSize;
	}
	
	// �ѹ��� �� �ɰ����� ���� �� �ִ� ����
	int iNotBrokenPutSize = GetNotBrokenPutSize();

	// �ִ� �ε����� �������� �ʴ� ��Ȳ
	if ( iNotBrokenPutSize >= iSize )
	{
		m_iWritePos += iSize;

		if (m_iWritePos == m_iBufferSize)
		{
			m_iWritePos = 0;
		}
	}

	// �ִ� �ε����� �������� ��Ȳ
	else
	{
		m_iWritePos = iSize - iNotBrokenPutSize;
	}

	return iSize;
}


/////////////////////////////////////////////////////////////////////////
// ������ ��� ����Ÿ ����.
//
// Parameters: ����.
// Return: ����.
/////////////////////////////////////////////////////////////////////////
void	CStreamSQ::ClearBuffer(void)
{
	m_iReadPos = 0;
	m_iWritePos = 0;
}

/////////////////////////////////////////////////////////////////////////
// ������ ������ ����.
//
// Parameters: ����.
// Return: (char *) ���� ������.
/////////////////////////////////////////////////////////////////////////
char*	CStreamSQ::GetBufferPtr(void)
{
	return m_chpBuffer;
}

/////////////////////////////////////////////////////////////////////////
// ������ ReadPos ������ ����.
//
// Parameters: ����.
// Return: (char *) ���� ������.
/////////////////////////////////////////////////////////////////////////
char*	CStreamSQ::GetReadBufferPtr(void)
{
	return &m_chpBuffer[m_iReadPos];
}
/////////////////////////////////////////////////////////////////////////
// ������ WritePos ������ ����.
//
// Parameters: ����.
// Return: (char *) ���� ������.
/////////////////////////////////////////////////////////////////////////
char*	CStreamSQ::GetWriteBufferPtr(void)
{
	return &m_chpBuffer[m_iWritePos];
}