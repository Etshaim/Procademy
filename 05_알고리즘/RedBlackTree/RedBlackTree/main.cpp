#include "RedBlackTree.h"
#include <stdio.h>

void main(void)
{
	CRedBlackTree Tree;

	
	Tree.InsertNode(11);

	Tree.InsertNode(10);

	Tree.InsertNode(12);
	Tree.InsertNode(13);
	Tree.InsertNode(14);
	Tree.InsertNode(15);
	Tree.InsertNode(16);
	Tree.InsertNode(17);

	Tree.InsertNode(9);
	Tree.InsertNode(8);
	Tree.InsertNode(7);
	Tree.InsertNode(6);
	Tree.InsertNode(5);


	Tree.InsertNode(5);
	Tree.InsertNode(6);
	Tree.InsertNode(3);
	Tree.InsertNode(9);
	Tree.InsertNode(100);
	Tree.InsertNode(20);
	Tree.InsertNode(1);
	Tree.InsertNode(2);
	Tree.InsertNode(4);
	Tree.InsertNode(7);
	Tree.InsertNode(8);
	Tree.InsertNode(111);

	Tree.InsertNode(112);

	Tree.InsertNode(113);

	Tree.InsertNode(114);

	Tree.InsertNode(101);

	Tree.InsertNode(24);

	Tree.InsertNode(23);

	Tree.InsertNode(22);

	Tree.InsertNode(25);

	Tree.InsertNode(26);


	//Tree.PreorderPrint();	// 전위순회 출력

	Tree.InorderPrint();	// 중위순회 출력

	//Tree.PostorderPrint();	// 후위순회 출력
}