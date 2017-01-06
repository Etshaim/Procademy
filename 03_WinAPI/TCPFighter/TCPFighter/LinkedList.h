#ifndef __DOUBLE_LINKED_LIST_TEMPLATE_H__
#define __DOUBLE_LINKED_LIST_TEMPLATE_H__

template<typename T>
class CDoubleLinkedList
{
public:
	struct NODE
	{
		T		data;
		NODE	*pPrev;
		NODE	*pNext;
	};

public:

	// LinkedList�� �ִ� �����͸� Ž���ϱ� ���� �ݺ���
	class Iterator
	{
		// LinkedList������ ITERATOR Ŭ������ 
		//��� ����� ������ �� �ְ� ���� �ο�
		friend class CDoubleLinkedList;

	public:
		Iterator(NODE *pNode = nullptr)
		{
			this->m_pNode = pNode;
		}


		T& operator*()
		{
			return m_pNode->data;
		}

		Iterator& operator++()
		{
			this->m_pNode = this->m_pNode->pNext;
			return *this;
		}

		const Iterator operator++(int)
		{
			Iterator temp = *this;
			this->m_pNode = this->m_pNode->pNext;
			return temp;
		}

		bool	operator==(const Iterator iterator)
		{
			if (m_pNode == iterator.m_pNode)
			{
				return true;
			}
			return false;
		}

		bool	operator!=(const Iterator iterator)
		{
			if (m_pNode != iterator.m_pNode)
			{
				return true;
			}
			return false;
		}

		NODE	*GetNode(void)
		{
			return m_pNode;
		}

	public:
		NODE	*m_pNode;

	};

	CDoubleLinkedList();
	~CDoubleLinkedList();

	// ������ ����
	void PushFront(const T& data);

	// �ڷ� ����
	void PushBack(const T& data);

	// iter �������� pop_front
	T&	PopFront(Iterator& iter);

	// ���� ����
	bool Erase(NODE* node);
	Iterator& Erase(Iterator& iter);

	// ��ü ����
	void Clear(void);

	// ���� Ȯ��
	int	GetSize(void)	{return m_iSize;}

	Iterator Begin(void)
	{
		Iterator iter;
		iter.m_pNode = m_pHead->pNext;

		return iter;
	}

	Iterator End(void)
	{
		Iterator iter;
		iter.m_pNode = m_pTail;

		return iter;
	}

private:
	// �� ��� �����
	NODE* CreateNewNode(const T & data = NULL);

	// ��� �����ϱ�
	void	HangNode(NODE *pNow, NODE *pTarget);

	NODE*	m_pHead;
	NODE*	m_pTail;

	// size�� �ʿ��Ѱ�
	int		m_iSize;
};



// ������
template<typename T>
CDoubleLinkedList<T>::CDoubleLinkedList(void)
	: m_pHead(NULL), m_pTail(NULL), m_iSize(0)
{
	m_pHead = CreateNewNode();
	m_pTail = CreateNewNode();

	m_pHead->pNext = m_pTail;
	m_pTail->pPrev = m_pHead;
}

// �Ҹ���
template<typename T>
CDoubleLinkedList<T>::~CDoubleLinkedList(void)
{
	Clear();
}

// ������ ����
template<typename T>
void CDoubleLinkedList<T>::PushFront(const T& data)
{
	NODE* pNewNode = CreateNewNode(data);

	HangNode(m_pHead, pNewNode);

	m_iSize++;
}

// �ڷ� ����
template<typename T>
void CDoubleLinkedList<T>::PushBack(const T& data)
{
	NODE* pNewNode = CreateNewNode(data);

	HangNode(m_pTail->pPrev, pNewNode);

	m_iSize++;
}

template<typename T>
T&	CDoubleLinkedList<T>::PopFront(Iterator& iter)
{
	NODE  *pNode = iter.m_pNode;

	// pNode�� head�� tail�̸� ���� ó������ ���� ��
	if (pNode == m_pHead || pNode == m_pTail)
	{
		
	}

	// ����� ���� iter++ ó�� ����
	// ���� ����� iter++�ϸ� �� ��
	iter++;

	// ���� ��� ���ĸ� �����ϰ�
	pNode->pPrev->pNext = pNode->pNext;
	pNode->pNext->pPrev = pNode->pPrev;

	m_iSize--;

	return pNode->data;

}


// ���� ����
template<typename T>
bool CDoubleLinkedList<T>::Erase(NODE* pNode)
{
	if (NULL != pNode)
	{
		pNode->pPrev->pNext = pNode->pNext;
		pNode->pNext->pPrev = pNode->pPrev;

		delete pNode;

		m_iSize--;

		return true;
	}
	return false;
}

// ���� ����
template<typename T>
typename  CDoubleLinkedList<T>::Iterator& CDoubleLinkedList<T>::Erase(typename CDoubleLinkedList<T>::Iterator& iter)
{

	NODE *pNode = iter.m_pNode;

	// pNode�� head�� tail�̸� ���� ó������ ���� ��
	if (pNode == m_pHead || pNode == m_pTail)
	{
		return iter;
	}

	// ����� ���� iter++ ó�� ����
	// ���� ����� iter++�ϸ� �� ��
	iter++;

	// ���� ��� ���ĸ� �����ϰ�
	pNode->pPrev->pNext = pNode->pNext;
	pNode->pNext->pPrev = pNode->pPrev;

	delete pNode;

	m_iSize--;

	return iter;
}

//��ü ����
template<typename T>
void CDoubleLinkedList<T>::Clear(void)
{
	NODE *pTemp = nullptr;

	while (1)
	{
		pTemp = m_pHead->pNext;

		if (pTemp == m_pTail)
		{
			return;
		}

		Erase(pTemp);

		m_iSize = 0;
	}
}

template<typename T>
void CDoubleLinkedList<T>::HangNode(NODE *pTarget, NODE *pNow)
{
	pNow->pPrev = pTarget;
	pNow->pNext = pTarget->pNext;

	pTarget->pNext->pPrev = pNow;
	pTarget->pNext = pNow;
}

template<typename T>
typename CDoubleLinkedList<T>::NODE* CDoubleLinkedList<T>::CreateNewNode(const T& data = NULL)
{
	NODE* pNewNode = new NODE;

	pNewNode->data = data;
	pNewNode->pPrev = NULL;
	pNewNode->pNext = NULL;

	return pNewNode;
}


#endif //	!__DOUBLE_LINKED_LIST_TEMPLATE_H__