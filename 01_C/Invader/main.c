#include "Invader.h"
#include "Console.h"

void main(void)
{
	cs_Initial();

	Initial();

	// ������ ���� ����
	// �� ������ 1�� ���� 1������
	while (1)
	{
		// 1. ����
		if (!KeyProcess())
		{
			break;
		}
		EnemyAI();
		ShotProcess();

		// 3. �׷��� ��ο�
		BufferClear();
		DrawPlayer();
		DrawEnemy();
		DrawBullet();

		// 4. ȭ�� ǥ��
		BufferFlip();

		// 2. ���� ���� üũ
		switch (GameClearCheck())
		{
		case 1:
			cs_MoveCursor(20, 10);
			printf_s("���� ����!!!\n\n");
			exit(0);
			break;

		case 2:
			cs_MoveCursor(20, 10);
			printf_s("���� Ŭ����!!!\n\n");
			exit(0);
			break;
		}

		Sleep(50);
	}
}
