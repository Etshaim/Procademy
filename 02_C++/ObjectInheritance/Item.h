#ifndef ITEM__H__
#define ITEM__H__

#include "Common.h"

class Item : public My_Object
{
public:
	Item();

	virtual ~Item();

	virtual void Print() = 0;
	virtual void InputData(void) = 0;

protected:
	int _type;
	int _price;
};


#endif	//ITEM__H__
