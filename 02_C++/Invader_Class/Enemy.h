#ifndef ENEMY__H_
#define ENEMY__H_

#include "Object.h"

class CEnemy : public CObject
{
public :
	CEnemy();
	~CEnemy();

	void Initialization(int posX, int posY);
	void Draw(void);
	void Action(void);

private:
	

	bool _direction;
	int _createX;		// 최초 생성위치(x좌표)
};

#endif // ENEMY__H_