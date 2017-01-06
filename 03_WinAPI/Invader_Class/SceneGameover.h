#ifndef		SCENEGAMEOVER_H__
#define		SCENEGAMEOVER_H__

#include "Scene.h"
#include "SceneManager.h"
#include "SceneGame.h"

class CSceneGameover : public CScene
{
public:
	CSceneGameover();
	virtual ~CSceneGameover();

	virtual bool Run();
	void Draw();
};

#endif		//!SCENEGAMEOVER_H__
