#include "ObjectManager.h"

CObjectManager::CObjectManager()
{
	
}

CObjectManager::~CObjectManager()
{

}

void CObjectManager::Initialization(void)
{
	CGameManager *gameManager = CGameManager::GetInstance();

	// 플레이어 초기화
	player = CPlayer::GetInstance();

	player->Initialization(39, 19);

	gameManager->objectList.PushBack((CObject*)player);

	// enemy 초기화

	int posX = 17;
	int posY = 1;

	for (int count = 0; count < dfENEMY_MAX; ++count)
	{
		if (count % 10 == 0)
		{
			posX = 17;
			posY = posY + 2;
		}

		enemies[count].Initialization(posX, posY);

		posX += 5;
	}

	// 총알 초기화
	for (int count = 0; count < dfSHOT_MAX; ++count)
	{
		bullets[count].Initialization(0, 0);
	}
}

bool CObjectManager::GameClearCheck(void)
{
	player = CPlayer::GetInstance();

	return player->GetVisible();
}

bool CObjectManager::GameOverCheck(void)
{
	for (int count = 0; count < dfENEMY_MAX; ++count)
	{

	}

	return false;
}


