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

// �ʱ�ȭ
void Init(void)
{
	
}

// �޴� ���
void PrintMenu(void)
{
	std::cout << "1.����" << std::endl;
	std::cout << "2.����" << std::endl;
	std::cout << "3.���" << std::endl;
	std::cout << "0.����" << std::endl;
	std::cout << "->";
}

// �޴� �Է�
int InputMenu(void)
{
	int num = 0;

	std::cin >> num;

	return num;
}

// ���μ���
bool Process(void)
{
	PrintMenu();

	switch (InputMenu())
	{
	case 1:	// ����
		CreateObj();
		++count;
		break;

	case 2:	// ����
		DeleteObj();
		break;

	case 3:	// ���
		PrintAll();
		break;

	case 0: // ����
		std::cout << "���α׷��� �����մϴ�." << std::endl;
		return false;

	default:
		std::cout << "error:Process" << std::endl;
		break;
	}

	return true;
}

// ����
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

// ������Ʈ ���� �Է�
int InputObjectNum(void)
{
	int objectNum = 0;

	std::cout << "���ο� ������Ʈ ����" << std::endl;
	std::cout << "1.Player / 2.NPC / 3.Fly / 4.Tiger / 5.Sword / 6.Shiled " << std::endl;
	std::cout << "->";
	std::cin >> objectNum;

	return objectNum;
}

// ����
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

// ��ü ���
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

// ����
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