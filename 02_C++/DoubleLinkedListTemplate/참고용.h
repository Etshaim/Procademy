#include <iostream>

template <typename T>
class CList
{

	struct NODE
	{
		T   data;
		NODE*  pPrev;
		NODE*  pNext;
	};
public:

	class Iterator
	{
		friend class CList;
		NODE*  pNode;

	public:
		T&    operator*()
		{
			return pNode->data;
		}
		Iterator&  operator++()
		{
			this->pNode = this->pNode->pNext;
			return *this;
		}
		const Iterator operator++(int)
		{
			Iterator temp = *this;
			this->pNode = this->pNode->pNext;
			return temp;
		}
		bool  operator==(Iterator it)
		{
			if (pNode == it.pNode)
				return true;
			return false;
		}
		bool  operator!=(Iterator it)
		{
			if (pNode != it.pNode)
				return true;
			return false;
		}
	};
private:
	NODE* m_pHead;
	NODE* m_pTail;
	NODE m_End;

public:
	CList()
	{
		m_pTail = &m_End;
		m_End.data = NULL;
		m_End.pPrev = NULL;
		m_End.pNext = NULL;
		m_pHead = m_pTail;
	}
	Iterator Begin()
	{
		Iterator it;
		it.pNode = m_pHead;
		return it;
	}
	Iterator End()
	{
		Iterator it;
		it.pNode = m_pTail;
		return it;
	}
	void  PushBack(T data)
	{
		NODE* pNode = new NODE;
		pNode->data = data;
		pNode->pPrev = NULL;
		pNode->pNext = m_pTail;

		if (m_pHead != m_pTail)
		{
			m_pTail->pPrev->pNext = pNode;
			pNode->pPrev = m_pTail->pPrev;
		}
		else
		{
			m_pHead = pNode;
		}
		m_pTail->pPrev = pNode;
	}
	Iterator Erase(Iterator it)
	{
		Iterator temp;
		NODE*  pNode = NULL;

		temp.pNode = NULL;

		if (it.pNode == m_pHead)
		{
			pNode = m_pHead;
			m_pHead = m_pHead->pNext;
			m_pHead->pPrev = NULL;
			temp.pNode = pNode->pNext;
			delete pNode;
		}
		else if (it.pNode != m_pTail)
		{
			pNode = it.pNode;
			pNode->pNext->pPrev = pNode->pPrev;
			pNode->pPrev->pNext = pNode->pNext;
			temp.pNode = pNode->pNext;
			delete pNode;
		}
		return temp;
	}
	bool  IsEmpty()
	{
		if (m_pHead == m_pTail)
			return true;
		return false;
	}
};