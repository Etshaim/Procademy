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
	};

	CDoubleLinkedList();
	~CDoubleLinkedList();

	// 앞으로 삽입
	void PushFront(const T& element);

	// 뒤로 삽입
	void PushBack(const T& element);

	// 선택 삽입
	void PushSelect(int where, const T& element);
	 
	// 선택 삭제
	void PopSelect(int where);
	
	// 전체 삭제
	void DeleteAll(void);

	// 전체 출력
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
	// 새 노드 만들기
	NODE* CreateNewNODE(const T& element)
	{
		NODE* newNODE = new NewNODE;
		newNODE->data = element;
		newNODE->pNext = NULL;

		return newNODE;
	}

	// 인덱스 체크( 유효한지 )
	bool CheckIndex(int where);

	//// 앞, 뒤 어디부터 탐색할지 결정
	//bool CheckSearchFront(int where);
	
	//// 앞에서부터 탐색
	//NODE* SearchFront(int where);

	//// 뒤에서부터 탐색
	//NODE* SearchBack(int where);

	NODE*	_pHead;
	NODE*	_pTail;
	int		_size;
};


// 생성자
template<typename T>
CDoubleLinkedList<T>::CDoubleLinkedList(void)
	: _pHead(NULL)
	, _size(0)
{

}

// 소멸자
template<typename T>
CDoubleLinkedList<T>::~CDoubleLinkedList(void)
{
	DeleteAll();
}

// 앞으로 삽입
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

// 뒤로 삽입
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

// 선택 삽입
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

// 선택 삭제
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
	
	// 전체 삭제
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

//	// 새 노드 만들기
//template<typename T>
//NODE* CDoubleLinkedList<T>::CreateNewNODE(const T& element)
//{
//	NODE* newNODE = new NewNODE;
//	newNODE->data = element;
//	newNODE->pNext = NULL;
//
//	return newNODE;
//}

	// 인덱스 체크( 유효한지 )
template<typename T>
bool CDoubleLinkedList<T>::CheckIndex(int where)
{
	if (where < 0 || where > m_size)
	{
		return false;
	}

	return true;
}

//	// 앞, 뒤 어디부터 탐색할지 결정
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
//	// 앞에서부터 탐색
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
//	// 뒤에서부터 탐색
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