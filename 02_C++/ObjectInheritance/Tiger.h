#ifndef TIGER__H__
#define TIGER__H__

#include "Monster.h"

class Tiger : public Monster
{
public:
	// ������
	Tiger();
	// �ı���
	virtual ~Tiger();

	virtual void Print();
	virtual void InputData(void);
};


#endif	//TIGER__H__