#include <stdio.h>

void main(void)
{
	unsigned int data = 0;	// �� ������
	unsigned int temp = 0;	// ��Ʈ ������ �ӽ� ������

	int inputBit	= 0;	
	int inputNum	= 0;

	int byteSize	= 0;
	int count		= 0;

	byteSize = sizeof(data);

	while(1)
	{
		printf_s("��Ʈ��ġ (1~4) :");
		scanf_s("%d",&inputBit);

		if( !( 1 <= inputBit && inputBit <= 4 ))
		{
			printf_s("��Ʈ ������ �ʰ��Ͽ����ϴ�.\n");
			continue;
		}

		printf_s("�� (0~255) :");
		scanf_s("%d", &inputNum);

		if( !( 0 <= inputNum && inputNum <= 255 ))
		{
			printf_s("�� ������ �ʰ��Ͽ����ϴ�.\n");
			continue;
		}
		else
		{
			temp = ~( 255 << ((inputBit - 1) * 8));
			data = data & temp;

			data = data |  ( inputNum << ((inputBit - 1) * 8));
		} 
		
		//< ��Ʈ �� ��ŭ �ݺ� ���
		for ( count = 0; count < byteSize; ++count)
		{
			temp = data << (count * 8);
			temp = temp >> ((byteSize - 1) * 8);

			printf_s("%d�� ° byte : ", 4 - count );
			printf_s("%d\n",temp);
		}

		printf_s("\n");
		printf_s("%08x\n", data);
		printf_s("\n");
	}
}