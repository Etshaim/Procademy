#include "Fly.h"
#include <iostream>

Fly::Fly()
{
	InputData();
	std::cout << "+ Fly 생성 ";
	Print();
}

Fly::~Fly()
{
	std::cout << "- Fly 삭제 ";
	Print();
}

void Fly::InputData()
{
	std::cout << "Fly 데이터 HP:";
	std::cin >> _HP;

	std::cout << "Fly 데이터 Level:";
	std::cin >> _level;

	std::cout << "Fly 데이터 Speed:";
	std::cin >> _speed;

	std::cout << "Fly 데이터 Height:";
	std::cin >> _height;
}


void Fly::Print()
{
	std::cout << "# Fly 정보 ";
	std::cout << "(이름:" << _name << " / 좌표:" << _x << "." << _y
		<< " / HP:" << _HP << " / Level:" << _level << " / Speed:" << _speed << " / Height::" << _height << ")" << std::endl;
}