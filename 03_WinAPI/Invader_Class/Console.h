#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "Common.h"


class CConsole
{
public:
	static CConsole* GetInstance(void)
	{
		static CConsole _consol;

		return &_consol;
	}

	char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

public:
	//< �ܼ� ��� ���� �غ� �۾�
	void Initial(void);

	//< �ܼ� ȭ���� Ŀ���� x,y ��ǥ�� �̵�
	void MoveCursor(int posX, int posY);

	//< ������ ������ ȭ������ ����ִ� �Լ�
	void BufferFlip(void);

	//< �ܼ� ȭ���� �ʱ�ȭ
	void ClearScreen(void);


private:
	CConsole();		// ������
	~CConsole();	// �ı���

	HANDLE hConsole;

};

//extern HANDLE hConsole;
//extern char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];
//
////< �ܼ� ��� ���� �غ� �۾�
//void cs_Initial(void);
//
////< �ܼ� ȭ���� Ŀ���� x,y ��ǥ�� �̵�
//void cs_MoveCursor(int posX, int posY);
//
////< ������ ������ ȭ������ ����ִ� �Լ�
//void cs_BufferFlip(void);
//
////< �ܼ� ȭ���� �ʱ�ȭ
//void cs_ClearScreen(void);

#endif //!CONSOLE_H_