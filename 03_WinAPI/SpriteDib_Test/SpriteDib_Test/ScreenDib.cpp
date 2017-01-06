#include "stdafx.h"
#include "ScreenDib.h"

CScreenDib::CScreenDib(int iWidth, int iHeight, int iColorBit)
	: m_bypBuffer(NULL), m_iWidth(0), m_iHeight(0), m_iPitch(0), m_iColorBit(0), m_iBufferSize(0)
{
	// 스크린 버퍼 생성 함수 호출
	CreateDibBuffer(iWidth, iHeight, iColorBit);
}

CScreenDib::~CScreenDib()
{
	ReleaseDibBuffer();
}

void CScreenDib::DrawBuffer(HWND hWnd, int iX, int iY)
{
	// DC 구하는 방법
	/*HDC hdc;
	hdc = GetDC(hWnd);
	각종 출력문에서 hdc를 사용한다.
	ReleaseDC(hWnd, hdc);*/

	HDC hdc;

	hdc = GetDC(hWnd);

	SetDIBitsToDevice(hdc,
		iX,
		iY,
		m_iWidth,
		m_iHeight,
		0,
		0,
		0,
		m_iHeight,
		m_bypBuffer,
		&m_stDibInfo,
		DIB_RGB_COLORS);

	ReleaseDC(hWnd, hdc);
}

BYTE* CScreenDib::GetDibBuffer(void)
{
	return m_bypBuffer;
}

int CScreenDib::GetWidth(void)
{
	return m_iWidth;
}

int CScreenDib::GetHeight(void)
{
	return m_iHeight;
}

int CScreenDib::GetPitch(void)
{
	return m_iPitch;
}

void CScreenDib::CreateDibBuffer(int iWidth, int iHeight, int iColorBit)
{
	// 멤버 변수 정보 세팅
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	m_iColorBit = iColorBit;

	// BITMAPINFO 세팅

	m_stDibInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_stDibInfo.bmiHeader.biWidth = m_iWidth;
	m_stDibInfo.bmiHeader.biHeight = -m_iHeight;
	m_stDibInfo.bmiHeader.biPlanes = 1;
	m_stDibInfo.bmiHeader.biBitCount = m_iColorBit;
	m_stDibInfo.bmiHeader.biCompression = BI_RGB;

	// 이미지 사이즈를 계산하여 버퍼용 이미지 동적 할당
	m_iPitch = ((m_iColorBit >> 3) * (m_iWidth)+3) & ~3;

	m_iBufferSize = m_iPitch * m_iHeight;

	m_bypBuffer = new BYTE[m_iBufferSize];

	memset(m_bypBuffer, 0xff, m_iBufferSize);
}

void CScreenDib::ReleaseDibBuffer(void)
{
	// 메모리 해제
	delete[]m_bypBuffer;
}