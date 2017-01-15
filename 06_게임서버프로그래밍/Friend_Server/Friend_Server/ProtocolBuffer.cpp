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
// 패킷 초기화.
//
// 메모리 할당을 여기서 하므로, 함부로 호출하면 안된다. 
//
// Parameters: (int)BufferSize.
// Return: 없음.
//////////////////////////////////////////////////////////////////////////
void	CProtocolBuffer::Initial(int iBufferSize)
{
	// 그냥 배열 사용
	if (eBUFFER_DEFAULT == iBufferSize)
	{
		m_chpBuffer = nullptr;
		m_chpReadPos = m_chaBufferDefault;
		m_chpReadPos = m_chaBufferDefault;
	}
	// 동적할당
	else
	{
		m_chpBuffer = new char[iBufferSize];

		m_chpReadPos = m_chpBuffer;
		m_chpWritePos = m_chpBuffer;
	}

	Clear();
}

//////////////////////////////////////////////////////////////////////////
// 패킷  파괴.
//
// Parameters: 없음.
// Return: 없음.
//////////////////////////////////////////////////////////////////////////
void	CProtocolBuffer::Release(void)
{
	if (nullptr != m_chpBuffer)
	{
		delete[] m_chpBuffer;
	}
}

//////////////////////////////////////////////////////////////////////////
// 패킷 청소.
//
// Parameters: 없음.
// Return: 없음.
//////////////////////////////////////////////////////////////////////////
void	CProtocolBuffer::Clear(void)
{
	// 디폴트 버퍼 사용
	if (nullptr == m_chpBuffer)
	{
		memset(m_chaBufferDefault, 0, m_iBufferSize);

		m_chpReadPos = m_chaBufferDefault;
		m_chpWritePos = m_chaBufferDefault;
	}
	// 동적할당 사용
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
// 버퍼 Pos 이동. (음수이동은 안됨)
// GetBufferPtr 함수를 이용하여 외부에서 강제로 버퍼 내용을 수정할 경우 사용. 
//
// Parameters: (int) 이동 사이즈.
// Return: (int) 이동된 사이즈.
//////////////////////////////////////////////////////////////////////////
int		CProtocolBuffer::MoveWritePos(int iSize)
{
	// 음수 이동은 안 되므로 iSize <= 0 이면 끝
	if (0 >= iSize)
	{
		return 0;
	}

	int iWritePos = 0;
	int iWritePosTemp = 0;
	int iMoveSize = 0; // 이동한 거리


	if (nullptr == m_chpBuffer)
	{

		iWritePos = m_chpWritePos - m_chaBufferDefault;
	}
	else
	{
		iWritePos = m_chpWritePos - m_chpBuffer;
	}

	iWritePosTemp = iWritePos;

	// 버퍼 사이즈보다는 크지 않게
	iWritePosTemp = min(iWritePosTemp + iSize, m_iBufferSize);

	iMoveSize = iWritePosTemp - iWritePos;

	// 실제로 옮겨준다
	m_chpWritePos = m_chpWritePos + iMoveSize;

	// 이동한만큼 datasize 크게
	//m_iDataSize = m_iDataSize + iMoveSize;
	m_iDataSize = m_chpWritePos - m_chpReadPos;

	// 이동한만큼 리턴해줌
	return iMoveSize;
}

int		CProtocolBuffer::MoveReadPos(int iSize)
{
	// 음수 이동은 안 되므로 iSize <= 0 이면 끝
	if (0 >= iSize)
	{
		return 0;
	}

	// 현재 버퍼에 사용중인 사이즈만큼 넘어가면 안 됨
	iSize = min(iSize, m_iDataSize);

	// 실제로 옮겨준다
	m_chpReadPos = m_chpReadPos + iSize;

	// 이동한만큼 리턴해줌
	return iSize;

}

/* ============================================================================= */
// 연산자 오퍼레이터.
/* ============================================================================= */

CProtocolBuffer&	CProtocolBuffer::operator = (CProtocolBuffer &cSrcPacket)
{
	CProtocolBuffer *temp = new CProtocolBuffer(cSrcPacket.m_iBufferSize);

	int iReadPos = cSrcPacket.m_chpReadPos - cSrcPacket.GetBufferPtr();
	int iWritePos = cSrcPacket.m_chpWritePos - cSrcPacket.GetBufferPtr();

	memcpy_s(temp->m_chpBuffer, temp->m_iBufferSize,
		cSrcPacket.GetBufferPtr(), cSrcPacket.m_iBufferSize);

	// 버퍼 사이즈와 사용 중인 사이즈 복사
	temp->m_iBufferSize = cSrcPacket.m_iBufferSize;
	temp->m_iDataSize = cSrcPacket.m_iDataSize;

	// read write pos도 세팅해 줌
	temp->m_chpReadPos = m_chpBuffer + iReadPos;
	temp->m_chpWritePos = m_chpBuffer + iWritePos;

	return (*temp);
}
//////////////////////////////////////////////////////////////////////////
// 넣기.	각 변수 타입마다 모두 만듬.
//////////////////////////////////////////////////////////////////////////

CProtocolBuffer&	CProtocolBuffer::operator << (BYTE byValue)
{
	PutData((char*)&byValue, sizeof(BYTE));

	return *this;
}

CProtocolBuffer&	CProtocolBuffer::operator << (char chValue)
{
	PutData((char*)&chValue, sizeof(char));

	return *this;
}

CProtocolBuffer&	CProtocolBuffer::operator << (short shValue)
{
	PutData((char*)&shValue, sizeof(short));

	return *this;
}

CProtocolBuffer&	CProtocolBuffer::operator << (WORD wValue)
{
	PutData((char*)&wValue, sizeof(WORD));

	return *this;
}

CProtocolBuffer&	CProtocolBuffer::operator << (int iValue)
{
	PutData((char*)&iValue, sizeof(int));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator << (DWORD dwValue)
{
	PutData((char*)&dwValue, sizeof(DWORD));

	return *this;
}
CProtocolBuffer&	CProtocolBuffer::operator << (float fValue)
{
	PutData((char*)&fValue, sizeof(float));

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
// 빼기.	각 변수 타입마다 모두 만듬.
//////////////////////////////////////////////////////////////////////////

CProtocolBuffer&	CProtocolBuffer::operator >> (BYTE &byValue)
{
	GetData((char*)&byValue, sizeof(BYTE));

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
// 데이타 얻기.
//
// Parameters: (char *)Dest 포인터. (int)Size.
// Return: (int)복사한 사이즈.
//////////////////////////////////////////////////////////////////////////
int		CProtocolBuffer::GetData(char *chpDest, int iSize)
{
	// iSize가 0 이하면 나감
	if (0 >= iSize)
	{
		return 0;
	}

	// 얻을 데이터의 크기는 m_iDataSize가 최대임
	iSize = min(iSize, m_iDataSize);

	if (nullptr == m_chpBuffer)
	{
		if (0 != memcpy_s(chpDest, iSize, m_chpReadPos, iSize))
		{
			return 0;
		}
	}
	else
	{
		if (0 != memcpy_s(chpDest, iSize, m_chpReadPos, iSize))
		{
			return 0;
		}
	}

	// readpos 옮기기
	m_chpReadPos = m_chpReadPos + iSize;


	return iSize;

}

//////////////////////////////////////////////////////////////////////////
// 데이타 삽입.
//
// Parameters: (char *)Src 포인터. (int)SrcSize.
// Return: (int)복사한 사이즈.
//////////////////////////////////////////////////////////////////////////
int		CProtocolBuffer::PutData(char *chpSrc, int iSrcSize)
{
	int iWritePos = 0;
	int iExtraSize = 0; // 버퍼에 남은 사이즈


						// 음수만큼 삽입은 말이 안 되므로
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

	// 넣을 크기는 여분크기보다 크면 안 됨
	iSrcSize = min(iSrcSize, iExtraSize);

	// 데이터 삽입
	if (nullptr == m_chpBuffer)
	{
		memcpy_s(m_chpWritePos, iExtraSize, chpSrc, iSrcSize);
	}
	else
	{
		memcpy_s(m_chpWritePos, iExtraSize, chpSrc, iSrcSize);
	}

	// m_chpWritePos 이동
	m_chpWritePos = m_chpWritePos + iSrcSize;

	return iSrcSize;
}