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
	// Ű ���μ���
	if (!keyProcess())
	{
		return true;
	}

	// 1. ������ ��� Game������ �Ѿ
	if (_cursorY == 10 && GetAsyncKeyState(VK_RETURN))
	{
		// �� ���¸� game���� ����
		CSceneManager::GetInstance()->SetSceneType(CSceneManager::GAME);

		return true;
	}

	// 2. ������ ��� ���α׷� ����
	else if (_cursorY == 11 && GetAsyncKeyState(VK_RETURN))
	{
		printf_s("������ �����մϴ�.\n");

		system("pause");

		return false;
	}

	// ���� Ŭ����
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
	memcpy(&(CConsole::GetInstance()->screenBuffer[10][30]), "1. ���� ����", 30);
	memcpy(&(CConsole::GetInstance()->screenBuffer[11][30]), "2. ���� ����", 30);
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