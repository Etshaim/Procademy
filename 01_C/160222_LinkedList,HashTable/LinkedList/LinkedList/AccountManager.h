#ifndef ACCOUTMANAGER_H
#define ACCOUTMANAGER_H

#include <stdio.h>
#include <Windows.h>

typedef struct NODE
{
	int data;			// ������
	struct NODE *pPrev;	// �� ���
	struct NODE *pNext;	// �� ���
}Node;

// �޴� ���
void PrintMenu();

// �޴� �Է�
int InputMenu();

// ������ �Է�
int Inputdata();

// �ʱ�ȭ
void InitNode();

// �� �տ� ����
void PushFront(int data);

// �� �ڿ� ����
void PushBack(int data);

// ��ü ���
void PrintAll();

// �˻�
void LookUpdata(int data);

// ���� ����
void Deletedata(int data);

// ��ü ����
void DeleteAll();

#endif // ACCOUTMANAGER_H

