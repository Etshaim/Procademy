#ifndef DOUBLELINKEDLIST_TEMPLATE__H__
#define DOUBLELINKEDLIST_TEMPLATE__H__

#include <iostream>

template<typename T>
class CDoubleLinkedList
{
public :
	struct NODE
	{
		T data;
		NODE *pPrev;
		NODE *pNext;
	};

public :
	class ITERATOR 
	{
		friend class CDoubleLinkedList;
		NODE* pNode;

	public :
		T& operator*()
		{
			return pNode->data;
		}

		ITERATOR& operator++()
		{
			this->pNode = this -> pNode -> pNext;
			return *this;
		}

		bool  operator==(ITERATOR iterator)
		{
			if (pNode == iterator.pNode)
				return true;
			return false;
		}
		bool  operator!=(ITERATOR iterator)
		{
			if (pNode != iterator.pNode)
				return true;
			return false;
		}
	};

	CDoubleLinkedList();
	~CDoubleLinkedList();

	// ������ ����
	void PushFront(const T& element);

	// �ڷ� ����
	void PushBack(const T& element);

	// ���� ����
	void PushSelect(int where, const T& element);

	// ���� ����
	void PopSelect(int where);

	// ��ü ����
	void DeleteAll(void);

	// ��ü ���
	void PrintAll(void);

	ITERATOR Begin()
	{
		ITERATOR it;
		it.pNode = _pHead;

		return it;
	}

	ITERATOR End()
	{
		ITERATOR it;
		it.pNode = _pTail;

		return it;
	}

private:
	// �� ��� �����
	NODE* CreateNewNODE(const T& element)
	{
		NODE* newNODE = new NODE;
		newNODE->data = element;
		newNODE->pNext = NULL;

		return newNODE;
	}

	// �ε��� üũ( ��ȿ���� )
	bool CheckIndex(int where);

	//// ��, �� ������ Ž������ ����
	//bool CheckSearchFront(int where);

	//// �տ������� Ž��
	//NODE* SearchFront(int where);

	//// �ڿ������� Ž��
	//NODE* SearchBack(int where);

	NODE*	_pHead;
	NODE*	_pTail;
	int		_size;
};


// ������
template<typename T>
CDoubleLinkedList<T>::CDoubleLinkedList(void)
	: _pHead(NULL)
	, _size(0)
{

}

// �Ҹ���
template<typename T>
CDoubleLinkedList<T>::~CDoubleLinkedList(void)
{
	DeleteAll();
}

// ������ ����
template<typename T>
void CDoubleLinkedList<T>::PushFront(const T& element)
{
	NODE* newNODE = CreateNewNODE(element);

	if (_pHead == NULL)
	{
		_pHead = newNODE;
		_pTail = newNODE;
	}
	else
	{
		_pHead = pPrev = newNODE;
		newNODE->mpNext = m_pHead;
		m_pHead = newNODE;
	}

	_size++;
}

// �ڷ� ����
template<typename T>
void CDoubleLinkedList<T>::PushBack(const T& element)
{
	NODE* newNODE = CreateNewNODE(element);

	if (_pHead == NULL)
	{
		_pHead = newNODE;
		_pTail = newNODE;
	}
	else
	{
		newNODE->pPrev = _pTail;
		_pTail->pNext = newNODE;
		_pTail = newNODE;
	}

	_size++;
}

// ���� ����
template<typename T>
void CDoubleLinkedList<T>::PushSelect(int where, const T& element)
{
	if (CheckIndex(where) == false)
	{
		return;
	}

	NODE* temp = NULL;
	NODE* newNODE = CreateNewNODE(element);


	if (CheckSearchFront(where))
	{
		temp = SearchFront(where);
	}
	else
	{
		temp = SearchBack(where);
	}

	newNODE->pNext = temp;
	newNODE->pNext = temp->pPrev;

	temp->pPrev->pNext = newNODE;
	temp->pPrev = newNODE;

	_size++;

}

// ���� ����
template<typename T>
void CDoubleLinkedList<T>::PopSelect(int where)
{
	if (CheckWhere(where) == flase)
	{
		return;
	}

	NODE* temp = NULL;

	if (CheckSearchFront(where))
	{
		temp = SearchFront(where);
	}
	else
	{
		temp = SearchBack(where);
	}

	temp->pPrev->pNext = temp->pNext;
	temp->pNext->pPrev = temp->pPrev;

	delete temp;

	_size--;
}

// ��ü ����
template<typename T>
void CDoubleLinkedList<T>::DeleteAll(void)
{
	NODE* temp = _pHead;

	while (temp != NULL)
	{
		_pHead = temp->pNext;

		delete temp;
		temp = _pHead;
	}

	_size = 0;
}

//	// �� ��� �����
//template<typename T>
//NODE* CDoubleLinkedList<T>::CreateNewNODE(const T& element)
//{
//	NODE* newNODE = new NewNODE;
//	newNODE->data = element;
//	newNODE->pNext = NULL;
//
//	return newNODE;
//}

// �ε��� üũ( ��ȿ���� )
template<typename T>
bool CDoubleLinkedList<T>::CheckIndex(int where)
{
	if (where < 0 || where > m_size)
	{
		return false;
	}

	return true;
}

//	// ��, �� ������ Ž������ ����
//template<typename T>
//bool CDoubleLinkedList<T>::CheckSearchFront(int where)
//{
//	if (where < _size / 2)
//	{
//		return true;
//	}
//
//	return false;
//}
//
//	// �տ������� Ž��
//template<typename T>
//NODE* CDoubleLinkedList<T>::SearchFront(int where)
//{
//	NODE* temp = _pHead;
//
//	for (int i = 0; i < where; ++i)
//	{
//		temp = temp->pNext;
//	}
//
//	return temp;
//}
//
//	// �ڿ������� Ž��
//template<typename T>
//NODE* CDoubleLinkedList<T>::SearchBack(int where)
//{
//	NODE* temp = _pTail;
//
//	int count = _size - where - 1;
//
//	for (int i = 0; i < count; ++i)
//	{
//		temp = temp->pPrev;
//	}
//
//	return temp;
//}

#endif //DOUBLELINKEDLIST_T__H__