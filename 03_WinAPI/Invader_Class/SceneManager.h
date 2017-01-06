#ifndef	SCENEMANAGER_H__
#define	SCENEMANAGER_H__

#include "Scene.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneGameover.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Console.h"


class CSceneManager
{
public:
	enum eSCENETYPE
	{
		TITLE,
		GAME,
		GAMEOVER
	};

	// ΩÃ±€≈Ê
	static CSceneManager*	GetInstance(void)
	{
		static CSceneManager _sceneManager;

		return &_sceneManager;
	}

public:
	bool Run(void);
	bool SetSceneType(enum eSCENETYPE);

private:
	CSceneManager();
	~CSceneManager();

private:
	int		_type;			// æ¿ ≈∏¿‘
	//eSCENETYPE	_type;
	CScene	*_pScene;
};

#endif	// !SCENEMANAGER_H__
