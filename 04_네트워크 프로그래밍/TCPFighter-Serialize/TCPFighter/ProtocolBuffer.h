/*---------------------------------------------------------------
��Ʈ��ũ ��Ŷ�� Ŭ����.
�����ϰ� ��Ŷ�� ������� ����Ÿ�� In, Out �Ѵ�.

- ����.
CAyaPacket cPacket;
�ֱ�.
clPacket << 40030;	or	clPacket << iValue;	(int �ֱ�)
clPacket << 3;		or	clPacket << byValue;(BYTE �ֱ�)
clPacket << 1.4;	or	clPacket << fValue;	(float �ֱ�)

����.
clPacket >> iValue;	(int ����)
clPacket >> byValue;(BYTE ����)
clPacket >> fValue;	(float ����)

!.	���ԵǴ� ����Ÿ FIFO ������ �����ȴ�.
ť�� �ƴϹǷ�, �ֱ�(<<).����(>>) �� ȥ���ؼ� ����ϸ� �ȵȴ�.
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
		eBUFFER_DEFAULT = 512		// ��Ŷ�� �⺻ ���� ������.
	};

	//////////////////////////////////////////////////////////////////////////
	// ������, �ı���.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CProtocolBuffer();
	CProtocolBuffer(int iBufferSize);

	virtual	~CProtocolBuffer();

	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ �ʱ�ȭ.
	//
	// �޸� �Ҵ��� ���⼭ �ϹǷ�, �Ժη� ȣ���ϸ� �ȵȴ�. 
	//
	// Parameters: (int)BufferSize.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	Initial(int iBufferSize = eBUFFER_DEFAULT);


	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ  �ı�.
	//
	// Parameters: ����.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	Release(void);


	//////////////////////////////////////////////////////////////////////////
	// ��Ŷ û��.
	//
	// Parameters: ����.
	// Return: ����.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void);


	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)��Ŷ ���� ������ ���.
	//////////////////////////////////////////////////////////////////////////
	int		GetBufferSize(void) { return m_iBufferSize; }


	//////////////////////////////////////////////////////////////////////////
	// ���� ������� ������ ���.
	//
	// Parameters: ����.
	// Return: (int)������� ����Ÿ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetDataSize(void) { return m_iDataSize; }


	//////////////////////////////////////////////////////////////////////////
	// ���� ������ ���.
	//
	// Parameters: ����.
	// Return: (char *)���� ������.
	//////////////////////////////////////////////////////////////////////////
	char	*GetBufferPtr(void);


	//////////////////////////////////////////////////////////////////////////
	// ���� Pos �̵�. (�����̵��� �ȵ�)
	// GetBufferPtr �Լ��� �̿��Ͽ� �ܺο��� ������ ���� ������ ������ ��� ���. 
	//
	// Parameters: (int) �̵� ������.
	// Return: (int) �̵��� ������.
	//////////////////////////////////////////////////////////////////////////
	int		MoveWritePos(int iSize);
	int		MoveReadPos(int iSize);

	/* ============================================================================= */
	// ������ ���۷�����.
	/* ============================================================================= */

	CProtocolBuffer	&operator = (CProtocolBuffer &clSrcAyaPacket);
	//////////////////////////////////////////////////////////////////////////
	// �ֱ�.	�� ���� Ÿ�Ը��� ��� ����.
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
	// ����.	�� ���� Ÿ�Ը��� ��� ����.
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
	// ����Ÿ ���.
	//
	// Parameters: (char *)Dest ������. (int)Size.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char *chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// ����Ÿ ����.
	//
	// Parameters: (char *)Src ������. (int)SrcSize.
	// Return: (int)������ ������.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char *chpSrc, int iSrcSize);

protected:
	//------------------------------------------------------------
	// ��Ŷ���� / ���� ������.
	//------------------------------------------------------------
	char	m_chaBufferDefault[eBUFFER_DEFAULT];	
	char	*m_chpBuffer;		// �ʿ�����
	int		m_iBufferSize;

	//------------------------------------------------------------
	// ������ ���� ��ġ, ���� ��ġ.
	//------------------------------------------------------------
	char	*m_chpReadPos;
	char	*m_chpWritePos;

	//------------------------------------------------------------
	// ���� ���ۿ� ������� ������.
	//------------------------------------------------------------
	int		m_iDataSize;	// m_chpWritePos - m_chpReadPos ?
							// m_chpWritePos ?
};

#endif // !__PROTOCOL_BUFFER_H__
