#include "Tiger.h"
#include <iostream>

Tiger::Tiger()
{
	InputData();
	std::cout << "+ Tiger 생성 ";
	Print();
}

Tiger::~Tiger()
{
	std::cout << "- Tiger 삭제 ";
	Print();
}

void Tiger::InputData()
{
	std::cout << "Tiger 데이터 HP:";
	std::cin >> _HP;

	std::cout << "Tiger 데이터 Level:";
	std::cin >> _level;

	std::cout << "Tiger 데이터 Speed:";
	std::cin >> _speed;

	
}


void Tiger::Print()
{
	std::cout << "# Tiger 정보 ";
	std::cout << "(이름:" << _name << " / 좌표:" << _x << "." << _y
		<< " / HP:" << _HP << " / Level:" << _level << " / Speed:" << _speed <<  ")" << std::endl;
}