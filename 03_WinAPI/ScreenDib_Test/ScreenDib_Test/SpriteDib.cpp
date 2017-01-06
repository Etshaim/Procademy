#include "stdafx.h"
#include "SpriteDib.h"

// ������, �ı���
CSpriteDib::CSpriteDib(int iMaxSprite, DWORD dwColorKey)
{
	// �ִ� �о�� ������ŭ �̸� �Ҵ� �޴´�.
	m_stpSprite = new st_SPRITE[iMaxSprite];
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
BOOL CSpriteDib::LoadDibSprite(int iSpriteIndex, char *szFileName, int iCenterPointX, int iCenterPointY)
{
	HANDLE hFile;
	DWORD	dwRead;

	int iPitch;
	int iImageSize;

	BITMAPFILEHEADER stFileHeader;
	BITMAPINFOHEADER stInfoHeader;
	
	RGBQUAD rgb[256];

	hFile = CreateFile((LPCWSTR)szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	if ( INVALID_HANDLE_VALUE == hFile )
	{
		return FALSE;
	}

	// ���� ������ �ű� �� ��� �ű� ���ΰ�
	// BITMAPFILEHEADER BITMAPINFOHEADER RGBQUAD ���ʷ� �д� ���
	// stFileHeader.bfOffBits (�̹����� ���۵Ǵ� offset ��ġ)�� �̿�?

	// ��Ʈ�� ����� ���� BMP ���� Ȯ��
	ReadFile(hFile, &stFileHeader, sizeof(BITMAPFILEHEADER), &dwRead, NULL);
	ReadFile(hFile, &stInfoHeader, sizeof(BITMAPINFOHEADER), &dwRead, NULL);
	ReadFile(hFile, rgb, sizeof(RGBQUAD), &dwRead, NULL);
	
		// ����, ������ ��ġ���� ���Ѵ�.
	iPitch = ((stInfoHeader.biBitCount >> 3) * stInfoHeader.biWidth + 3) & ~3;

		// ��������Ʈ ����ü�� ũ�� ����
	m_stpSprite[iSpriteIndex].iWidth = stInfoHeader.biWidth;
	m_stpSprite[iSpriteIndex].iHeight = stInfoHeader.biHeight;
	m_stpSprite[iSpriteIndex].iPitch = iPitch;
	m_stpSprite[iSpriteIndex].iCenterPointX = iCenterPointX;
	m_stpSprite[iSpriteIndex].iCenterPointX = iCenterPointY;

		// �̹����� ���� ��ü ũ�⸦ ���ϰ�, �޸� �Ҵ�
			// �̹����� ���� ũ�� ���ϱ�
	iImageSize = iPitch * m_stpSprite[iSpriteIndex].iHeight;
				// �Ǵ�
	//iImageSize = stFileHeader.bfSize - stFileHeader.bfOffBits;

			// �޸� �Ҵ�
	m_stpSprite[iSpriteIndex].bypImage = new BYTE[iImageSize];
	BYTE *pbyTemp = new BYTE[iImageSize];	// �ӽ� ����

	// �ӽ� ���ۿ� �б�
	ReadFile(hFile, pbyTemp, sizeof(iImageSize), &dwRead, NULL);
	
	// �������鼭 ����
	
	BYTE *dst = m_stpSprite[iSpriteIndex].bypImage;
	BYTE *src = pbyTemp;

	int iHeight = m_stpSprite[iSpriteIndex].iHeight;

	for (int iCount = 0; iCount < iHeight; ++iCount)
	{
		memcpy(&dst[iHeight - iCount - 1], src, iPitch);
		src += iPitch;
	}
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
	delete[] pbyTemp;

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
		memset(	&m_stpSprite[iSpriteIndex], 0, sizeof( st_SPRITE )	);
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
	DWORD *pSrc = (DWORD*) m_stpSprite[iSpriteIndex].bypImage;
	DWORD *pDest = NULL;	// ��� ��ǥ�� ���� ������ Y;
	
	int iSpriteWidth;
	int iSpriteHeight;

 	BYTE *pDestOrigin;
	BYTE *pSpriteOrigin;
	//-------------------------------------------
	// ��� �������� ��ǥ ���
	//-------------------------------------------

	//-------------------------------------------
	// ��ܿ� ���� ��������Ʈ ��� ��ġ ���( ��� Ŭ���� )
	// �ϴܿ� ������ ��� ( �ϴ� Ŭ���� )
	// ���� ��� ��ġ ��� ( ���� Ŭ���� )
	// ������ ��� ��ġ ��� ( ���� Ŭ���� )
	//-------------------------------------------
	
	//-------------------------------------------
	// ȭ�鿡 ���� ��ġ�� �̵�
	//-------------------------------------------

	//-------------------------------------------
	//��ü ũ�⸦ ���鼭 �ȼ����� ����� ó���� �ϸ� �׸� ���
	//-------------------------------------------


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
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100)
{

}