#include "Common.h"
#include <iostream>


My_Object::My_Object()
{
	InputData();
}

My_Object::~My_Object()
{
	std::cout << "Baseobject 파괴 " << std::endl;
}

void My_Object::InputData()
{
	std::cout << "공통 데이터 X:";
	std::cin >> _x;

	std::cout << "공통 데이터 y:";
	std::cin >> _y;

	std::cout << "공통 데이터 Name:";

	//std::cin.clear();
	std::cin.ignore(256, '\n');

	std::cin.getline(_name, 16, '\n');
}

bool My_Object::CheckPos(int x, int y)
{
	if (x == _x && y == _y)
	{
		return true;
	}
	return false;
}