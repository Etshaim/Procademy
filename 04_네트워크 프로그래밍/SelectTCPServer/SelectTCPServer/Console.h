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

#endif //__CONSOLE__