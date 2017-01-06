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
	CMemoryPool(int iBlockNum, bool bPlacementNew = false);
	virtual	~CMemoryPool();


	//////////////////////////////////////////////////////////////////////////
	// 블럭 하나를 할당받는다.
	//
	// Parameters: 없음.
	// Return: (DATA *) 데이타 블럭 포인터.
	//////////////////////////////////////////////////////////////////////////
	DATA	*Alloc(void);

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
	st_BLOCK_NODE	*m_pFreeNode; // stpNextBlock
	st_BLOCK_NODE	*m_pMemoryBlock;
	int				m_iAllocCount;
	int				m_iUseCount;
	bool			m_bPlacementNew;
};

template<typename DATA>
CMemoryPool<DATA>::CMemoryPool(int iBlockNum, bool bPlacementNew)
	:	m_pFreeNode(nullptr),
		m_iAllocCount(iBlockNum),
		m_iUseCount(0),
		m_bPlacementNew(bPlacementNew)
{
	// placementNew 사용
	if (true == bPlacementNew)
	{
		m_pMemoryBlock = (st_BLOCK_NODE*)malloc(sizeof(st_BLOCK_NODE) * iBlockNum);
		memset(m_pMemoryBlock, 0, sizeof(st_BLOCK_NODE) * iBlockNum);

		st_BLOCK_NODE *pTemp = (st_BLOCK_NODE *)m_pMemoryBlock;

		// stpNextBlock가 다음 노드를 가리키게 세팅
		for (int iCount = 1; iCount < iBlockNum; iCount++)
		{
			pTemp[iCount - 1].stpNextBlock = &(m_pMemoryBlock[iCount]);
		}

		// 마지막은 nullptr
		pTemp[iBlockNum - 1].stpNextBlock = nullptr;
	}

	// placementNew 사용 안 함
	else
	{
		m_pMemoryBlock = new st_BLOCK_NODE[iBlockNum];
		memset(m_pMemoryBlock, 0, sizeof(st_BLOCK_NODE) * iBlockNum);

		st_BLOCK_NODE *pTemp = (st_BLOCK_NODE *)m_pMemoryBlock;
		
		// stpNextBlock가 다음 노드를 가리키게 세팅
		for (int iCount = 1; iCount < iBlockNum; iCount++)
		{
			pTemp[iCount - 1].stpNextBlock = &(m_pMemoryBlock[iCount]);
		}

		// 마지막은 nullptr
		pTemp[iBlockNum - 1].stpNextBlock = nullptr;

	}

	// m_pFreeNode가 처음을 가리키게 세팅
	m_pFreeNode = (st_BLOCK_NODE*)m_pMemoryBlock;
}


template<typename DATA>
CMemoryPool<DATA>::~CMemoryPool()
{
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
	// 만약 더 이상 할당할게 없으면
	if (nullptr == m_pFreeNode)
	{
		// 일단 alloc할 위치를 임시 변수에 저장해둔다
		st_BLOCK_NODE *pTemp = m_pFreeNode;

		pTemp = new st_BLOCK_NODE;
		memset(pTemp, 0, sizeof(st_BLOCK_NODE));

		m_iAllocCount++;
		m_iUseCount++;

		m_pFreeNode = nullptr;

		return &(pTemp->data);
	}


	// placementnew 사용하지 않음
	if (false == m_bPlacementNew)
	{
		// 일단 alloc할 위치를 임시 변수에 저장해둔다
		st_BLOCK_NODE *pTemp = m_pFreeNode;

		// freenode가 다음을 가리키게 한다
		m_pFreeNode = m_pFreeNode->stpNextBlock;

		m_iUseCount++;


		return &(pTemp->data);
	}

	// placementnew 사용
	else
	{
		st_BLOCK_NODE *pTemp = m_pFreeNode;

		// freenode가 다음을 가리키게 한다
		m_pFreeNode = m_pFreeNode->stpNextBlock;


		new (pTemp)st_BLOCK_NODE;
		
		m_iUseCount++;


		return &(pTemp->data);

	}
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
	//placementnew 사용
	if (true == m_bPlacementNew)
	{
		// 해제한 블록이 FreeNode가 가리키고 있는 곳을 가리키게 함
		st_BLOCK_NODE* pTemp;

		pTemp = (st_BLOCK_NODE*)((char*)pData - sizeof(st_BLOCK_NODE*));

		pTemp->stpNextBlock = m_pFreeNode;

		// FreeNode는 해제한 블록을 가리키게 함
		m_pFreeNode = pTemp;

		// 파괴자 호출
		m_pFreeNode->~st_BLOCK_NODE();

		m_iUseCount--;

		return true;
	}
	// placementnew 사용 안 함
	else
	{
		// 해제한 블록이 FreeNode가 가리키고 있는 곳을 가리키게 함
		st_BLOCK_NODE* pTemp;

		pTemp = (st_BLOCK_NODE*)((char*)pData - sizeof(st_BLOCK_NODE*));

		pTemp->stpNextBlock = m_pFreeNode;

		// FreeNode는 해제한 블록을 가리키게 함
		m_pFreeNode = pTemp;

		m_iUseCount--;

		return true;
	}
}

#endif	// !__MEMORY_POOL__