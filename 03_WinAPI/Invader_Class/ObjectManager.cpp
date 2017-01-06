#include "ObjectManager.h"
#include "DoubleLinkedList_Template.h"

CObjectManager::CObjectManager()
{
	
}

CObjectManager::~CObjectManager()
{

}

// 플레이어 생성
bool CObjectManager::CreatePlayer(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible)
{
	CPlayer *player = new CPlayer(objectType, posX, posY, visible);
	
	_objectList.PushBack(player);

	return true;
}

// 적 생성
bool CObjectManager::CreateEnemy(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible)
{
	CEnemy *enemy = new CEnemy(objectType, posX, posY, visible);

	_objectList.PushBack(enemy);

	return true;
}

// 총알 생성
bool CObjectManager::CreateBullet(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible, bool fromEnemy)
{
	CBullet *bullet = new CBullet(objectType, posX, posY, visible, fromEnemy);

	_objectList.PushBack(bullet);

	return true;
}
//
//void CObjectManager::Initialization(void)
//{
//	CGameManager *gameManager = CGameManager::GetInstance();
//
//	// 플레이어 초기화
//	player = CPlayer::GetInstance();
//
//	player->Initialization(39, 19);
//
//	gameManager->objectList.PushBack((CObject*)player);
//
//	// enemy 초기화
//
//	int posX = 17;
//	int posY = 1;
//
//	for (int count = 0; count < dfENEMY_MAX; ++count)
//	{
//		if (count % 10 == 0)
//		{
//			posX = 17;
//			posY = posY + 2;
//		}
//
//		enemies[count].Initialization(posX, posY);
//
//		posX += 5;
//	}
//
//	// 총알 초기화
//	for (int count = 0; count < dfSHOT_MAX; ++count)
//	{
//		bullets[count].Initialization(0, 0);
//	}
//}



