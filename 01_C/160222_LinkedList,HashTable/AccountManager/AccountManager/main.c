#include "Func.h"

void main(void)
{
	while (1)
	{
		system("cls");
		PrintMenu();

		switch (InputMenu())
		{
			case 1:	// ����
			{
				Join();
			}
			break;

			case 2:	// �α���
			{
				LogIn();
			}
			break;

			case 3: // ��ü ȸ������
			{
				PrintAll();
			}
			break;

			case 4:	// ID �˻�
			{
				LookUpHash();
			}
			break;

			case 5:	// Ż��
			{
				DeleteAccount();
			}
			break;

			case 6:	// ��ü ����
			{
				DeleteAll();
			}
			break;

			default:
			{
				printf_s("error");
			}
			break;
		}
		system("pause");
	}
}