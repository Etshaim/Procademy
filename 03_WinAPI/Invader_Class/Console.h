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
	//< 콘솔 제어를 위한 준비 작업
	void Initial(void);

	//< 콘솔 화면의 커서를 x,y 좌표로 이동
	void MoveCursor(int posX, int posY);

	//< 버퍼의 내용을 화면으로 찍어주는 함수
	void BufferFlip(void);

	//< 콘솔 화면을 초기화
	void ClearScreen(void);


private:
	CConsole();		// 생성자
	~CConsole();	// 파괴자

	HANDLE hConsole;

};

//extern HANDLE hConsole;
//extern char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];
//
////< 콘솔 제어를 위한 준비 작업
//void cs_Initial(void);
//
////< 콘솔 화면의 커서를 x,y 좌표로 이동
//void cs_MoveCursor(int posX, int posY);
//
////< 버퍼의 내용을 화면으로 찍어주는 함수
//void cs_BufferFlip(void);
//
////< 콘솔 화면을 초기화
//void cs_ClearScreen(void);

#endif //!CONSOLE_H_