#include "Invader.h"
#include "Console.h"

void main(void)
{
	cs_Initial();

	Initial();

	// 게임의 메인 루프
	// 이 루프가 1번 돌면 1프레임
	while (1)
	{
		// 1. 로직
		if (!KeyProcess())
		{
			break;
		}
		EnemyAI();
		ShotProcess();

		// 3. 그래픽 드로우
		BufferClear();
		DrawPlayer();
		DrawEnemy();
		DrawBullet();

		// 4. 화면 표시
		BufferFlip();

		// 2. 게임 종료 체크
		switch (GameClearCheck())
		{
		case 1:
			cs_MoveCursor(20, 10);
			printf_s("게임 오버!!!\n\n");
			exit(0);
			break;

		case 2:
			cs_MoveCursor(20, 10);
			printf_s("게임 클리어!!!\n\n");
			exit(0);
			break;
		}

		Sleep(50);
	}
}
