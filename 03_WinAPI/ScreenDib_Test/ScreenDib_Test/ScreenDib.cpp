
#include "stdafx.h"
#include "ScreenDib.h"

CScreenDib::CScreenDib(int iWidth, int iHeight, int iColorBit)
	: m_bypBuffer(NULL), m_iWidth(0), m_iHeight(0), m_iPitch(0), m_iColorBit(0), m_iBufferSize(0)
{
	// ��ũ�� ���� ���� �Լ� ȣ��
	CreateDibBuffer(iWidth, iHeight, iColorBit);
}

CScreenDib::~CScreenDib()
{
	ReleaseDibBuffer();
}

void CScreenDib::DrawBuffer(HWND hWnd, int iX, int iY)
{
	// DC ���ϴ� ���
	/*HDC hdc;
	hdc = GetDC(hWnd);
	���� ��¹����� hdc�� ����Ѵ�.
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
	// ��� ���� ���� ����
	m_iWidth	= iWidth;
	m_iHeight	= iHeight;
	m_iColorBit = iColorBit;

	// BITMAPINFO ����
	m_stDibInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_stDibInfo.bmiHeader.biWidth = m_iWidth;
	m_stDibInfo.bmiHeader.biHeight = -m_iHeight;
	m_stDibInfo.bmiHeader.biPlanes = 1;
	m_stDibInfo.bmiHeader.biBitCount = m_iColorBit;
	m_stDibInfo.bmiHeader.biCompression = BI_RGB;

	// �̹��� ����� ����Ͽ� ���ۿ� �̹��� ���� �Ҵ�
	m_iPitch = (  (m_iColorBit >> 3 ) *  m_iWidth + 3 ) & ~3;

	m_iBufferSize = m_iPitch * m_iHeight;

	m_bypBuffer = new BYTE[m_iBufferSize];
}

void CScreenDib::ReleaseDibBuffer(void)
{
	// �޸� ����
	delete []m_bypBuffer;
}