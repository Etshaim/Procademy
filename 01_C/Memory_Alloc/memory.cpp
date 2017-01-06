#include <iostream>
#include <string>

#define MemoryAlloc(x,y)	Alloc<x>(y, __FILE__, __LINE__ )

using namespace std;

struct MEMORY_ALLOC_DATA
{
	std::string	fileName;		// file �̸�
	int			lineNum;
	int			size;
	int			memoryAddress;
	bool		bUse;
};

MEMORY_ALLOC_DATA arrData[100];

int g_arrCount = 0;

// ����ü �迭 (����Ʈ�� �ص� ��)
// �����Ҵ� �� ������ ������ �����ϸ� ��
// ���� : �����̸�, ����, ������, Ÿ��.
// define ��ũ�� ���

//< Release
// MemoryRelease(p) : �굵 ���ø�
// delete 

// template <tyename T> Release(T p)
// {
//		delete []p;
// }


//< �Ҵ�
template <typename T> T* Alloc(int num, std::string fileName, int line);

//< ����
template <typename T> void MemoryRelease(T *type);

//< ���
void PrintAlloc(void);

void main(void)
{
	int		*p1 = MemoryAlloc(int, 4);
	double	*p2 = MemoryAlloc(double, 4);
	char	*p3 = MemoryAlloc(char, 1000);

	MemoryRelease(p1);
	MemoryRelease(p2);

	PrintAlloc();
}

//< �Ҵ�
template <typename T> T* Alloc(int num, std::string fileName, int line)
{
	if (g_arrCount > 100)
	{
		return NULL;
	}

	T *pType = new T[num];

	arrData[g_arrCount].fileName = fileName;
	arrData[g_arrCount].lineNum = line;
	arrData[g_arrCount].size = sizeof(T) * num;
	arrData[g_arrCount].memoryAddress = (int) pType;
	arrData[g_arrCount].bUse = true;

	g_arrCount++;

	return pType;
}

//< ����
template <typename T> void MemoryRelease(T *type)
{
	for (int i = 0; i < g_arrCount; ++i)
	{
		if (arrData[i].memoryAddress == (int)type)
		{
			arrData[i].bUse = false;
			delete []type;
			return;
		}
	}
	
	return;
}

//< ���
void PrintAlloc(void)
{
	int totalSize = 0;

	for (int i = 0; i < g_arrCount; ++i)
	{
		totalSize += arrData[i].size;
	}

	printf_s("Total Alloc Size : %d\n", totalSize);
	printf_s("Total Alloc Count : %d\n\n", g_arrCount);

	for (int i = 0; i < g_arrCount; ++i)
	{
		if (arrData[i].bUse == true)
		{
			printf_s("Not Release Memory : [0x%x] %dBytes\n"
				, arrData[i].memoryAddress
				, arrData[i].size);

			cout << "File : " << arrData[i].fileName << " : " << arrData[i].lineNum << endl << endl;
			/*printf_s("File : %s : %d"
				, arrData[i].fileName
				, arrData[i].lineNum);*/
		}
	}
}
// PrintAlloc
// total alloc size : 734
// total alloc count : 6
// not realse memory : [0x48dd20] 150 Bytes
// file : new_log.cpp : 200