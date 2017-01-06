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

//< 콘솔 제어를 위한 준비 작업
void CConsole::Initial(void)
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
void CConsole::MoveCursor(int posX, int posY)
{
	COORD stCoord;
	stCoord.X = posX;
	stCoord.Y = posY;

	//< 원하는 위치로 커서를 이동
	SetConsoleCursorPosition(hConsole, stCoord);
}

//< 버퍼의 내용을 화면으로 찍어주는 함수
void CConsole::BufferFlip(void)
{
	int count = 0;

	for (count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		MoveCursor(0, count);

		printf(screenBuffer[count]);
	}
}

//< 콘솔 화면을 초기화
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

	////< 화면 크기만큼 세로, 가로 이중 for문 사용
	//// 각각 좌표마다 printf(" "); 공백을 전부 출력
	//for (countY = 0; countY < dfSCREEN_HEIGHT; ++countY)
	//{
	//	for (countX = 0; countX < dfSCREEN_WIDTH; ++countX)
	//	{
	//		cs_MoveCursor(countX, countY);
	//		printf_s(" ");
	//	}
	//}
}