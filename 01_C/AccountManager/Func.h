#ifndef FUNC_H
#define FUNC_H

#include "Define.h"
#include "Node.h"

// �޴� ���
void PrintMenu();

// �޴� �Է�
int InputMenu();

// ������ �Է�
void InputData(Node **pNode);

// �н����� ���ڵ�
int EncodePassword(char password[], int len);


//==================
// Menu
//==================
void Join();			// >>1. ����
void LogIn();			// 2. �α���		(hash)
void PrintAll();		// >>3. ��ü ȸ������
void LookUpAccount();	// >>4. ID �˻�	(hash)
void DeleteAccount();	// 5. Ż��
void DeleteAll();		// >>6. ��ü ����

	
// ==================
// Double Linked List
// ==================

void PushFront();		// �� �տ� ����
void PushBack();		// �� �ڿ� ����

void PrintAll();		// ��ü ���
void DeleteAll();		// ��ü ����

// ==================
// HashTable
// ==================


void PushHash(Node* pNode);		// �ؽ� ���̺� ����
int GetKey( char id[], int len );	// key ���ϱ�
void LookUpHash(  );		// �ؽ� ���̺��� �˻�
void DeleteHash( char temp[], int len );	// �ؽ� ���̺��� ����

#endif // FUNC_H