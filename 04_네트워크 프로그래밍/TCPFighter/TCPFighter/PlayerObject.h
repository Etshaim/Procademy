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


	// ĳ���Ͱ� �ٶ󺸰� �ִ� ���� 2����
	void	SetSideDirection(BYTE byDir);
	BYTE	GetSideDirection(void);

	//void	SetMoveDirection(BYTE byMoveDir);
	//BYTE	GetMoveDirection(void);

	BYTE	GetHP(void);
	void	SetHP(BYTE chHP);

	void	InputActionProc(void);
	BOOL	IsPlayer(void);

	// ��ġ �̵�
	void	MovePositon(void);

	

	// �Է� ���� �׼����� ����
	void	SetActionAttack1(void);
	void	SetActionAttack2(void);
	void	SetActionAttack3(void);
	void	SetActionMove(void);
	void	SetActionStand(void);

	// �� ĳ���ͷ� ����
	void	SetMyCharacter(void);
	BOOL	CheckMyCharacter(void);


private:
	BOOL	m_bSendPacket;
	BOOL	m_bPlayerCharacter;	// �� ĳ���� �Ǵ�
	BYTE	m_byHP;

	BYTE	m_byActionInput;

	BYTE	m_byActionCur;
	BYTE	m_byActionOld;

	// ��,�� ĳ���� ����
	BYTE	m_bySideDirCur;
	BYTE	m_bySideDirOld;

	//// 8���� �̵� ����
	//BYTE	m_byMoveDirCur;
	//BYTE	m_byMoveDirOld;


};
#endif // !__PLAYER_OBJECT__