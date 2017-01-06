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

void CGameManager::BufferFlip(void)	// 버퍼의 내용을 화면으로 
{
	int count = 0;

	for (count = 0; count < dfSCREEN_HEIGHT; ++count)
	{
		cs_MoveCursor(0, count);
		std::cout << screenBuffer[count];
	}
}

void CGameManager::BufferClear(void)	// 화면 버퍼를 지워주는 함수
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
				

				// 게임 시작, 종료 선택
				std::cout << "== INVADER == " << std::endl;
				std::cout << "1. 게임 시작" << std::endl;
				std::cout << "2. 게임 종료" << std::endl;
				std::cout << ">> ";

				int menuNum = 0;

				std::cin >> menuNum;

				switch (menuNum)
				{
				case 1:
				{
					

					_state = PLAYING;

					std::cout << "게임을 시작합니다." << std::endl;

					system("pause");
				}
				break;

				case 2:
				{

					std::cout << "게임을 종료합니다" << std::endl;
					
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
					std::cout << "잘 못 입력하셨습니다." << std::endl;

					system("pause");

					system("cls");
				}
				break;
				}
			}
			break;

			case PLAYING:
			{
				// 오브젝트 초기화( 위치, 개수 등)
				CObjectManager* objectManager = CObjectManager::GetInstance();

				objectManager->Initialization();

				while (1)
				{
					// 적을 다 없애거나 플레이어가 죽을 때까지 진행
					// 적을 다 없앤거는 점수로 확인

					// 액션
					CDoubleLinkedList<CObject*>::ITERATOR iterator;

					for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
					{
						(*iterator)->Action();
					}

					// 버퍼 지우기
					BufferClear();

					// draw
					for (iterator = objectList.Begin(); iterator != objectList.End(); ++iterator)
					{
						(*iterator)->Draw();
					}

					// 화면 표시
					BufferFlip();

					// 게임 종료 체크
					if (GameClearCheck())
					{
						// 게임 클리어 변수
						_gameClear = true;

						// 게임 상태
						_state = ENDING;
					}

					// 슬립
					Sleep(50);
				}
			}
			break;

			case ENDING:
			{
				// 클리어 여부 판단해서 클리어 또는 실패 출력
				if (true == _gameClear)
				{
					std::cout << "CLEAR" << std::endl;
				}
				else
				{
					std::cout << "FAILED" << std::endl;
				}

				system("pause");

				// STARTING으로 돌아감
			}
			break;
		}

		
	}
}