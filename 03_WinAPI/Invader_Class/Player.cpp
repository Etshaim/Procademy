#include "Player.h"
#include "ObjectManager.h"
#include "DoubleLinkedList_Template.h"
#include "Object.h"
#include "GameManager.h"

CPlayer::CPlayer(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible = true)
	: CObject(objectType, posX, posY, visible)
{
	
}

CPlayer::~CPlayer()
{

}


void CPlayer::Draw(void)
{
	if (_visible)
	{
		CConsole::GetInstance()->screenBuffer[_posY][_posX] = '#';
		//SpriteDraw(_posX, _posY, '#');
	}
}

bool CPlayer::Action(void)
{
	// 예외처리. visible이 false이면 죽은 상태
	if (!_visible)
	{
		return false;
	}

	// 키 입력 처리
	if (!(KeyProcess()))
	{
		return false;
	}

	// 생존 확인
	// 충돌 했으면
	if ( true == CollisionCheck() )
	{
		// 게임 오버 상태로 설정
		CGameManager::GetInstance()->SetState(CGameManager::eSTATE::GAMEOVER);

		return false;
	}
	// 충돌하지 않았으면
	else
	{
		return true;
	}
	
}

void CPlayer::CreateBullet()
{
	CObjectManager::GetInstance()->CreateBullet(CObject::OBJECT_TYPE::PLAYER_BULLET, _posX, _posY, true, false);
}


bool CPlayer::KeyProcess(void)
{
	// 왼쪽 방향키
	if (GetAsyncKeyState(VK_LEFT) & 0X8000)
	{
		_posX--;
	}

	// 오른쪽 방향키
	if (GetAsyncKeyState(VK_RIGHT) & 0X8000)
	{
		_posX++;
	}

	// 위쪽 방향키
	if (GetAsyncKeyState(VK_UP) & 0X8000)
	{
		_posY--;
	}

	// 아래쪽 방향키
	if (GetAsyncKeyState(VK_DOWN) & 0X8000)
	{
		_posY++;
	}

	_posX = max(_posX, 4);
	_posX = min(_posX, dfSCREEN_WIDTH - 4);

	_posY = max(_posY, 15);
	_posY = min(_posY, dfSCREEN_HEIGHT - 1);

	// 미사일 발사
	if (GetAsyncKeyState(VK_SPACE) & 0X8000)
	{
		CreateBullet();
	}

	// 종료 esc
	if (GetAsyncKeyState(VK_ESCAPE) & 0X8000)
	{
		return false;
	}

	return true;
}

bool CPlayer::CollisionCheck(void)
{
	CObjectManager *objectManager = CObjectManager::GetInstance();

	CDoubleLinkedList<CObject*>::ITERATOR iterator;

	for (iterator = objectManager->_objectList.Begin(); iterator != objectManager->_objectList.End(); ++iterator)
	{
		if ((*iterator)->CheckObjectType() == CObject::OBJECT_TYPE::ENEMY_BULLET)
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