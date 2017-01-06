#ifndef ENEMY__H_
#define ENEMY__H_

#include "Object.h"

class CEnemy : public CObject
{
public :
	CEnemy(enum OBJECT_TYPE objectType, int posX, int posY, bool visible);
	~CEnemy();

	void Draw(void);
	bool Action(void);
	void CreateBullet(void);
	bool CollisionCheck(void);

private:
	bool _direction;
	int _createX;		// ���� ������ġ(x��ǥ)
};

#endif // ENEMY__H_