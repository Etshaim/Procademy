#ifndef NPC__H__
#define NPC__H__

#include "Common.h"

class NPC : public My_Object
{
public:
	NPC();

	~NPC();

	virtual void Print();
	virtual void InputData(void);

private:
	int _type;
};


#endif	//NPC__H__
