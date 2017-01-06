#include <Windows.h>
#include <stdio.h>
#include "Console.h"

//HANDLE hConsole;
//
//char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

CConsole::CConsole()
{

}

CConsole::~CConsole()
{
}

//< �ܼ� ��� ���� �غ� �۾�
void CConsole::Initial(void)
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
void CConsole::MoveCursor(int posX, int posY)
{
	COORD stCoord;
	stCoord.X = posX;
	stCoord.Y = posY;

	//< ���ϴ� ��ġ�� Ŀ���� �̵�
	SetConsoleCursorPosition(hConsole, stCoord);
}

//< ������ ������ ȭ������ ����ִ� �Լ�
void CConsole::BufferFlip(void)
{
	int count = 0;

	for (count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		MoveCursor(0, count);

		printf(screenBuffer[count]);
	}
}

//< �ܼ� ȭ���� �ʱ�ȭ
void CConsole::ClearScreen(void)
{
	int count = 0;

	for (count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		memset(screenBuffer[count], ' ', dfSCREEN_WIDTH);
		screenBuffer[count][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
	//int countX = 0;
	//int countY = 0;

	////< ȭ�� ũ�⸸ŭ ����, ���� ���� for�� ���
	//// ���� ��ǥ���� printf(" "); ������ ���� ���
	//for (countY = 0; countY < dfSCREEN_HEIGHT; ++countY)
	//{
	//	for (countX = 0; countX < dfSCREEN_WIDTH; ++countX)
	//	{
	//		cs_MoveCursor(countX, countY);
	//		printf_s(" ");
	//	}
	//}
}