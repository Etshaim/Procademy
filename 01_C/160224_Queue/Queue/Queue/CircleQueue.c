#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>

#define	MAX		5

int Queue[MAX];
int write = 0;
int read = 0;

//============
//< queue
//============
//< 초기화
void Init();

//< 쓰기
void InQueue(int data);

//< 읽기
bool DeQueue(int *pData);

//============
//< menu
//============


//< menu
// 메뉴 출력
void PrintMenu(void);

// 메뉴 입력
int InputMenu(void);

// PUT
void PutData(int *pData);

// GET
void GetData(int *pData);

//< 전체 출력
void PrintAll();

void main(void)
{
	int data = 0;
	int *pData = &data;

	while (1)
	{
		system("cls");
		PrintMenu();

		switch (InputMenu())
		{
		case 1:      // Inqueue
			PutData(pData);
			break;
		case 2:      // Dequeue
			GetData(pData);
			break;
		case 3:      // print
			PrintAll();
			break;
		case 0:      // end
			printf_s("프로그램을 종료합니다.\n");
			return;
		default:   // error
			printf_s("error\n");
			break;
		}
		system("pause");
	}
}

//< menu
// 메뉴 출력
void PrintMenu(void)
{
	printf_s("## Circle Queue\n");
	printf_s("1. Inqueue\n");
	printf_s("2. Dequeue\n");
	printf_s("3. Print\n");
	printf_s("0. End\n");
}

// 메뉴 입력
int InputMenu(void)
{
	while (1)
	{
		int inputNum = 0;

		printf_s("-> ");
		scanf_s("%d", &inputNum);

		if (0 <= inputNum && inputNum <= 3)
		{
			return inputNum;
		}

		printf_s("잘못된 숫자 입력됨\n");
	}
}

//< queue
//< 초기화
void Init()
{
	//write	= Queue;
	//read	= Queue;
}

//< 쓰기
void InQueue(int data)
{	
	//< 다음 인덱스가 read이면 꽉 찬 상태로 봄
	if ( (write + 1) % MAX == read )
	{
		printf_s("Full\n");

		return;
	}

	Queue[write] = data;

	++write;

	write %= MAX;

}

//< 읽기
bool DeQueue(int *data)
{
	if (read == write)
	{
		printf_s("empty\n");

		return false;
	}

	*data = Queue[read];

	++read;
	read %= MAX;

	return true;
}

// PUT
void PutData(int *pData)
{
	printf_s("data 입력 : ");
	scanf_s("%d", pData);

	InQueue(*pData);
}

// GET
void GetData(int *pData)
{
	if ( DeQueue(pData) )
	{
		printf_s("data : %d", *pData);
	}
}

//< 전체 출력
void PrintAll()
{
	int i = 0;

	int count = read;

	if (read == write)
	{
		printf_s("empty\n");
		return;
	}

	while (1)
	{
		if (count == write)
		{
			break;
		}

		printf_s("[%05d] ", Queue[ count % MAX ]);

		++count;
		count %= MAX;
	}
	printf_s("\n");
}