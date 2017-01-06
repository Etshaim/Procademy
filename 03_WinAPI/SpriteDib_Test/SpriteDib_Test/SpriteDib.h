#ifndef __SPRITE_DIB__
#define __SPRITE_DIB__

#include "stdafx.h"

class CSpriteDib
{
public:
	struct st_SPRITE
	{
		BYTE	*bypImage;		// ��������Ʈ �̹��� ������
		int		iWidth;			// Width
		int		iHeight;		// Height
		int		iPitch;			// Pitch

		int		iCenterPointX;	// ���� x
		int		iCenterPointY;	// ���� y
	};

	////////////////////////////////////////////////////////////////////////
	// ������, �ı���
	////////////////////////////////////////////////////////////////////////
	CSpriteDib(int iMaxSprite, DWORD dwColorKey);
	virtual ~CSpriteDib();


	////////////////////////////////////////////////////////////////////////
	// LoadDibSprite.
	// BMP ������ �о �ϳ��� ���������� �����Ѵ�
	// Ư�� BMP ������ ���� Index ��ȣ�� ��������Ʈ�� �ε�
	////////////////////////////////////////////////////////////////////////
	BOOL LoadDibSprite(int iSpriteIndex, TCHAR *szFileName, int iCenterPointX, int iCenterPointY);


	////////////////////////////////////////////////////////////////////////
	// ReleaseSprite
	// BMP ������ �о �ϳ��� ���������� ����
	// ���� index ��ȣ�� ��������Ʈ ����
	////////////////////////////////////////////////////////////////////////
	void ReleaseSprite(int iSpriteIndex);

	////////////////////////////////////////////////////////////////////////
	// DrawSprite
	// Ư�� �ޤĸ� ��ġ�� ��������Ʈ�� ����Ѵ�
	// Index�� ��������Ʈ�� Ư�� �޸� ���� x,y ��ǥ�� ���(Į��Ű ó��)
	////////////////////////////////////////////////////////////////////////
	void DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY,
		BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);

	// DrawSprite�� ������ 50%������ ���(��� ��)
	void DrawSprite50(int iSpriteIndex, int iDrawX, int iDrawY,
		BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);

	// DrawSprite�� ������ ���� ���� �迭�� ���(��� ��)
	void DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY,
		BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);

	////////////////////////////////////////////////////////////////////////
	// DrawImage
	// Ư�� �޸� ��ġ�� �̹����� ���( Ŭ���� ó�� )
	// DrawSprite�� ������ Į��Ű ó���� ����( ��¥ �̹��� ��¿� )
	////////////////////////////////////////////////////////////////////////
	void DrawImage(int iSpriteIndex, int iDrawX, int iDrawY,
		BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);

protected:
	//----------------------------------------------------------------------------
	// Sprite �迭 ����
	//----------------------------------------------------------------------------
	int			m_iMaxSprite;	// ��������Ʈ ��ü ����( �����ڿ��� �Է¹��� ��ġ)
	st_SPRITE	*m_stpSprite;	// st_SPRITE ����ü ���� �Ҵ� �迭�� ������

	//----------------------------------------------------------------------------
	// ���� �������� ����� �÷�
	//----------------------------------------------------------------------------
	DWORD		m_dwCololKey;	// ��������Ʈ ó�� �� ���Ǵ� ����� (�÷�Ű)

};

#endif // __SPRITE_DIB__