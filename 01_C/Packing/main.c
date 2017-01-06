#include "Packing.h"

void main(void)
{
	// 메뉴 출력
	PrintMenu();

	// 메뉴 입력
	switch (InputMenu())
	{
	case PACKING:
		PackingProcess();
		//printf_s("패킹하기\n");
		break;

	case UN_PACKING:
		UnPackingProcess();
		//printf_s("패킹풀기\n");
		break;

	default:
		printf_s("error");
		break;
	}
}