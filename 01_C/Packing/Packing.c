#include "Packing.h"

//< �޴� ���
void PrintMenu()
{
	printf_s("1. ��ŷ�ϱ�\n");
	printf_s("2. ��ŷǮ��\n");
	printf_s("-> ");
}

//< �޴� �Է�
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
			printf_s("�ٽ� �Է�\n");
		}
	}
}

//< ��ŷ ���μ���
void PackingProcess()
{
	PackHeader	packHeader;	// ��� ����
	FileInfo	*pFileInfo;	// ���� ���� ������

	char packingName[FILE_NAME_LEN] = { 0, };	
								// ^ ��ŷ ������ �̸�
	int count = 0;				// ���ͷ�����
	int fileNum = 0;			// ��ŷ�� ���� ��
	int maxSize = 0;			// ���� ū ���� ������
	char *buff = NULL;			// ���� ������ ����
	
	FILE *pSrcFile = NULL;		// ���� ���� ������ ����
	FILE *pDestFile = NULL;		// dest ���� ������ ����


	//< FileInfo �Է�
	fileNum = InputFileInfo(&pFileInfo);

	//< PackHeader �Է�
	InputHeaderInfo(&packHeader, fileNum);

	//< ��ŷ ���� �̸� �Է¹ޱ�
	InputPackingName(packingName);

	//< ���Ͽ� ����
	pDestFile = fopen(packingName, "wb");
	
	//< ��� ���Ͽ� ����
	fwrite(&packHeader, sizeof(PackHeader), 1, pDestFile);

	//< ���� ���� ���Ͽ� ����
	for (count = 0; count < fileNum; ++count)
	{
		fwrite(&pFileInfo[count], sizeof(FileInfo), 1, pDestFile);
	}
	
	//< ���� ū ������ ������ ���ϱ�
	for (count = 0; count < fileNum; ++count)
	{
		maxSize = maxSize > pFileInfo[count].fileSize ? maxSize : pFileInfo[count].fileSize;
	}

	//< ���� ū ���� ������ ũ��� ���� �����Ҵ�
	buff = (char*)malloc(maxSize);
	
	//< ���� ���� ���� ���ۿ� �Űܼ� dest���Ͽ� ����
	for (count = 0; count < fileNum; ++count)
	{
		pSrcFile = fopen(pFileInfo[count].fileName, "rb");

		if (pSrcFile != NULL)
		{
			fread(buff, pFileInfo[count].fileSize, 1, pSrcFile);
			fwrite(buff, pFileInfo[count].fileSize, 1, pDestFile);
		}

		fclose(pSrcFile);	// ���� �ݱ�
	}

	fclose(pDestFile);	// ���� �ݱ�
	free(pFileInfo);	// �����Ҵ� ����
	free(buff);			// �����Ҵ� ����
}

//< ��� ���� �Է�
void InputHeaderInfo( PackHeader *packHeader, int packingNum )
{
	packHeader->num = packingNum;
	packHeader->key = KEY;
}

//< ���� ���� �Է�
int InputFileInfo(FileInfo **ppFileInfo)
{
	FILE *pFile = NULL;
	int fileNum = 0;
	int count	= 0;	// iterator

	// ���� ���� �Է� �ޱ�
	fileNum = InputFileNum();

	// ���� ������ŭ ����ü �Ҵ�
	*ppFileInfo = (FileInfo*)malloc(sizeof(FileInfo) * fileNum);

	// ���� ������ŭ �ݺ�
	for (count = 0; count < fileNum; ++count)
	{
		// ���� �̸� �Է� 
		InputFileName(ppFileInfo, count);

		// ���� ������ ���ϱ�
		if (!GetFileSize(ppFileInfo, count))
		{
			// false ���� �� �ٽ� �̸� �Է� ����
			--count;
			continue;
		}
	}

	return fileNum;
}

// ���� ���� �Է�
int InputFileNum()
{
	int fileNum = 0;

	printf_s("���� ���� : ");
	scanf_s("%d", &fileNum);

	return fileNum;
}

// ���� �̸� �Է�
void InputFileName(FileInfo **ppFileInfo, int count)
{
	printf_s("���� �̸� : ");
	scanf_s("%s", (*ppFileInfo)[count].fileName, FILE_NAME_LEN);

	//fseek(stdin, 0, SEEK_END);
	//fgets(pFileInfo[count].fileName, FILE_NAME_LEN, stdin);
	//gets_s(temp, FILE_NAME_LEN);
	//strcpy_s(pFileInfo[count].fileName, FILE_NAME_LEN, temp);
}

// ���� ũ�� ���ϱ�
bool GetFileSize(FileInfo **ppFileInfo, int count)
{
	FILE *pFile = NULL;

	// �Է� ���� ���� �̸��� ���� ����
	pFile = fopen((*ppFileInfo)[count].fileName, "rb");

	if (pFile != NULL)
	{
		fseek(pFile, 0, SEEK_END);

		// ���� ������ �Է�
		(*ppFileInfo)[count].fileSize = ftell(pFile);

		fclose(pFile);

		return true;
	}
	
	// �ش� ������ �������� ���� ��� false ����
	return false;
}

// ��ŷ ���� �̸� �Է�
void InputPackingName(char packingName[])
{
	printf_s("��ŷ ���� �̸� : ");
	scanf_s("%s", packingName, FILE_NAME_LEN);
}

//-----------------------------
// ��ŷ Ǯ��
//-----------------------------
//< ����ŷ ���μ���
void UnPackingProcess()
{
	PackHeader	packHeader;
	FileInfo	*pFileInfo	= NULL;
	
	FILE		*pSrcFile	= NULL;
	FILE		*pDestFile	= NULL;

	int			count		= 0;		// ���ͷ�����
	int			fileNum		= 0;		// ����ŷ�� ���� ��
	int			maxSize		= 0;		// ���� ū ���� ������

	char		*buff = NULL;			// ���� ������ ����
	char		unPackingName[FILE_NAME_LEN] = { 0, };
	

	// ���� �̸� �Է� �ޱ�
	InputUnPackingName(unPackingName);

	pSrcFile = fopen( unPackingName, "rb" );
	
	if (pSrcFile == NULL)
	{
		printf_s("������ �������� �ʽ��ϴ�.\n");
		system("pause");
		return;
	}

	fread(&packHeader, sizeof(PackHeader), 1, pSrcFile);

	// Ű �˻�
	if (packHeader.key != KEY)
	{
		printf_s("key���� ��ġ���� �ʽ��ϴ�.\n");
		system("pause");
		return;
	}

	// FileInfo �б�
	fileNum = packHeader.num;

	pFileInfo = (FileInfo*)malloc(sizeof(FileInfo) * fileNum);
	
	for (count = 0; count < fileNum; ++count)
	{
		fread(&pFileInfo[count], sizeof(FileInfo), 1, pSrcFile);
		maxSize = maxSize > pFileInfo[count].fileSize ? maxSize : pFileInfo[count].fileSize;
	}
	
	buff = (char*)malloc(maxSize);

	// ���� ����
	for (count = 0; count < fileNum; ++count)
	{
		pDestFile = fopen(pFileInfo[count].fileName, "wb");

		fread(buff, pFileInfo[count].fileSize, 1, pSrcFile);

		fwrite(buff, pFileInfo[count].fileSize, 1, pDestFile);

		fclose(pDestFile);

		printf_s("%s\n", pFileInfo[count].fileName);
	}
	 
	free(buff);			// �����Ҵ� ����
	free(pFileInfo);	// �����Ҵ� ����
	fclose(pSrcFile);	// ���� �ݱ�
}

void InputUnPackingName(char unPackingName[])
{
	printf_s("�и��� ��ŷ ���� �̸� : ");
	scanf_s("%s", unPackingName, FILE_NAME_LEN);
}