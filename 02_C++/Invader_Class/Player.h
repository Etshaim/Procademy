#ifndef PLAYER__H_
#define PLAYER__H_

#include "Object.h"

class CPlayer : public CObject
{
public :
	static CPlayer*	GetInstance(void)
	{
		static CPlayer player;

		return &player;
	}

	void Initialization(int posX, int posY);
	void Draw(void);
	void Action(void);

private:
	CPlayer();
	~CPlayer();

	int _HP;

};
#endif	// PLAYER_H_