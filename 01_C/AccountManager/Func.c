#include "Func.h"

// 메뉴 출력
void PrintMenu()
{
	printf_s("## MENU ##\n");
	printf_s("1.가입\n");
	printf_s("2.로그인\n");
	printf_s("3.전체 회원보기\n");
	printf_s("4.ID 검색\n");
	printf_s("5.탈퇴\n");
	printf_s("6.전체 삭제\n");
	printf_s("-> ");
}

// 메뉴 입력
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
			printf_s("error : 1 ~ 6 사이의 숫자를 입력");
		}
	}
}

// 데이터 입력
void InputData(Node **pNode)
{
	char tempPW[12] = { 0, };

	// ID 입력
	printf_s("ID : ");
	scanf_s("%s", (*pNode)->_data.id, 12);

	// Password 입력
	printf_s("PASSWORD : ");
	scanf_s("%s", tempPW, 12);

	// password 인코딩
	(*pNode)->_data.password = EncodePassword( tempPW, 12 );

	// Name 입력
	printf_s("NAME : ");
	scanf_s("%s", (*pNode)->_data.name, 12);
}

// 패스워드 인코딩
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


// 가입
void Join()
{
	PushBack();
}

// ==================
// Double Linked List
// ==================

void PushFront()	// 맨 앞에 삽입
{
	Node *pNewNode = ( Node* )malloc( sizeof( Node ) );

	// data 입력
	InputData(&pNewNode);

	//< 처음 삽입하는 경우 head이자 tail
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
		pHead->pPrev = NULL;		// 항상 head의 prev와 tail의 next는 NULL로
	}
}

void LogIn()
{
	char bfID[12] = { 0, };		// 입력받은 아이디 임시 저장
	char bfPW[12] = { 0, };		// 입력받은 패스워드
	int logIn = 0;

	int index = 0;
	Hash *pHash = NULL;

	// 아이디 입력
	printf_s("ID : ");
	scanf_s("%s", bfID, 12);

	// 비밀번호 입력
	printf_s("PASSWORD : ");
	scanf_s("%s", bfPW, 12);

	// 아이디 검색 ( 유무 판단 )

		//< 아이디 변환(key얻기)
	index = GetKey(bfID, 12);

	pHash = hashTable[index];

	while(pHash != NULL)
	{
		if( strcmp( pHash->_hsData.id, bfID ) == 0 )
		{
			//printf_s("ID 발견\n");
			
			logIn = 1;

			// 비밀번호 비교
			if ( pHash->_hsData.password == (EncodePassword(bfPW, 12) ))
			{
				printf_s("%s 로그인 성공\n", bfID);
			}
			else
			{
				printf_s("PASSWORD 틀림\n");
			}
		}
		
		pHash = pHash->pNext;
	}

	if( logIn == 0 )
	{
		printf_s("ID가 없음\n");
	}
}

void DeleteAccount()
{
	// 전체 데이터에서 지우기
	
	Node* pNode = pHead;
	char bfID[12] = { 0, };

	Hash* pHash = NULL;
	int count = 0;
	int index = 0;

	int logIn = 0;

	//char bfPW[12] = { 0, };

	// 아이디 입력

	// 아이디 입력
	printf_s("ID : ");
	scanf_s("%s", bfID, 12);

	// 비밀번호 입력
	//printf_s("PASSWORD : ");
	//scanf_s("%s", bfPW, 12);

	while( pNode != NULL )
	{
		// 일치하는 아이디가 있으면
		if ( (strcmp( pNode->_data.id, bfID )) == 0)
		{
			Node* pTemp = pNode->pNext;

			if (pNode == pHead && pNode == pTail)
			{
				pHead = NULL;
				pTail = NULL;

				free(pNode);
				++count;

				//< 해쉬테이블에서도 삭제
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

			//< 해쉬테이블에서도 삭제
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
		printf_s("ID 없음\n");
	}
}

void PushBack()	// 맨 뒤에 삽입
{
	Node *pNewNode = (Node*)malloc(sizeof(Node));
	
	// data 입력
	InputData(&pNewNode);

	//< 처음 삽입하는 경우 head이자 tail
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
		pTail->pNext = NULL;		// 항상 head의 prev와 tail의 next는 NULL로
	}

	// 해쉬 테이블에 삽입
	PushHash(pNewNode);
}

void PrintAll()				// 전체 출력
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
		printf_s("data가 없음\n");
	}
}

void DeleteAll()			// 전체 삭제
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

void PushHash(Node* pNode)	// 해쉬 테이블에 삽입
{
	Hash *pHash = ( Hash* )malloc( sizeof(Hash) );
	Hash *pTemp = NULL;

	pHash->pNext = NULL;
	pHash->pPrev = NULL;

	//< 데이터 복사
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

int GetKey( char id[], int len )			// key 구하기
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


void LookUpHash( )	// 해쉬 테이블에서 검색
{
	char temp[12] = { 0, };
	int index = 0;

	Hash *pHash = NULL;

	//< 아이디 입력
	printf_s("ID 입력 : ");
	scanf_s("%s", temp, 12);

	//< 아이디 변환(key얻기)
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

void DeleteHash( char temp[], int len )	// 해쉬 테이블에서 삭제
{
	//char temp[12] = { 0, };
	int index = 0;

	Hash *pHash = NULL;

	//< 아이디 변환(key얻기)
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