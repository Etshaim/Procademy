#ifndef __BASE_OBJECT__
#define __BASE_OBJECT__

class CBaseObject
{
public:
	CBaseObject()
	{
	}

	CBaseObject(int objectType, int iPosX, int iPosY, int iObjectID);
	virtual ~CBaseObject();

public:
	virtual void Action(void) = 0;
	virtual void Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch) = 0;


	void ActionInput(DWORD dwAction);
	
	int GetCurX(void);
	int GetCurY(void);

	int GetObjectID(void);
	int GetObjectType(void);
	int GetOldX(void);
	int GetOldY(void);
	int GetSprite(void);
	BOOL IsEndFrame(void);

	void NextFrame(void);

	void SetCurPosition(int posX, int posY);
	void SetObjectID(int objectID);
	void SetObejctType(int objectType);
	void SetOldPositon(int posX, int posY);
	void SetPosition(int posX, int posY);
	void SetSprite(int iSpriteStart, int iSpriteMax, int iFramDelay);

protected:
	BOOL	m_bEndFrame;		// 애니메이션이 끝났다는 것을 확인하는 용도로 사용
								// 1회성 애니메이션 출력 후 파괴 되어야 하는 객체 ( 폭파, 이펙트 등 )
								// 1회 애니메이션 후 다른 동작으로 변경 되어야 하는 액션
	DWORD	m_dwActionInput;
	int		m_iCurX;
	int		m_iCurY;
	int		m_iDelayCount;
	int		m_iFrameDelay;
	int		m_iObjectID;		// 검색할 때 사용
	int		m_iObjectType;
	int		m_iOldX;
	int		m_iOldY;
	int		m_iSpriteEnd;
	int		m_iSpriteNow;
	int		m_iSpriteStart;		// ㅇ
	int		m_iSpriteMax;
};


#endif // !__BASE_OBJECT__