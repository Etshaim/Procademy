#ifndef FUNC_H
#define FUNC_H

#include "Define.h"
#include "Node.h"

// 메뉴 출력
void PrintMenu();

// 메뉴 입력
int InputMenu();

// 데이터 입력
void InputData(Node **pNode);

// 패스워드 인코딩
int EncodePassword(char password[], int len);


//==================
// Menu
//==================
void Join();			// >>1. 가입
void LogIn();			// 2. 로그인		(hash)
void PrintAll();		// >>3. 전체 회원보기
void LookUpAccount();	// >>4. ID 검색	(hash)
void DeleteAccount();	// 5. 탈퇴
void DeleteAll();		// >>6. 전체 삭제

	
// ==================
// Double Linked List
// ==================

void PushFront();		// 맨 앞에 삽입
void PushBack();		// 맨 뒤에 삽입

void PrintAll();		// 전체 출력
void DeleteAll();		// 전체 삭제

// ==================
// HashTable
// ==================


void PushHash(Node* pNode);		// 해쉬 테이블에 삽입
int GetKey( char id[], int len );	// key 구하기
void LookUpHash(  );		// 해쉬 테이블에서 검색
void DeleteHash( char temp[], int len );	// 해쉬 테이블에서 삭제

#endif // FUNC_H