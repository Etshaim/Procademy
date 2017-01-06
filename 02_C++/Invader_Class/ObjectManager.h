#ifndef		OBJECTMANAGER__H_
#define		OBJECTMANAGER__H_

#include "GameManager.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

#define	dfENEMY_MAX		30		// 적군의 최대치
#define dfSHOT_MAX		50		// 총알의 최대치

class CObjectManager
{
public:
	static CObjectManager* GetInstance(void)
	{
		static CObjectManager objectManager;

		return &objectManager;
	}
	
	// 초기화
	void Initialization(void);

	// 게임 클리어 체크
	bool GameClearCheck(void);

	// 게임 오버 체크
	bool GameOverCheck(void);

	CPlayer	*player;
	CEnemy	enemies[dfENEMY_MAX];	// 적군
	CBullet	bullets[dfSHOT_MAX];	// 총알

private:
	CObjectManager();
	~CObjectManager();

	// 초기화
	


	
};

#endif		//OBJECTMANAGER__H_