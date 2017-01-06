#ifndef ACCOUTMANAGER_H
#define ACCOUTMANAGER_H

#include <stdio.h>
#include <Windows.h>

typedef struct NODE
{
	int data;			// 데이터
	struct NODE *pPrev;	// 전 노드
	struct NODE *pNext;	// 앞 노드
}Node;

// 메뉴 출력
void PrintMenu();

// 메뉴 입력
int InputMenu();

// 데이터 입력
int Inputdata();

// 초기화
void InitNode();

// 맨 앞에 삽입
void PushFront(int data);

// 맨 뒤에 삽입
void PushBack(int data);

// 전체 출력
void PrintAll();

// 검색
void LookUpdata(int data);

// 선택 삭제
void Deletedata(int data);

// 전체 삭제
void DeleteAll();

#endif // ACCOUTMANAGER_H

