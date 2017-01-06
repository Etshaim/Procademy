#include "GameManager.h"
#include "ObjectManager.h"

//using namespace GM;

CGameManager::CGameManager()
{
	StageInitial();
}

CGameManager::~CGameManager()
{
}

bool CGameManager::Process(void)
{
	// 게임 상태 체크
	_state = CheckGameState();

	// 게임오버 시 false리턴
	if (GAMEOVER == GetState())
	{
		// 모든 오브젝트 삭제
		CObjectManager::GetInstance()->_objectList.RemoveAll();

		return false;
	}
	// 게임클리어 시 false리턴
	else if (GAMECLEAR == GetState())
	{
		// 모든 오브젝트 삭제
		CObjectManager::GetInstance()->_objectList.RemoveAll();

		return false;
	}

	CObjectManager *objectManager = CObjectManager::GetInstance();

	// 오브젝트 리스트 순회하며 action
	CDoubleLinkedList<CObject*>::ITERATOR iterator;

	iterator = objectManager->_objectList.Begin();

	while (true)
	{
		// 리턴값이 false이면 오브젝트가 죽은 상태
		// 죽은 오브젝트는 삭제
		if (!(*iterator)->Action())
		{
			//objectManager->_objectList.RemoveAt(iterator++).GetNode());
		}

		iterator++;

		if (iterator == objectManager->_objectList.End())
		{
			break;
		}
	}
	//for (iterator = objectManager->_objectList.Begin(); iterator != objectManager->_objectList.End(); ++iterator)
	//{
	//	// 리턴값이 false이면 오브젝트가 죽은 상태
	//	// 죽은 오브젝트는 삭제
	//	if ( !(*iterator)->Action() )
	//	{
	//		//objectManager->_objectList.RemoveAt(iterator++).GetNode());
	//	}
	//}


	// 버퍼 지우기
	CConsole::GetInstance()->ClearScreen();

	// draw
	for (iterator = objectManager->_objectList.Begin(); iterator != objectManager->_objectList.End(); ++iterator)
	{
		(*iterator)->Draw();
	}

	// 화면 표시
	CConsole::GetInstance()->BufferFlip();

	return true;
}

void CGameManager::StageInitial()
{
	CObjectManager *objectManager = CObjectManager::GetInstance();

	// 플레이어 생성
	objectManager->CreatePlayer(CObject::OBJECT_TYPE::PLAYER, 39, 19, true);

	// 적 생성
	int posX = 17;
	int posY = 1;

	//dfENEMY_MAX
	for (int count = 0; count < dfENEMY_MAX; ++count)
	{
		// 10마리 마다 y축 바꾸기
		if (count % 10 == 0)
		{
			posX = 17;
			posY = posY + 2;
		}

		objectManager->CreateEnemy(CObject::OBJECT_TYPE::ENEMY, posX, posY, true);

		posX = posX + 5;
	}

}
CGameManager::eSTATE CGameManager::GetState(void)
{
	return _state;
}

void CGameManager::SetState(CGameManager::eSTATE state)
{
	_state = state;
}


CGameManager::eSTATE CGameManager::CheckGameState(void)
{
	// 예외 처리
	// 이미 게임 오버 상태(플레이어 죽음)라면 게임오버 상태 유지
	if (GAMEOVER == _state)
	{
		return GAMEOVER;
	}

	CObjectManager *objectManager = CObjectManager::GetInstance();

	CDoubleLinkedList<CObject*>::ITERATOR iterator;

	// 오브젝트 리스트 순회하며 적이 있는지 검사
	for (iterator = objectManager->_objectList.Begin(); iterator != objectManager->_objectList.End(); ++iterator)
	{
		// 만약 적이 있으면
		if ( CObject::OBJECT_TYPE::ENEMY == (*iterator)->CheckObjectType())
		{
			return PLAYING;
		}
	}

	// 이곳까지 도달했다면 적이 없는 상태, 즉 클리어
	return GAMECLEAR;
}