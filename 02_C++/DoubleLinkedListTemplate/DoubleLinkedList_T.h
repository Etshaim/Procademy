#ifndef DOUBLELINKEDLIST_T__H__
#define DOUBLELINKEDLIST_T__H__

#include <iostream>

template<typename T>
class TDoubleLinkedList
{
public :
	struct Node
	{
		T _data;
		Node *_pPrev;
		Node *_pNext;
	};

public :
	TDoubleLinkedList(void);
	~TDoubleLinkedList(void);

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

private:
	// 새 노드 만들기
	Node* CreateNewNode(const T& element)
	{
		Node* newNode = new NewNode;
		newNode->_data = element;
		newNode->_pNext = NULL;

		return newNode;
	}

	// 인덱스 체크( 유효한지 )
	bool CheckIndex(int where);

	// 앞, 뒤 어디부터 탐색할지 결정
	bool CheckSearchFront(int where);

	// 앞에서부터 탐색
	Node* SearchFront(int where);

	// 뒤에서부터 탐색
	Node* SearchBack(int where);

	Node*	_pHead;
	Node*	_pTail;
	int			_size;
};


// 생성자
template<typename T>
TDoubleLinkedList<T>::TDoubleLinkedList(void)
	: _pHead(NULL)
	, _size(0)
{

}

// 소멸자
template<typename T>
TDoubleLinkedList<T>::~TDoubleLinkedList(void)
{
	DeleteAll();
}

// 앞으로 삽입
template<typename T>
void TDoubleLinkedList<T>::PushFront(const T& element)
{
	Node* newNode = CreateNewNode(element);

	if (_pHead == NULL)
	{
		_pHead = newNode;
		_pTail = newNode;
	}
	else
	{
		_pHead = _pPrev = newNode;
		newNode->m_pNext = m_pHead;
		m_pHead = newNode;
	}

	_size++;
}

// 뒤로 삽입
template<typename T>
void TDoubleLinkedList<T>::PushBack(const T& element)
{
	Node* newNode = CreateNewNode(element);

	if (_pHead == NULL)
	{
		_pHead = newNode;
		_pTail = newNode;
	}
	else
	{
		newNode->_pPrev = _pTail;
		_pTail->_pNext = newNode;
		_pTail = newNode;
	}

	_size++;
}

// 선택 삽입
template<typename T>
void TDoubleLinkedList<T>::PushSelect(int where, const T& element)
{
	if (CheckIndex(where) == false)
	{
		return;
	}

	Node* temp = NULL;
	Node* newNode = CreateNewNode(element);
	

	if (CheckSearchFront(where))
	{
		temp = SearchFront(where);
	}
	else
	{
		temp = SearchBack(where);
	}

	newNode->_pNext = temp;
	newNode->_pNext = temp->_pPrev;

	temp->_pPrev->_pNext = newNode;
	temp->_pPrev = newNode;

	_size++;
	
}

// 선택 삭제
template<typename T>
void TDoubleLinkedList<T>::PopSelect(int where)
{
	if (CheckWhere(where) == flase)
	{
		return;
	}

	Node* temp = NULL;

	if (CheckSearchFront(where))
	{
		temp = SearchFront(where);
	}
	else
	{
		temp = SearchBack(where);
	}

	temp->_pPrev->_pNext = temp->_pNext;
	temp->_pNext->_pPrev = temp->_pPrev;

	delete temp;

	_size--;
}
	
	// 전체 삭제
template<typename T>
void TDoubleLinkedList<T>::DeleteAll(void)
{
	Node* temp = _pHead;

	while (temp != NULL)
	{
		_pHead = temp->_pNext;

		delete temp;
		temp = _pHead;
	}

	_size = 0;
}

//	// 새 노드 만들기
//template<typename T>
//Node* TDoubleLinkedList<T>::CreateNewNode(const T& element)
//{
//	Node* newNode = new NewNode;
//	newNode->_data = element;
//	newNode->_pNext = NULL;
//
//	return newNode;
//}

	// 인덱스 체크( 유효한지 )
template<typename T>
bool TDoubleLinkedList<T>::CheckIndex(int where)
{
	if (where < 0 || where > m_size)
	{
		return false;
	}

	return true;
}

	// 앞, 뒤 어디부터 탐색할지 결정
template<typename T>
bool TDoubleLinkedList<T>::CheckSearchFront(int where)
{
	if (where < _size / 2)
	{
		return true;
	}

	return false;
}

	// 앞에서부터 탐색
template<typename T>
T* TDoubleLinkedList<T>::SearchFront(int where)
{
	Node* temp = _pHead;

	for (int i = 0; i < where; ++i)
	{
		temp = temp->_pNext;
	}

	return temp;
}

	// 뒤에서부터 탐색
template<typename T>
Node* TDoubleLinkedList<T>::SearchBack(int where)
{
	Node* temp = _pTail;

	int count = _size - where - 1;

	for (int i = 0; i < count; ++i)
	{
		temp = temp->_pPrev;
	}

	return temp;
}

#endif //DOUBLELINKEDLIST_T__H__