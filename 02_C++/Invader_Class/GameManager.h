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
	
		// 싱글톤
	static CGameManager*	GetInstance(void)
	{
		static CGameManager _gameManager;

		return &_gameManager;
	}

	CDoubleLinkedList<CObject*> objectList;		// 오브젝트 리스트(플레이어, 적, 총알 배열)

	//CObject* objectPool[dfOBJECT_MAX];			
	
	char screenBuffer[dfSCREEN_HEIGHT][dfSCREEN_WIDTH];

	void Get_point();		// 점수 획득
	void _stateChange();		// 상태 변경
	void GameLoop();		// 게임 루프
	e_state	_state;			// 현재 _state

private:
	CGameManager();
	~CGameManager();

	CStage* _scene[e_state::MAX];

	bool GameClearCheck();	// 게임 클리어 체크
	void BufferFlip(void);	// 버퍼의 내용을 화면으로 
	void BufferClear(void);	// 화면 버퍼를 지워주는 함수

	//e_state	_state;			// 현재 _state
	int		_point;			// 현재 점수
	bool	_gameClear;		// 게임 클리어 여부

	


};

#endif // !GAMEMANAGER__H_