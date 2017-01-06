#include "Bullet.h"

CBullet::CBullet()
{
	
}

CBullet::~CBullet()
{

}

void CBullet::Initialization(int posX, int posY)
{
	_posX = 0;
	_posY = 0;
	_visible = false;


}

void CBullet::Draw(void)
{
	char shot = '\0';

	if (_visible)
	{
		if (_fromEnemy)
		{
			shot = 'v';
		}
		else
		{
			shot = '^';
		}

		SpriteDraw(_posX, _posY, shot);
	}
}

void CBullet::Action(void)
{

}