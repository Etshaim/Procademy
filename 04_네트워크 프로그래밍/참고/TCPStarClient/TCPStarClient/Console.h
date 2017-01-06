#ifndef __CONSOLE__
#define __CONSOLE__

#define dfSCREEN_WIDTH		81
#define dfSCREEN_HEIGHT		24



//< 콘솔 제어를 위한 준비 작업
void cs_Initial(void);

//< 콘솔 화면의 커서를 x,y 좌표로 이동
void cs_MoveCursor(int posX, int posY);

//< 콘솔 화면을 초기화
void cs_ClearScreen(void);

#endif //__CONSOLE__