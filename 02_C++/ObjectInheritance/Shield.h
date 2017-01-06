#ifndef SHIELD__H__
#define SHIELD__H__

#include "Item.h"

class Shield : public Item
{
public:
	// 생성자
	Shield();
	// 파괴자
	virtual ~Shield();

	virtual void Print();
	virtual void InputData(void);

private:
	int _defense;
};


#endif	//SHIELD__H__