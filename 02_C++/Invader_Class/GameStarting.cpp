#include "GameStarting.h"


CGameStarting::CGameStarting()
{
}


CGameStarting::~CGameStarting()
{
}


void CGameStarting::init()
{
}

void CGameStarting::run()
{
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


		CGameManager::GetInstance()->_state = CGameManager::PLAYING;

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

void CGameStarting::running()
{
	init();
	run();
}