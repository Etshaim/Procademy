#ifndef PLAYER__H_
#define PLAYER__H_

#include "Object.h"
#include "SceneGame.h"

class CPlayer : public CObject
{
public :
	CPlayer(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible);
	~CPlayer();

	void Draw(void);
	bool Action(void);
	void CreateBullet(void);

	bool CollisionCheck(void);
	
	bool KeyProcess(void);


};
#endif	// PLAYER_H_