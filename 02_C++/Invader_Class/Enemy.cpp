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
	// ��ġ ����
	SetPos(posX, posY);

	// ���� x��ǥ
	_createX = posX;

	// Ȱ��ȭ
	_visible = true;

	// �̵� ����
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
		// ���� �̵� ó��
		// false�� ���� �̵�, true�� ������ �̵�
		if (true == _direction)
		{
			_posX++;
		}
		else
		{
			_posX--;
		}

		// ���� ��ġ �������� 10ĭ �̻� �̵��ϸ� ���� ��ȯ
		if (abs(_createX - _posX) >= 10)
		{
			_direction = !_direction;
		}

		// ���� ����ó��, 1% Ȯ���� �Ѿ��� ��
		if ((rand() % 100 + 1) > 99)
		{
			//CreateBullet(g_Enemy[count].x, g_Enemy[count].y, true);
		}

	}
}