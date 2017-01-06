#include "Fly.h"
#include <iostream>

Fly::Fly()
{
	InputData();
	std::cout << "+ Fly ���� ";
	Print();
}

Fly::~Fly()
{
	std::cout << "- Fly ���� ";
	Print();
}

void Fly::InputData()
{
	std::cout << "Fly ������ HP:";
	std::cin >> _HP;

	std::cout << "Fly ������ Level:";
	std::cin >> _level;

	std::cout << "Fly ������ Speed:";
	std::cin >> _speed;

	std::cout << "Fly ������ Height:";
	std::cin >> _height;
}


void Fly::Print()
{
	std::cout << "# Fly ���� ";
	std::cout << "(�̸�:" << _name << " / ��ǥ:" << _x << "." << _y
		<< " / HP:" << _HP << " / Level:" << _level << " / Speed:" << _speed << " / Height::" << _height << ")" << std::endl;
}