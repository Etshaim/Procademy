#include "Player.h"
#include <iostream>

Player::Player()
{
	InputData();
	std::cout << "+ Player ���� ";
	Print();
}

Player::~Player()
{
	std::cout << "- Player ���� ";
	Print();
}

void Player::InputData()
{
	std::cout << "Player ������ HP:";
	std::cin >> _HP;

	std::cout << "Player ������ Money:";
	std::cin >> _money;

	std::cout << "Player ������ Level:";
	std::cin >> _level;
}


void Player::Print()
{
	std::cout << "# Player ���� ";
	std::cout << "(�̸�:" << _name << " / ��ǥ:" << _x << "." << _y
		<< " / HP:" << _HP << " / Money::" << _money << " / Level:" << _level << ")" << std::endl;
}