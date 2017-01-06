#ifndef SWORD__H__
#define SWORD__H__

#include "Item.h"

class Sword : public Item
{
public:
	// 생성자
	Sword();
	// 파괴자
	virtual ~Sword();

	virtual void Print();
	virtual void InputData(void);

private:
	int _attack;
};


#endif	//SWORD__H__