#ifndef BULLET__H_
#define BULLET__H_

#include "Object.h"

class CBullet : public CObject
{
public :
	CBullet();
	~CBullet();

	void Initialization(int posX, int posY);
	void Draw(void);
	void Action(void);

private:
	bool _fromEnemy;
};
#endif //BULLET__H_