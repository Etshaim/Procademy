#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <Windows.h>
#include <stdbool.h>
#include "Invader.h"


//< �ʱ�ȭ
void Initial(void)
{
	int x		= 0;
	int y		= 0;
	int count	= 0;

	srand( (int) time (NULL));

	// �÷��̾� ���� ������ �ʱ�ȭ
	// HP : 3
	// 39, 19 ��ġ���� ���� ��

	g_Player.HP = 3;
	g_Player.visible = true;

	g_Player.x = 39;
	g_Player.y = 19;

	// ���� ��ġ 
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

	// �Ѿ� ��� �ʱ�ȭ
	for (count = 0; count < dfSHOT_MAX; ++count)
	{
		g_Bullet[count].visible = false;
	}
}

//< ������ ������ ȭ������ ����ִ� �Լ�
void BufferFlip(void)
{
	int count = 0;

	for (count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		cs_MoveCursor(0, count);
		printf_s( screenBuffer[count] );
	}

}

//< ȭ�� ���۸� �����ִ� �Լ�
void BufferClear(void)
{
	int count = 0;

	for (count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		memset(screenBuffer[count], ' ', dfSCREEN_WIDTH);
		screenBuffer[count][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}

//< ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���
void SpriteDraw(int x, int y, char sprite)
{
	if (x < 0 || y < 0 || x >= dfSCREEN_WIDTH - 1 || y >= dfSCREEN_HEIGHT)
	{
		return;
	}

	screenBuffer[y][x] = sprite;
}

//< Ű���� �Է�
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

	// �÷��̾� �̵� �ݰ� ����
	g_Player.x = max(g_Player.x, 4);
	g_Player.x = min(g_Player.x, dfSCREEN_WIDTH - 4);

	g_Player.y = max(g_Player.y, 15);
	g_Player.y = min(g_Player.y, dfSCREEN_HEIGHT - 1);

	// �̻��� Ű
	if(GetAsyncKeyState(VK_CONTROL) & 0x8001)
	{
		CreateBullet(g_Player.x, g_Player.y, false);
	}

	// esc Ű
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		return false;
	}

	return true;
}

//< Ư�� ��ġ��  �Ѿ� ����
// true ����
// false �Ʊ�
void CreateBullet(int x, int y, bool enemy)
{
	int count = 0;

	for (count = 0; count < dfSHOT_MAX; ++count)
	{
		// ��ü �Ѿ� �迭���� ����ִ� �Ѿ� ã��
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

//< �Ѿ��� �̵� ó��
void ShotProcess(void)
{
	int count = 0;
	int collisionEnemy = 0;

	for (count = 0; count < dfSHOT_MAX; ++count)
	{
		if (g_Bullet[count].visible)
		{
			// �Ѿ� �̵�
			// ���� �Ѿ��� ������ �Ʒ���, �Ʊ� �Ѿ��� �Ʒ����� ����
			if (g_Bullet[count].enemy)
			{
				g_Bullet[count].y++;
			}
			else
			{
				g_Bullet[count].y--;
			}

			// �Ѿ� ���� üũ
			// ȭ�� ������ �Ѿ �Ѿ��� �����
			if (g_Bullet[count].x < 0 || g_Bullet[count].y < 0 ||
				g_Bullet[count].x >= (dfSCREEN_WIDTH - 1) || g_Bullet[count].y >= dfSCREEN_HEIGHT - 1)
			{
				g_Bullet[count].visible = false;
			}

			// �Ѿ��� �浹 ó��
			// �Ʊ��� �Ѿ��� ������ ��� üũ
			// ������ �Ѿ��� �÷��̾�� üũ
			if (g_Bullet[count].enemy)
			{
				// ������ �Ѿ��� �÷��̾ ������(��ǥ�� ������)
				if (g_Bullet[count].x == g_Player.x && g_Bullet[count].y == g_Player.y)
				{
					-- g_Player.HP;
					
					//HP�� 0�� �Ǹ� ����. visible�� false��
					if (g_Player.HP <= 0)
					{
						g_Player.visible = false;
					}

					// �浹�� �Ѿ��� ����
					g_Bullet[count].visible = false;
				}
				
			}
			else  //g_Bullet[count].enemy
			{
				// �÷��̾��� �Ѿ��̸� ������ �浹 üũ

				collisionEnemy = CollisionEnemy(g_Bullet[count].x, g_Bullet[count].y);

				// �浹�� enemy�� ������
				if (collisionEnemy >= 0)
				{
					// HP ���̱�
					-- g_Enemy[collisionEnemy].HP;

					// HP�� 0���� �۾����� visible false
					if (g_Enemy[collisionEnemy].HP <= 0)
					{
						g_Enemy[collisionEnemy].visible = false;
					}

					// �浹�� �Ѿ��� �����
					g_Bullet[count].visible = false;
				}
			}
		}
	}
}

//< �浹ó�� üũ
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

//< ������ �̵��� ������ ó��
void EnemyAI(void)
{
	int count = 0;

	// ����ִ� ������ AI ó��
	for (count = 0; count < dfENEMY_MAX; ++count)
	{
		
		if (true == g_Enemy[count].visible)
		{
			// ���� �̵� ó��
			// false�� ���� �̵�, true�� ������ �̵�
			if (true == g_Enemy[count].direction)
			{
				g_Enemy[count].x++;
			}
			else
			{
				g_Enemy[count].x--;
			}

			// ���� ��ġ �������� 10ĭ �̻� �̵��ϸ� ���� ��ȯ
			if (abs(g_Enemy[count].createX - g_Enemy[count].x) >= 10)
			{
				g_Enemy[count].direction = !g_Enemy[count].direction;
			}

			// ���� ����ó��, 1% Ȯ���� �Ѿ��� ��
			if ((rand() % 100 + 1) > 99)
			{
				CreateBullet(g_Enemy[count].x, g_Enemy[count].y, true);
			}

		}
	}
}

//< ���� Ŭ���� üũ
// 0 : x / 1 : ���ӿ��� / 2 : ���� Ŭ����
int GameClearCheck(void)
{
	int count = 0;

	// ���� Ŭ���� üũ
	// ��� ������ ������ Ŭ����
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

	// ���� ���� üũ
	// �÷��̾ �׾����� ���� ����
	if (!g_Player.visible)
	{
		return 1;
	}

	return 0;
}

//< ��ũ�� ���ۿ� �÷��̾� �׸���
void DrawPlayer(void)
{
	if (g_Player.visible)
	{
		SpriteDraw(g_Player.x, g_Player.y, '#');
	}
}


//< ��ũ�� ���ۿ� ���� �׸���
void DrawEnemy(void)
{
	int count = 0;

	for (count = 0; count < dfENEMY_MAX; ++count)
	{
		// ��� �ִ� �����̸� �׸���
		if (g_Enemy[count].visible)
		{
			SpriteDraw(g_Enemy[count].x, g_Enemy[count].y, '@');
		}
	}
}

//< ��ũ�� ���ۿ� �Ѿ� �׸���
void DrawBullet(void)
{
	int count = 0;

	char shot = '\0';

	for (count = 0; count < dfSHOT_MAX; ++count)
	{
		// ����ִ� �Ѿ��̸� �׸���
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