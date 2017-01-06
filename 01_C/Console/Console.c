#include <Windows.h>
#include <stdio.h>
#include <memory.h>
#include "Console.h"

HANDLE hConsole;

char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

//< 콘솔 제어를 위한 준비 작업
void cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	// 화면의 커서를 안보이게 설정
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;	// 커서 크기

	// 콘솔 화면 핸들을 구함
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo( hConsole, &stConsoleCursor );
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
	for( countY = 0; countY < dfSCREEN_HEIGHT; ++countY)
	{
		for( countX = 0; countX < dfSCREEN_WIDTH; ++countX )
		{
			cs_MoveCursor(countX, countY);
			printf_s(" ");
		}
	}
}

//< 버퍼의 내용을 화면으로 찍어주는 함수
void Buffer_Flip(void)
{
	int count = 0;

	for( count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		cs_MoveCursor( 0, count );
		printf_s( "%s", screenBuffer[count] );
	}
}

//< 화면 버퍼를 지워주는 함수
void Buffer_Clear(void)
{
	int countX;
	int countY;

	//< 화면 크기만큼 세로, 가로 이중 for문 사용
	for( countY = 0; countY < dfSCREEN_HEIGHT; ++countY)
	{
		for( countX = 0; countX < dfSCREEN_WIDTH; ++countX )
		{
			if( countX == 80 )
			{
				screenBuffer[countY][countX] = '\0';
				continue;
			}
		
			screenBuffer[countY][countX] = ' ';
		}
	}
}

//< 버퍼의 특정 위치에 원하는 문자를 출력
void Sprite_Draw(int x, int y, char sprite)
{
	screenBuffer[y][x] = sprite;
}

//< key 입력
void KeyProcess(int *x, int *y)
{
	if(GetAsyncKeyState(VK_UP) & 0x8000) (*y)--;
	if(GetAsyncKeyState(VK_DOWN) & 0x8000) (*y)++;
	if(GetAsyncKeyState(VK_LEFT) & 0x8000) (*x)--;
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000) (*x)++;

	/*switch( GetAsyncKeyState(VK_LEFT) )
	{
	case 0x0001:
		--(*x);
		break;
	case 0x8000:
		--(*x);
		break;
	case 0x8001:
		--(*x);
		break;
	default :
		break;
	}

	switch( GetAsyncKeyState(VK_RIGHT) )
	{
	case 0x0001:
		++(*x);
		break;
	case 0x8000:
		++(*x);
		break;
	case 0x8001:
		++(*x);
		break;
	default :
		break;
	}

	switch( GetAsyncKeyState(VK_DOWN) )
	{
	case 0x0001:
		++(*y);
		break;
	case 0x8000:
		++(*y);
		break;
	case 0x8001:
		++(*y);
		break;
	default :
		break;
	}

	switch( GetAsyncKeyState(VK_UP) )
	{
	case 0x0001:
		--(*y);
		break;
	case 0x8000:
		--(*y);
		break;
	case 0x8001:
		--(*y);
		break;
	default :
		break;
	}*/
}