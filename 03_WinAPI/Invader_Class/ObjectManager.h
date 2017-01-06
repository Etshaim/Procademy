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
	
	CDoubleLinkedList<CObject*> _objectList;		// ������Ʈ ����Ʈ(�÷��̾�, ��, �Ѿ� �迭)

	// �÷��̾� ����
	bool CreatePlayer(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible);

	// �� ����
	bool CreateEnemy(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible);

	// �Ѿ� ����
	bool CreateBullet(CObject::OBJECT_TYPE objectType, int posX, int posY, bool visible, bool fromEnemy);

	//CPlayer	*player;
	//CEnemy	enemies[dfENEMY_MAX];	// ����
	//CBullet	bullets[dfSHOT_MAX];	// �Ѿ�

private:
	CObjectManager();
	~CObjectManager();
	
	
};

#endif		//OBJECTMANAGER__H_