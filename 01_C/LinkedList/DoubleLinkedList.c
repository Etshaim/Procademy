#include <stdio.h>
#include <windows.h>

typedef struct NODE
{
	int data;			// 데이터
	struct NODE *pPrev;	// 전 노드
	struct NODE *pNext;	// 앞 노드
}Node;

Node *head = NULL;		// head
Node *tail = NULL;		// tail

// 메뉴 출력
void PrintMenu();

// 메뉴 입력
int InputMenu();

// 데이터 입력
int Inputdata();

// 초기화
void InitNode();

// 맨 앞에 삽입
void PushFront(int data);

// 맨 뒤에 삽입
void PushBack(int data);

// 전체 출력
void PrintAll();

// 검색
void LookUpdata(int data);

// 선택 삭제
void Deletedata(int data);

// 전체 삭제
void DeleteAll();

void main(void)
{
	while (1)
	{
		system("cls");
		PrintMenu();

		switch (InputMenu())
		{
			case 1:	// 새 데이터 삽입(시작)
			{
				PushFront( Inputdata() );
			}
			break;

			case 2:	// 새 데이터 삽입(끝)
			{
				PushBack( Inputdata() );
			}
			break;

			case 3: // 출력
			{
				PrintAll();
			}
			break;

			case 4:	// 삭제
			{
				Deletedata( Inputdata() );
			}
			break;

			case 5:	// 검색
			{
				LookUpdata(Inputdata());
			}
			break;

			case 6:	// 모두 삭제
			{
				DeleteAll();
			}
			break;

			default:
			{
				printf_s("error");
			}
			break;
		}
		system("pause");
	}

	
}

// 메뉴 출력
void PrintMenu()
{
	printf_s("## MENU ##\n");
	printf_s("1.새 데이터 삽입(시작)\n");
	printf_s("2.새 데이터 삽입(끝)\n");
	printf_s("3.출력\n");
	printf_s("4.삭제\n");
	printf_s("5.검색\n");
	printf_s("6.모두 삭제\n");
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

int Inputdata()
{
	int data = 0;

	printf_s("data 입력(정수) : ");
	scanf_s("%d", &data);

	return data;
}

// 맨 앞에 삽입
void PushFront(int data)
{
	Node *pNewNode = (Node*)malloc(sizeof(Node));
	pNewNode->data = data;

	//< 처음 삽입하는 경우 head이자 tail
	if (head == NULL)
	{
		head = pNewNode;
		tail = pNewNode;

		pNewNode->pPrev = NULL;
		pNewNode->pNext = NULL;
	}
	else
	{
		head->pPrev = pNewNode;
		pNewNode->pNext = head;
		head = pNewNode;
		head->pPrev = NULL;		// 항상 head의 prev와 tail의 next는 NULL로
	}
}

// 맨 뒤에 삽입
void PushBack(int data)
{
	Node *pNewNode = (Node*)malloc(sizeof(Node));
	pNewNode->data = data;

	//< 처음 삽입하는 경우 head이자 tail
	if (tail == NULL)
	{
		head = pNewNode;
		tail = pNewNode;

		pNewNode->pPrev = NULL;
		pNewNode->pNext = NULL;
	}
	else
	{
		tail->pNext = pNewNode;

		pNewNode->pPrev = tail;
		tail = pNewNode;
		tail->pNext = NULL;		// 항상 head의 prev와 tail의 next는 NULL로
	}
}

// 검색
void LookUpdata(int data)
{
	Node* pNode = head;
	int count = 0;

	//< tail까지 검색. tail 다음은 NULL임.
	while (pNode != NULL)
	{
		if (pNode->data == data)
		{
			printf_s("data : %d\n", pNode->data);
			++count;
		}
		pNode = pNode->pNext;
	}

	if (count == 0)
	{
		printf_s("발견된 data가 없음");
	}
}

// 선택 삭제
void Deletedata(int data)
{
	Node* pNode = head;

	//< tail까지 검색하여 삭제. tail 다음은 NULL
	while (pNode != NULL)
	{
		if (pNode->data == data)
		{
			Node* pTemp = pNode->pNext;

			if (pNode == head && pNode == tail)
			{
				head = NULL;
				tail = NULL;

				free(pNode);
				break;
			}

			if (pNode == head)
			{
				pNode->pNext->pPrev = NULL;
				head = pNode->pNext;
			}
			else if (pNode == tail)
			{
				pNode->pPrev->pNext = NULL;
				tail = pNode->pPrev;
			}
			else
			{
				pNode->pPrev->pNext = pNode->pNext;
				pNode->pNext->pPrev = pNode->pPrev;
			}
			free(pNode);

			pNode = pTemp;
		}
		else
		{
			pNode = pNode->pNext;
		}
	}
}

// 전체 출력
void PrintAll()
{
	Node *pNode = head;
	int count = 0;

	while (pNode != NULL)
	{
		printf_s("data : %d\n", pNode->data);
		pNode = pNode->pNext;

		++count;
	}

	if (count == 0)
	{
		printf_s("data가 없음");
	}
}

// 전체 삭제
void DeleteAll()
{
	Node *pNode = head;

	while (pNode != NULL)
	{
		head = pNode->pNext;
		
		free(pNode);
		pNode = head;
	}
}