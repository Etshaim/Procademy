#ifndef GAMEMANAGER__H_
#define GAMEMANAGER__H_

#include "ObjectManager.h"
#include "Object.h"
#include "Console.h"
#include "DoubleLinkedList_Template.h"
#include "GameStarting.h"

#define		dfOBJECT_MAX	100

class CGameManager
{
public :
	enum e_state
	{
		STARTING,
		PLAYING,
		ENDING,
		MAX
	};
	
		// �̱���
	static CGameManager*	GetInstance(void)
	{
		static CGameManager _gameManager;

		return &_gameManager;
	}

	CDoubleLinkedList<CObject*> objectList;		// ������Ʈ ����Ʈ(�÷��̾�, ��, �Ѿ� �迭)

	//CObject* objectPool[dfOBJECT_MAX];			
	
	char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

	void Get_point();		// ���� ȹ��
	void _stateChange();		// ���� ����
	void GameLoop();		// ���� ����
	e_state	_state;			// ���� _state

private:
	CGameManager();
	~CGameManager();

	CStage* _scene[e_state::MAX];

	bool GameClearCheck();	// ���� Ŭ���� üũ
	void BufferFlip(void);	// ������ ������ ȭ������ 
	void BufferClear(void);	// ȭ�� ���۸� �����ִ� �Լ�

	//e_state	_state;			// ���� _state
	int		_point;			// ���� ����
	bool	_gameClear;		// ���� Ŭ���� ����

	


};

#endif // !GAMEMANAGER__H_