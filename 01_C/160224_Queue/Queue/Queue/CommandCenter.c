#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>

#define MAX	5

typedef struct UNIT
{
	DWORD	CreateTick;
	int		index;
}Unit;

Unit unitQueue[MAX];

int write = 0;
int read = 0;
int count = 0;

//======
//	menu
//======

// 화면 출력
void Draw(void);

// 메뉴 출력
void PrintMenu(void);

// 메뉴 입력
int InputMenu(void);

// 컨트롤 타워 출력
void PrintControlTower(void);

// 입력
void PutData(void);
//========
//	queue
//========

//< 쓰기
void InQueue(Unit* pUnit);

//< 읽기
bool DeQueue(Unit *pData);

//< peek
bool Peek(Unit *pData, int pos);

//======
//	menu
//======
void main(void)
{
	while (1)
	{
		system("cls");

		Draw();

		if (_kbhit())
		{
			switch (_getch())
			{
			case '1':
				PutData();
				break;
			case '2':
				printf_s("프로그램을 종료합니다.\n");
				return;
				break;
			default:
				printf_s("error");
				break;
			}
		}
		Sleep(200);
	}
	
}

// 화면 출력
void Draw(void)
{
	// 메뉴 출력
	PrintMenu();

	// 진행상황 출력
	PrintControlTower();
}

// 메뉴 출력
void PrintMenu(void)
{
	printf_s("====================================\n");
	printf_s("[ 1 : 유닛 생성 요청 ] [ 2 : 종료 ]\n");
	printf_s("====================================\n");
}

// 메뉴 입력
int InputMenu(void)
{
	while (1)
	{
		int inputNum = 0;

		inputNum = _getch();
	}
}

// 컨트롤 타워 출력
void PrintControlTower(void)
{
	int i = 0;

	int index = read;

	Unit tUnit;

	if (read == write)
	{
		printf_s("empty\n");
		return;
	}

	while (1)
	{
		if (index == write)
		{
			break;
		}
		Peek(&tUnit, index);
		
		// % 출력
		printf_s("PROGRESS : ");
		printf_s("%lu	", ( GetTickCount() - tUnit.CreateTick) / 100);
		printf_s("====================================\n");

		++index;
		index %= MAX;
	}
	printf_s("\n");
}

// PUT
void PutData(void)
{
	Unit temp;

	temp.CreateTick = GetTickCount();
	temp.index = count + 1;

	InQueue(&temp);
}

//< 쓰기
void InQueue(Unit *pUnit)
{
	//< 다음 인덱스가 read이면 꽉 찬 상태로 봄
	if ((write + 1) % MAX == read)
	{
		printf_s("Full\n");

		return;
	}

	unitQueue[write].CreateTick = pUnit->CreateTick;
	unitQueue[write].index = pUnit->index;

	++write;

	write %= MAX;
}

//< 읽기
bool DeQueue(Unit *pData)
{
	if (read == write)
	{
		printf_s("empty\n");

		return false;
	}

	// pData에 값 넣기

	++read;
	read %= MAX;

	return true;
}

//< 읽기
bool Peek(Unit *pData, int pos )
{
	if (read == write)
	{
		printf_s("empty\n");

		return false;
	}
	
	pData->CreateTick	= unitQueue[ (read + pos) % MAX ].CreateTick;
	pData->index		= unitQueue[(read + pos) % MAX].index;

	return true;
}