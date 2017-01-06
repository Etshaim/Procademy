#include <stdio.h>
#include <math.h>
#include <Windows.h>

void main(void)
{
	int		angle = 0;
	double	radian = 0;
	int		temp = 0;
	int		i = 0;

	for (angle = 0; angle < 360 * 10; angle += 15)
	{
		radian = angle * 3.14 / 180;
		temp = (int)(sin(radian) * 20);

		printf_s("%d", temp);

		for (i = 0; i < 20 + temp; ++i)
		{
			printf_s(" ");
		}

		printf_s("*\n");
		Sleep(100);
	}
}