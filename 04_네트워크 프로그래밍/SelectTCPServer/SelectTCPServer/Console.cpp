#include <Windows.h>
#include <stdio.h>
#include "Console.h"

HANDLE hConsole;

//< 콘솔 제어를 위한 준비 작업
void cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	// 화면의 커서를 안보이게 설정
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;	// 커서 크기

	// 콘솔 화면 핸들을 구함
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}

//< 콘솔 화면의 커서를 x,y 좌표로 이동
void cs_MoveCursor(int posX, int posY)
{
	COORD stCoord;
	stCoord.X = posX;
	stCoord.Y = posY;

	//< 원하는 위치로 커서를 이동
	SetConsoleCursorPosition(hConsole, stCoord);
}

//< 콘솔 화면을 초기화
void cs_ClearScreen(void)
{
	int countX;
	int countY;

	//< 화면 크기만큼 세로, 가로 이중 for문 사용
	// 각각 좌표마다 printf(" "); 공백을 전부 출력
	for (countY = 0; countY < dfSCREEN_HEIGHT; ++countY)
	{
		for (countX = 0; countX < dfSCREEN_WIDTH; ++countX)
		{
			cs_MoveCursor(countX, countY);
			printf_s(" ");
		}
	}
}