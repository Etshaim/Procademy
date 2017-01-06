#include "Bullet.h"
#include "ObjectManager.h"

CBullet::CBullet(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible, bool fromEnemy)
	: _fromEnemy(fromEnemy), CObject(objectType, posX, posY, visible)
{
	
}

CBullet::~CBullet()
{

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
			shot = 'A';
		}
		CConsole::GetInstance()->screenBuffer[_posY][_posX] = shot;
	}
}

bool CBullet::Action(void)
{
	// visible이 false이면 죽은 상태
	if (!_visible)
	{
		return false;
	}

	// 적군 총알 
	if (_fromEnemy == true)
	{
		// 아래로 이동
		_posY++;
	}

	// 플레이어 총알
	else
	{
		// 위로 이동
		_posY--;
	}

	// 총알 범위 체크
	// 정해진 구역 벗어나면 지우기
	if (_posX < 0 || _posY < 0 || _posX >= dfSCREEN_WIDTH - 1 || _posY >= dfSCREEN_HEIGHT - 1)
	{
		_visible = false;

		return false;
	}

	return true;
}

