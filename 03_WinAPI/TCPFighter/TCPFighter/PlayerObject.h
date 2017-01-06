#ifndef __PLAYER_OBJECT__
#define __PLAYER_OBJECT__

#include "BaseObject.h"

class CPlayerObject : public CBaseObject
{
public :
	CPlayerObject() {}
	CPlayerObject(int objectType, int iPosX, int iPosY, int iDir, int iObjectID);

	virtual ~CPlayerObject();


	virtual void	Action(void);
	
	virtual void	Draw(BYTE *bypDest, int iDestWidth, 
						int iDestHeight, int iDestPitch);

	void	ActionProc(void);
	int		GetDirection(void);
	char	GetHP(void);
	void	InputActionProc(void);
	BOOL	IsPlayer(void);
	void	SetActionAttack1(void);
	void	SetActionAttack2(void);
	void	SetActionAttack3(void);
	void	SetActionMove(void);
	void	SetActionStand(void);
	void	SetDirection(int iDirection);
	void	SetHP(char chHP);
	//< Effect생성
	void	CreateEffect(int iPosX, int iPosY, int iObjectID);

private:
	BOOL	m_bPlayerCharacter;	// 내 캐릭터 판단
	char	m_chHP;
	DWORD	m_dwActionCur;
	DWORD	m_dwActionOld;
	int		m_iDirCur;
	int		m_iDirOld;

};
#endif // !__PLAYER_OBJECT__