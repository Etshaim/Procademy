#ifndef GAMEMANAGER__H_
#define GAMEMANAGER__H_

#include "DoubleLinkedList_Template.h"
#include "Object.h"

//namespace GM
//{

	class CGameManager
	{
	public:
		enum eSTATE
		{
			PLAYING,
			GAMEOVER,
			GAMECLEAR
		};
		// 싱글톤
		static CGameManager*	GetInstance(void)
		{
			static CGameManager _gameManager;

			return &_gameManager;
		}
		bool Process(void);

		void StageInitial(void);	// 스테이지 초기화
		eSTATE GetState(void);		// 게임 상태 얻기
		void SetState(eSTATE);		// 상태 설정

		eSTATE CheckGameState(void);	// 게임 상태 체크


	private:
		CGameManager();
		~CGameManager();

	private:
		bool		_gameClear;		// 게임 클리어 여부
		eSTATE		_state;			// 현재 게임 상태

		//CDoubleLinkedList<CObject*> objectList;		// 오브젝트 리스트(플레이어, 적, 총알 배열)
	};
//}
#endif	//!GAMEMANAGER__H_