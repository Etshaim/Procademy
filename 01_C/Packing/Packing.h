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

//< ���� ���
typedef struct PACK_HEADER
{
	unsigned int key;
	int			num;
}PackHeader;

//< ���� ����
typedef struct FILE_INFO
{
	int fileSize;
	char fileName[16];
}FileInfo;

//-----------------------------
// �޴�
//-----------------------------

//< �޴� ���
void PrintMenu();

//< �޴� �Է�
int InputMenu();

//-----------------------------
// ��ŷ �ϱ�
//-----------------------------

//< ��ŷ ���μ���
void PackingProcess();
//========================================================
//< ��� ���� �Է�
void InputHeaderInfo(PackHeader	*packHeader, int fileNum);

//========================================================
//< ���� ���� �Է�
int InputFileInfo(FileInfo **ppFileInfo);

// ���� ���� �Է�
int InputFileNum();

// ���� �̸� �Է�
void InputFileName(FileInfo **ppFileInfo, int count);

// ���� ũ�� ���ϱ�
bool GetFileSize(FileInfo **ppFileInfo, int count);

// ��ŷ ���� �̸� �Է�
void InputPackingName(char packingName[]);

//-----------------------------
// ��ŷ Ǯ��
//-----------------------------
//< ����ŷ ���μ���
void UnPackingProcess();

// ��ŷ ���� �̸� �Է� �ޱ�
void InputUnPackingName(char unPackingName[]);

// key �˻�

#endif //PACKING_H