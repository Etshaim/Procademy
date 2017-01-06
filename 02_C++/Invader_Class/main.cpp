#include <iostream>
#include "GameManager.h"

void main(void)
{
	CGameManager *gameManager = CGameManager::GetInstance();

	gameManager->GameLoop();
}