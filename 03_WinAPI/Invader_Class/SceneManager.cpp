#include "SceneManager.h"

CSceneManager::CSceneManager()
{
	_type = TITLE;
	_pScene = NULL;

	// �ܼ� �̱���
	CConsole::GetInstance()->Initial();
}

CSceneManager::~CSceneManager()
{

}

bool CSceneManager::Run(void)
{
	// _pScene�� NULL�� �ƴϸ� ���� ��ȯ���� ���� ����
	if (_pScene != NULL)
	{
		_pScene->Run();

		return true;
	}

	// _pScene�� NULL�̸� ���� ���� �̰ų� ���� ��ȯ �� ����
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
		// �������� ����̳� Gameover��
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