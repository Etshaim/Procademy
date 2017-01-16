#ifndef __EFFECT_OBJECT__
#define __EFFECT_OBJECT__


class CEffectObject : public CBaseObject 
{
public :
	CEffectObject(int objectType, DWORD dwAttackID, DWORD dwDamageID);
	virtual ~CEffectObject();

public :
	virtual void	Action(void);
	virtual void	Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);

private:
	BOOL	m_bEffectStart;
	
	// 공격자 ID
	DWORD	m_dwAttackID;
	
	// 피격자 ID
	DWORD	m_dwDamageID;

	// 공격 방향
	int		m_iAttackDir;

};


#endif // !__EFFECT_OBJECT__