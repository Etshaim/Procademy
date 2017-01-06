/*---------------------------------------------------------------
MemoryPool.

메모리 풀 클래스.
특정 데이타(구조체,클래스,변수)를 일정량 할당 후 나눠쓴다.

- 사용법.

procademy::CMemoryPool<DATA> MemPool(300, FALSE);
DATA *pData = MemPool.Alloc();

pData 사용

MemPool.Free(pData);
----------------------------------------------------------------*/
#ifndef  __MEMORY_POOL__
#define  __MEMORY_POOL__

#include <assert.h>
#include <new.h>
#include <malloc.h>

// LinkedList 전방선언
template<typename T> class CDoubleLinkedList;

template <typename DATA>
class CMemoryPool
{
private:
	/* **************************************************************** */
	// 각 블럭 앞에 사용될 노드 구조체.
	/* **************************************************************** */
	struct st_BLOCK_NODE
	{
		st_BLOCK_NODE()
		{
			stpNextBlock = NULL;
		}

		st_BLOCK_NODE	*stpNextBlock;
		DATA			data;
	};

public:
	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Parameters:	(int) 최대 블럭 개수.
	//				(bool) 생성자 호출 여부.
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CMemoryPool(int iBlockNum = 0, bool bPlacementNew = false);
	virtual	~CMemoryPool();


	//////////////////////////////////////////////////////////////////////////
	// 블럭 하나를 할당받는다.
	//
	// Parameters: 없음.
	// Return: (DATA *) 데이타 블럭 포인터.
	//////////////////////////////////////////////////////////////////////////
	DATA	*Alloc(void);
	DATA	*AdditionalAlloc(void);


	//////////////////////////////////////////////////////////////////////////
	// 사용중이던 블럭을 해제한다.
	//
	// Parameters: (DATA *) 블럭 포인터.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(DATA *pData);


	//////////////////////////////////////////////////////////////////////////
	// 현재 확보 된 블럭 개수를  얻는다. (메모리풀 내부의 전체 개수)
	//
	// Parameters: 없음.
	// Return: (int) 메모리 풀 내부 전체 개수
	//////////////////////////////////////////////////////////////////////////
	int		GetAllocCount(void) { return m_iAllocCount; }

	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//
	// Parameters: 없음.
	// Return: (int) 사용중인 블럭 개수.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return m_iUseCount; }

private:
	st_BLOCK_NODE	*m_pFreeNode; 
	st_BLOCK_NODE	*m_pMemoryBlock;
	int				m_iAllocCount;
	int				m_iUseCount;
	bool			m_bPlacementNew;
};

template<typename DATA>
CMemoryPool<DATA>::CMemoryPool(int iBlockNum, bool bPlacementNew)
	: m_pFreeNode(nullptr),
	  m_iAllocCount(iBlockNum),
	  m_iUseCount(0),
	  m_bPlacementNew(bPlacementNew)
{
	// iBlockNum이 0이면 Alloc 요청이 올 때마다 할당해서 줄 것임
	if (0 >= iBlockNum)
	{
		m_iAllocCount = 0;
		m_bPlacementNew = false;
		return;
	}

	// placementNew 사용
	if (true == bPlacementNew)
	{
		m_pMemoryBlock = (st_BLOCK_NODE*)malloc(sizeof(st_BLOCK_NODE) * iBlockNum);
	}

	// placementNew 사용 안 함
	else
	{
		m_pMemoryBlock = new st_BLOCK_NODE[iBlockNum];
	}

	st_BLOCK_NODE *pTemp = (st_BLOCK_NODE *)m_pMemoryBlock;

	// stpNextBlock가 다음 노드를 가리키게 세팅
	for (int iCount = 1; iCount < iBlockNum; iCount++)
	{
		pTemp[iCount - 1].stpNextBlock = &(m_pMemoryBlock[iCount]);
	}

	// 마지막은 nullptr
	pTemp[iBlockNum - 1].stpNextBlock = nullptr;

	// m_pFreeNode가 처음을 가리키게 세팅
	m_pFreeNode = (st_BLOCK_NODE*)m_pMemoryBlock;
}


template<typename DATA>
CMemoryPool<DATA>::~CMemoryPool()
{
	// 만약 m_pMemoryBlock이 NULL이면 필요할 때 하나씩 할당한 것이므로
	if (nullptr == m_pMemoryBlock)
	{	
		while (nullptr != m_pFreeNode)
		{
			st_BLOCK_NODE *pTemp = m_pFreeNode;

			// 다음 노드를 향하게 하고
			m_pFreeNode = m_pFreeNode->stpNextBlock;

			delete pTemp;
		}
	}

	// placementNew
	if (true == m_bPlacementNew)
	{
		free(m_pMemoryBlock);
	}

	// placementNew 사용 안 함
	else
	{
		delete[] m_pMemoryBlock;
	}
}


//////////////////////////////////////////////////////////////////////////
// 블럭 하나를 할당받는다.
//
// Parameters: 없음.
// Return: (DATA *) 데이타 블럭 포인터.
//////////////////////////////////////////////////////////////////////////
template<typename DATA>
DATA*	CMemoryPool<DATA>::Alloc(void)
{
	// 처음 할당할 블럭들이 없으면(더 이상 남아있지 않으면) 새로 만들어서 줌
	if (nullptr == m_pFreeNode)
	{
		return AdditionalAlloc();
	}

	// 일단 alloc할 위치를 임시 변수에 저장해둔다
	st_BLOCK_NODE *pTemp = m_pFreeNode;

	// freenode가 다음을 가리키게 한다
	m_pFreeNode = m_pFreeNode->stpNextBlock;

	m_iUseCount++;

	// placementnew 사용하는 경우
	// malloc으로 공간만 할당한 경우
	if (true == m_bPlacementNew)
	{
		new (&(pTemp->data)) DATA;
	}

	return &(pTemp->data);
}

template<typename DATA>
DATA * CMemoryPool<DATA>::AdditionalAlloc(void)
{
	// 일단 alloc할 위치를 임시 변수에 저장해둔다
	st_BLOCK_NODE *pTemp = new st_BLOCK_NODE;

	m_iAllocCount++;
	m_iUseCount++;

	return &(pTemp->data);
}

//////////////////////////////////////////////////////////////////////////
// 사용중이던 블럭을 해제한다.
//
// Parameters: (DATA *) 블럭 포인터.
// Return: (BOOL) TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////
template<typename DATA>
bool	CMemoryPool<DATA>::Free(DATA *pData)
{
	if (nullptr == pData)
	{
		return false;
	}

	// Free한 블럭의 DATA의 주소를 받은 것이므로
	// stpNextBlock 쪽으로 주소를 옮겨준다
	// DATA앞에 위치하므로 포인터 변수 크기만큼 앞으로 이동하면 됨
	st_BLOCK_NODE* pTemp = (st_BLOCK_NODE*)((char*)pData - sizeof(st_BLOCK_NODE*));

	// m_pFreeNode -> Free한 블럭 -> 가장 위에 있던 블럭이 되도록 포인터 조절
	pTemp->stpNextBlock = m_pFreeNode;
	m_pFreeNode = pTemp;

	m_iUseCount--;

	// 만약 malloc으로 할당 받았다면 파괴자 호출
	if (true == m_bPlacementNew)
	{
		pTemp->data.~DATA();
	}

	return true;
}

#endif	// !__MEMORY_POOL__