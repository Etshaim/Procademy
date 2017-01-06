#ifndef __SPRITE_DIB__
#define __SPRITE_DIB__

#include "stdafx.h"

class CSpriteDib
{
public:
	struct st_SPRITE
	{
		BYTE	*bypImage;		// 스프라이트 이미지 포인터
		int		iWidth;			// Width
		int		iHeight;		// Height
		int		iPitch;			// Pitch

		int		iCenterPointX;	// 중점 x
		int		iCenterPointY;	// 중점 y
	};

	////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자
	////////////////////////////////////////////////////////////////////////
	CSpriteDib(int iMaxSprite, DWORD dwColorKey);
	virtual ~CSpriteDib();


	////////////////////////////////////////////////////////////////////////
	// LoadDibSprite.
	// BMP 파일을 읽어서 하나의 프레임으로 저장한다
	// 특정 BMP 파일을 지정 Index 번호의 스프라이트로 로드
	////////////////////////////////////////////////////////////////////////
	BOOL LoadDibSprite(int iSpriteIndex, TCHAR *szFileName, int iCenterPointX, int iCenterPointY);


	////////////////////////////////////////////////////////////////////////
	// ReleaseSprite
	// BMP 파일을 읽어서 하나의 프레임으로 저장
	// 지정 index 번호의 스프라이트 삭제
	////////////////////////////////////////////////////////////////////////
	void ReleaseSprite(int iSpriteIndex);

	////////////////////////////////////////////////////////////////////////
	// DrawSprite
	// 특정 메ㅔ모리 위치에 스프라이트를 출력한다
	// Index의 스프라이트를 특정 메모리 버퍼 x,y 좌표에 출력(칼라키 처리)
	////////////////////////////////////////////////////////////////////////
	void DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY,
		BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);

	// DrawSprite와 같으나 50%반투명 출력(없어도 됨)
	void DrawSprite50(int iSpriteIndex, int iDrawX, int iDrawY,
		BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);

	// DrawSprite와 같으나 붉은 색상 계열로 출력(없어도 됨)
	void DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY,
		BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch);

	////////////////////////////////////////////////////////////////////////
	// DrawImage
	// 특정 메모리 위치에 이미지를 출력( 클리핑 처리 )
	// DrawSprite와 같으나 칼라키 처리가 없음( 통짜 이미지 출력용 )
	////////////////////////////////////////////////////////////////////////
	void DrawImage(int iSpriteIndex, int iDrawX, int iDrawY,
		BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);

protected:
	//----------------------------------------------------------------------------
	// Sprite 배열 정보
	//----------------------------------------------------------------------------
	int			m_iMaxSprite;	// 스프라이트 전체 개수( 생성자에서 입력받은 수치)
	st_SPRITE	*m_stpSprite;	// st_SPRITE 구조체 동적 할당 배열용 포인터

	//----------------------------------------------------------------------------
	// 투명 색상으로 사용할 컬러
	//----------------------------------------------------------------------------
	DWORD		m_dwCololKey;	// 스프라이트 처리 시 사용되는 투명색 (컬러키)

};

#endif // __SPRITE_DIB__