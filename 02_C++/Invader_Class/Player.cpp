#include "Player.h"

CPlayer::CPlayer()
{
	
}

CPlayer::~CPlayer()
{

}

void CPlayer::Initialization(int posX, int posY)
{
	// ��ġ ����
	SetPos(posX, posY);

	// ü�� ����
	_HP = 3;

	// Ȱ��ȭ
	_visible = true;
}

void CPlayer::Draw(void)
{
	if (_visible)
	{
		SpriteDraw(_posX, _posY, '#');
	}
}

void CPlayer::Action(void)
{

}