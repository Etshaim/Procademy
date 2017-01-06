#include "Func.h"

// �޴� ���
void PrintMenu()
{
	printf_s("## MENU ##\n");
	printf_s("1.����\n");
	printf_s("2.�α���\n");
	printf_s("3.��ü ȸ������\n");
	printf_s("4.ID �˻�\n");
	printf_s("5.Ż��\n");
	printf_s("6.��ü ����\n");
	printf_s("-> ");
}

// �޴� �Է�
int InputMenu()
{
	while (1)
	{
		int inputNum;

		scanf_s("%d", &inputNum);

		if (0 < inputNum || inputNum < 7)
		{
			return inputNum;
		}
		else
		{
			printf_s("error : 1 ~ 6 ������ ���ڸ� �Է�");
		}
	}
}

// ������ �Է�
void InputData(Node **pNode)
{
	char tempPW[12] = { 0, };

	// ID �Է�
	printf_s("ID : ");
	scanf_s("%s", (*pNode)->_data.id, 12);

	// Password �Է�
	printf_s("PASSWORD : ");
	scanf_s("%s", tempPW, 12);

	// password ���ڵ�
	(*pNode)->_data.password = EncodePassword( tempPW, 12 );

	// Name �Է�
	printf_s("NAME : ");
	scanf_s("%s", (*pNode)->_data.name, 12);
}

// �н����� ���ڵ�
int EncodePassword(char password[], int len)
{
	int count = 0;
	int EncPassword = 0;

	for ( count = 0; count < len; ++count )
	{
		EncPassword = EncPassword + password[count];	
	}

	EncPassword = EncPassword ^ 0x18181818;

	return EncPassword;
}


// ����
void Join()
{
	PushBack();
}

// ==================
// Double Linked List
// ==================

void PushFront()	// �� �տ� ����
{
	Node *pNewNode = ( Node* )malloc( sizeof( Node ) );

	// data �Է�
	InputData(&pNewNode);

	//< ó�� �����ϴ� ��� head���� tail
	if (pHead == NULL)
	{
		pHead = pNewNode;
		pTail = pNewNode;

		pNewNode->pPrev = NULL;
		pNewNode->pNext = NULL;
	}
	else
	{
		pHead->pPrev = pNewNode;
		pNewNode->pNext = pHead;
		pHead = pNewNode;
		pHead->pPrev = NULL;		// �׻� head�� prev�� tail�� next�� NULL��
	}
}

void LogIn()
{
	char bfID[12] = { 0, };		// �Է¹��� ���̵� �ӽ� ����
	char bfPW[12] = { 0, };		// �Է¹��� �н�����
	int logIn = 0;

	int index = 0;
	Hash *pHash = NULL;

	// ���̵� �Է�
	printf_s("ID : ");
	scanf_s("%s", bfID, 12);

	// ��й�ȣ �Է�
	printf_s("PASSWORD : ");
	scanf_s("%s", bfPW, 12);

	// ���̵� �˻� ( ���� �Ǵ� )

		//< ���̵� ��ȯ(key���)
	index = GetKey(bfID, 12);

	pHash = hashTable[index];

	while(pHash != NULL)
	{
		if( strcmp( pHash->_hsData.id, bfID ) == 0 )
		{
			//printf_s("ID �߰�\n");
			
			logIn = 1;

			// ��й�ȣ ��
			if ( pHash->_hsData.password == (EncodePassword(bfPW, 12) ))
			{
				printf_s("%s �α��� ����\n", bfID);
			}
			else
			{
				printf_s("PASSWORD Ʋ��\n");
			}
		}
		
		pHash = pHash->pNext;
	}

	if( logIn == 0 )
	{
		printf_s("ID�� ����\n");
	}
}

void DeleteAccount()
{
	// ��ü �����Ϳ��� �����
	
	Node* pNode = pHead;
	char bfID[12] = { 0, };

	Hash* pHash = NULL;
	int count = 0;
	int index = 0;

	int logIn = 0;

	//char bfPW[12] = { 0, };

	// ���̵� �Է�

	// ���̵� �Է�
	printf_s("ID : ");
	scanf_s("%s", bfID, 12);

	// ��й�ȣ �Է�
	//printf_s("PASSWORD : ");
	//scanf_s("%s", bfPW, 12);

	while( pNode != NULL )
	{
		// ��ġ�ϴ� ���̵� ������
		if ( (strcmp( pNode->_data.id, bfID )) == 0)
		{
			Node* pTemp = pNode->pNext;

			if (pNode == pHead && pNode == pTail)
			{
				pHead = NULL;
				pTail = NULL;

				free(pNode);
				++count;

				//< �ؽ����̺����� ����
				DeleteHash( bfID, 12 );

				break;
			}

			if (pNode == pHead)
			{
				pNode->pNext->pPrev = NULL;
				pHead = pNode->pNext;
			}
			else if (pNode == pTail)
			{
				pNode->pPrev->pNext = NULL;
				pTail = pNode->pPrev;
			}
			else
			{
				pNode->pPrev->pNext = pNode->pNext;
				pNode->pNext->pPrev = pNode->pPrev;
			}

			free(pNode);

			++count;

			pNode = NULL;

			//< �ؽ����̺����� ����
			DeleteHash( bfID, 12 );
			break;
		}
		else
		{
			pNode = pNode->pNext;
		}
	}

	if ( count == 0 )
	{
		printf_s("ID ����\n");
	}
}

void PushBack()	// �� �ڿ� ����
{
	Node *pNewNode = (Node*)malloc(sizeof(Node));
	
	// data �Է�
	InputData(&pNewNode);

	//< ó�� �����ϴ� ��� head���� tail
	if (pTail == NULL)
	{
		pHead = pNewNode;
		pTail = pNewNode;

		pNewNode->pPrev = NULL;
		pNewNode->pNext = NULL;
	}
	else
	{
		pTail->pNext = pNewNode;

		pNewNode->pPrev = pTail;
		pTail = pNewNode;
		pTail->pNext = NULL;		// �׻� head�� prev�� tail�� next�� NULL��
	}

	// �ؽ� ���̺� ����
	PushHash(pNewNode);
}

void PrintAll()				// ��ü ���
{
	Node *pNode = pHead;
	int count = 0;

	while (pNode != NULL)
	{
		printf_s("ID : %s\n", pNode->_data.id);
		printf_s("PASSWORD : %d\n", pNode->_data.password);
		printf_s("NAME : %s\n", pNode->_data.name);
		

		pNode = pNode->pNext;

		++count;
	}

	if (count == 0)
	{
		printf_s("data�� ����\n");
	}
}

void DeleteAll()			// ��ü ����
{
	Node *pNode = pHead;

	while (pNode != NULL)
	{
		pHead = pNode->pNext;
		
		free(pNode);
		pNode = pHead;
	}
}

// ==================
// HashTable
// ==================

void PushHash(Node* pNode)	// �ؽ� ���̺� ����
{
	Hash *pHash = ( Hash* )malloc( sizeof(Hash) );
	Hash *pTemp = NULL;

	pHash->pNext = NULL;
	pHash->pPrev = NULL;

	//< ������ ����
	strcpy(pHash->_hsData.id, pNode->_data.id);
	strcpy(pHash->_hsData.name, pNode->_data.name);
	pHash->_hsData.password = pNode->_data.password;

	pHash->key = GetKey( pNode->_data.id, 12 );

	
	if( hashTable[ pHash->key ] == NULL )
	{
		hashTable[ pHash->key ] = pHash;
	}
	else
	{
		pTemp = hashTable[ pHash->key ];

		while(pTemp->pNext != NULL)
		{
			pTemp = pTemp->pNext;
		}

		pTemp->pNext = pHash;
		pHash->pPrev = pTemp;
	}
}

int GetKey( char id[], int len )			// key ���ϱ�
{
	int count = 0;
	int key = 0;

	for( count = 0; count < len; ++count )
	{
		key = key + id[count];	
	}

	key %= 10;

	return key;
}


void LookUpHash( )	// �ؽ� ���̺��� �˻�
{
	char temp[12] = { 0, };
	int index = 0;

	Hash *pHash = NULL;

	//< ���̵� �Է�
	printf_s("ID �Է� : ");
	scanf_s("%s", temp, 12);

	//< ���̵� ��ȯ(key���)
	index = GetKey(temp, 12);

	pHash = hashTable[index];

	while(pHash != NULL)
	{
		if( strcmp( pHash->_hsData.id, temp ) == 0 )
		{
			printf_s("ID : %s\n", pHash->_hsData.id);
			printf_s("PASSWORD : %d\n", pHash->_hsData.password);
			printf_s("NAME : %s\n", pHash->_hsData.name);
		}
		
		pHash = pHash->pNext;
	}
}

void DeleteHash( char temp[], int len )	// �ؽ� ���̺��� ����
{
	//char temp[12] = { 0, };
	int index = 0;

	Hash *pHash = NULL;

	//< ���̵� ��ȯ(key���)
	index = GetKey(temp, 12);

	pHash = hashTable[index];

	while(pHash != NULL)
	{
		if( strcmp( pHash->_hsData.id, temp ) == 0 )
		{
			if ( pHash->pPrev == NULL && pHash->pPrev == NULL )
			{
				free(pHash);
				pHash = NULL;
			}
			else if( pHash->pPrev == NULL )
			{
				pHash->pNext->pPrev = NULL;
				free(pHash);
				pHash = NULL;
			}
			else if( pHash->pNext == NULL )
			{
				pHash->pPrev->pNext = NULL;
				free(pHash);
				pHash = NULL;
			}
			else
			{
				pHash->pPrev->pNext = pHash->pNext;
				pHash->pNext->pPrev = pHash->pPrev;
				free(pHash);
				pHash = NULL;
			}
			break;
		}
		
		pHash = pHash->pNext;
	}
}