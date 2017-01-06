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

	// LinkedList에 있는 데이터를 탐색하기 위한 반복자
	class Iterator
	{
		// LinkedList에서는 ITERATOR 클래스의 
		//모든 멤버에 접근할 수 있게 권한 부여
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

	// 앞으로 삽입
	void PushFront(const T& data);

	// 뒤로 삽입
	void PushBack(const T& data);

	// 왼쪽에 걸기
	void HangLeft(Iterator move, Iterator target);

	// 선택 삭제
	bool Erase(NODE* node);
	Iterator& Erase(Iterator& iter);

	// Node 이동
	

	// 전체 삭제
	void Clear(void);


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
	// 새 노드 만들기
	NODE* CreateNewNode(const T & data = NULL);

	// 노드 연결하기
	void	HangNode(NODE *pNow, NODE *pTarget);

	NODE*	m_pHead;
	NODE*	m_pTail;

	// size가 필요한가
	int		m_iSize;
};



// 생성자
template<typename T>
CDoubleLinkedList<T>::CDoubleLinkedList(void)
	: m_pHead(NULL), m_pTail(NULL), m_iSize(0)
{
	m_pHead = CreateNewNode();
	m_pTail = CreateNewNode();

	m_pHead->pNext = m_pTail;
	m_pTail->pPrev = m_pHead;
}

// 소멸자
template<typename T>
CDoubleLinkedList<T>::~CDoubleLinkedList(void)
{
	Clear();
}

// 앞으로 삽입
template<typename T>
void CDoubleLinkedList<T>::PushFront(const T& data)
{
	NODE* pNewNode = CreateNewNode(data);

	HangNode(m_pHead, pNewNode);

	m_iSize++;
}

// 뒤로 삽입
template<typename T>
void CDoubleLinkedList<T>::PushBack(const T& data)
{
	NODE* pNewNode = CreateNewNode(data);

	HangNode(m_pTail->pPrev, pNewNode);

	m_iSize++;
}

template<typename T>
void CDoubleLinkedList<T>::HangLeft(Iterator move, Iterator target)
{
	// 일단 move의 prev와 next를 연결해줌
	
	move.m_pNode->pPrev->pNext = move.m_pNode->pNext;
	move.m_pNode->pNext->pPrev = move.m_pNode->pPrev;

	// move가 target과 target의 prev 사이에 붙음
	move.m_pNode->pNext = target.m_pNode;
	move.m_pNode->pPrev = target.m_pNode->pPrev;

	target.m_pNode->pPrev->pNext = move.m_pNode;
	target.m_pNode->pPrev = move.m_pNode;
}


// 선택 삭제
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

// 선택 삭제
template<typename T>
typename  CDoubleLinkedList<T>::Iterator& CDoubleLinkedList<T>::Erase( typename CDoubleLinkedList<T>::Iterator& iter)
{
	
	NODE *pNode = iter.m_pNode;
	
	// pNode가 head나 tail이면 따로 처리하지 않을 것
	if (pNode == m_pHead || pNode == m_pTail)
	{
		return iter;
	}

	// 지우기 전에 iter++ 처리 먼저
	// 먼저 지우면 iter++하면 안 됨
	iter.m_pNode = iter.m_pNode->pPrev;
	
	// 지울 노드 전후를 연결하고
	pNode->pPrev->pNext = pNode->pNext;
	pNode->pNext->pPrev = pNode->pPrev;

	delete pNode;

	m_iSize--;

	return iter;
}

//전체 삭제
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