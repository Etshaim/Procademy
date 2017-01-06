#include <Windows.h>
#include <stdio.h>
#include "Console.h"

HANDLE hConsole;

//< �ܼ� ��� ���� �غ� �۾�
void cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	// ȭ���� Ŀ���� �Ⱥ��̰� ����
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;	// Ŀ�� ũ��

	// �ܼ� ȭ�� �ڵ��� ����
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}

//< �ܼ� ȭ���� Ŀ���� x,y ��ǥ�� �̵�
void cs_MoveCursor(int posX, int posY)
{
	COORD stCoord;
	stCoord.X = posX;
	stCoord.Y = posY;

	//< ���ϴ� ��ġ�� Ŀ���� �̵�
	SetConsoleCursorPosition(hConsole, stCoord);
}

//< �ܼ� ȭ���� �ʱ�ȭ
void cs_ClearScreen(void)
{
	int countX;
	int countY;

	//< ȭ�� ũ�⸸ŭ ����, ���� ���� for�� ���
	// ���� ��ǥ���� printf(" "); ������ ���� ���
	for (countY = 0; countY < dfSCREEN_HEIGHT; ++countY)
	{
		for (countX = 0; countX < dfSCREEN_WIDTH; ++countX)
		{
			cs_MoveCursor(countX, countY);
			printf_s(" ");
		}
	}
}