#include "stdafx.h"
#include "ProtocolBuffer.h"

CProtocolBuffer::CProtocolBuffer()
	: m_iBufferSize(eBUFFER_DEFAULT)
{
	Initial();
}

CProtocolBuffer::CProtocolBuffer(int iBufferSize)
	: m_iBufferSize(iBufferSize)
{
	Initial(m_iBufferSize);
}

CProtocolBuffer::~CProtocolBuffer()
{
	Release();
}


//////////////////////////////////////////////////////////////////////////
// ��Ŷ �ʱ�ȭ.
//
// �޸� �Ҵ��� ���⼭ �ϹǷ�, �Ժη� ȣ���ϸ� �ȵȴ�. 
//
// Parameters: (int)BufferSize.
// Return: ����.
//////////////////////////////////////////////////////////////////////////
void	CProtocolBuffer::Initial(int iBufferSize)
{
	// �׳� �迭 ���
	if (eBUFFER_DEFAULT == iBufferSize)
	{
		m_chpBuffer = nullptr;
		m_chpReadPos = m_chaBufferDefault;
		m_chpReadPos = m_chaBufferDefault;
	}
	// �����Ҵ�
	else
	{
		m_chpBuffer = new char[iBufferSize];

		m_chpReadPos = m_chpBuffer;
		m_chpWritePos = m_chpBuffer;
	}
	
	Clear();
}

//////////////////////////////////////////////////////////////////////////
// ��Ŷ  �ı�.
//
// Parameters: ����.
// Return: ����.
//////////////////////////////////////////////////////////////////////////
void	CProtocolBuffer::Release(void)
{
	if (nullptr != m_chpBuffer)
	{
		delete[] m_chpBuffer;
	}
}

//////////////////////////////////////////////////////////////////////////
// ��Ŷ û��.
//
// Parameters: ����.
// Return: ����.
//////////////////////////////////////////////////////////////////////////
void	CProtocolBuffer::Clear(void)
{
	// ����Ʈ ���� ���
	if (nullptr == m_chpBuffer)
	{
		memset(m_chaBufferDefault, 0, m_iBufferSize);
		
		m_chpReadPos = m_chaBufferDefault;
		m_chpWritePos = m_chaBufferDefault;
	}
	// �����Ҵ� ���
	else
	{
		memset(m_chpBuffer, 0, m_iBufferSize);

		m_chpReadPos = m_chpBuffer;
		m_chpWritePos = m_chpBuffer;
	}
	
	m_iDataSize = 0;	
}


char * CProtocolBuffer::GetBufferPtr(void)
{
	if (nullptr == m_chpBuffer)
	{
		return m_chaBufferDefault;
	}
	else
	{
		return m_chpBuffer;
	}
	
}

//////////////////////////////////////////////////////////////////////////
// ���� Pos �̵�. (�����̵��� �ȵ�)
// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
//
// Parameters: (int) �̵� ������.
// Return: (int) �̵��� ������.
//////////////////////////////////////////////////////////////////////////
int		CProtocolBuffer::MoveWritePos(int iSize)
{
	// ���� �̵��� �� �ǹǷ� iSize <= 0 �̸� ��
	if (0 >= iSize)
	{
		return 0;
	}

	int iWritePos		= 0;
	int iWritePosTemp	= 0;
	int iMoveSize		= 0; // �̵��� �Ÿ�

	
	if (nullptr == m_chpBuffer)
	{
		
		iWritePos = m_chpWritePos - m_chaBufferDefault;
	}
	else
	{
		iWritePos = m_chpWritePos - m_chpBuffer;
	}
	
	iWritePosTemp = iWritePos;

	// ���� ������ٴ� ũ�� �ʰ�
	iWritePosTemp = min(iWritePosTemp + iSize, m_iBufferSize);

	iMoveSize	= iWritePosTemp - iWritePos;
	
	// ������ �Ű��ش�
	m_chpWritePos = m_chpWritePos + iMoveSize;

	// �̵��Ѹ�ŭ datasize ũ��
	//m_iDataSize = m_iDataSize + iMoveSize;
	m_iDataSize = m_chpWritePos - m_chpReadPos;

	// �̵��Ѹ�ŭ ��������
	return iMoveSize;
}

int		CProtocolBuffer::MoveReadPos(int iSize)
{
	// ���� �̵��� �� �ǹǷ� iSize <= 0 �̸� ��
	if (0 >= iSize)
	{
		return 0;
	}

	// ���� ���ۿ� ������� �����ŭ �Ѿ�� �� ��
	iSize = min(iSize, m_iDataSize);

	// ������ �Ű��ش�
	m_chpReadPos = m_chpReadPos + iSize;
	
	// �̵��Ѹ�ŭ ��������
	return iSize;

}

/* ============================================================================= */
// ������ ���۷�����.
/* ============================================================================= */

CProtocolBuffer&	CProtocolBuffer::operator = (CProtocolBuffer &cSrcPacket)
{
	CProtocolBuffer *temp = new CProtocolBuffer(cSrcPacket.m_iBufferSize);

	int iReadPos	= cSrcPacket.m_chpReadPos - cSrcPacket.GetBufferPtr();
	int iWritePos	= cSrcPacket.m_chpWritePos - cSrcPacket.GetBufferPtr();

	memcpy_s(temp->m_chpBuffer, temp->m_iBufferSize,
		cSrcPacket.GetBufferPtr(), cSrcPacket.m_iBufferSize);

	// ���� ������� ��� ���� ������ ����
	temp->m_iBufferSize = cSrcPacket.m_iBufferSize;
	temp->m_iDataSize	= cSrcPacket.m_iDataSize;
	
	// read write pos�� ������ ��
	temp->m_chpReadPos	= m_chpBuffer + iReadPos;
	temp->m_chpWritePos = m_chpBuffer + iWritePos;

	return (*temp);
}
//////////////////////////////////////////////////////////////////////////
// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
//////////////////////////////////////////////////////////////////////////

CProtocolBuffer&	CProtocolBuffer::operator << (BYTE byValue)
{
	PutData( (char*)&byValue, sizeof(BYTE) );

	return *this;
}

CProtocolBuffer&	CProtocolBuffer::operator << (char chValue)
{
	PutData( (char*)&chValue, sizeof(char) );

	return *this;
}

CProtocolBuffer&	CProtocolBuffer::operator << (short shValue)
{
	PutData( (char*)&shValue, sizeof(short) );

	return *this;
}

CProtocolBuffer&	CProtocolBuffer::operator << (WORD wValue)
{
	PutData( (char*)&wValue, sizeof(WORD));

	return *this;
}

CProtocolBuffer&	CProtocolBuffer::operator << (int iValue)
{
	PutData( (char*)&iValue, sizeof(int));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator << (DWORD dwValue)
{
	PutData((char*)&dwValue, sizeof(DWORD));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator << (float fValue)
{
	PutData( (char*)&fValue, sizeof(float) );

	return *this;
}

CProtocolBuffer&	CProtocolBuffer::operator << (__int64 iValue)
{
	PutData((char*)&iValue, sizeof(__int64));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator << (double dValue)
{
	PutData((char*)&dValue, sizeof(double));

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// ����.	�� ���� Ÿ�Ը��� ��� ����.
//////////////////////////////////////////////////////////////////////////

CProtocolBuffer&	CProtocolBuffer::operator >> (BYTE &byValue)
{
	GetData( (char*)&byValue, sizeof(BYTE) );

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator >> (char &chValue)
{
	GetData((char*)&chValue, sizeof(char));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator >> (short &shValue)
{
	GetData((char*)&shValue, sizeof(short));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator >> (WORD &wValue)
{
	GetData((char*)&wValue, sizeof(WORD));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator >> (int &iValue)
{
	GetData((char*)&iValue, sizeof(int));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator >> (DWORD &dwValue)
{
	GetData((char*)&dwValue, sizeof(DWORD));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator >> (float &fValue)
{
	GetData((char*)&fValue, sizeof(float));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator >> (__int64 &iValue)
{
	GetData((char*)&iValue, sizeof(__int64));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator >> (double &dValue)
{
	GetData((char*)&dValue, sizeof(double));

	return *this;
}

//////////////////////////////////////////////////////////////////////////
// ����Ÿ ���.
//
// Parameters: (char *)Dest ������. (int)Size.
// Return: (int)������ ������.
//////////////////////////////////////////////////////////////////////////
int		CProtocolBuffer::GetData(char *chpDest, int iSize)
{
	// iSize�� 0 ���ϸ� ����
	if (0 >= iSize)
	{
		return 0;
	}

	// ���� �������� ũ��� m_iDataSize�� �ִ���
	iSize = min(iSize, m_iDataSize);

	if( nullptr == m_chpBuffer )
	{
		if (0 != memcpy_s(chpDest, iSize, m_chpReadPos, iSize) )
		{
			return 0;
		}
	}
	else
	{
		if (0 != memcpy_s(chpDest, iSize, m_chpReadPos, iSize) )
		{
			return 0;
		}
	}

	// readpos �ű��
	m_chpReadPos = m_chpReadPos + iSize;


	return iSize;
	
}

//////////////////////////////////////////////////////////////////////////
// ����Ÿ ����.
//
// Parameters: (char *)Src ������. (int)SrcSize.
// Return: (int)������ ������.
//////////////////////////////////////////////////////////////////////////
int		CProtocolBuffer::PutData(char *chpSrc, int iSrcSize)
{
	int iWritePos = 0;
	int iExtraSize = 0; // ���ۿ� ���� ������


	// ������ŭ ������ ���� �� �ǹǷ�
	if (0 >= iSrcSize)
	{
		return 0;
	}

	if (nullptr == m_chpBuffer)
	{
		
		iWritePos = m_chpWritePos - m_chaBufferDefault;
	}
	else
	{
		iWritePos = m_chpWritePos - m_chpBuffer;
	}

	iExtraSize = m_iBufferSize - iWritePos;

	// ���� ũ��� ����ũ�⺸�� ũ�� �� ��
	iSrcSize = min(iSrcSize, iExtraSize);

	// ������ ����
	if (nullptr == m_chpBuffer)
	{
		memcpy_s(m_chpWritePos, iExtraSize, chpSrc, iSrcSize);
	}
	else
	{
		memcpy_s(m_chpWritePos, iExtraSize, chpSrc, iSrcSize);
	}

	// m_chpWritePos �̵�
	m_chpWritePos = m_chpWritePos + iSrcSize;
	
	return iSrcSize;
}