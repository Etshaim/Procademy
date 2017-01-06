#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <Windows.h>
#include <stdbool.h>
#include "Console.h"

#define	dfENEMY_MAX		30		// ������ �ִ�ġ
#define dfSHOT_MAX		50		// �Ѿ��� �ִ�ġ
char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// �÷��̾� ����ü
typedef struct PLAYER
{
	int HP;

	int x;
	int y;

	bool visible;		// ���� ����

}Player;



// �� ����ü
typedef struct ENEMY
{
	int HP;
	
	int x;
	int y;

	int createX;		// ���� ������ġ(x��ǥ)
	bool direction;		// �̵� ����
	bool visible;		// ���� ����

}Enemy;

// �Ѿ� ����ü
typedef struct BULLET
{
	int x;
	int y;

	bool visible;	// ��������� ����
	bool enemy;		// ������ �Ѿ����� �Ʊ��� �Ѿ�����

}Bullet;

// �÷��̾� �ν��Ͻ�
Player g_Player;

// ���� �ν��Ͻ�
Enemy	g_Enemy[dfENEMY_MAX];

// �Ѿ� �ִ�ġ �ν��Ͻ�
Bullet	g_Bullet[dfSHOT_MAX];

//< �ʱ�ȭ
void Initial(void);

//< ������ ������ ȭ������ ����ִ� �Լ�
void BufferFlip(void);

//< ȭ�� ���۸� �����ִ� �Լ�
void BufferClear(void);

//< ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���
void SpriteDraw(int x, int y, char sprite);

//< Ű���� �Է�
bool KeyProcess(void);

//< Ư�� ��ġ��  �Ѿ� ����
// true ����
// false �Ʊ�
void CreateBullet(int x, int y, bool enemy);

//< �Ѿ��� �̵� ó��
void ShotProcess(void);

//< ������ �̵��� ������ ó��
void EnemyAI(void);

//< �浹ó�� üũ
int CollisionEnemy(int x, int y);

//< ���� Ŭ���� üũ
// 0 : x / 1 : ���ӿ��� / 2 : ���� Ŭ����
int GameClearCheck(void);

//< ��ũ�� ���ۿ� �÷��̾� �׸���
void DrawPlayer(void);

//< ��ũ�� ���ۿ� ���� �׸���
void DrawEnemy(void);

//< ��ũ�� ���ۿ� �Ѿ� �׸���
void DrawBullet(void);

