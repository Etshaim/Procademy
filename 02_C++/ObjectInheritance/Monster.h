#ifndef MONSTER__H__
#define MONSTER__H__

#include "Common.h"

class Monster : public My_Object
{
public:
	Monster();

	virtual ~Monster();

	virtual void Print()=0;
	virtual void InputData(void) = 0;

protected:
	int _HP;
	int _level;
	int _speed;
};


#endif	//MONSTER__H__
