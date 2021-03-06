#include "stdafx.h"
#include "SpriteDib.h"

// 생성자, 파괴자
CSpriteDib::CSpriteDib(int iMaxSprite, DWORD dwColorKey)
{
	// 최대 읽어올 개수만큼 미리 할당 받는다.
	m_stpSprite = new st_SPRITE[iMaxSprite];
}

CSpriteDib::~CSpriteDib()
{
	int iCount;

	// 전체를 돌면서 모두 지움
	for (iCount = 0; iCount > m_iMaxSprite; ++iCount)
	{
		ReleaseSprite(iCount);
	}
}
////////////////////////////////////////////////////////////////////////
// LoadDibSprite
// BMP 파일을 읽어서 하나의 프레임으로 저장
////////////////////////////////////////////////////////////////////////
// 특정 BMP 파일을 지정 Index 번호의 스프라이트로 로드
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

	// 파일 포인터 옮길 때 어떻게 옮길 것인가
	// BITMAPFILEHEADER BITMAPINFOHEADER RGBQUAD 차례로 읽는 방법
	// stFileHeader.bfOffBits (이미지가 시작되는 offset 위치)를 이용?

	// 비트맵 헤더를 열어 BMP 파일 확인
	ReadFile(hFile, &stFileHeader, sizeof(BITMAPFILEHEADER), &dwRead, NULL);
	ReadFile(hFile, &stInfoHeader, sizeof(BITMAPINFOHEADER), &dwRead, NULL);
	ReadFile(hFile, rgb, sizeof(RGBQUAD), &dwRead, NULL);
	
		// 한줄, 한줄의 피치값을 구한다.
	iPitch = ((stInfoHeader.biBitCount >> 3) * stInfoHeader.biWidth + 3) & ~3;

		// 스프라이트 구조체에 크기 저장
	m_stpSprite[iSpriteIndex].iWidth = stInfoHeader.biWidth;
	m_stpSprite[iSpriteIndex].iHeight = stInfoHeader.biHeight;
	m_stpSprite[iSpriteIndex].iPitch = iPitch;
	m_stpSprite[iSpriteIndex].iCenterPointX = iCenterPointX;
	m_stpSprite[iSpriteIndex].iCenterPointX = iCenterPointY;

		// 이미지에 대한 전체 크기를 구하고, 메모리 할당
			// 이미지에 대한 크기 구하기
	iImageSize = iPitch * m_stpSprite[iSpriteIndex].iHeight;
				// 또는
	//iImageSize = stFileHeader.bfSize - stFileHeader.bfOffBits;

			// 메모리 할당
	m_stpSprite[iSpriteIndex].bypImage = new BYTE[iImageSize];
	BYTE *pbyTemp = new BYTE[iImageSize];	// 임시 버퍼

	// 임시 버퍼에 읽기
	ReadFile(hFile, pbyTemp, sizeof(iImageSize), &dwRead, NULL);
	
	// 뒤집으면서 복사
	
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

	if (NULL != m_stpSprite[iSpriteIndex].bypImage)
	{
		//--------------------------------------
		// 삭제 후 초기화
		//--------------------------------------
		delete[] m_stpSprite[iSpriteIndex].bypImage;
		memset(	&m_stpSprite[iSpriteIndex], 0, sizeof( st_SPRITE )	);
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
	DWORD *pSrc = (DWORD*) m_stpSprite[iSpriteIndex].bypImage;
	DWORD *pDest = NULL;	// 출력 좌표에 중점 보정된 Y;
	
	int iSpriteWidth;
	int iSpriteHeight;

 	BYTE *pDestOrigin;
	BYTE *pSpriteOrigin;
	//-------------------------------------------
	// 출력 중점으로 좌표 계산
	//-------------------------------------------

	//-------------------------------------------
	// 상단에 대한 스프라이트 출력 위치 계산( 상단 클리핑 )
	// 하단에 사이즈 계산 ( 하단 클리핑 )
	// 왼쪽 출력 위치 계산 ( 좌측 클리핑 )
	// 오른쪽 출력 위치 계산 ( 우측 클리핑 )
	//-------------------------------------------
	
	//-------------------------------------------
	// 화면에 찍을 위치로 이동
	//-------------------------------------------

	//-------------------------------------------
	//전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력
	//-------------------------------------------


}

// DrawSprite와 같으나 50%반투명 출력(없어도 됨)
void CSpriteDib::DrawSprite50(int iSpriteIndex, int iDrawX, int iDrawY,
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{

}

// DrawSprite와 같으나 붉은 색상 계열로 출력(없어도 됨)
void CSpriteDib::DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY,
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{

}

// DrawSprite와 같으나 칼라키 처리가 없음( 통짜 이미지 출력용 )
void CSpriteDib::DrawImage(int iSpriteIndex, int iDrawX, int iDrawY,
	BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100)
{

}