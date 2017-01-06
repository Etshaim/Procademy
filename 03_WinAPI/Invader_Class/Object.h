#ifndef		OBJECT__H_
#define		OBJECT__H_

#include "Console.h"

class CObject
{
public:
	enum OBJECT_TYPE
	{
		PLAYER,
		ENEMY,
		PLAYER_BULLET,
		ENEMY_BULLET
	};

	CObject(enum OBJECT_TYPE objectType, int posX, int posY, bool visible);
	virtual ~CObject();

	virtual void	Draw(void)		= 0;
	virtual bool	Action(void)	= 0;

	//void SpriteDraw(int posX, int posY, char sprite);

	int GetPosX(void);
	int GetPosY(void);

	void SetPosX(int posX);
	void SetPosY(int posY);

	//void GetPos(int* posX, int* posY);	// 좌표 가져오기
	//void SetPos(int posX, int posY);	// 좌표 세팅하기
	//void Spawn(void);					// 나타나게
	
	void Destroy(void);					// 사라지게
	
	bool CheckVisible(void);			

	OBJECT_TYPE CheckObjectType(void);

	//virtual void	Initialization(int posX, int posY)	= 0;
		

protected:
	OBJECT_TYPE	_objectType;
	int					_posX;
	int					_posY;
	bool				_visible;
};

#endif		//OBJECT__H_