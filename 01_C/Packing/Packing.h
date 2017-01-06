#ifndef PACKING_H
#define PACKING_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PACKING			1
#define UN_PACKING		2
#define FILE_NAME_LEN	16
#define KEY				0x18181818

//< 파일 헤더
typedef struct PACK_HEADER
{
	unsigned int key;
	int			num;
}PackHeader;

//< 파일 정보
typedef struct FILE_INFO
{
	int fileSize;
	char fileName[16];
}FileInfo;

//-----------------------------
// 메뉴
//-----------------------------

//< 메뉴 출력
void PrintMenu();

//< 메뉴 입력
int InputMenu();

//-----------------------------
// 패킹 하기
//-----------------------------

//< 패킹 프로세스
void PackingProcess();
//========================================================
//< 헤더 정보 입력
void InputHeaderInfo(PackHeader	*packHeader, int fileNum);

//========================================================
//< 파일 정보 입력
int InputFileInfo(FileInfo **ppFileInfo);

// 파일 개수 입력
int InputFileNum();

// 파일 이름 입력
void InputFileName(FileInfo **ppFileInfo, int count);

// 파일 크기 구하기
bool GetFileSize(FileInfo **ppFileInfo, int count);

// 패킹 파일 이름 입력
void InputPackingName(char packingName[]);

//-----------------------------
// 패킹 풀기
//-----------------------------
//< 언패킹 프로세스
void UnPackingProcess();

// 패킹 파일 이름 입력 받기
void InputUnPackingName(char unPackingName[]);

// key 검사

#endif //PACKING_H