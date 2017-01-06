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
	// ���� ���� üũ
	_state = CheckGameState();

	// ���ӿ��� �� false����
	if (GAMEOVER == GetState())
	{
		// ��� ������Ʈ ����
		CObjectManager::GetInstance()->_objectList.RemoveAll();

		return false;
	}
	// ����Ŭ���� �� false����
	else if (GAMECLEAR == GetState())
	{
		// ��� ������Ʈ ����
		CObjectManager::GetInstance()->_objectList.RemoveAll();

		return false;
	}

	CObjectManager *objectManager = CObjectManager::GetInstance();

	// ������Ʈ ����Ʈ ��ȸ�ϸ� action
	CDoubleLinkedList<CObject*>::ITERATOR iterator;

	iterator = objectManager->_objectList.Begin();

	while (true)
	{
		// ���ϰ��� false�̸� ������Ʈ�� ���� ����
		// ���� ������Ʈ�� ����
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
	//	// ���ϰ��� false�̸� ������Ʈ�� ���� ����
	//	// ���� ������Ʈ�� ����
	//	if ( !(*iterator)->Action() )
	//	{
	//		//objectManager->_objectList.RemoveAt(iterator++).GetNode());
	//	}
	//}


	// ���� �����
	CConsole::GetInstance()->ClearScreen();

	// draw
	for (iterator = objectManager->_objectList.Begin(); iterator != objectManager->_objectList.End(); ++iterator)
	{
		(*iterator)->Draw();
	}

	// ȭ�� ǥ��
	CConsole::GetInstance()->BufferFlip();

	return true;
}

void CGameManager::StageInitial()
{
	CObjectManager *objectManager = CObjectManager::GetInstance();

	// �÷��̾� ����
	objectManager->CreatePlayer(CObject::OBJECT_TYPE::PLAYER, 39, 19, true);

	// �� ����
	int posX = 17;
	int posY = 1;

	//dfENEMY_MAX
	for (int count = 0; count < dfENEMY_MAX; ++count)
	{
		// 10���� ���� y�� �ٲٱ�
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
	// ���� ó��
	// �̹� ���� ���� ����(�÷��̾� ����)��� ���ӿ��� ���� ����
	if (GAMEOVER == _state)
	{
		return GAMEOVER;
	}

	CObjectManager *objectManager = CObjectManager::GetInstance();

	CDoubleLinkedList<CObject*>::ITERATOR iterator;

	// ������Ʈ ����Ʈ ��ȸ�ϸ� ���� �ִ��� �˻�
	for (iterator = objectManager->_objectList.Begin(); iterator != objectManager->_objectList.End(); ++iterator)
	{
		// ���� ���� ������
		if ( CObject::OBJECT_TYPE::ENEMY == (*iterator)->CheckObjectType())
		{
			return PLAYING;
		}
	}

	// �̰����� �����ߴٸ� ���� ���� ����, �� Ŭ����
	return GAMECLEAR;
}