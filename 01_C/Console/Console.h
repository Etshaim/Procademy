#ifndef __CONSOLE__
#define __CONSOLE__

#define dfSCREEN_WIDTH		81
#define dfSCREEN_HEIGHT		24

//< �ܼ� ��� ���� �غ� �۾�
void cs_Initial(void);

//< �ܼ� ȭ���� Ŀ���� x,y ��ǥ�� �̵�
void cs_MoveCursor(int posX, int posY);

//< �ܼ� ȭ���� �ʱ�ȭ
void cs_ClearScreen(void);


//< ������ ������ ȭ������ ����ִ� �Լ�
void Buffer_Flip(void);

//< ȭ�� ���۸� �����ִ� �Լ�
void Buffer_Clear(void);

//< ������ Ư�� ��ġ�� ���ϴ� ���ڸ� ���
void Sprite_Draw(int x, int y, char sprite);

//< key �Է�
void KeyProcess(int *x, int *y);

#endif //__CONSOLE__