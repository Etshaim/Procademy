#include "Common.h"
#include "SceneGame.h"
#include "Object.h"
#include "ObjectManager.h"
#include "SceneGame.h"
#include "GameManager.h"

CSceneGame::CSceneGame()
{
	
}

CSceneGame::~CSceneGame()
{
}

bool CSceneGame::Run(void)
{
	if (!CGameManager::GetInstance()->Process())
	{
		return false;
	}

	return true;
}
//
//
//	// ���� ���� üũ
//	// ���ӿ��� �� false����
//	if ( GAMEOVER == GetState() )
//	{
//		return false;
//	}
//	// ����Ŭ���� �� false����
//	else if ( GAMECLEAR == GetState() )
//	{
//		return false;
//	}
//
//	CObjectManager *objectManager = CObjectManager::GetInstance();
//
//	// ������Ʈ ����Ʈ ��ȸ�ϸ� action
//	CDoubleLinkedList<CObject*>::ITERATOR iterator;
//
//	for (iterator = objectManager->_objectList.Begin(); iterator != objectManager->_objectList.End(); ++iterator)
//	{
//		// ���ϰ��� false�̸� ������Ʈ�� ���� ����
//		if (	(*iterator)->Action()	)
//		{
//
//		}
//	}
//
//	// ���� �����
//	CConsole::GetInstance()->ClearScreen();
//
//	// draw
//	for (iterator = objectManager->_objectList.Begin(); iterator != objectManager->_objectList.End(); ++iterator)
//	{
//		(*iterator)->Draw();
//	}
//
//	// ȭ�� ǥ��
//	CConsole::GetInstance()->BufferFlip();
//
//	return true;
//}
//
//void CSceneGame::StageInitial()
//{
//	CObjectManager *objectManager = CObjectManager::GetInstance();
//
//	// �÷��̾� ����
//	objectManager->CreatePlayer(CObject::OBJECT_TYPE::PLAYER, 39, 19, true);
//
//	// �� ����
//	int posX = 17;
//	int posY = 1;
//
//	for (int count = 0; dfENEMY_MAX > count; ++count)
//	{
//		// 10���� ���� y�� �ٲٱ�
//		if (count % 10 == 0)
//		{
//			posX = 17;
//			posY = posY + 2;
//		}
//		
//		objectManager->CreateEnemy(CObject::OBJECT_TYPE::ENEMY ,posX, posY, true);
//
//		posX = posX + 5;
//	}
//	
//}
//CSceneGame::eSTATE CSceneGame::GetState(void)
//{
//	return _state;
//}
//
//void CSceneGame::SetState(CSceneGame::eSTATE state)
//{
//	_state = state;
//}