#ifndef SWORD__H__
#define SWORD__H__

#include "Item.h"

class Sword : public Item
{
public:
	// ������
	Sword();
	// �ı���
	virtual ~Sword();

	virtual void Print();
	virtual void InputData(void);

private:
	int _attack;
};


#endif	//SWORD__H__