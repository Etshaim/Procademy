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

		// ��ũ�� ���۸� ����
		Buffer_Clear();

		// ��ũ�� ���ۿ� ��������Ʈ ���
		Sprite_Draw(x, y, 'A');

		// ��ũ�� ���۸� ȭ������ ���
		Buffer_Flip();

		// ������ ���߱�� ��� 10 Frame
		Sleep(100);

	}
	//// ȭ�� ���� ��
	//cs_Initial();
	//
	//cs_MoveCursor(0, 0); // Ŀ���� 0,0 ��ġ��
	//printf_s("abcde");	// 0, 0 ��ġ�� �۾��� ����

	//cs_MoveCursor(20,10); // Ŀ���� 20,10 ��ġ��
	//printf_s("abcde");	//  20,10 ��ġ�� �۾��� ����
}