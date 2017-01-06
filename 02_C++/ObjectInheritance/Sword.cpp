#include "Sword.h"
#include <iostream>

Sword::Sword()
{
	InputData();
	std::cout << "+ Sword ���� ";
	Print();
}

Sword::~Sword()
{
	std::cout << "- Sword ���� ";
	Print();
}

void Sword::InputData()
{
	std::cout << "Sword ������ Type:";
	std::cin >> _type;

	std::cout << "Sword ������ Price:";
	std::cin >> _price;

	std::cout << "Sword ������ Attack:";
	std::cin >> _attack;
}


void Sword::Print()
{
	std::cout << "# Sword ���� ";
	std::cout << "(�̸�:" << _name << " / ��ǥ:" << _x << "." << _y
		<< " / Type:" << _type << " / Price:" << _price << " / Attack:" << _attack << ")" << std::endl;
}