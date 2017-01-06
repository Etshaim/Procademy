#include <stdio.h>
#include <Windows.h>
#include "Console.h"

void main(void)
{
	int x = 0;
	int y = 0;

	cs_Initial();

	x = 5;
	y = 5;

	while(1)
	{
		KeyProcess( &x, &y );

		// 스크린 버퍼를 지움
		Buffer_Clear();

		// 스크린 버퍼에 스프라이트 출력
		Sprite_Draw(x, y, 'A');

		// 스크린 버퍼를 화면으로 출력
		Buffer_Flip();

		// 프레임 맞추기용 대기 10 Frame
		Sleep(100);

	}
	//// 화면 지울 때
	//cs_Initial();
	//
	//cs_MoveCursor(0, 0); // 커서를 0,0 위치로
	//printf_s("abcde");	// 0, 0 위치에 글씨를 찍음

	//cs_MoveCursor(20,10); // 커서를 20,10 위치로
	//printf_s("abcde");	//  20,10 위치에 글씨를 찍음
}