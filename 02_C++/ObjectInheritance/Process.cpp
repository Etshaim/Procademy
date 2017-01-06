#include "Process.h"
#include <iostream>

#define MAX_OBJECT	100


enum OBJ
{
	OBJ_PLAYER = 1,
	OBJ_NPC,
	OBJ_FLY,
	OBJ_TIGER,
	OBJ_SWORD,
	OBJ_SHIELD,

};

My_Object	*objectPool[MAX_OBJECT];
int			count = 0;

// 초기화
void Init(void)
{
	
}

// 메뉴 출력
void PrintMenu(void)
{
	std::cout << "1.생성" << std::endl;
	std::cout << "2.삭제" << std::endl;
	std::cout << "3.출력" << std::endl;
	std::cout << "0.종료" << std::endl;
	std::cout << "->";
}

// 메뉴 입력
int InputMenu(void)
{
	int num = 0;

	std::cin >> num;

	return num;
}

// 프로세스
bool Process(void)
{
	PrintMenu();

	switch (InputMenu())
	{
	case 1:	// 생성
		CreateObj();
		++count;
		break;

	case 2:	// 삭제
		DeleteObj();
		break;

	case 3:	// 출력
		PrintAll();
		break;

	case 0: // 종료
		std::cout << "프로그램을 종료합니다." << std::endl;
		return false;

	default:
		std::cout << "error:Process" << std::endl;
		break;
	}

	return true;
}

// 생성
void CreateObj(void)
{
	switch (InputObjectNum())
	{
	case OBJ_PLAYER:
		{
			Player *player = new Player;
			objectPool[count] = player;
		}
		break;

	case OBJ_NPC:
	{
		NPC *npc = new NPC;
		objectPool[count] = npc;
	}
		break;
	case OBJ_FLY:
	{
		Fly *fly = new Fly;
		objectPool[count] = fly;
	}
		break;

	case OBJ_TIGER:
	{
		Tiger *tiger = new Tiger;
		objectPool[count] = tiger;
	}
		break;

	case OBJ_SWORD:
	{
		Sword *sword = new Sword;
		objectPool[count] = sword;
	}
		break;

	case OBJ_SHIELD:
	{
		Shield *shield = new Shield;
		objectPool[count] = shield;
	}
		break;

	default:
		std::cout << "error : CreateObj" << std::endl;
		--count;
		break;
	}
}

// 오브젝트 종류 입력
int InputObjectNum(void)
{
	int objectNum = 0;

	std::cout << "새로운 오브젝트 생성" << std::endl;
	std::cout << "1.Player / 2.NPC / 3.Fly / 4.Tiger / 5.Sword / 6.Shiled " << std::endl;
	std::cout << "->";
	std::cin >> objectNum;

	return objectNum;
}

// 삭제
void DeleteObj(void)
{
	int x = 0;
	int y = 0;

	std::cout << "X:";
	std::cin >> x;

	std::cout << "Y:";
	std::cin >> y;

	for (int i = 0; i < count; ++i)
	{
		if ( objectPool[i]->CheckPos(x, y) == true )
		{
			if (objectPool != NULL)
			{
				delete objectPool[i];
				objectPool[i] = NULL;
			}
		}
	}

}

// 전체 출력
void PrintAll(void)
{
	for (int i = 0; i < count; ++i)
	{
		if (objectPool[i] != NULL)
		{
			objectPool[i]->Print();
		}
	}
}

// 해제
void deleteAll(void)
{
	for (int i = 0; i < count; ++i)
	{
		if (objectPool[i] != NULL)
		{
			delete objectPool[i];
		}
		
	}
}