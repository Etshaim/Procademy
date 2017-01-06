#include "stdafx.h"
#include "SpriteDib.h"
#include "ScreenDIB.h"

#define MYRGB(r,g,b)          ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))

#define MyGetBValue(rgb)      (LOBYTE(rgb))

#define MyGetGValue(rgb)      (LOBYTE(((WORD)(rgb)) >> 8))

#define MyGetRValue(rgb)      (LOBYTE((rgb)>>16))


extern	 CScreenDib		g_cScreenDib;

// 생성자, 파괴자
CSpriteDib::CSpriteDib(int iMaxSprite, DWORD dwColorKey)
	: m_iMaxSprite(iMaxSprite)
{
	// 최대 읽어올 개수만큼 미리 할당 받는다.
	m_stpSprite = new st_SPRITE[ m_iMaxSprite ];
	ZeroMemory(m_stpSprite, sizeof(st_SPRITE)*m_iMaxSprite);
}

CSpriteDib::~CSpriteDib()
{
	int iCount;

	// 전체를 돌면서 모두 지움
	for (iCount = 0; iCount < m_iMaxSprite; ++iCount)
	{
		ReleaseSprite(iCount);
	}

	delete[] m_stpSprite;
}
////////////////////////////////////////////////////////////////////////
// LoadDibSprite
// BMP 파일을 읽어서 하나의 프레임으로 저장
////////////////////////////////////////////////////////////////////////
// 특정 BMP 파일을 지정 Index 번호의 스프라이트로 로드
BOOL CSpriteDib::LoadDibSprite(int iSpriteIndex, TCHAR *szFileName, int iCenterPointX, int iCenterPointY)
{
	HANDLE	hFile;
	DWORD	dwRead;

	int iPitch;
	int iImageSize;

	BITMAPFILEHEADER stFileHeader;
	BITMAPINFOHEADER stInfoHeader;

	//RGBQUAD rgb[256];

	hFile = CreateFile((TCHAR*)szFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		return FALSE;
	}

	// 파일 포인터 옮길 때 어떻게 옮길 것인가
	// BITMAPFILEHEADER BITMAPINFOHEADER RGBQUAD 차례로 읽는 방법
	// stFileHeader.bfOffBits (이미지가 시작되는 offset 위치)를 이용?

	// 비트맵 헤더를 열어 BMP 파일 확인
	if (!ReadFile(hFile, &stFileHeader, sizeof(BITMAPFILEHEADER), &dwRead, NULL))
	{
		return FALSE;
	}

	if (!ReadFile(hFile, &stInfoHeader, sizeof(BITMAPINFOHEADER), &dwRead, NULL))
	{
		return FALSE;
	}
	
	//ReadFile(hFile, rgb, sizeof(RGBQUAD), &dwRead, NULL);

	// 한줄, 한줄의 피치값을 구한다.
	iPitch = ((stInfoHeader.biBitCount >> 3) * stInfoHeader.biWidth + 3) & ~3;

	// 스프라이트 구조체에 크기 저장
	m_stpSprite[iSpriteIndex].iWidth = stInfoHeader.biWidth;
	m_stpSprite[iSpriteIndex].iHeight = stInfoHeader.biHeight;
	m_stpSprite[iSpriteIndex].iPitch = iPitch;
	m_stpSprite[iSpriteIndex].iCenterPointX = iCenterPointX;
	m_stpSprite[iSpriteIndex].iCenterPointY = iCenterPointY;

	// 이미지에 대한 전체 크기를 구하고, 메모리 할당
	// 이미지에 대한 크기 구하기
	iImageSize = iPitch * m_stpSprite[iSpriteIndex].iHeight;
	// 또는
	//iImageSize = stFileHeader.bfSize - stFileHeader.bfOffBits;

	// 메모리 할당
	m_stpSprite[iSpriteIndex].bypImage = new BYTE[iImageSize];
	BYTE *pbyTemp = new BYTE[iImageSize];	// 임시 버퍼

	memset(m_stpSprite[iSpriteIndex].bypImage, 0, iImageSize);
	memset(pbyTemp, 0, iImageSize);

	//< 테스트 용
	//ReadFile(hFile, m_stpSprite[iSpriteIndex].bypImage, iImageSize, &dwRead, NULL);
	
	//임시 버퍼에 읽기
	if (!ReadFile(hFile, pbyTemp, iImageSize, &dwRead, NULL))
	{
		return FALSE;
	}

	// 뒤집으면서 복사
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

// 지정 index 번호의 스프라이트 삭제
void CSpriteDib::ReleaseSprite(int iSpriteIndex)
{
	//----------------------------------------
	// 최대 할당된 스프라이트를 넘어서면 안 됨
	//----------------------------------------
	if (m_iMaxSprite <= iSpriteIndex)
	{
		return;
	}

	if (nullptr != m_stpSprite[iSpriteIndex].bypImage)
	{
		//--------------------------------------
		// 삭제 후 초기화
		//--------------------------------------
		delete[] m_stpSprite[iSpriteIndex].bypImage;
		//memset(&m_stpSprite[iSpriteIndex], 0, sizeof(st_SPRITE));
	}
}



// Index의 스프라이트를 특정 메모리 버퍼 x,y 좌표에 출력(칼라키 처리)
void CSpriteDib::DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY,
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	//-------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-------------------------------------------
	//< 최대 스프라이트 개수 초과
	if (m_iMaxSprite <= iSpriteIndex)
	{
		return;
	}
	//< 로드되지 않는 스프라이트
	if (NULL == m_stpSprite[iSpriteIndex].bypImage)
	{
		return;
	}

	//-------------------------------------------
	// 지역 변수로 필요 정보 셋팅
	//-------------------------------------------
	int iSpritePitch = m_stpSprite[iSpriteIndex].iPitch;

	int iDrawPosX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;	// 출력 좌표에 중점 보정 된 X
	int iDrawPosY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;;	// 출력 좌표에 중점 보정 된 Y

	DWORD *pSrc		= (DWORD*)m_stpSprite[iSpriteIndex].bypImage;		// 스프라이트 이미지 포인터
	DWORD *pDest	= (DWORD*)bypDest;									// 출력 버퍼 포인터 + 출력 좌표에 대한 포인터 계산

	int iSpriteWidth	= m_stpSprite[iSpriteIndex].iWidth;
	// 출력 너비 조절
	iSpriteWidth = iSpriteWidth*iDrawLen / 100;
	
	int iSpriteHeight	= m_stpSprite[iSpriteIndex].iHeight;

	BYTE *pSpriteOrigin	= (BYTE*)pSrc;
	BYTE *pDestOrigin	= (BYTE*)pDest;
	
	

	//-------------------------------------------
	// 상단에 대한 스프라이트 출력 위치 계산( 상단 클리핑 )
	// 하단에 사이즈 계산 ( 하단 클리핑 )
	// 왼쪽 출력 위치 계산 ( 좌측 클리핑 )
	// 오른쪽 출력 위치 계산 ( 우측 클리핑 )
	//-------------------------------------------

	//< 1. 상단 영역 클리핑
	if( iDrawPosY < 0 )
	{
		// 스프라이트 시작 포인터를 잘려진만큼 내림
		pSpriteOrigin += abs(iDrawPosY) * m_stpSprite[iSpriteIndex].iPitch;

		// 스프라이트 높이 값 줄어듬
		iSpriteHeight -= abs(iDrawPosY);	

		// iDrawPos를 0으로
		iDrawPosY = 0;
	}
	
	//< 2. 하단 영역 클리핑
	if ( (iDrawPosY + iSpriteHeight) > iDestHeight)
	{
		// 스프라이트 높이 값 줄어듬
		iSpriteHeight -= (iDrawPosY + iSpriteHeight) - iDestHeight;

		iSpriteHeight = max(0, iSpriteHeight);
	}

	//< 3. 좌측 영역 클리핑
	if (iDrawPosX < 0)
	{
		// 스프라이트 시작 포인터를 잘려진만큼 이동
		pSpriteOrigin += abs(iDrawPosX) * sizeof(DWORD);

		// 스프라이트 너비 값 줄어듬
		iSpriteWidth -= abs(iDrawPosX);

		// iDrawPosX를 0으로
		iDrawPosX = 0;
	}

	//< 4. 우측 영역 클리핑
	if ( (iDrawPosX + iSpriteWidth) > iDestWidth)
	{
		// 너비가 줄어듬
		iSpriteWidth -= ((iDrawPosX + iSpriteWidth) - iDestWidth);
		
		iSpriteWidth = max(0, iSpriteWidth);


	}
	//-------------------------------------------
	// 화면에 찍을 위치로 이동
	//-------------------------------------------
	pDestOrigin += iDrawPosX * sizeof(DWORD) + iDrawPosY * iDestPitch;

	//-------------------------------------------
	//전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력
	//-------------------------------------------
	for (int iCountY = 0; iCountY < iSpriteHeight; ++iCountY)
	{
		pDest	= (DWORD*)pDestOrigin;
		pSrc	= (DWORD*)pSpriteOrigin;

		for (int iCountX = 0; iCountX < iSpriteWidth; ++iCountX)
		{
			if ( ( *pSrc & 0x00ffffff ) != 0x00ffffff )
			{
				*pDest = *pSrc;
			}
			++pDest;
			++pSrc;
		}

		pDestOrigin		+= iDestPitch;		// 버퍼 피치.
		pSpriteOrigin	+= iSpritePitch;	// 스프라이트 피치
	}

}

// DrawSprite와 같으나 50%반투명 출력(없어도 됨)
void CSpriteDib::DrawSprite50(int iSpriteIndex, int iDrawX, int iDrawY,
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	//-------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-------------------------------------------
	//< 최대 스프라이트 개수 초과
	if (m_iMaxSprite <= iSpriteIndex)
	{
		return;
	}
	//< 로드되지 않는 스프라이트
	if (NULL == m_stpSprite[iSpriteIndex].bypImage)
	{
		return;
	}

	//-------------------------------------------
	// 지역 변수로 필요 정보 셋팅
	//-------------------------------------------
	int iSpritePitch = m_stpSprite[iSpriteIndex].iPitch;

	int iDrawPosX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;	// 출력 좌표에 중점 보정 된 X
	int iDrawPosY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;;	// 출력 좌표에 중점 보정 된 Y

	DWORD *pSrc = (DWORD*)m_stpSprite[iSpriteIndex].bypImage;			// 스프라이트 이미지 포인터
	DWORD *pDest = (DWORD*)bypDest;										// 출력 버퍼 포인터 + 출력 좌표에 대한 포인터 계산

	int iSpriteWidth = m_stpSprite[iSpriteIndex].iWidth;
	int iSpriteHeight = m_stpSprite[iSpriteIndex].iHeight;

	BYTE *pSpriteOrigin = (BYTE*)pSrc;
	BYTE *pDestOrigin = (BYTE*)pDest;

	//-------------------------------------------
	// 상단에 대한 스프라이트 출력 위치 계산( 상단 클리핑 )
	// 하단에 사이즈 계산 ( 하단 클리핑 )
	// 왼쪽 출력 위치 계산 ( 좌측 클리핑 )
	// 오른쪽 출력 위치 계산 ( 우측 클리핑 )
	//-------------------------------------------

	//< 1. 상단 영역 클리핑
	if (iDrawPosY < 0)
	{
		// 스프라이트 시작 포인터를 잘려진만큼 내림
		pSpriteOrigin += abs(iDrawPosY) * m_stpSprite[iSpriteIndex].iPitch;

		// 스프라이트 높이 값 줄어듬
		iSpriteHeight -= abs(iDrawPosY);

		// iDrawPos를 0으로
		iDrawPosY = 0;
	}

	//< 2. 하단 영역 클리핑
	if ((iDrawPosY + iSpriteHeight) > iDestHeight)
	{
		// 스프라이트 높이 값 줄어듬
		iSpriteHeight -= (iDrawPosY + iSpriteHeight) - iDestHeight;

		iSpriteHeight = max(0, iSpriteHeight);
	}

	//< 3. 좌측 영역 클리핑
	if (iDrawPosX < 0)
	{
		// 스프라이트 시작 포인터를 잘려진만큼 이동
		pSpriteOrigin += abs(iDrawPosX) * sizeof(DWORD);

		// 스프라이트 너비 값 줄어듬
		iSpriteWidth -= abs(iDrawPosX);

		// iDrawPosX를 0으로
		iDrawPosX = 0;
	}

	//< 4. 우측 영역 클리핑
	if ((iDrawPosX + iSpriteWidth) > iDestWidth)
	{
		// 너비가 줄어듬
		iSpriteWidth -= (iDrawPosX + iSpriteWidth) - iDestWidth;

		iSpriteWidth = max(0, iSpriteWidth);
	}
	//-------------------------------------------
	// 화면에 찍을 위치로 이동
	//-------------------------------------------
	pDestOrigin += iDrawPosX * sizeof(DWORD) + iDrawPosY * iDestPitch;

	//-------------------------------------------
	//전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력
	//-------------------------------------------
	for (int iCountY = 0; iCountY < iSpriteHeight; ++iCountY)
	{
		pDest = (DWORD*)pDestOrigin;
		pSrc = (DWORD*)pSpriteOrigin;

		for (int iCountX = 0; iCountX < iSpriteWidth; ++iCountX)
		{
			if ((*pSrc & 0x00ffffff) != 0x00ffffff)
			{
				// 반투명 효과
				// >>1 하고 최상위 1bit 없애기

				DWORD srcTemp = *pSrc;
				DWORD dstTemp = *pDest;

				*pDest = ( (srcTemp >> 1) & 0x007f7f7f ) + ( (dstTemp >> 1) & 0x007f7f7f );

				//*pDest = *pSrc;
			}
			++pDest;
			++pSrc;
		}

		pDestOrigin += iDestPitch;		// 버퍼 피치.
		pSpriteOrigin += iSpritePitch;	// 스프라이트 피치
	}
}

// DrawSprite와 같으나 붉은 색상 계열로 출력(없어도 됨)
void CSpriteDib::DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY,
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	//-------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-------------------------------------------
	//< 최대 스프라이트 개수 초과
	if (m_iMaxSprite <= iSpriteIndex)
	{
		return;
	}
	//< 로드되지 않는 스프라이트
	if (NULL == m_stpSprite[iSpriteIndex].bypImage)
	{
		return;
	}

	//-------------------------------------------
	// 지역 변수로 필요 정보 셋팅
	//-------------------------------------------
	int iSpritePitch = m_stpSprite[iSpriteIndex].iPitch;

	int iDrawPosX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;	// 출력 좌표에 중점 보정 된 X
	int iDrawPosY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;;	// 출력 좌표에 중점 보정 된 Y

	DWORD *pSrc = (DWORD*)m_stpSprite[iSpriteIndex].bypImage;		// 스프라이트 이미지 포인터
	DWORD *pDest = (DWORD*)bypDest;									// 출력 버퍼 포인터 + 출력 좌표에 대한 포인터 계산

	int iSpriteWidth = m_stpSprite[iSpriteIndex].iWidth;
	int iSpriteHeight = m_stpSprite[iSpriteIndex].iHeight;

	BYTE *pSpriteOrigin = (BYTE*)pSrc;
	BYTE *pDestOrigin = (BYTE*)pDest;

	//-------------------------------------------
	// 상단에 대한 스프라이트 출력 위치 계산( 상단 클리핑 )
	// 하단에 사이즈 계산 ( 하단 클리핑 )
	// 왼쪽 출력 위치 계산 ( 좌측 클리핑 )
	// 오른쪽 출력 위치 계산 ( 우측 클리핑 )
	//-------------------------------------------

	//< 1. 상단 영역 클리핑
	if (iDrawPosY < 0)
	{
		// 스프라이트 시작 포인터를 잘려진만큼 내림
		pSpriteOrigin += abs(iDrawPosY) * m_stpSprite[iSpriteIndex].iPitch;

		// 스프라이트 높이 값 줄어듬
		iSpriteHeight -= abs(iDrawPosY);

		// iDrawPos를 0으로
		iDrawPosY = 0;
	}

	//< 2. 하단 영역 클리핑
	if ((iDrawPosY + iSpriteHeight) > iDestHeight)
	{
		// 스프라이트 높이 값 줄어듬
		iSpriteHeight -= (iDrawPosY + iSpriteHeight) - iDestHeight;

		iSpriteHeight = max(0, iSpriteHeight);
	}

	//< 3. 좌측 영역 클리핑
	if (iDrawPosX < 0)
	{
		// 스프라이트 시작 포인터를 잘려진만큼 이동
		pSpriteOrigin += abs(iDrawPosX) * sizeof(DWORD);

		// 스프라이트 너비 값 줄어듬
		iSpriteWidth -= abs(iDrawPosX);

		// iDrawPosX를 0으로
		iDrawPosX = 0;
	}

	//< 4. 우측 영역 클리핑
	if ((iDrawPosX + iSpriteWidth) > iDestWidth)
	{
		// 너비가 줄어듬
		iSpriteWidth -= ((iDrawPosX + iSpriteWidth) - iDestWidth);

		iSpriteWidth = max(0, iSpriteWidth);


	}
	//-------------------------------------------
	// 화면에 찍을 위치로 이동
	//-------------------------------------------
	pDestOrigin += iDrawPosX * sizeof(DWORD) + iDrawPosY * iDestPitch;

	//-------------------------------------------
	//전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력
	//-------------------------------------------
	for (int iCountY = 0; iCountY < iSpriteHeight; ++iCountY)
	{
		pDest = (DWORD*)pDestOrigin;
		pSrc = (DWORD*)pSpriteOrigin;

		for (int iCountX = 0; iCountX < iSpriteWidth; ++iCountX)
		{
			if ((*pSrc & 0x00ffffff) != 0x00ffffff)
			{
				DWORD dwTemp	= *pSrc;
				BYTE *pbyTemp	= (BYTE*)&dwTemp;

				BYTE r = MyGetRValue(dwTemp);
				BYTE g = MyGetGValue(dwTemp) / 2;;
				BYTE b = MyGetBValue(dwTemp) / 2;

				/*BYTE r = *(pbyTemp + 2);
				BYTE g = (*(pbyTemp + 3)) / 2;
				BYTE b = (*(pbyTemp + 4)) / 2;*/
				
				*pDest = MYRGB(r, g, b);

				//*pDest = dwTemp;

			}
			++pDest;
			++pSrc;
		}

		pDestOrigin += iDestPitch;		// 버퍼 피치.
		pSpriteOrigin += iSpritePitch;	// 스프라이트 피치
	}
}

// DrawSprite와 같으나 칼라키 처리가 없음( 통짜 이미지 출력용 )
void CSpriteDib::DrawImage(int iSpriteIndex, int iDrawX, int iDrawY,
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen)
{
	//-------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-------------------------------------------
	//< 최대 스프라이트 개수 초과
	if (m_iMaxSprite <= iSpriteIndex)
	{
		return;
	}
	//< 로드되지 않는 스프라이트
	if (NULL == m_stpSprite[iSpriteIndex].bypImage)
	{
		return;
	}

	//-------------------------------------------
	// 지역 변수로 필요 정보 셋팅
	//-------------------------------------------
	int iSpritePitch = m_stpSprite[iSpriteIndex].iPitch;

	int iDrawPosX = iDrawX - m_stpSprite[iSpriteIndex].iCenterPointX;	// 출력 좌표에 중점 보정 된 X
	int iDrawPosY = iDrawY - m_stpSprite[iSpriteIndex].iCenterPointY;;	// 출력 좌표에 중점 보정 된 Y

	DWORD *pSrc = (DWORD*)m_stpSprite[iSpriteIndex].bypImage;		// 스프라이트 이미지 포인터
	DWORD *pDest = (DWORD*)bypDest;									// 출력 버퍼 포인터 + 출력 좌표에 대한 포인터 계산

	int iSpriteWidth = m_stpSprite[iSpriteIndex].iWidth;
	int iSpriteHeight = m_stpSprite[iSpriteIndex].iHeight;

	BYTE *pSpriteOrigin = (BYTE*)pSrc;
	BYTE *pDestOrigin = (BYTE*)pDest;

	//-------------------------------------------
	// 상단에 대한 스프라이트 출력 위치 계산( 상단 클리핑 )
	// 하단에 사이즈 계산 ( 하단 클리핑 )
	// 왼쪽 출력 위치 계산 ( 좌측 클리핑 )
	// 오른쪽 출력 위치 계산 ( 우측 클리핑 )
	//-------------------------------------------

	//< 1. 상단 영역 클리핑
	if (iDrawPosY < 0)
	{
		// 스프라이트 시작 포인터를 잘려진만큼 내림
		pSpriteOrigin += abs(iDrawPosY) * m_stpSprite[iSpriteIndex].iPitch;

		// 스프라이트 높이 값 줄어듬
		iSpriteHeight -= abs(iDrawPosY);

		// iDrawPos를 0으로
		iDrawPosY = 0;
	}

	//< 2. 하단 영역 클리핑
	if ((iDrawPosY + iSpriteHeight) > iDestHeight)
	{
		// 스프라이트 높이 값 줄어듬
		iSpriteHeight -= (iDrawPosY + iSpriteHeight) - iDestHeight;

		iSpriteHeight = max(0, iSpriteHeight);
	}

	//< 3. 좌측 영역 클리핑
	if (iDrawPosX < 0)
	{
		// 스프라이트 시작 포인터를 잘려진만큼 이동
		pSpriteOrigin += abs(iDrawPosX) * sizeof(DWORD);

		// 스프라이트 너비 값 줄어듬
		iSpriteWidth -= abs(iDrawPosX);

		// iDrawPosX를 0으로
		iDrawPosX = 0;
	}

	//< 4. 우측 영역 클리핑
	if ((iDrawPosX + iSpriteWidth) > iDestWidth)
	{
		// 너비가 줄어듬
		iSpriteWidth -= ((iDrawPosX + iSpriteWidth) - iDestWidth);

		iSpriteWidth = max(0, iSpriteWidth);


	}
	//-------------------------------------------
	// 화면에 찍을 위치로 이동
	//-------------------------------------------
	pDestOrigin += iDrawPosX * sizeof(DWORD) + iDrawPosY * iDestPitch;

	//-------------------------------------------
	//전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력
	//-------------------------------------------
	for (int iCountY = 0; iCountY < iSpriteHeight; ++iCountY)
	{
		pDest = (DWORD*)pDestOrigin;
		pSrc = (DWORD*)pSpriteOrigin;

		for (int iCountX = 0; iCountX < iSpriteWidth; ++iCountX)
		{
			*pDest = *pSrc;

			++pDest;
			++pSrc;
		}

		pDestOrigin += iDestPitch;		// 버퍼 피치.
		pSpriteOrigin += iSpritePitch;	// 스프라이트 피치
	}
}