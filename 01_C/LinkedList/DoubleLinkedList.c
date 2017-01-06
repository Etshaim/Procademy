#include <stdio.h>
#include <windows.h>

typedef struct NODE
{
	int data;			// ������
	struct NODE *pPrev;	// �� ���
	struct NODE *pNext;	// �� ���
}Node;

Node *head = NULL;		// head
Node *tail = NULL;		// tail

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

void main(void)
{
	while (1)
	{
		system("cls");
		PrintMenu();

		switch (InputMenu())
		{
			case 1:	// �� ������ ����(����)
			{
				PushFront( Inputdata() );
			}
			break;

			case 2:	// �� ������ ����(��)
			{
				PushBack( Inputdata() );
			}
			break;

			case 3: // ���
			{
				PrintAll();
			}
			break;

			case 4:	// ����
			{
				Deletedata( Inputdata() );
			}
			break;

			case 5:	// �˻�
			{
				LookUpdata(Inputdata());
			}
			break;

			case 6:	// ��� ����
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

// �޴� ���
void PrintMenu()
{
	printf_s("## MENU ##\n");
	printf_s("1.�� ������ ����(����)\n");
	printf_s("2.�� ������ ����(��)\n");
	printf_s("3.���\n");
	printf_s("4.����\n");
	printf_s("5.�˻�\n");
	printf_s("6.��� ����\n");
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

int Inputdata()
{
	int data = 0;

	printf_s("data �Է�(����) : ");
	scanf_s("%d", &data);

	return data;
}

// �� �տ� ����
void PushFront(int data)
{
	Node *pNewNode = (Node*)malloc(sizeof(Node));
	pNewNode->data = data;

	//< ó�� �����ϴ� ��� head���� tail
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
		head->pPrev = NULL;		// �׻� head�� prev�� tail�� next�� NULL��
	}
}

// �� �ڿ� ����
void PushBack(int data)
{
	Node *pNewNode = (Node*)malloc(sizeof(Node));
	pNewNode->data = data;

	//< ó�� �����ϴ� ��� head���� tail
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
		tail->pNext = NULL;		// �׻� head�� prev�� tail�� next�� NULL��
	}
}

// �˻�
void LookUpdata(int data)
{
	Node* pNode = head;
	int count = 0;

	//< tail���� �˻�. tail ������ NULL��.
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
		printf_s("�߰ߵ� data�� ����");
	}
}

// ���� ����
void Deletedata(int data)
{
	Node* pNode = head;

	//< tail���� �˻��Ͽ� ����. tail ������ NULL
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

// ��ü ���
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
		printf_s("data�� ����");
	}
}

// ��ü ����
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