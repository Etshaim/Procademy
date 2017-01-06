#include "Enemy.h"
#include <Windows.h>

CEnemy::CEnemy()
{

}

CEnemy::~CEnemy()
{

}

void CEnemy::Initialization(int posX, int posY)
{
	// 위치 지정
	SetPos(posX, posY);

	// 시작 x좌표
	_createX = posX;

	// 활성화
	_visible = true;

	// 이동 방향
	_direction = true;
}

void CEnemy::Draw(void)
{
	if (_visible)
	{
		SpriteDraw(_posX, _posY, '@');
	}
}

void CEnemy::Action(void)
{
	if (_visible)
	{
		// 적군 이동 처리
		// false면 왼쪽 이동, true면 오른쪽 이동
		if (true == _direction)
		{
			_posX++;
		}
		else
		{
			_posX--;
		}

		// 생성 위치 기준으로 10칸 이상 이동하면 방향 전환
		if (abs(_createX - _posX) >= 10)
		{
			_direction = !_direction;
		}

		// 적군 공격처리, 1% 확률로 총알을 쏨
		if ((rand() % 100 + 1) > 99)
		{
			//CreateBullet(g_Enemy[count].x, g_Enemy[count].y, true);
		}

	}
}