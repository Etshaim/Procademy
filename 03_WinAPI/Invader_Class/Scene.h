#ifndef SCENE__H_
#define SCENE__H_

class CScene
{
public:
	CScene();
	virtual ~CScene();

	virtual bool Run(void) = 0;
};

#endif	//!SCENE__H_
