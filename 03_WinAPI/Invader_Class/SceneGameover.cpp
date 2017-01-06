#include "Common.h"
#include "SceneGameOver.h"
#include "Object.h"
#include "ObjectManager.h"
#include "SceneGame.h"


CSceneGameover::CSceneGameover()
{

}

CSceneGameover::~CSceneGameover()
{

}

bool CSceneGameover::Run()
{

	// 버퍼 클리어
	CConsole::GetInstance()->ClearScreen();

	// draw
	Draw();

	// flip
	CConsole::GetInstance()->BufferFlip();

	return true;

}

void CSceneGameover::Draw()
{
	memcpy(&(CConsole::GetInstance()->screenBuffer[10][35]), "Game Over", 30);
}