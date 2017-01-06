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

	// �÷��̾� �ʱ�ȭ
	player = CPlayer::GetInstance();

	player->Initialization(39, 19);

	gameManager->objectList.PushBack((CObject*)player);

	// enemy �ʱ�ȭ

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

	// �Ѿ� �ʱ�ȭ
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


