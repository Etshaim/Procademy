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

// LinkedList ���漱��
template<typename T> class CDoubleLinkedList;

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
	CMemoryPool(int iBlockNum = 0, bool bPlacementNew = false);
	virtual	~CMemoryPool();


	//////////////////////////////////////////////////////////////////////////
	// �� �ϳ��� �Ҵ�޴´�.
	//
	// Parameters: ����.
	// Return: (DATA *) ����Ÿ �� ������.
	//////////////////////////////////////////////////////////////////////////
	DATA	*Alloc(void);
	DATA	*AdditionalAlloc(void);


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
	// iBlockNum�� 0�̸� Alloc ��û�� �� ������ �Ҵ��ؼ� �� ����
	if (0 >= iBlockNum)
	{
		m_iAllocCount = 0;
		m_bPlacementNew = false;
		return;
	}

	// placementNew ���
	if (true == bPlacementNew)
	{
		m_pMemoryBlock = (st_BLOCK_NODE*)malloc(sizeof(st_BLOCK_NODE) * iBlockNum);
	}

	// placementNew ��� �� ��
	else
	{
		m_pMemoryBlock = new st_BLOCK_NODE[iBlockNum];
	}

	st_BLOCK_NODE *pTemp = (st_BLOCK_NODE *)m_pMemoryBlock;

	// stpNextBlock�� ���� ��带 ����Ű�� ����
	for (int iCount = 1; iCount < iBlockNum; iCount++)
	{
		pTemp[iCount - 1].stpNextBlock = &(m_pMemoryBlock[iCount]);
	}

	// �������� nullptr
	pTemp[iBlockNum - 1].stpNextBlock = nullptr;

	// m_pFreeNode�� ó���� ����Ű�� ����
	m_pFreeNode = (st_BLOCK_NODE*)m_pMemoryBlock;
}


template<typename DATA>
CMemoryPool<DATA>::~CMemoryPool()
{
	// ���� m_pMemoryBlock�� NULL�̸� �ʿ��� �� �ϳ��� �Ҵ��� ���̹Ƿ�
	if (nullptr == m_pMemoryBlock)
	{	
		while (nullptr != m_pFreeNode)
		{
			st_BLOCK_NODE *pTemp = m_pFreeNode;

			// ���� ��带 ���ϰ� �ϰ�
			m_pFreeNode = m_pFreeNode->stpNextBlock;

			delete pTemp;
		}
	}

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
	// ó�� �Ҵ��� ������ ������(�� �̻� �������� ������) ���� ���� ��
	if (nullptr == m_pFreeNode)
	{
		return AdditionalAlloc();
	}

	// �ϴ� alloc�� ��ġ�� �ӽ� ������ �����صд�
	st_BLOCK_NODE *pTemp = m_pFreeNode;

	// freenode�� ������ ����Ű�� �Ѵ�
	m_pFreeNode = m_pFreeNode->stpNextBlock;

	m_iUseCount++;

	// placementnew ����ϴ� ���
	// malloc���� ������ �Ҵ��� ���
	if (true == m_bPlacementNew)
	{
		new (&(pTemp->data)) DATA;
	}

	return &(pTemp->data);
}

template<typename DATA>
DATA * CMemoryPool<DATA>::AdditionalAlloc(void)
{
	// �ϴ� alloc�� ��ġ�� �ӽ� ������ �����صд�
	st_BLOCK_NODE *pTemp = new st_BLOCK_NODE;

	m_iAllocCount++;
	m_iUseCount++;

	return &(pTemp->data);
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
	if (nullptr == pData)
	{
		return false;
	}

	// Free�� ���� DATA�� �ּҸ� ���� ���̹Ƿ�
	// stpNextBlock ������ �ּҸ� �Ű��ش�
	// DATA�տ� ��ġ�ϹǷ� ������ ���� ũ�⸸ŭ ������ �̵��ϸ� ��
	st_BLOCK_NODE* pTemp = (st_BLOCK_NODE*)((char*)pData - sizeof(st_BLOCK_NODE*));

	// m_pFreeNode -> Free�� �� -> ���� ���� �ִ� ���� �ǵ��� ������ ����
	pTemp->stpNextBlock = m_pFreeNode;
	m_pFreeNode = pTemp;

	m_iUseCount--;

	// ���� malloc���� �Ҵ� �޾Ҵٸ� �ı��� ȣ��
	if (true == m_bPlacementNew)
	{
		pTemp->data.~DATA();
	}

	return true;
}

#endif	// !__MEMORY_POOL__