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

private:
	// �� ��� �����
	Node* CreateNewNode(const T& element)
	{
		Node* newNode = new NewNode;
		newNode->_data = element;
		newNode->_pNext = NULL;

		return newNode;
	}

	// �ε��� üũ( ��ȿ���� )
	bool CheckIndex(int where);

	// ��, �� ������ Ž������ ����
	bool CheckSearchFront(int where);

	// �տ������� Ž��
	Node* SearchFront(int where);

	// �ڿ������� Ž��
	Node* SearchBack(int where);

	Node*	_pHead;
	Node*	_pTail;
	int			_size;
};


// ������
template<typename T>
TDoubleLinkedList<T>::TDoubleLinkedList(void)
	: _pHead(NULL)
	, _size(0)
{

}

// �Ҹ���
template<typename T>
TDoubleLinkedList<T>::~TDoubleLinkedList(void)
{
	DeleteAll();
}

// ������ ����
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

// �ڷ� ����
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

// ���� ����
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

// ���� ����
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
	
	// ��ü ����
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

//	// �� ��� �����
//template<typename T>
//Node* TDoubleLinkedList<T>::CreateNewNode(const T& element)
//{
//	Node* newNode = new NewNode;
//	newNode->_data = element;
//	newNode->_pNext = NULL;
//
//	return newNode;
//}

	// �ε��� üũ( ��ȿ���� )
template<typename T>
bool TDoubleLinkedList<T>::CheckIndex(int where)
{
	if (where < 0 || where > m_size)
	{
		return false;
	}

	return true;
}

	// ��, �� ������ Ž������ ����
template<typename T>
bool TDoubleLinkedList<T>::CheckSearchFront(int where)
{
	if (where < _size / 2)
	{
		return true;
	}

	return false;
}

	// �տ������� Ž��
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

	// �ڿ������� Ž��
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