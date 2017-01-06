#include "Packing.h"

//< 메뉴 출력
void PrintMenu()
{
	printf_s("1. 패킹하기\n");
	printf_s("2. 패킹풀기\n");
	printf_s("-> ");
}

//< 메뉴 입력
int InputMenu()
{
	int inputNum = 0;

	while (1)
	{
		scanf_s("%d", &inputNum);

		if (0 < inputNum && inputNum < 3)
		{
			return inputNum;
		}
		else
		{
			printf_s("다시 입력\n");
		}
	}
}

//< 패킹 프로세스
void PackingProcess()
{
	PackHeader	packHeader;	// 헤더 정보
	FileInfo	*pFileInfo;	// 파일 정보 포인터

	char packingName[FILE_NAME_LEN] = { 0, };	
								// ^ 패킹 파일의 이름
	int count = 0;				// 이터레이터
	int fileNum = 0;			// 패킹될 파일 수
	int maxSize = 0;			// 가장 큰 파일 사이즈
	char *buff = NULL;			// 버퍼 포인터 변수
	
	FILE *pSrcFile = NULL;		// 원본 파일 포인터 변수
	FILE *pDestFile = NULL;		// dest 파일 포인터 변수


	//< FileInfo 입력
	fileNum = InputFileInfo(&pFileInfo);

	//< PackHeader 입력
	InputHeaderInfo(&packHeader, fileNum);

	//< 패킹 파일 이름 입력받기
	InputPackingName(packingName);

	//< 파일에 쓰기
	pDestFile = fopen(packingName, "wb");
	
	//< 헤더 파일에 쓰기
	fwrite(&packHeader, sizeof(PackHeader), 1, pDestFile);

	//< 파일 정보 파일에 쓰기
	for (count = 0; count < fileNum; ++count)
	{
		fwrite(&pFileInfo[count], sizeof(FileInfo), 1, pDestFile);
	}
	
	//< 가장 큰 파일의 사이즈 구하기
	for (count = 0; count < fileNum; ++count)
	{
		maxSize = maxSize > pFileInfo[count].fileSize ? maxSize : pFileInfo[count].fileSize;
	}

	//< 가장 큰 파일 사이즈 크기로 버퍼 동적할당
	buff = (char*)malloc(maxSize);
	
	//< 원본 파일 내용 버퍼에 옮겨서 dest파일에 쓰기
	for (count = 0; count < fileNum; ++count)
	{
		pSrcFile = fopen(pFileInfo[count].fileName, "rb");

		if (pSrcFile != NULL)
		{
			fread(buff, pFileInfo[count].fileSize, 1, pSrcFile);
			fwrite(buff, pFileInfo[count].fileSize, 1, pDestFile);
		}

		fclose(pSrcFile);	// 파일 닫기
	}

	fclose(pDestFile);	// 파일 닫기
	free(pFileInfo);	// 동적할당 해제
	free(buff);			// 동적할당 해제
}

//< 헤더 정보 입력
void InputHeaderInfo( PackHeader *packHeader, int packingNum )
{
	packHeader->num = packingNum;
	packHeader->key = KEY;
}

//< 파일 정보 입력
int InputFileInfo(FileInfo **ppFileInfo)
{
	FILE *pFile = NULL;
	int fileNum = 0;
	int count	= 0;	// iterator

	// 파일 개수 입력 받기
	fileNum = InputFileNum();

	// 파일 개수만큼 구조체 할당
	*ppFileInfo = (FileInfo*)malloc(sizeof(FileInfo) * fileNum);

	// 파일 개수만큼 반복
	for (count = 0; count < fileNum; ++count)
	{
		// 파일 이름 입력 
		InputFileName(ppFileInfo, count);

		// 파일 사이즈 구하기
		if (!GetFileSize(ppFileInfo, count))
		{
			// false 리턴 시 다시 이름 입력 받음
			--count;
			continue;
		}
	}

	return fileNum;
}

// 파일 개수 입력
int InputFileNum()
{
	int fileNum = 0;

	printf_s("파일 개수 : ");
	scanf_s("%d", &fileNum);

	return fileNum;
}

// 파일 이름 입력
void InputFileName(FileInfo **ppFileInfo, int count)
{
	printf_s("파일 이름 : ");
	scanf_s("%s", (*ppFileInfo)[count].fileName, FILE_NAME_LEN);

	//fseek(stdin, 0, SEEK_END);
	//fgets(pFileInfo[count].fileName, FILE_NAME_LEN, stdin);
	//gets_s(temp, FILE_NAME_LEN);
	//strcpy_s(pFileInfo[count].fileName, FILE_NAME_LEN, temp);
}

// 파일 크기 구하기
bool GetFileSize(FileInfo **ppFileInfo, int count)
{
	FILE *pFile = NULL;

	// 입력 받은 파일 이름의 파일 열기
	pFile = fopen((*ppFileInfo)[count].fileName, "rb");

	if (pFile != NULL)
	{
		fseek(pFile, 0, SEEK_END);

		// 파일 사이즈 입력
		(*ppFileInfo)[count].fileSize = ftell(pFile);

		fclose(pFile);

		return true;
	}
	
	// 해당 파일이 존재하지 않을 경우 false 리턴
	return false;
}

// 패킹 파일 이름 입력
void InputPackingName(char packingName[])
{
	printf_s("패킹 파일 이름 : ");
	scanf_s("%s", packingName, FILE_NAME_LEN);
}

//-----------------------------
// 패킹 풀기
//-----------------------------
//< 언패킹 프로세스
void UnPackingProcess()
{
	PackHeader	packHeader;
	FileInfo	*pFileInfo	= NULL;
	
	FILE		*pSrcFile	= NULL;
	FILE		*pDestFile	= NULL;

	int			count		= 0;		// 이터레이터
	int			fileNum		= 0;		// 언패킹될 파일 수
	int			maxSize		= 0;		// 가장 큰 파일 사이즈

	char		*buff = NULL;			// 버퍼 포인터 변수
	char		unPackingName[FILE_NAME_LEN] = { 0, };
	

	// 파일 이름 입력 받기
	InputUnPackingName(unPackingName);

	pSrcFile = fopen( unPackingName, "rb" );
	
	if (pSrcFile == NULL)
	{
		printf_s("파일이 존재하지 않습니다.\n");
		system("pause");
		return;
	}

	fread(&packHeader, sizeof(PackHeader), 1, pSrcFile);

	// 키 검사
	if (packHeader.key != KEY)
	{
		printf_s("key값이 일치하지 않습니다.\n");
		system("pause");
		return;
	}

	// FileInfo 읽기
	fileNum = packHeader.num;

	pFileInfo = (FileInfo*)malloc(sizeof(FileInfo) * fileNum);
	
	for (count = 0; count < fileNum; ++count)
	{
		fread(&pFileInfo[count], sizeof(FileInfo), 1, pSrcFile);
		maxSize = maxSize > pFileInfo[count].fileSize ? maxSize : pFileInfo[count].fileSize;
	}
	
	buff = (char*)malloc(maxSize);

	// 파일 생성
	for (count = 0; count < fileNum; ++count)
	{
		pDestFile = fopen(pFileInfo[count].fileName, "wb");

		fread(buff, pFileInfo[count].fileSize, 1, pSrcFile);

		fwrite(buff, pFileInfo[count].fileSize, 1, pDestFile);

		fclose(pDestFile);

		printf_s("%s\n", pFileInfo[count].fileName);
	}
	 
	free(buff);			// 동적할당 해제
	free(pFileInfo);	// 동적할당 해제
	fclose(pSrcFile);	// 파일 닫기
}

void InputUnPackingName(char unPackingName[])
{
	printf_s("분리할 패킹 파일 이름 : ");
	scanf_s("%s", unPackingName, FILE_NAME_LEN);
}