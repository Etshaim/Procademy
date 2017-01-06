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
	BOOL	m_bEndFrame;		// �ִϸ��̼��� �����ٴ� ���� Ȯ���ϴ� �뵵�� ���
								// 1ȸ�� �ִϸ��̼� ��� �� �ı� �Ǿ�� �ϴ� ��ü ( ����, ����Ʈ �� )
								// 1ȸ �ִϸ��̼� �� �ٸ� �������� ���� �Ǿ�� �ϴ� �׼�
	DWORD	m_dwActionInput;
	int		m_iCurX;
	int		m_iCurY;
	int		m_iDelayCount;
	int		m_iFrameDelay;
	int		m_iObjectID;		// �˻��� �� ���
	int		m_iObjectType;
	int		m_iOldX;
	int		m_iOldY;
	int		m_iSpriteEnd;
	int		m_iSpriteNow;
	int		m_iSpriteStart;		// ��
	int		m_iSpriteMax;
};


#endif // !__BASE_OBJECT__