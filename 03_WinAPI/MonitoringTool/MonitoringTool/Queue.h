#ifndef QUEUE__H_

#include "stdafx.h"

#define dfQUEUE_ARRAY_MAX		100

template <typename T>
class CQueue
{
public :
	CQueue();	// ������
	~CQueue();	// �ı���
	
	struct Node
	{
		T	_data;
	};

	BOOL	Enqueue(T data);
	BOOL	Dequeue(T* pData);
	BOOL	Peek(T* pData, int pos);
	int		GetFront(void);
	int		GetRear(void);
	int		GetSize(void);

	T		_lastData;

private:
	int		_capacity;		// �뷮
	int		_front;			// front �ε���
	int		_rear;			// rear �ε���
	
	Node*	_pQueue;		
};

template <typename T>
CQueue<T>::CQueue()
	: _front(0), _rear(0), _capacity(dfQUEUE_ARRAY_MAX)
{
	_pQueue = new Node[dfQUEUE_ARRAY_MAX + 1];
}

template <typename T>
CQueue<T>::~CQueue()
{
	delete []_pQueue;

	_pQueue = NULL;
}


template <typename T>
BOOL CQueue<T>::Enqueue(T data)
{
	//< ���� �ε����� front�̸� �� �� ���·� ��
	if ( (_rear + 1) % _capacity == _front )
	{
		return false;
	}

	int position = 0;

	// ���� �ε����� 
	if (_rear == _capacity)
	{
		position = _rear;
		_rear = 0;
	}
	else
	{
		position = _rear;
		_rear++;
	}

	_pQueue[position]._data = data;

	return true;
}

template <typename T>
BOOL CQueue<T>::Dequeue(T* pData)
{
	// front�� rear�� ������ ť�� �� ������
	if (_front == _rear)
	{
		return false;
	}

	int position = _front;

	if (_front == _capacity)
	{
		_front = 0;
	}
	else
	{
		_front++;
	}

	*pData = _pQueue[position]._data;

	return true;
}

template <typename T>
BOOL CQueue<T>::Peek(T* pData, int pos)
{
	// front�� rear�� ������ ť�� �� ������
	if (_front == _rear)
	{
		return false;
	}


	*pData = _pQueue[ (_front+pos) % _capacity ]._data;

	return true;
}

template <typename T>
int CQueue<T>::GetFront(void)
{
	return _front;
}

template <typename T>
int CQueue<T>::GetRear(void)
{
	return _rear;
}


#endif // !QUEUE__H_
