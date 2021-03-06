#pragma once
#include "stdafx.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////
// www.gamecodi.com						이주행 master@gamecodi.com
//
//
/////////////////////////////////////////////////////////////////////
/*---------------------------------------------------------------

Aya Library - Stream SingleQueue

원형 FIFO 스트리밍 큐.

----------------------------------------------------------------*/
class CStreamQueue
{
public:

	/*---------------------------------------------------------------
	AyaStreamSQ Enum.

	----------------------------------------------------------------*/
	enum e_CSTREAMQUEUE
	{
		eBUFFER_DEFAULT = 20960,		// 버퍼의 기본 크기.
		eBUFFER_BLANK = 1			 	// 확실한 구분을 위해 8Byte 의 빈공간.
	};

public:

	/////////////////////////////////////////////////////////////////////////
	// 생성자(임의 크기 지정), 파괴자.
	//
	//
	/////////////////////////////////////////////////////////////////////////
	CStreamQueue(void);
	CStreamQueue(int iBufferSize);

	virtual	~CStreamQueue(void);

	/////////////////////////////////////////////////////////////////////////
	// 버퍼 생성 & 큐의 초기화.
	//
	// Parameters: (int)버퍼용량.
	// Return: 없음.
	/////////////////////////////////////////////////////////////////////////
	void	Initial(int iBufferSize);

	/////////////////////////////////////////////////////////////////////////
	// 버퍼 전체의 용량 얻음.
	//
	// Parameters: 없음.
	// Return: (int)버퍼용량.
	/////////////////////////////////////////////////////////////////////////
	int		GetBufferSize(void);

	/////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 용량 얻기.
	//
	// Parameters: 없음.
	// Return: (int)사용중인 용량.
	/////////////////////////////////////////////////////////////////////////
	int		GetOccupyingSize(void);

	/////////////////////////////////////////////////////////////////////////
	// 현재 버퍼에 남은 용량 얻기.
	//
	// Parameters: 없음.
	// Return: (int)남은용량.
	/////////////////////////////////////////////////////////////////////////
	int		GetRemainingSize(void);

	/////////////////////////////////////////////////////////////////////////
	// 버퍼 포인터로 외부에서 한방에 읽고, 쓸 수 있는 길이.
	// (끊기지 않은 길이)
	//
	// Parameters: 없음.
	// Return: (int)사용가능 용량.
	/////////////////////////////////////////////////////////////////////////
	int		GetNotCirculatedEnqueuingSize(void);
	int		GetNotCirculatedDequeuingSize(void);


	/////////////////////////////////////////////////////////////////////////
	// m_iRear 에 데이타 넣음.
	//
	// Parameters: (char *)데이타 포인터. (int)크기. 
	// Return: (int)넣은 크기.
	/////////////////////////////////////////////////////////////////////////
	int		Enqueue(char *chpData, int iSize);

	/////////////////////////////////////////////////////////////////////////
	// m_iFront 에서 데이타 가져옴. m_iFront 이동.
	//
	// Parameters: (char *)데이타 포인터. (int)크기.
	// Return: (int)가져온 크기.
	/////////////////////////////////////////////////////////////////////////
	int		Dequeue(char *chpDest, int iSize);

	/////////////////////////////////////////////////////////////////////////
	// m_iFront 에서 데이타 읽어옴. m_iFront 고정.
	//
	// Parameters: (char *)데이타 포인터. (int)크기.
	// Return: (int)가져온 크기.
	/////////////////////////////////////////////////////////////////////////
	int		Peek(char *chpDest, int iSize);

	/////////////////////////////////////////////////////////////////////////
	// 원하는 길이만큼 읽기위치 에서 삭제.
	//
	// Parameters: (int)삭제 사이즈
	// Return: (int)결과 삭제 사이즈
	/////////////////////////////////////////////////////////////////////////
	int     RemoveData(int iSize);

	/////////////////////////////////////////////////////////////////////////
	// 원하는 길이만큼 쓰기위치를 이동
	//
	// Parameters: (int)이동 사이즈
	// Return: (int)결과 이동 사이즈
	/////////////////////////////////////////////////////////////////////////
	int		MoveRear(int iSize);

	/////////////////////////////////////////////////////////////////////////
	// 버퍼의 모든 데이타 삭제.
	//
	// Parameters: 없음.
	// Return: 없음.
	/////////////////////////////////////////////////////////////////////////
	void	ClearBuffer(void);

	/////////////////////////////////////////////////////////////////////////
	// 버퍼의 포인터 얻음.
	//
	// Parameters: 없음.
	// Return: (char *) 버퍼 포인터.
	/////////////////////////////////////////////////////////////////////////
	char*	GetBufferPtr(void);

	/////////////////////////////////////////////////////////////////////////
	// 버퍼의 m_iFront 포인터 얻음.
	//
	// Parameters: 없음.
	// Return: (char *) 버퍼 포인터.
	/////////////////////////////////////////////////////////////////////////
	char*	GetFrontBufferPtr(void);

	/////////////////////////////////////////////////////////////////////////
	// 버퍼의 m_iRear 포인터 얻음.
	//
	// Parameters: 없음.
	// Return: (char *) 버퍼 포인터.
	/////////////////////////////////////////////////////////////////////////
	char*	GetRearBufferPtr(void);


protected:

	//------------------------------------------------------------
	// 버퍼 포인터.
	//------------------------------------------------------------
	char				*m_chpBuffer;

	//------------------------------------------------------------
	// 버퍼 사이즈.
	//------------------------------------------------------------
	int					m_iBufferSize;

	//------------------------------------------------------------
	// 버퍼의 읽기 위치, 쓰기 위치.
	//------------------------------------------------------------
	int					m_iFront;
	int					m_iRear;

};
