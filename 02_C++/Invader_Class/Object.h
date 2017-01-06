#ifndef		OBJECT__H_
#define		OBJECT__H_

#include "Console.h"

class CObject
{
public:
	CObject();
	virtual ~CObject();

	void GetPos(int* posX, int* posY);	// ��ǥ ��������
	void SetPos(int posX, int posY);	// ��ǥ �����ϱ�

	bool GetVisible();
	void Setvisible(bool visible);

	void Spawn(void);					// ��Ÿ����
	void Destroy(void);					// �������
	void SpriteDraw(int posX, int posY, char sprite);
	bool CheckVisible(void);

	virtual void	Initialization(int posX, int posY)	= 0;
	virtual void	Draw(void)				= 0;					
	virtual void	Action(void)			= 0;		
	
protected:
	int		_posX;
	int		_posY;
	bool	_visible;
};

#endif		//OBJECT__H_