CFile file;
BITMAPFILEHEADER bmfh;
DWORD dwFileSize, dwDibSize;
BYTE* pDib = new BYTE[dwDibSize];

// 테스트 bmp 읽어오기 
file.Open(_T("test.bmp"), CFile::modeRead | CFile::shareDenyWrite, NULL);
dwFileSize = (DWORD)file.GetLength();
// 전체사이즈에서 BITMAPFILEHEADER 을 빼면 DIB 사이즈가 된다
dwDibSize = dwFileSize - sizeof(BITMAPFILEHEADER);

file.Read(&bmfh, sizeof(BITMAPFILEHEADER));// fileheader 구조체 읽기
file.Read(pDib, dwDibSize);// DIB 읽기
file.Close();

BITMAPINFOHEADER* pBmh = (BITMAPINFOHEADER*)pDib;
int nWidth = pBmh->biWidth;
int nHeight = pBmh->biHeight;
int nBit = pBmh->biBitCount;
BYTE* lpBits = NULL;

// 실제 데이타 위치 
if (nBit > 8)
{
	lpBits = (BYTE*)pDib + sizeof(BITMAPINFOHEADER);
}
else
{
	lpBits = (BYTE*)pDib + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << nBit);
}
// 데이타 화면에 표현 
CClinetDC dc(this);

// StretchDIBits 이용
::StretchDIBits(dc.m_hDC,  // handle to device context
	nWidth + 10,    // x-coordinate of upper-left corner of dest. rectangle
	0,                  // y-coordinate of upper-left corner of dest. rectangle
	nWidth,          // width of destination rectangle
	nHeight,        // height of destination rectangle
	0,                 // x-coordinate of upper-left corner of source rectangle
	0,                 // y-coordinate of upper-left corner of source rectangle
	nWidth,         // width of source rectangle 
	nHeight,       // height of source rectangle
	lpBits,           // address of bitmap bits
	(LPBITMAPINFO)pDib,//LPBITMAPINFO address of bitmap data
	DIB_RGB_COLORS, // usage flags
	SRCCOPY);// raster operation code

// SetDIBitsToDevice 이용
::SetDIBitsToDevice(dc.m_hDC,// handle to device context
	0,
	0,
	nWidth,
	nHeight,
	0,// x-coordinate of lower-left corner of source rect. 
	0,// y-coordinate of lower-left corner of source rect.
	0,// first scan line in array
	nHeight,// number of scan lines
	lpBits,// address of array with DIB bits
	(LPBITMAPINFO)pDib,// address of structure with bitmap info.
	DIB_RGB_COLORS);