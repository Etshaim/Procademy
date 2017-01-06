#include "Common.h"
#include "SceneManager.h"

void main(void)
{
	CSceneManager *pSceneManager = CSceneManager::GetInstance();

	while (true)
	{
		if ( !pSceneManager->Run() )
		{
			printf_s("���� ����");
			break;
		}

		Sleep(50);
	}
}