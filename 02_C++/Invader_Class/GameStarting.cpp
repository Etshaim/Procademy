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
	std::cout << "1. ���� ����" << std::endl;
	std::cout << "2. ���� ����" << std::endl;
	std::cout << ">> ";

	int menuNum = 0;

	std::cin >> menuNum;

	switch (menuNum)
	{
	case 1:
	{


		CGameManager::GetInstance()->_state = CGameManager::PLAYING;

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

void CGameStarting::running()
{
	init();
	run();
}