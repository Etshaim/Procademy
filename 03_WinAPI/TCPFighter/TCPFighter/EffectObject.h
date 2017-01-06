#ifndef __EFFECT_OBJECT__
#define __EFFECT_OBJECT__

#include "BaseObject.h"

class CEffectObject : public CBaseObject 
{
public :
	CEffectObject(int objectType, int iPosX, int iPosY, int dwAttackID, int iDirection);
	virtual ~CEffectObject();

public :
	virtual void	Action(void);
	virtual void	Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);

private:
	BOOL	m_bEffectStart;
	
	// ������ ID
	DWORD	m_dwAttackID;

	// ���� ����
	int		m_iAttackDir;

};


#endif // !__EFFECT_OBJECT__