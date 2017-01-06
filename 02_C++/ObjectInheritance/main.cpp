#include <iostream>
#include "Process.h"

void main()
{
	Init();

	while ( Process() )
	{
		std::cout << std::endl << std::endl;

		system("pause");
		system("cls");
	}

	deleteAll();
}