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
//	// 위치 지정
//	SetPos(posX, posY);
//
//	// 시작 x좌표
//	_createX = posX;
//
//	// 활성화
//	_visible = true;
//
//	// 이동 방향
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
		if ((rand() % 100 + 1) > 98)
		{
			CreateBullet();
			//CObjectManager::GetInstance()->CreateBullet(CObject::OBJECT_TYPE::ENEMY_BULLET, _posX, _posY, true, true);
			//CreateBullet(g_Enemy[count].x, g_Enemy[count].y, true);
		}

		// 생존 확인
		// 생존 확인
		// 충돌 했으면
		if (CollisionCheck() == true)
		{
			return false;
		}
		// 충돌하지 않았으면
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