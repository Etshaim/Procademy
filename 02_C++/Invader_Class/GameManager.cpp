#include "GameManager.h"
#include <iostream>
#include <Windows.h>


CGameManager::CGameManager()
{
	_scene[STARTING] = (CStage*)(new CGameStarting());
		//_scene[STARTING] = new
		//_scene[STARTING] = new
}

CGameManager::~CGameManager()
{

}

bool CGameManager::GameClearCheck()
{
	if (_point >= dfENEMY_MAX)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CGameManager::BufferFlip(void)	// ������ ������ ȭ������ 
{
	int count = 0;

	for (count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		cs_MoveCursor(0, count);
		std::cout << screenBuffer[count];
	}
}

void CGameManager::BufferClear(void)	// ȭ�� ���۸� �����ִ� �Լ�
{
	int count = 0;

	for (count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		memset(screenBuffer[count], ' ', dfSCREEN_WIDTH);
		screenBuffer[count][dfSCREEN_WIDTH - 1] = (char)NULL;
	}
}


void CGameManager::GameLoop()
{
	while (1)
	{
		switch (_state)
		{
			case STARTING:
			{
				

				// ���� ����, ���� ����
				std::cout << "== INVADER == " << std::endl;
				std::cout << "1. ���� ����" << std::endl;
				std::cout << "2. ���� ����" << std::endl;
				std::cout << ">> ";

				int menuNum = 0;

				std::cin >> menuNum;

				switch (menuNum)
				{
				case 1:
				{
					

					_state = PLAYING;

					std::cout << "������ �����մϴ�." << std::endl;

					system("pause");
				}
				break;

				case 2:
				{

					std::cout << "������ �����մϴ�" << std::endl;
					
					//system("pause");
					//do 
					//{
					//	//std::cout << std::endl << "Press the Enter key to continue";
					//} while (std::cin.get() != '\n');

					return;

				}
				break;

				default:
				{
					std::cout << "�� �� �Է��ϼ̽��ϴ�." << std::endl;

					system("pause");

					system("cls");
				}
				break;
				}
			}
			break;

			case PLAYING:
			{
				// ������Ʈ �ʱ�ȭ( ��ġ, ���� ��)
				CObjectManager* objectManager = CObjectManager::GetInstance();

				objectManager->Initialization();

				while (1)
				{
					// ���� �� ���ְų� �÷��̾ ���� ������ ����
					// ���� �� ���ذŴ� ������ Ȯ��

					// �׼�
					CDoubleLinkedList<CObject*>::ITERATOR iterator;

					for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
					{
						(*iterator)->Action();
					}

					// ���� �����
					BufferClear();

					// draw
					for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
					{
						(*iterator)->Draw();
					}

					// ȭ�� ǥ��
					BufferFlip();

					// ���� ���� üũ
					if (GameClearCheck())
					{
						// ���� Ŭ���� ����
						_gameClear = true;

						// ���� ����
						_state = ENDING;
					}

					// ����
					Sleep(50);
				}
			}
			break;

			case ENDING:
			{
				// Ŭ���� ���� �Ǵ��ؼ� Ŭ���� �Ǵ� ���� ���
				if (true == _gameClear)
				{
					std::cout << "CLEAR" << std::endl;
				}
				else
				{
					std::cout << "FAILED" << std::endl;
				}

				system("pause");

				// STARTING���� ���ư�
			}
			break;
		}

		
	}
}