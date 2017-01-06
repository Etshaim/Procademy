#include "Sword.h"
#include <iostream>

Sword::Sword()
{
	InputData();
	std::cout << "+ Sword 생성 ";
	Print();
}

Sword::~Sword()
{
	std::cout << "- Sword 삭제 ";
	Print();
}

void Sword::InputData()
{
	std::cout << "Sword 데이터 Type:";
	std::cin >> _type;

	std::cout << "Sword 데이터 Price:";
	std::cin >> _price;

	std::cout << "Sword 데이터 Attack:";
	std::cin >> _attack;
}


void Sword::Print()
{
	std::cout << "# Sword 정보 ";
	std::cout << "(이름:" << _name << " / 좌표:" << _x << "." << _y
		<< " / Type:" << _type << " / Price:" << _price << " / Attack:" << _attack << ")" << std::endl;
}