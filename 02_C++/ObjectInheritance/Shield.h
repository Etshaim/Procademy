#ifndef SHIELD__H__
#define SHIELD__H__

#include "Item.h"

class Shield : public Item
{
public:
	// ������
	Shield();
	// �ı���
	virtual ~Shield();

	virtual void Print();
	virtual void InputData(void);

private:
	int _defense;
};


#endif	//SHIELD__H__