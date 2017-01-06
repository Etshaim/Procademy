#include "Object.h"
#include "Console.h"
#include "GameManager.h"

CObject::CObject()
{

}

CObject::~CObject()
{

}

void CObject::GetPos(int* posX, int* posY)
{
	*posX = _posX;
	*posY = _posY;
}

void CObject::SetPos(int posX, int posY)
{
	_posX = posX;
	_posY = posY;
}

bool CObject::GetVisible(void)
{
	return _visible;
}

void CObject::Setvisible(bool visible)
{
	_visible = visible;
}

void CObject::Spawn(void)
{
	_visible = true;
}

void CObject::SpriteDraw(int posX, int posY, char sprite)
{
	CGameManager *gameManager = CGameManager::GetInstance();

	if (_posX < 0 || _posY < 0 || _posX >= dfSCREEN_WIDTH - 1 || _posY >= dfSCREEN_HEIGHT)
	{
		return;
	}

	gameManager->screenBuffer[_posY][_posX] = sprite;
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