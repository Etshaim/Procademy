#ifndef		SCENETITLE_H__
#define		SCENETITLE_H__

#include "Scene.h"
#include "SceneManager.h"

class CSceneTitle : public CScene
{
public:
	CSceneTitle();
	virtual ~CSceneTitle();

	virtual bool Run();

	void DrawTitle(void);
	void DrawCursor(void);

	bool keyProcess(void);

private:
	int	_cursorX;
	int _cursorY;
};

#endif		//!SCENETITLE_H__
