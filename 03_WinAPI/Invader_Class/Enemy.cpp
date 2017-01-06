#include "Enemy.h"
#include "ObjectManager.h"
#include <Windows.h>

CEnemy::CEnemy(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible)
	: _direction(true), _createX(posX), CObject(objectType, posX, posY, visible)
{

}

CEnemy::~CEnemy()
{

}

//void CEnemy::Initialization(int posX, int posY)
//{
//	// ��ġ ����
//	SetPos(posX, posY);
//
//	// ���� x��ǥ
//	_createX = posX;
//
//	// Ȱ��ȭ
//	_visible = true;
//
//	// �̵� ����
//	_direction = true;
//}

void CEnemy::Draw(void)
{
	if (_visible)
	{
		CConsole::GetInstance()->screenBuffer[_posY][_posX] = '@';
		//SpriteDraw(_posX, _posY, '@');
	}
}

bool CEnemy::Action(void)
{
	if (!_visible)
	{
		return false;
	}

	else
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
		if ((rand() % 100 + 1) > 98)
		{
			CreateBullet();
			//CObjectManager::GetInstance()->CreateBullet(CObject::OBJECT_TYPE::ENEMY_BULLET, _posX, _posY, true, true);
			//CreateBullet(g_Enemy[count].x, g_Enemy[count].y, true);
		}

		// ���� Ȯ��
		// ���� Ȯ��
		// �浹 ������
		if (CollisionCheck() == true)
		{
			return false;
		}
		// �浹���� �ʾ�����
		else
		{
			return true;
		}

	}
}

void CEnemy::CreateBullet()
{
	CObjectManager::GetInstance()->CreateBullet(CObject::OBJECT_TYPE::ENEMY_BULLET, _posX, _posY, true, true);
}

bool CEnemy::CollisionCheck(void)
{
	CObjectManager *objectManager = CObjectManager::GetInstance();

	CDoubleLinkedList<CObject*>::ITERATOR iterator;

	for (iterator = objectManager->_objectList.Begin(); iterator != objectManager->_objectList.End(); ++iterator)
	{
		if ((*iterator)->CheckObjectType() == CObject::OBJECT_TYPE::PLAYER_BULLET)
		{
			if ((*iterator)->GetPosX() == _posX && (*iterator)->GetPosY() == _posY)
			{
				_visible = false;

				return true;
			}
		}
	}
	
	return false;
}