#ifndef PLAYER__H__
#define PLAYER__H__

#include "Common.h"

class Player : public My_Object
{
public:
	Player();

	~Player();

	virtual void Print();
	virtual void InputData(void);

private:
	int _HP;
	int _money;
	int _level;
};


#endif	//PLAYER__H__
