#include <stdio.h>
#include <tchar.h>
#include "Tree.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CTree<int> Tree;

	Tree.CreateNode(0, 0);
	Tree.CreateNode(1, 0);
	Tree.CreateNode(2, 0);
	Tree.CreateNode(3, 0);

	Tree.CreateNode(10, 1);
	Tree.CreateNode(11, 1);
	Tree.CreateNode(12, 1);

	Tree.CreateNode(20, 2);
	Tree.CreateNode(21, 2);
	Tree.CreateNode(22, 2);

	Tree.CreateNode(30, 3);
	Tree.CreateNode(31, 3);
	Tree.CreateNode(32, 3);

	Tree.Print();

	return 0;
}