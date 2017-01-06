#ifndef TIGER__H__
#define TIGER__H__

#include "Monster.h"

class Tiger : public Monster
{
public:
	// 생성자
	Tiger();
	// 파괴자
	virtual ~Tiger();

	virtual void Print();
	virtual void InputData(void);
};


#endif	//TIGER__H__