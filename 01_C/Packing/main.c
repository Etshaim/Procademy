#include "Packing.h"

void main(void)
{
	// �޴� ���
	PrintMenu();

	// �޴� �Է�
	switch (InputMenu())
	{
	case PACKING:
		PackingProcess();
		//printf_s("��ŷ�ϱ�\n");
		break;

	case UN_PACKING:
		UnPackingProcess();
		//printf_s("��ŷǮ��\n");
		break;

	default:
		printf_s("error");
		break;
	}
}