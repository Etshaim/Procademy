#include "Common.h"
#include <iostream>


My_Object::My_Object()
{
	InputData();
}

My_Object::~My_Object()
{
	std::cout << "Baseobject �ı� " << std::endl;
}

void My_Object::InputData()
{
	std::cout << "���� ������ X:";
	std::cin >> _x;

	std::cout << "���� ������ y:";
	std::cin >> _y;

	std::cout << "���� ������ Name:";

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