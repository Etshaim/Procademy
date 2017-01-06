#ifndef __BASE_OBJECT__
#define __BASE_OBJECT__

class CBaseObject
{
public:
	CBaseObject(int objectType, WORD wPosX, WORD wPosY, int iObjectID);
	virtual ~CBaseObject();

public:
	virtual void Action(void) = 0;
	virtual void Draw(BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch) = 0;

	WORD GetCurX(void);
	WORD GetCurY(void);

	int GetObjectID(void);
	int GetObjectType(void);
	WORD GetOldX(void);
	WORD GetOldY(void);
	int GetSprite(void);
	
	BOOL IsEndFrame(void);
	void NextFrame(void);

	void SetCurPosition(WORD posX, WORD posY);
	void SetObjectID(int objectID);
	void SetObejctType(int objectType);
	void SetOldPositon(WORD posX, WORD posY);
	void SetPosition(WORD posX, WORD posY);
	void SetSprite(int iSpriteStart, int iSpriteMax, int iFramDelay);

protected:
	BOOL	m_bEndFrame;		// 애니메이션이 끝났다는 것을 확인하는 용도로 사용
								// 1회성 애니메이션 출력 후 파괴 되어야 하는 객체 ( 폭파, 이펙트 등 )
								// 1회 애니메이션 후 다른 동작으로 변경 되어야 하는 액션
	
	WORD	m_wCurX;
	WORD	m_wCurY;

	WORD	m_wOldX;
	WORD	m_wOldY;

	int		m_iDelayCount;
	int		m_iFrameDelay;
	int		m_iObjectID;		// 검색할 때 사용
	int		m_iObjectType;
	
	int		m_iSpriteEnd;
	int		m_iSpriteNow;
	int		m_iSpriteStart;		// ㅇ
	int		m_iSpriteMax;
};


#endif // !__BASE_OBJECT__