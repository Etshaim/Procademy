#ifndef FLY__H__
#define FLY__H__

#include "Monster.h"

class Fly : public Monster
{
public:
	// 생성자
	Fly();
	// 파괴자
	virtual ~Fly();

	virtual void Print();
	virtual void InputData(void);

private:
	int _height;
};


#endif	//FLY__H__