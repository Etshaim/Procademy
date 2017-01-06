#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <Windows.h>
#include <stdbool.h>
#include "Invader.h"


//< 초기화
void Initial(void)
{
	int x		= 0;
	int y		= 0;
	int count	= 0;

	srand( (int) time (NULL));

	// 플레이어 최초 데이터 초기화
	// HP : 3
	// 39, 19 위치에서 시작 함

	g_Player.HP = 3;
	g_Player.visible = true;

	g_Player.x = 39;
	g_Player.y = 19;

	// 적군 배치 
	x = 17;
	y = 1;

	for (count = 0; count < dfENEMY_MAX; ++count)
	{
		if (count % 10 == 0)
		{
			x = 17;
			y = y + 2;
		}

		g_Enemy[count].HP = 1;
		g_Enemy[count].x = x;
		g_Enemy[count].y = y;
		g_Enemy[count].createX = x;
		g_Enemy[count].visible = true;
		g_Enemy[count].direction = true;

		x += 5;
	}

	// 총알 모두 초기화
	for (count = 0; count < dfSHOT_MAX; ++count)
	{
		g_Bullet[count].visible = false;
	}
}

//< 버퍼의 내용을 화면으로 찍어주는 함수
void BufferFlip(void)
{
	int count = 0;

	for (count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		cs_MoveCursor(0, count);
		printf_s( screenBuffer[count] );
	}

}

//< 화면 버퍼를 지워주는 함수
void BufferClear(void)
{
	int count = 0;

	for (count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		memset(screenBuffer[count], ' ', dfSCREEN_WIDTH);
		screenBuffer[count][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}

//< 버퍼의 특정 위치에 원하는 문자를 출력
void SpriteDraw(int x, int y, char sprite)
{
	if (x < 0 || y < 0 || x >= dfSCREEN_WIDTH - 1 || y >= dfSCREEN_HEIGHT)
	{
		return;
	}

	screenBuffer[y][x] = sprite;
}

//< 키보드 입력
bool KeyProcess(void)
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8001)
	{
		g_Player.x -= 1;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8001)
	{
		g_Player.x += 1;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8001)
	{
		g_Player.y -= 1;
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		g_Player.y += 1;
	}

	// 플레이어 이동 반경 제한
	g_Player.x = max(g_Player.x, 4);
	g_Player.x = min(g_Player.x, dfSCREEN_WIDTH - 4);

	g_Player.y = max(g_Player.y, 15);
	g_Player.y = min(g_Player.y, dfSCREEN_HEIGHT - 1);

	// 미사일 키
	if(GetAsyncKeyState(VK_CONTROL) & 0x8001)
	{
		CreateBullet(g_Player.x, g_Player.y, false);
	}

	// esc 키
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		return false;
	}

	return true;
}

//< 특정 위치에  총알 생성
// true 적군
// false 아군
void CreateBullet(int x, int y, bool enemy)
{
	int count = 0;

	for (count = 0; count < dfSHOT_MAX; ++count)
	{
		// 전체 총알 배열에서 비어있는 총알 찾기
		if (!g_Bullet[count].visible)
		{
			g_Bullet[count].visible = true;
			g_Bullet[count].enemy = enemy;
			g_Bullet[count].x = x;
			g_Bullet[count].y = y;

			break;
		}
	}
}

//< 총알의 이동 처리
void ShotProcess(void)
{
	int count = 0;
	int collisionEnemy = 0;

	for (count = 0; count < dfSHOT_MAX; ++count)
	{
		if (g_Bullet[count].visible)
		{
			// 총알 이동
			// 적군 총알은 위에서 아래로, 아군 총알은 아래에서 위로
			if (g_Bullet[count].enemy)
			{
				g_Bullet[count].y++;
			}
			else
			{
				g_Bullet[count].y--;
			}

			// 총알 범위 체크
			// 화면 영역을 넘어간 총알은 지우기
			if (g_Bullet[count].x < 0 || g_Bullet[count].y < 0 ||
				g_Bullet[count].x >= (dfSCREEN_WIDTH - 1) || g_Bullet[count].y >= dfSCREEN_HEIGHT - 1)
			{
				g_Bullet[count].visible = false;
			}

			// 총알의 충돌 처리
			// 아군의 총알은 적군과 모두 체크
			// 적군의 총알은 플레이어와 체크
			if (g_Bullet[count].enemy)
			{
				// 적군의 총알이 플레이어에 맞으면(좌표가 같으면)
				if (g_Bullet[count].x == g_Player.x && g_Bullet[count].y == g_Player.y)
				{
					-- g_Player.HP;
					
					//HP가 0이 되면 죽음. visible을 false로
					if (g_Player.HP <= 0)
					{
						g_Player.visible = false;
					}

					// 충돌된 총알을 지움
					g_Bullet[count].visible = false;
				}
				
			}
			else  //g_Bullet[count].enemy
			{
				// 플레이어의 총알이면 적군과 충돌 체크

				collisionEnemy = CollisionEnemy(g_Bullet[count].x, g_Bullet[count].y);

				// 충돌한 enemy가 있으면
				if (collisionEnemy >= 0)
				{
					// HP 줄이기
					-- g_Enemy[collisionEnemy].HP;

					// HP가 0보다 작아지면 visible false
					if (g_Enemy[collisionEnemy].HP <= 0)
					{
						g_Enemy[collisionEnemy].visible = false;
					}

					// 충돌된 총알은 지운다
					g_Bullet[count].visible = false;
				}
			}
		}
	}
}

//< 충돌처리 체크
int CollisionEnemy(int x, int y)
{
	int count = 0;

	for (count = 0; count < dfENEMY_MAX; ++count)
	{
		if (g_Enemy[count].visible && g_Enemy[count].x == x && g_Enemy[count].y == y)
		{
			return count;
		}
	}
	return -1;
}

//< 적군의 이동과 공격을 처리
void EnemyAI(void)
{
	int count = 0;

	// 살아있는 적군만 AI 처리
	for (count = 0; count < dfENEMY_MAX; ++count)
	{
		
		if (true == g_Enemy[count].visible)
		{
			// 적군 이동 처리
			// false면 왼쪽 이동, true면 오른쪽 이동
			if (true == g_Enemy[count].direction)
			{
				g_Enemy[count].x++;
			}
			else
			{
				g_Enemy[count].x--;
			}

			// 생성 위치 기준으로 10칸 이상 이동하면 방향 전환
			if (abs(g_Enemy[count].createX - g_Enemy[count].x) >= 10)
			{
				g_Enemy[count].direction = !g_Enemy[count].direction;
			}

			// 적군 공격처리, 1% 확률로 총알을 쏨
			if ((rand() % 100 + 1) > 99)
			{
				CreateBullet(g_Enemy[count].x, g_Enemy[count].y, true);
			}

		}
	}
}

//< 게임 클리어 체크
// 0 : x / 1 : 게임오버 / 2 : 게임 클리어
int GameClearCheck(void)
{
	int count = 0;

	// 게임 클리어 체크
	// 모든 적군이 죽으면 클리어
	for (count = 0; count < dfENEMY_MAX; ++count)
	{
		if (g_Enemy[count].visible)
		{
			break;
		}
	}

	if (count == dfENEMY_MAX)
	{
		return 2;
	}

	// 게임 오버 체크
	// 플레이어가 죽었으면 게임 오버
	if (!g_Player.visible)
	{
		return 1;
	}

	return 0;
}

//< 스크린 버퍼에 플레이어 그리기
void DrawPlayer(void)
{
	if (g_Player.visible)
	{
		SpriteDraw(g_Player.x, g_Player.y, '#');
	}
}


//< 스크린 버퍼에 적군 그리기
void DrawEnemy(void)
{
	int count = 0;

	for (count = 0; count < dfENEMY_MAX; ++count)
	{
		// 살아 있는 적군이면 그리기
		if (g_Enemy[count].visible)
		{
			SpriteDraw(g_Enemy[count].x, g_Enemy[count].y, '@');
		}
	}
}

//< 스크린 버퍼에 총알 그리기
void DrawBullet(void)
{
	int count = 0;

	char shot = '\0';

	for (count = 0; count < dfSHOT_MAX; ++count)
	{
		// 살아있는 총알이면 그리기
		if (g_Bullet[count].visible)
		{
			if ( true == g_Bullet[count].enemy)
			{
				shot = 'v';
			}
			else
			{
				shot = '^';
			}
			
			SpriteDraw(g_Bullet[count].x, g_Bullet[count].y, shot);
		}
	}
}