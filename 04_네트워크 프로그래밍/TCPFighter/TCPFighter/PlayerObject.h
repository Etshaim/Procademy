#ifndef __PLAYER_OBJECT__
#define __PLAYER_OBJECT__

#include "BaseObject.h"

class CPlayerObject : public CBaseObject
{
public :
	CPlayerObject(
		int objectType, WORD wPosX, WORD wPosY, 
		BYTE byHP, BYTE iDir, int iObjectID);

	virtual ~CPlayerObject();

	virtual void	Action(void);
	
	virtual void	Draw(BYTE *bypDest, int iDestWidth, 
						int iDestHeight, int iDestPitch);

public:
	void	ActionProc(void);

	
	void	InputAction(BYTE byAction);


	// 캐릭터가 바라보고 있는 방향 2방향
	void	SetSideDirection(BYTE byDir);
	BYTE	GetSideDirection(void);

	//void	SetMoveDirection(BYTE byMoveDir);
	//BYTE	GetMoveDirection(void);

	BYTE	GetHP(void);
	void	SetHP(BYTE chHP);

	void	InputActionProc(void);
	BOOL	IsPlayer(void);

	// 위치 이동
	void	MovePositon(void);

	

	// 입력 받은 액션으로 세팅
	void	SetActionAttack1(void);
	void	SetActionAttack2(void);
	void	SetActionAttack3(void);
	void	SetActionMove(void);
	void	SetActionStand(void);

	// 내 캐릭터로 설정
	void	SetMyCharacter(void);
	BOOL	CheckMyCharacter(void);


private:
	BOOL	m_bSendPacket;
	BOOL	m_bPlayerCharacter;	// 내 캐릭터 판단
	BYTE	m_byHP;

	BYTE	m_byActionInput;

	BYTE	m_byActionCur;
	BYTE	m_byActionOld;

	// 좌,우 캐릭터 시점
	BYTE	m_bySideDirCur;
	BYTE	m_bySideDirOld;

	//// 8방향 이동 방향
	//BYTE	m_byMoveDirCur;
	//BYTE	m_byMoveDirOld;


};
#endif // !__PLAYER_OBJECT__