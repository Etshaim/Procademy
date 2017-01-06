#include <stdio.h>

int iData = 10;
int iNum = 0;

void func();

void main(void)
{
	iData = 0;
	iNum = 10;

	func();
}

void func()
{
	for (int iCount = 0; iCount < 10; iCount++)
	{
		printf("%d\n", iData + iCount);
	}
	
}