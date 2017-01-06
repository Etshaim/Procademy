#include "Tiger.h"
#include <iostream>

Tiger::Tiger()
{
	InputData();
	std::cout << "+ Tiger ���� ";
	Print();
}

Tiger::~Tiger()
{
	std::cout << "- Tiger ���� ";
	Print();
}

void Tiger::InputData()
{
	std::cout << "Tiger ������ HP:";
	std::cin >> _HP;

	std::cout << "Tiger ������ Level:";
	std::cin >> _level;

	std::cout << "Tiger ������ Speed:";
	std::cin >> _speed;

	
}


void Tiger::Print()
{
	std::cout << "# Tiger ���� ";
	std::cout << "(�̸�:" << _name << " / ��ǥ:" << _x << "." << _y
		<< " / HP:" << _HP << " / Level:" << _level << " / Speed:" << _speed <<  ")" << std::endl;
}