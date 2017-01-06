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
		// �̱���
		static CGameManager*	GetInstance(void)
		{
			static CGameManager _gameManager;

			return &_gameManager;
		}
		bool Process(void);

		void StageInitial(void);	// �������� �ʱ�ȭ
		eSTATE GetState(void);		// ���� ���� ���
		void SetState(eSTATE);		// ���� ����

		eSTATE CheckGameState(void);	// ���� ���� üũ


	private:
		CGameManager();
		~CGameManager();

	private:
		bool		_gameClear;		// ���� Ŭ���� ����
		eSTATE		_state;			// ���� ���� ����

		//CDoubleLinkedList<CObject*> objectList;		// ������Ʈ ����Ʈ(�÷��̾�, ��, �Ѿ� �迭)
	};
//}
#endif	//!GAMEMANAGER__H_