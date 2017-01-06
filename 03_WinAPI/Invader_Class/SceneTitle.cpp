#include "Common.h"
#include "SceneTitle.h"
#include "Object.h"
#include "ObjectManager.h"
#include "SceneGame.h"
#include "Console.h"

CSceneTitle::CSceneTitle()
{
	_cursorX = 28;
	_cursorY = 10;
}

CSceneTitle::~CSceneTitle()
{
}


bool CSceneTitle::Run(void)
{
	// 키 프로세스
	if (!keyProcess())
	{
		return true;
	}

	// 1. 시작일 경우 Game씬으로 넘어감
	if (_cursorY == 10 && GetAsyncKeyState(VK_RETURN))
	{
		// 씬 상태를 game으로 변경
		CSceneManager::GetInstance()->SetSceneType(CSceneManager::GAME);

		return true;
	}

	// 2. 종료일 경우 프로그램 종료
	else if (_cursorY == 11 && GetAsyncKeyState(VK_RETURN))
	{
		printf_s("게임을 종료합니다.\n");

		system("pause");

		return false;
	}

	// 버퍼 클리어
	CConsole::GetInstance()->ClearScreen();

	// draw
	DrawTitle();

	// flip
	CConsole::GetInstance()->BufferFlip();

	return true;
}

void CSceneTitle::DrawTitle(void)
{
	memcpy(&(CConsole::GetInstance()->screenBuffer[9][30]), "== INVADER ==", 30);
	memcpy(&(CConsole::GetInstance()->screenBuffer[10][30]), "1. 게임 시작", 30);
	memcpy(&(CConsole::GetInstance()->screenBuffer[11][30]), "2. 게임 종료", 30);
	CConsole::GetInstance()->screenBuffer[_cursorY][_cursorX] = '>';
	DrawCursor();
}

void CSceneTitle::DrawCursor(void)
{
	CConsole::GetInstance()->screenBuffer[_cursorX][_cursorY] = '>';
}

bool CSceneTitle::keyProcess(void)
{
	if (GetAsyncKeyState(VK_DOWN) & 0X8000 || GetAsyncKeyState(VK_DOWN) & 0x0001 || GetAsyncKeyState(VK_DOWN) & 0x8001)
	{
		_cursorY++;
	}

	if (GetAsyncKeyState(VK_UP) & 0X8000 || GetAsyncKeyState(VK_UP) & 0x0001 || GetAsyncKeyState(VK_UP) & 0x8001)
	{
		_cursorY--;
	}

	_cursorY = max(10, _cursorY);

	_cursorY = min(11, _cursorY);

	return true;
}