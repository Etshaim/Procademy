#include <stdio.h>

void main(void)
{
	unsigned int data = 0;	// 본 데이터
	unsigned int temp = 0;	// 비트 반전용 임시 데이터

	int inputBit	= 0;	
	int inputNum	= 0;

	int byteSize	= 0;
	int count		= 0;

	byteSize = sizeof(data);

	while(1)
	{
		printf_s("비트위치 (1~4) :");
		scanf_s("%d",&inputBit);

		if( !( 1 <= inputBit && inputBit <= 4 ))
		{
			printf_s("비트 범위를 초과하였습니다.\n");
			continue;
		}

		printf_s("값 (0~255) :");
		scanf_s("%d", &inputNum);

		if( !( 0 <= inputNum && inputNum <= 255 ))
		{
			printf_s("값 범위를 초과하였습니다.\n");
			continue;
		}
		else
		{
			temp = ~( 255 << ((inputBit - 1) * 8));
			data = data & temp;

			data = data |  ( inputNum << ((inputBit - 1) * 8));
		} 
		
		//< 비트 수 만큼 반복 출력
		for ( count = 0; count < byteSize; ++count)
		{
			temp = data << (count * 8);
			temp = temp >> ((byteSize - 1) * 8);

			printf_s("%d번 째 byte : ", 4 - count );
			printf_s("%d\n",temp);
		}

		printf_s("\n");
		printf_s("%08x\n", data);
		printf_s("\n");
	}
}