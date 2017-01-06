/*---------------------------------------------------------------
네트워크 패킷용 클래스.
간편하게 패킷에 순서대로 데이타를 In, Out 한다.

- 사용법.
CAyaPacket cPacket;
넣기.
clPacket << 40030;	or	clPacket << iValue;	(int 넣기)
clPacket << 3;		or	clPacket << byValue;(BYTE 넣기)
clPacket << 1.4;	or	clPacket << fValue;	(float 넣기)

빼기.
clPacket >> iValue;	(int 빼기)
clPacket >> byValue;(BYTE 빼기)
clPacket >> fValue;	(float 빼기)

!.	삽입되는 데이타 FIFO 순서로 관리된다.
큐가 아니므로, 넣기(<<).빼기(>>) 를 혼합해서 사용하면 안된다.
----------------------------------------------------------------*/

#ifndef  __PROTOCOL_BUFFER_H__
#define  __PROTOCOL_BUFFER_H__

#include <Windows.h>

class CProtocolBuffer
{
public:
	/*---------------------------------------------------------------
	Enum.
	----------------------------------------------------------------*/

	enum enProtocolBuffer
	{
		eBUFFER_DEFAULT = 512		// 패킷의 기본 버퍼 사이즈.
	};

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CProtocolBuffer();
	CProtocolBuffer(int iBufferSize);

	virtual	~CProtocolBuffer();

	//////////////////////////////////////////////////////////////////////////
	// 패킷 초기화.
	//
	// 메모리 할당을 여기서 하므로, 함부로 호출하면 안된다. 
	//
	// Parameters: (int)BufferSize.
	// Return: 없음.
	//////////////////////////////////////////////////////////////////////////
	void	Initial(int iBufferSize = eBUFFER_DEFAULT);


	//////////////////////////////////////////////////////////////////////////
	// 패킷  파괴.
	//
	// Parameters: 없음.
	// Return: 없음.
	//////////////////////////////////////////////////////////////////////////
	void	Release(void);


	//////////////////////////////////////////////////////////////////////////
	// 패킷 청소.
	//
	// Parameters: 없음.
	// Return: 없음.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void);


	//////////////////////////////////////////////////////////////////////////
	// 버퍼 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)패킷 버퍼 사이즈 얻기.
	//////////////////////////////////////////////////////////////////////////
	int		GetBufferSize(void) { return m_iBufferSize; }


	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)사용중인 데이타 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetDataSize(void) { return m_iDataSize; }


	//////////////////////////////////////////////////////////////////////////
	// 버퍼 포인터 얻기.
	//
	// Parameters: 없음.
	// Return: (char *)버퍼 포인터.
	//////////////////////////////////////////////////////////////////////////
	char	*GetBufferPtr(void);


	//////////////////////////////////////////////////////////////////////////
	// 버퍼 Pos 이동. (음수이동은 안됨)
	// GetBufferPtr 함수를 이용하여 외부에서 강제로 버퍼 내용을 수정할 경우 사용. 
	//
	// Parameters: (int) 이동 사이즈.
	// Return: (int) 이동된 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);

	/* ============================================================================= */
	// 연산자 오퍼레이터.
	/* ============================================================================= */

	CProtocolBuffer	&operator = (CProtocolBuffer &clSrcAyaPacket);
	//////////////////////////////////////////////////////////////////////////
	// 넣기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CProtocolBuffer	&operator << (BYTE byValue);
	CProtocolBuffer	&operator << (char chValue);
	CProtocolBuffer	&operator << (short shValue);
	CProtocolBuffer	&operator << (WORD wValue);
	CProtocolBuffer	&operator << (int iValue);
	CProtocolBuffer	&operator << (DWORD dwValue);
	CProtocolBuffer	&operator << (float fValue);
	CProtocolBuffer	&operator << (__int64 iValue);
	CProtocolBuffer	&operator << (double dValue);


	//////////////////////////////////////////////////////////////////////////
	// 빼기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CProtocolBuffer	&operator >> (BYTE &byValue);
	CProtocolBuffer	&operator >> (char &chValue);
	CProtocolBuffer	&operator >> (short &shValue);
	CProtocolBuffer	&operator >> (WORD &wValue);
	CProtocolBuffer	&operator >> (int &iValue);
	CProtocolBuffer	&operator >> (DWORD &dwValue);
	CProtocolBuffer	&operator >> (float &fValue);
	CProtocolBuffer	&operator >> (__int64 &iValue);
	CProtocolBuffer	&operator >> (double &dValue);

	//////////////////////////////////////////////////////////////////////////
	// 데이타 얻기.
	//
	// Parameters: (char *)Dest 포인터. (int)Size.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char *chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// 데이타 삽입.
	//
	// Parameters: (char *)Src 포인터. (int)SrcSize.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char *chpSrc, int iSrcSize);

protected:
	//------------------------------------------------------------
	// 패킷버퍼 / 버퍼 사이즈.
	//------------------------------------------------------------
	char	m_chaBufferDefault[eBUFFER_DEFAULT];	
	char	*m_chpBuffer;		// 필요한지
	int		m_iBufferSize;

	//------------------------------------------------------------
	// 버퍼의 읽을 위치, 넣을 위치.
	//------------------------------------------------------------
	char	*m_chpReadPos;
	char	*m_chpWritePos;

	//------------------------------------------------------------
	// 현재 버퍼에 사용중인 사이즈.
	//------------------------------------------------------------
	int		m_iDataSize;	// m_chpWritePos - m_chpReadPos ?
							// m_chpWritePos ?
};

#endif // !__PROTOCOL_BUFFER_H__
