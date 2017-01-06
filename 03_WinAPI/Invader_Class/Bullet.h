#ifndef BULLET__H_
#define BULLET__H_

#include "Object.h"

class CBullet : public CObject
{
public :
	CBullet(enum OBJECT_TYPE objectType, int posX, int posY, bool visible, bool fromEnemy);
	~CBullet();

	void Draw(void);
	bool Action(void);

	bool CollisionCheck(bool fromEnemy);

	bool CollisionPlayer(void);
	bool CollisionEnemy(void);

private:
	bool _fromEnemy;
};
#endif //BULLET__H_