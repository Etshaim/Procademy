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
//	// 게임 상태 체크
//	// 게임오버 시 false리턴
//	if ( GAMEOVER == GetState() )
//	{
//		return false;
//	}
//	// 게임클리어 시 false리턴
//	else if ( GAMECLEAR == GetState() )
//	{
//		return false;
//	}
//
//	CObjectManager *objectManager = CObjectManager::GetInstance();
//
//	// 오브젝트 리스트 순회하며 action
//	CDoubleLinkedList<CObject*>::ITERATOR iterator;
//
//	for (iterator = objectManager->_objectList.Begin(); iterator != objectManager->_objectList.End(); ++iterator)
//	{
//		// 리턴값이 false이면 오브젝트가 죽은 상태
//		if (	(*iterator)->Action()	)
//		{
//
//		}
//	}
//
//	// 버퍼 지우기
//	CConsole::GetInstance()->ClearScreen();
//
//	// draw
//	for (iterator = objectManager->_objectList.Begin(); iterator != objectManager->_objectList.End(); ++iterator)
//	{
//		(*iterator)->Draw();
//	}
//
//	// 화면 표시
//	CConsole::GetInstance()->BufferFlip();
//
//	return true;
//}
//
//void CSceneGame::StageInitial()
//{
//	CObjectManager *objectManager = CObjectManager::GetInstance();
//
//	// 플레이어 생성
//	objectManager->CreatePlayer(CObject::OBJECT_TYPE::PLAYER, 39, 19, true);
//
//	// 적 생성
//	int posX = 17;
//	int posY = 1;
//
//	for (int count = 0; dfENEMY_MAX > count; ++count)
//	{
//		// 10마리 마다 y축 바꾸기
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