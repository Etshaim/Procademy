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

	//void StageInitial(void);	// �������� �ʱ�ȭ
	//eSTATE GetState(void);		// ���� Ŭ���� üũ
	//void SetState(eSTATE);		// ���� ����

//private:
//	//int		_point;			// ���� ����
//	bool		_gameClear;		// ���� Ŭ���� ����
//	eSTATE		_state;			// ���� ���� ����
//
//	CDoubleLinkedList<CObject*> objectList;		// ������Ʈ ����Ʈ(�÷��̾�, ��, �Ѿ� �迭)
};

#endif	//!SCENEGAME_H__
