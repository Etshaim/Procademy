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
	BOOL	m_bEndFrame;		// �ִϸ��̼��� �����ٴ� ���� Ȯ���ϴ� �뵵�� ���
								// 1ȸ�� �ִϸ��̼� ��� �� �ı� �Ǿ�� �ϴ� ��ü ( ����, ����Ʈ �� )
								// 1ȸ �ִϸ��̼� �� �ٸ� �������� ���� �Ǿ�� �ϴ� �׼�
	
	WORD	m_wCurX;
	WORD	m_wCurY;

	WORD	m_wOldX;
	WORD	m_wOldY;

	int		m_iDelayCount;
	int		m_iFrameDelay;
	int		m_iObjectID;		// �˻��� �� ���
	int		m_iObjectType;
	
	int		m_iSpriteEnd;
	int		m_iSpriteNow;
	int		m_iSpriteStart;		// ��
	int		m_iSpriteMax;
};


#endif // !__BASE_OBJECT__