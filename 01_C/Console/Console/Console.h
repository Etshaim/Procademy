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


//< 버퍼의 내용을 화면으로 찍어주는 함수
void Buffer_Flip(void);

//< 화면 버퍼를 지워주는 함수
void Buffer_Clear(void);

//< 버퍼의 특정 위치에 원하는 문자를 출력
void Sprite_Draw(int x, int y, char sprite);

//< key 입력
void KeyProcess(int *x, int *y);

#endif //__CONSOLE__