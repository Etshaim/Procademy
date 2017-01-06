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

// ȭ�� ���
void Draw(void);

// �޴� ���
void PrintMenu(void);

// �޴� �Է�
int InputMenu(void);

// ��Ʈ�� Ÿ�� ���
void PrintControlTower(void);

// �Է�
void PutData(void);
//========
//	queue
//========

//< ����
void InQueue(Unit* pUnit);

//< �б�
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
				printf_s("���α׷��� �����մϴ�.\n");
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

// ȭ�� ���
void Draw(void)
{
	// �޴� ���
	PrintMenu();

	// �����Ȳ ���
	PrintControlTower();
}

// �޴� ���
void PrintMenu(void)
{
	printf_s("====================================\n");
	printf_s("[ 1 : ���� ���� ��û ] [ 2 : ���� ]\n");
	printf_s("====================================\n");
}

// �޴� �Է�
int InputMenu(void)
{
	while (1)
	{
		int inputNum = 0;

		inputNum = _getch();
	}
}

// ��Ʈ�� Ÿ�� ���
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
		
		// % ���
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

//< ����
void InQueue(Unit *pUnit)
{
	//< ���� �ε����� read�̸� �� �� ���·� ��
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

//< �б�
bool DeQueue(Unit *pData)
{
	if (read == write)
	{
		printf_s("empty\n");

		return false;
	}

	// pData�� �� �ֱ�

	++read;
	read %= MAX;

	return true;
}

//< �б�
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