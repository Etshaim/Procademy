#ifndef		OBJECT__H_
#define		OBJECT__H_

#include "Console.h"

class CObject
{
public:
	CObject();
	virtual ~CObject();

	void GetPos(int* posX, int* posY);	// 좌표 가져오기
	void SetPos(int posX, int posY);	// 좌표 세팅하기

	bool GetVisible();
	void Setvisible(bool visible);

	void Spawn(void);					// 나타나게
	void Destroy(void);					// 사라지게
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