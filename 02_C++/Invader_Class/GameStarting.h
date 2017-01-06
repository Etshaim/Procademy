#pragma once
#include "PreDefine.h"
#include "Stage.h"
#include "GameManager.h"

class CGameStarting : protected CStage
{
private:
	void init();
	void run();

public:
	CGameStarting();
	~CGameStarting();

	void running();
};

