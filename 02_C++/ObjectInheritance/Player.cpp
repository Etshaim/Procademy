#include "Player.h"
#include <iostream>

Player::Player()
{
	InputData();
	std::cout << "+ Player 생성 ";
	Print();
}

Player::~Player()
{
	std::cout << "- Player 삭제 ";
	Print();
}

void Player::InputData()
{
	std::cout << "Player 데이터 HP:";
	std::cin >> _HP;

	std::cout << "Player 데이터 Money:";
	std::cin >> _money;

	std::cout << "Player 데이터 Level:";
	std::cin >> _level;
}


void Player::Print()
{
	std::cout << "# Player 정보 ";
	std::cout << "(이름:" << _name << " / 좌표:" << _x << "." << _y
		<< " / HP:" << _HP << " / Money::" << _money << " / Level:" << _level << ")" << std::endl;
}