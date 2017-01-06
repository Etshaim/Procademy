#ifndef		OBJECTMANAGER__H_
#define		OBJECTMANAGER__H_

#include "GameManager.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

#define	dfENEMY_MAX		30		// ������ �ִ�ġ
#define dfSHOT_MAX		50		// �Ѿ��� �ִ�ġ

class CObjectManager
{
public:
	static CObjectManager* GetInstance(void)
	{
		static CObjectManager objectManager;

		return &objectManager;
	}
	
	// �ʱ�ȭ
	void Initialization(void);

	// ���� Ŭ���� üũ
	bool GameClearCheck(void);

	// ���� ���� üũ
	bool GameOverCheck(void);

	CPlayer	*player;
	CEnemy	enemies[dfENEMY_MAX];	// ����
	CBullet	bullets[dfSHOT_MAX];	// �Ѿ�

private:
	CObjectManager();
	~CObjectManager();

	// �ʱ�ȭ
	


	
};

#endif		//OBJECTMANAGER__H_