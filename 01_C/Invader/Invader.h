#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <Windows.h>
#include <stdbool.h>
#include "Console.h"

#define	dfENEMY_MAX		30		// 적군의 최대치
#define dfSHOT_MAX		50		// 총알의 최대치
char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

// 플레이어 구조체
typedef struct PLAYER
{
	int HP;

	int x;
	int y;

	bool visible;		// 생존 여부

}Player;



// 적 구조체
typedef struct ENEMY
{
	int HP;
	
	int x;
	int y;

	int createX;		// 최초 생성위치(x좌표)
	bool direction;		// 이동 방향
	bool visible;		// 생존 여부

}Enemy;

// 총알 구조체
typedef struct BULLET
{
	int x;
	int y;

	bool visible;	// 사용중인지 여부
	bool enemy;		// 적군의 총알인지 아군의 총알인지

}Bullet;

// 플레이어 인스턴스
Player g_Player;

// 적군 인스턴스
Enemy	g_Enemy[dfENEMY_MAX];

// 총알 최대치 인스턴스
Bullet	g_Bullet[dfSHOT_MAX];

//< 초기화
void Initial(void);

//< 버퍼의 내용을 화면으로 찍어주는 함수
void BufferFlip(void);

//< 화면 버퍼를 지워주는 함수
void BufferClear(void);

//< 버퍼의 특정 위치에 원하는 문자를 출력
void SpriteDraw(int x, int y, char sprite);

//< 키보드 입력
bool KeyProcess(void);

//< 특정 위치에  총알 생성
// true 적군
// false 아군
void CreateBullet(int x, int y, bool enemy);

//< 총알의 이동 처리
void ShotProcess(void);

//< 적군의 이동과 공격을 처리
void EnemyAI(void);

//< 충돌처리 체크
int CollisionEnemy(int x, int y);

//< 게임 클리어 체크
// 0 : x / 1 : 게임오버 / 2 : 게임 클리어
int GameClearCheck(void);

//< 스크린 버퍼에 플레이어 그리기
void DrawPlayer(void);

//< 스크린 버퍼에 적군 그리기
void DrawEnemy(void);

//< 스크린 버퍼에 총알 그리기
void DrawBullet(void);

