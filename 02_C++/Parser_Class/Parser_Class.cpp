#include "Parser_Class.h"
#include <Windows.h>

////////////////////////////////////////////////////////////
// 생성자, 파괴자
////////////////////////////////////////////////////////////
CParser::CParser()
{
	m_chpBuffer = new char[eBUFFER_SIZE];
	Initial();
}

////////////////////////////////////////////////////////////
// 초기화
////////////////////////////////////////////////////////////
CParser::~CParser()
{
	memset(m_chpBuffer, 0, eBUFFER_SIZE);
	m_iLoadSize = 0;

	m_iBufferAreaStart = -1;
	m_iBufferAreaEnd = -1;
	m_iBufferFocusPos = 0;

	m_bProvideAreaMode = FALSE;
}

////////////////////////////////////////////////////////////
// 텍스트 파일 열기
////////////////////////////////////////////////////////////
BOOL	CParser::LoadFile(char *szFileName)
{
	HANDLE	hFile;
	DWORD	dwRead;

	if (0 != m_iLoadSize)
	{
		Initial();
	}

	hFile = CreateFile(szFileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// 파일 오픈 여부
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return FALSE;
	}

	// 파일 크기 검사
	m_iLoadSize = GetFileSize(hFile, NULL);

	// 버퍼보다 큰지 검사
	if (eBUFFER_SIZE < m_iLoadSize)
	{
		CloseHandle(hFile);
		Initial();
		return FALSE;
	}

	// 다 읽기
	ReadFile(hFile, m_chpBuffer, m_iLoadSize, &dwRead, NULL);

	if (dwRead != (DWORD)m_iLoadSize)
	{
		CloseHandle(hFile);

		Initial();
		return FALSE;
	}

	CloseHandle(hFile);

	return TRUE;
}

////////////////////////////////////////////////////////////
//	구역 나누기
////////////////////////////////////////////////////////////
BOOL	CParser::ProvideArea(char *szAreaName)
{
	char	*chpBuff;
	char	chBuff[256];
	int		iLength;
	BOOL	bAreaFlag = FALSE;

	m_iBufferAreaStart	= -1;
	m_iBufferAreaEnd	= -1;
	m_iBufferFocusPos	= 0;

	m_bProvideAreaMode	= TRUE;
}

////////////////////////////////////////////////////////////
// 값 얻기
//		문자열
//		int
//		float
////////////////////////////////////////////////////////////
BOOL	CParser::GetValue(char *szName, char *szValue, int *ipBufferSize)
{

}

BOOL	CParser::GetValue(char *szName, int *ipValue)
{

}

BOOL	CParser::GetValue(char *szName, float *fpValue)
{

}


////////////////////////////////////////////////////////////
// 스페이스, 탭, 엔터코드, 주석 처리
// m_chBuffer에 로드된 정보를 m_iBufferFocusPos부터 처리하고
// 다음 위치를 찾아낸다
////////////////////////////////////////////////////////////
BOOL	CParser::SkipNoneCommand(void)
{
	char *chpBuffer;

	chpBuffer = m_chpBuffer + m_iBufferFocusPos;

	while (1)
	{
		if (m_iBufferFocusPos > m_iLoadSize ||
			(!m_bProvideAreaMode && m_iBufferFocusPos > m_iBufferAreaEnd))
		{
			return FALSE;
		}

		// 백스페이스	0x08
		// 탭			0x09
		// 라인 피드	0x0a
		// 캐리지 리턴	0x0d
		// 공백			0x20

		if (*chpBuffer == 0x20 || *chpBuffer == 0x0d ||
			*chpBuffer == 0x0a || *chpBuffer == 0x09 ||
			*chpBuffer == 0x08)
		{
			*chpBuffer = 0x20;
			++chpBuffer;
			++m_iBufferFocusPos;
		}

		// 주석 '//' 엔터 코드가 나올때까지 무시
		else if (*chpBuffer == '/' && *(chpBuffer + 1) == '/')
		{
			while (*chpBuffer != 0x0d)
			{
				*chpBuffer = 0x20;
				++m_iBufferFocusPos;
				++chpBuffer;

				if (m_iBufferFocusPos > m_iLoadSize ||
					(!m_bProvideAreaMode && m_iBufferFocusPos > m_iBufferAreaEnd))
				{
					return FALSE;
				}
			}
		}
		// 주석 '/*' 
		else if (*chpBuffer == '/' && *(chpBuffer + 1) == '*')
		{
			while (!(*chpBuffer == '*' && *(chpBuffer + 1) == '/'))
			{
				*chpBuffer = 0x20;

				++m_iBufferFocusPos;
				++chpBuffer;

				if (m_iBufferFocusPos > m_iLoadSize ||
					(!m_bProvideAreaMode && m_iBufferFocusPos > m_iBufferAreaEnd))
				{
					return FALSE;
				}
			}
			*chpBuffer = 0x20;
			++chpBuffer;
			++m_iBufferFocusPos;

			*chpBuffer = 0x20;
			++chpBuffer;
			++m_iBufferFocusPos;
		}
		else
		{
			break;
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////
// 다음 단어 얻기
// 현재 버퍼의 m_iBufferPos부터 다음 단어 위치를 얻는다
// m_iBufferFocusPos는 얻은 단어의 끝에 위치
////////////////////////////////////////////////////////////
BOOL	CParser::GetNextWord(char **chppBuffer, int *ipLength)
{
	char	*chpBufferTemp;

	// 공백 제거
	if (!SkipNoneCommand())
	{
		return FALSE;
	}

	if (m_iBufferFocusPos > m_iLoadSize ||
		(!m_bProvideAreaMode && m_iBufferFocusPos > m_iBufferAreaEnd))
	{
		return FALSE;
	}
	

	*chppBuffer = m_chpBuffer + m_iBufferFocusPos;
	chpBufferTemp = *chppBuffer;

	//////////
	while ( *chpBufferTemp != 0x20 )
	{
		++ipLength;
		++chpBufferTemp;

		if (m_iBufferFocusPos > m_iLoadSize ||
			(!m_bProvideAreaMode && m_iBufferFocusPos > m_iBufferAreaEnd))
		{
			return FALSE;
		}
	}

	return TRUE;
}

////////////////////////////////////////////////////////////
// 문자열 읽기
// m_iBufferFocusPos는 얻은 문자열 끝에 위치.
////////////////////////////////////////////////////////////
BOOL	CParser::GetStringWord(char **chppBuffer, int *ipLength)
{

}

