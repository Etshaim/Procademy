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
	// visible�� false�̸� ���� ����
	if (!_visible)
	{
		return false;
	}

	// ���� �Ѿ� 
	if (_fromEnemy == true)
	{
		// �Ʒ��� �̵�
		_posY++;
	}

	// �÷��̾� �Ѿ�
	else
	{
		// ���� �̵�
		_posY--;
	}

	// �Ѿ� ���� üũ
	// ������ ���� ����� �����
	if (_posX < 0 || _posY < 0 || _posX >= dfSCREEN_WIDTH - 1 || _posY >= dfSCREEN_HEIGHT - 1)
	{
		_visible = false;

		return false;
	}

	return true;
}

