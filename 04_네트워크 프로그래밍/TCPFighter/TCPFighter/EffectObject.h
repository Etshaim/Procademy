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
	
	// ������ ID
	DWORD	m_dwAttackID;
	
	// �ǰ��� ID
	DWORD	m_dwDamageID;

	// ���� ����
	int		m_iAttackDir;

};


#endif // !__EFFECT_OBJECT__