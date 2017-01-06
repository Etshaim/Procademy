#include "NPC.h"
#include <iostream>

NPC::NPC()
{
	InputData();
	std::cout << "+ NPC 생성 ";
	Print();
}

NPC::~NPC()
{
	std::cout << "- NPC 삭제 ";
	Print();
}

void NPC::InputData()
{
	std::cout << "NPC 데이터 Type 1. 상인  2. 경비병 3. 마을 주민" << std::endl << "->";

	std::cin >> _type;
}


void NPC::Print()
{
	std::cout << "# NPC 정보 ";
	std::cout << "(이름:" << _name << " / 좌표:" << _x << "." << _y
		<< " / Type:" << _type << ")" << std::endl;
}