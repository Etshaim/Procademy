#include "stdafx.h"
#include "SpriteDib.h"
#include "ScreenDIB.h"

extern	 CScreenDib		g_cScreenDib;

// ������, �ı���
CSpriteDib::CSpriteDib(int iMaxSprite, DWORD dwColorKey)
	: m_iMaxSprite(iMaxSprite)
{
	// �ִ� �о�� ������ŭ �̸� �Ҵ� �޴´�.
	m_stpSprite = new st_SPRITE[m_iMaxSprite];
}

CSpriteDib::~CSpriteDib()
{
	int iCount;

	// ��ü�� ���鼭 ��� ����
	for (iCount = 0; iCount > m_iMaxSprite; ++iCount)
	{
		ReleaseSprite(iCount);
	}
}
////////////////////////////////////////////////////////////////////////
// LoadDibSprite
// BMP ������ �о �ϳ��� ���������� ����
////////////////////////////////////////////////////////////////////////
// Ư�� BMP ������ ���� Index ��ȣ�� ��������Ʈ�� �ε�
BOOL CSpriteDib::LoadDibSprite(int iSpriteIndex, TCHAR *szFileName, int iCenterPointX, int iCenterPointY)
{
	HANDLE hFile;
	DWORD	dwRead;

	int iPitch;
	int iImageSize;

	BITMAPFILEHEADER stFileHeader;
	BITMAPINFOHEADER stInfoHeader;

	//RGBQUAD rgb[256];

	hFile = CreateFile((TCHAR*)szFileName/*L"Stand_L_01.bmp"*/, GENERIC_READ, NULL, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return FALSE;
	}

	// ���� ������ �ű� �� ��� �ű� ���ΰ�
	// BITMAPFILEHEADER BITMAPINFOHEADER RGBQUAD ���ʷ� �д� ���
	// stFileHeader.bfOffBits (�̹����� ���۵Ǵ� offset ��ġ)�� �̿�?

	// ��Ʈ�� ����� ���� BMP ���� Ȯ��
	ReadFile(hFile, &stFileHeader, sizeof(BITMAPFILEHEADER), &dwRead, NULL);
	ReadFile(hFile, &stInfoHeader, sizeof(BITMAPINFOHEADER), &dwRead, NULL);
	//ReadFile(hFile, rgb, sizeof(RGBQUAD), &dwRead, NULL);

	// ����, ������ ��ġ���� ���Ѵ�.
	iPitch = ((stInfoHeader.biBitCount >> 3) * stInfoHeader.biWidth + 3) & ~3;

	// ��������Ʈ ����ü�� ũ�� ����
	m_stpSprite[iSpriteIndex].iWidth = stInfoHeader.biWidth;
	m_stpSprite[iSpriteIndex].iHeight = stInfoHeader.biHeight;
	m_stpSprite[iSpriteIndex].iPitch = iPitch;
	m_stpSprite[iSpriteIndex].iCenterPointX = iCenterPointX;
	m_stpSprite[iSpriteIndex].iCenterPointY = iCenterPointY;

	// �̹����� ���� ��ü ũ�⸦ ���ϰ�, �޸� �Ҵ�
	// �̹����� ���� ũ�� ���ϱ�
	iImageSize = iPitch * m_stpSprite[iSpriteIndex].iHeight;
	// �Ǵ�
	//iImageSize = stFileHeader.bfSize - stFileHeader.bfOffBits;

	// �޸� �Ҵ�
	m_stpSprite[iSpriteIndex].bypImage = new BYTE[iImageSize];
	BYTE *pbyTemp = new BYTE[iImageSize];	// �ӽ� ����

	memset(m_stpSprite[iSpriteIndex].bypImage, 0, iImageSize);
	memset(pbyTemp, 0, iImageSize);

	//< �׽�Ʈ ��
	//ReadFile(hFile, m_stpSprite[iSpriteIndex].bypImage, iImageSize, &dwRead, NULL);

	//�ӽ� ���ۿ� �б�
	ReadFile(hFile, pbyTemp, iImageSize, &dwRead, NULL);

	// �������鼭 ����
	int iHeight = m_stpSprite[iSpriteIndex].iHeight;

	BYTE *dst = m_stpSprite[iSpriteIndex].bypImage;
	BYTE *src = pbyTemp + (iHeight - 1) * iPitch;

	for (int iCount = 0; iCount < iHeight; ++iCount)
	{
		memcpy(dst, src, iPitch);
		dst += iPitch;
		src -= iPitch;
	}

	delete[] pbyTemp;
	pbyTemp = NULL;
	src = NULL;

	CloseHandle(hFile);
	return TRUE;
}

// ���� index ��ȣ�� ��������Ʈ ����
void CSpriteDib::ReleaseSprite(int iSpriteIndex)
{
	//----------------------------------------
	// �ִ� �Ҵ�� ��������Ʈ�� �Ѿ�� �� ��
	//----------------------------------------
	if (m_iMaxSprite <= iSpriteIndex)
	{
		return;
	}

	if (NULL != m_stpSprite[iSpriteIndex].bypImage)
	{
		//--------------------------------------
		// ���� �� �ʱ�ȭ
		//--------------------------------------
		delete[] m_stpSprite[iSpriteIndex].bypImage;
		memset(&m_stpSprite[iSpriteIndex], 0, sizeof(st_SPRITE));
	}
}

// Index�� ��������Ʈ�� Ư�� �޸� ���� x,y ��ǥ�� ���(Į��Ű ó��)
void CSpriteDib::DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY,
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	//-------------------------------------------
	// �ִ� ��������Ʈ ������ �ʰ� �ϰų�, �ε���� �ʴ� ��������Ʈ��� ����
	//-------------------------------------------
	//< �ִ� ��������Ʈ ���� �ʰ�
	if (m_iMaxSprite <= iSpriteIndex)
	{
		return;
	}
	//< �ε���� �ʴ� ��������Ʈ
	if (NULL == m_stpSprite[iSpriteIndex].bypImage)
	{
		return;
	}

	//-------------------------------------------
	// ���� ������ �ʿ� ���� ����
	//-------------------------------------------
	int iSpritePitch = m_stpSprite[iSpriteIndex].iPitch;

	int iDrawPosX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;	// ��� ��ǥ�� ���� ���� �� X
	int iDrawPosY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;;	// ��� ��ǥ�� ���� ���� �� Y

	DWORD *pSrc = (DWORD*)m_stpSprite[iSpriteIndex].bypImage;			// ��������Ʈ �̹��� ������
	DWORD *pDest = (DWORD*)bypDest;										// ��� ���� ������ + ��� ��ǥ�� ���� ������ ���

	int iSpriteWidth = m_stpSprite[iSpriteIndex].iWidth;
	int iSpriteHeight = m_stpSprite[iSpriteIndex].iHeight;

	BYTE *pSpriteOrigin = (BYTE*)pSrc;
	BYTE *pDestOrigin = (BYTE*)pDest;

	//-------------------------------------------
	// ��ܿ� ���� ��������Ʈ ��� ��ġ ���( ��� Ŭ���� )
	// �ϴܿ� ������ ��� ( �ϴ� Ŭ���� )
	// ���� ��� ��ġ ��� ( ���� Ŭ���� )
	// ������ ��� ��ġ ��� ( ���� Ŭ���� )
	//-------------------------------------------

	//< 1. ��� ���� Ŭ����
	if (iDrawPosY < 0)
	{
		// ��������Ʈ ���� �����͸� �߷�����ŭ ����
		pSpriteOrigin += abs(iDrawPosY) * m_stpSprite[iSpriteIndex].iPitch;

		// ��������Ʈ ���� �� �پ��
		iSpriteHeight -= abs(iDrawPosY);

		// iDrawPos�� 0����
		iDrawPosY = 0;
	}

	//< 2. �ϴ� ���� Ŭ����
	if ((iDrawPosY + iSpriteHeight) > iDestHeight)
	{
		// ��������Ʈ ���� �� �پ��
		iSpriteHeight -= (iDrawPosY + iSpriteHeight) - iDestHeight;

		iSpriteHeight = max(0, iSpriteHeight);
	}

	//< 3. ���� ���� Ŭ����
	if (iDrawPosX < 0)
	{
		// ��������Ʈ ���� �����͸� �߷�����ŭ �̵�
		pSpriteOrigin += abs(iDrawPosX) * sizeof(DWORD);

		// ��������Ʈ �ʺ� �� �پ��
		iSpriteWidth -= abs(iDrawPosX);

		// iDrawPosX�� 0����
		iDrawPosX = 0;
	}

	//< 4. ���� ���� Ŭ����
	if ((iDrawPosX + iSpriteWidth) > iDestWidth)
	{
		// �ʺ� �پ��
		iSpriteWidth -= (iDrawPosX + iSpriteWidth) - iDestWidth;

		iSpriteHeight = max(0, iSpriteWidth);
	}
	//-------------------------------------------
	// ȭ�鿡 ���� ��ġ�� �̵�
	//-------------------------------------------
	pDestOrigin += iDrawPosX * sizeof(DWORD) + iDrawPosY * iDestPitch;

	//-------------------------------------------
	//��ü ũ�⸦ ���鼭 �ȼ����� ����� ó���� �ϸ� �׸� ���
	//-------------------------------------------
	for (int iCountY = 0; iCountY < iSpriteHeight; ++iCountY)
	{
		pDest = (DWORD*)pDestOrigin;
		pSrc = (DWORD*)pSpriteOrigin;

		for (int iCountX = 0; iCountX < iSpriteWidth; ++iCountX)
		{
			if ((*pSrc & 0x00ffffff) != 0x00ffffff)
			{
				*pDest = *pSrc;
			}
			++pDest;
			++pSrc;
		}

		pDestOrigin += iDestPitch;		// ���� ��ġ.
		pSpriteOrigin += iSpritePitch;	// ��������Ʈ ��ġ
	}

}

// DrawSprite�� ������ 50%������ ���(��� ��)
void CSpriteDib::DrawSprite50(int iSpriteIndex, int iDrawX, int iDrawY,
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{

}

// DrawSprite�� ������ ���� ���� �迭�� ���(��� ��)
void CSpriteDib::DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY,
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{

}

// DrawSprite�� ������ Į��Ű ó���� ����( ��¥ �̹��� ��¿� )
void CSpriteDib::DrawImage(int iSpriteIndex, int iDrawX, int iDrawY,
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{

}