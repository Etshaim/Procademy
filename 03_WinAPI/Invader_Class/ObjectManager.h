#ifndef		OBJECTMANAGER__H_
#define		OBJECTMANAGER__H_

#include "Common.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "DoubleLinkedList_Template.h"

class CObjectManager
{
public:
	static CObjectManager* GetInstance(void)
	{
		static CObjectManager objectManager;

		return &objectManager;
	}
	
	CDoubleLinkedList<CObject*> _objectList;		// 오브젝트 리스트(플레이어, 적, 총알 배열)

	// 플레이어 생성
	bool CreatePlayer(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible);

	// 적 생성
	bool CreateEnemy(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible);

	// 총알 생성
	bool CreateBullet(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible, bool fromEnemy);

	//CPlayer	*player;
	//CEnemy	enemies[dfENEMY_MAX];	// 적군
	//CBullet	bullets[dfSHOT_MAX];	// 총알

private:
	CObjectManager();
	~CObjectManager();
	
	
};

#endif		//OBJECTMANAGER__H_