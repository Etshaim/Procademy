#include "Shield.h"
#include <iostream>

Shield::Shield()
{
	InputData();
	std::cout << "+ Shield 생성 ";
	Print();
}

Shield::~Shield()
{
	std::cout << "- Shield 삭제 ";
	Print();
}

void Shield::InputData()
{
	std::cout << "Shield 데이터 Type:";
	std::cin >> _type;

	std::cout << "Shield 데이터 Price:";
	std::cin >> _price;

	std::cout << "Shield 데이터 Defense:";
	std::cin >> _defense;
}


void Shield::Print()
{
	std::cout << "# Shield 정보 ";
	std::cout << "(이름:" << _name << " / 좌표:" << _x << "." << _y
		<< " / Type:" << _type << " / Price:" << _price << " / Defense:" << _defense << ")" << std::endl;
}