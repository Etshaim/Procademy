#include "SceneManager.h"

CSceneManager::CSceneManager()
{
	_type = TITLE;
	_pScene = NULL;

	// 콘솔 싱글톤
	CConsole::GetInstance()->Initial();
}

CSceneManager::~CSceneManager()
{

}

bool CSceneManager::Run(void)
{
	// _pScene이 NULL이 아니면 씬이 전환되지 않은 것임
	if (_pScene != NULL)
	{
		_pScene->Run();

		return true;
	}

	// _pScene이 NULL이면 실행 직후 이거나 씬이 전환 된 것임
	switch (_type)
	{
	case TITLE:
	{
		_pScene = new CSceneTitle;
	}
	break;

	case GAME:
	{
		_pScene = new CSceneGame;
	}
	break;

	case GAMEOVER:
	{
		_pScene = new CSceneGameover;
	}
	break;

	default:
	{
		printf_s("error_ScenManager.h");
		return false;
	}
	break;
	}

	if ( !_pScene->Run() )
	{
		// 게임종료 명령이나 Gameover시
		return false;
	}

	return true;
}

bool CSceneManager::SetSceneType(enum eSCENETYPE type)
{
	_type = type;

	delete _pScene;
	_pScene = NULL;

	return true;
}