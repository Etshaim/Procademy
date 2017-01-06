#include <Windows.h>
#include <stdio.h>
#include <memory.h>
#include "Console.h"

HANDLE hConsole;

char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

//< �ܼ� ��� ���� �غ� �۾�
void cs_Initial(void)
{
	CONSOLE_CURSOR_INFO stConsoleCursor;

	// ȭ���� Ŀ���� �Ⱥ��̰� ����
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;	// Ŀ�� ũ��

	// �ܼ� ȭ�� �ڵ��� ����
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo( hConsole, &stConsoleCursor );
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
	for( countY = 0; countY < dfSCREEN_HEIGHT; ++countY)
	{
		for( countX = 0; countX < dfSCREEN_WIDTH; ++countX )
		{
			cs_MoveCursor(countX, countY);
			printf_s(" ");
		}
	}
}

//< ������ ������ ȭ������ ����ִ� �Լ�
void Buffer_Flip(void)
{
	int count = 0;

	for( count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		cs_MoveCursor( 0, count );
		printf_s( "%s", screenBuffer[count] );
	}
}

//< ȭ�� ���۸� �����ִ� �Լ�
void Buffer_Clear(void)
{
	int countX;
	int countY;

	//< ȭ�� ũ�⸸ŭ ����, ���� ���� for�� ���
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

//< ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���
void Sprite_Draw(int x, int y, char sprite)
{
	screenBuffer[y][x] = sprite;
}

//< key �Է�
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