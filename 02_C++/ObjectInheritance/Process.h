#ifndef PROCESS_H_
#define PROCESS_H_

#include "Common.h"
#include "Player.h"
#include "NPC.h"
#include "Fly.h"
#include "Tiger.h"
#include "Sword.h"
#include "Shield.h"

// �ʱ�ȭ
void Init(void);

// �޴� ���
void PrintMenu(void);

// �޴� �Է�
int InputMenu(void);

// ���μ���
bool Process(void);

// ����
void CreateObj(void);

// ����
void DeleteObj(void);

// ������Ʈ ���� �Է�
int InputObjectNum(void);

// ��ü ���
void PrintAll(void);

// ����
void deleteAll(void);

#endif//PROCESS_H_