#ifndef PROCESS_H_
#define PROCESS_H_

#include "Common.h"
#include "Player.h"
#include "NPC.h"
#include "Fly.h"
#include "Tiger.h"
#include "Sword.h"
#include "Shield.h"

// 초기화
void Init(void);

// 메뉴 출력
void PrintMenu(void);

// 메뉴 입력
int InputMenu(void);

// 프로세스
bool Process(void);

// 생성
void CreateObj(void);

// 삭제
void DeleteObj(void);

// 오브젝트 종류 입력
int InputObjectNum(void);

// 전체 출력
void PrintAll(void);

// 해제
void deleteAll(void);

#endif//PROCESS_H_