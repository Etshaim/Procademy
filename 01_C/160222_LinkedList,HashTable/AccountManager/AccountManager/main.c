#include "Func.h"

void main(void)
{
	while (1)
	{
		system("cls");
		PrintMenu();

		switch (InputMenu())
		{
			case 1:	// 가입
			{
				Join();
			}
			break;

			case 2:	// 로그인
			{
				LogIn();
			}
			break;

			case 3: // 전체 회원보기
			{
				PrintAll();
			}
			break;

			case 4:	// ID 검색
			{
				LookUpHash();
			}
			break;

			case 5:	// 탈퇴
			{
				DeleteAccount();
			}
			break;

			case 6:	// 전체 삭제
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