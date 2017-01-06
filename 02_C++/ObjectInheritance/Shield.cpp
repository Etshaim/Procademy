#include "Shield.h"
#include <iostream>

Shield::Shield()
{
	InputData();
	std::cout << "+ Shield ���� ";
	Print();
}

Shield::~Shield()
{
	std::cout << "- Shield ���� ";
	Print();
}

void Shield::InputData()
{
	std::cout << "Shield ������ Type:";
	std::cin >> _type;

	std::cout << "Shield ������ Price:";
	std::cin >> _price;

	std::cout << "Shield ������ Defense:";
	std::cin >> _defense;
}


void Shield::Print()
{
	std::cout << "# Shield ���� ";
	std::cout << "(�̸�:" << _name << " / ��ǥ:" << _x << "." << _y
		<< " / Type:" << _type << " / Price:" << _price << " / Defense:" << _defense << ")" << std::endl;
}