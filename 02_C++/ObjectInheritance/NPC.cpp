#include "NPC.h"
#include <iostream>

NPC::NPC()
{
	InputData();
	std::cout << "+ NPC ���� ";
	Print();
}

NPC::~NPC()
{
	std::cout << "- NPC ���� ";
	Print();
}

void NPC::InputData()
{
	std::cout << "NPC ������ Type 1. ����  2. ��� 3. ���� �ֹ�" << std::endl << "->";

	std::cin >> _type;
}


void NPC::Print()
{
	std::cout << "# NPC ���� ";
	std::cout << "(�̸�:" << _name << " / ��ǥ:" << _x << "." << _y
		<< " / Type:" << _type << ")" << std::endl;
}