/*---------------------------------------------------------------
MemoryPool.

�޸� Ǯ Ŭ����.
Ư�� ����Ÿ(����ü,Ŭ����,����)�� ������ �Ҵ� �� ��������.

- ����.

procademy::CMemoryPool<DATA> MemPool(300, FALSE);
DATA *pData = MemPool.Alloc();

pData ���

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
	// �� �� �տ� ���� ��� ����ü.
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
	// ������, �ı���.
	//
	// Parameters:	(int) �ִ� �� ����.
	//				(bool) ������ ȣ�� ����.
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CMemoryPool(int iBlockNum, bool bPlacementNew = false);
	virtual	~CMemoryPool();


	//////////////////////////////////////////////////////////////////////////
	// �� �ϳ��� �Ҵ�޴´�.
	//
	// Parameters: ����.
	// Return: (DATA *) ����Ÿ �� ������.
	//////////////////////////////////////////////////////////////////////////
	DATA	*Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// ������̴� ���� �����Ѵ�.
	//
	// Parameters: (DATA *) �� ������.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(DATA *pData);


	//////////////////////////////////////////////////////////////////////////
	// ���� Ȯ�� �� �� ������  ��´�. (�޸�Ǯ ������ ��ü ����)
	//
	// Parameters: ����.
	// Return: (int) �޸� Ǯ ���� ��ü ����
	//////////////////////////////////////////////////////////////////////////
	int		GetAllocCount(void) { return m_iAllocCount; }

	//////////////////////////////////////////////////////////////////////////
	// ���� ������� �� ������ ��´�.
	//
	// Parameters: ����.
	// Return: (int) ������� �� ����.
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
	// placementNew ���
	if (true == bPlacementNew)
	{
		m_pMemoryBlock = (st_BLOCK_NODE*)malloc(sizeof(st_BLOCK_NODE) * iBlockNum);
		memset(m_pMemoryBlock, 0, sizeof(st_BLOCK_NODE) * iBlockNum);

		st_BLOCK_NODE *pTemp = (st_BLOCK_NODE *)m_pMemoryBlock;

		// stpNextBlock�� ���� ��带 ����Ű�� ����
		for (int iCount = 1; iCount < iBlockNum; iCount++)
		{
			pTemp[iCount - 1].stpNextBlock = &(m_pMemoryBlock[iCount]);
		}

		// �������� nullptr
		pTemp[iBlockNum - 1].stpNextBlock = nullptr;
	}

	// placementNew ��� �� ��
	else
	{
		m_pMemoryBlock = new st_BLOCK_NODE[iBlockNum];
		memset(m_pMemoryBlock, 0, sizeof(st_BLOCK_NODE) * iBlockNum);

		st_BLOCK_NODE *pTemp = (st_BLOCK_NODE *)m_pMemoryBlock;
		
		// stpNextBlock�� ���� ��带 ����Ű�� ����
		for (int iCount = 1; iCount < iBlockNum; iCount++)
		{
			pTemp[iCount - 1].stpNextBlock = &(m_pMemoryBlock[iCount]);
		}

		// �������� nullptr
		pTemp[iBlockNum - 1].stpNextBlock = nullptr;

	}

	// m_pFreeNode�� ó���� ����Ű�� ����
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

	// placementNew ��� �� ��
	else
	{
		delete[] m_pMemoryBlock;
	}
}


//////////////////////////////////////////////////////////////////////////
// �� �ϳ��� �Ҵ�޴´�.
//
// Parameters: ����.
// Return: (DATA *) ����Ÿ �� ������.
//////////////////////////////////////////////////////////////////////////
template<typename DATA>
DATA*	CMemoryPool<DATA>::Alloc(void)
{
	// ���� �� �̻� �Ҵ��Ұ� ������
	if (nullptr == m_pFreeNode)
	{
		// �ϴ� alloc�� ��ġ�� �ӽ� ������ �����صд�
		st_BLOCK_NODE *pTemp = m_pFreeNode;

		pTemp = new st_BLOCK_NODE;
		memset(pTemp, 0, sizeof(st_BLOCK_NODE));

		m_iAllocCount++;
		m_iUseCount++;

		m_pFreeNode = nullptr;

		return &(pTemp->data);
	}


	// placementnew ������� ����
	if (false == m_bPlacementNew)
	{
		// �ϴ� alloc�� ��ġ�� �ӽ� ������ �����صд�
		st_BLOCK_NODE *pTemp = m_pFreeNode;

		// freenode�� ������ ����Ű�� �Ѵ�
		m_pFreeNode = m_pFreeNode->stpNextBlock;

		m_iUseCount++;


		return &(pTemp->data);
	}

	// placementnew ���
	else
	{
		st_BLOCK_NODE *pTemp = m_pFreeNode;

		// freenode�� ������ ����Ű�� �Ѵ�
		m_pFreeNode = m_pFreeNode->stpNextBlock;


		new (pTemp)st_BLOCK_NODE;
		
		m_iUseCount++;


		return &(pTemp->data);

	}
}

//////////////////////////////////////////////////////////////////////////
// ������̴� ���� �����Ѵ�.
//
// Parameters: (DATA *) �� ������.
// Return: (BOOL) TRUE, FALSE.
//////////////////////////////////////////////////////////////////////////
template<typename DATA>
bool	CMemoryPool<DATA>::Free(DATA *pData)
{
	//placementnew ���
	if (true == m_bPlacementNew)
	{
		// ������ ����� FreeNode�� ����Ű�� �ִ� ���� ����Ű�� ��
		st_BLOCK_NODE* pTemp;

		pTemp = (st_BLOCK_NODE*)((char*)pData - sizeof(st_BLOCK_NODE*));

		pTemp->stpNextBlock = m_pFreeNode;

		// FreeNode�� ������ ����� ����Ű�� ��
		m_pFreeNode = pTemp;

		// �ı��� ȣ��
		m_pFreeNode->~st_BLOCK_NODE();

		m_iUseCount--;

		return true;
	}
	// placementnew ��� �� ��
	else
	{
		// ������ ����� FreeNode�� ����Ű�� �ִ� ���� ����Ű�� ��
		st_BLOCK_NODE* pTemp;

		pTemp = (st_BLOCK_NODE*)((char*)pData - sizeof(st_BLOCK_NODE*));

		pTemp->stpNextBlock = m_pFreeNode;

		// FreeNode�� ������ ����� ����Ű�� ��
		m_pFreeNode = pTemp;

		m_iUseCount--;

		return true;
	}
}

#endif	// !__MEMORY_POOL__