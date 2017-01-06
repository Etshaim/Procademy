#include "Player.h"

CPlayer::CPlayer()
{
	
}

CPlayer::~CPlayer()
{

}

void CPlayer::Initialization(int posX, int posY)
{
	// 위치 지정
	SetPos(posX, posY);

	// 체력 설정
	_HP = 3;

	// 활성화
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