#ifndef		SCENEGAME_H__
#define		SCENEGAME_H__

#include "Scene.h"
#include "Object.h"
#include "ObjectManager.h"


class CSceneGame : public CScene
{
public:
	/*enum eSTATE
	{
		PLAYING,
		GAMEOVER,
		GAMECLEAR
	};*/

	CSceneGame();
	virtual ~CSceneGame();

	virtual bool Run(void);

	//void StageInitial(void);	// 스테이지 초기화
	//eSTATE GetState(void);		// 게임 클리어 체크
	//void SetState(eSTATE);		// 상태 설정

//private:
//	//int		_point;			// 현재 점수
//	bool		_gameClear;		// 게임 클리어 여부
//	eSTATE		_state;			// 현재 게임 상태
//
//	CDoubleLinkedList<CObject*> objectList;		// 오브젝트 리스트(플레이어, 적, 총알 배열)
};

#endif	//!SCENEGAME_H__
