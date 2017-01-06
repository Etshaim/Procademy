#include "Object.h"

CObject::CObject(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible = true)
	: _objectType(objectType), _posX(posX), _posY(posY), _visible(true)
{
	
}

CObject::~CObject()
{

}

int CObject::GetPosX(void)
{
	return _posX;
}

int CObject::GetPosY(void)
{
	return _posY;
}

void CObject::SetPosX(int posX)
{
	_posX = posX;
}

void CObject::SetPosY(int posY)
{
	_posY= posY;
}

bool CObject::CheckVisible(void)
{
	if (_visible)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CObject::Destroy(void)
{
	_visible = false;
}

CObject::OBJECT_TYPE	CObject::CheckObjectType(void)
{
	return _objectType;
}

