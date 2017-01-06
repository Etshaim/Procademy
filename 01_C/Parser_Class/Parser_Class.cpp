#include "Parser_Class.h"
#include <Windows.h>

////////////////////////////////////////////////////////////
// 생성자, 파괴자
//
////////////////////////////////////////////////////////////
AyaINIParse::AyaINIParse()
{
	_pBuffer = new char[BUFFER_SIZE];

	Initial();
}

AyaINIParse::~AyaINIParse()
{
	delete[] _pBuffer;
}

////////////////////////////////////////////////////////////
// Initial
//
// Prameter : 없음 
// Return	: 없음
////////////////////////////////////////////////////////////
void	AyaINIParse::Initial(void)
{
	memset(_pBuffer, 0, BUFFER_SIZE);

	_loadSize			= 0;

	_bufferareaStart	= -1;
	_bufferAreaEnd		= -1;
	_bufferFocusPos		= 0;

	_provideAreaMode = false;
}

////////////////////////////////////////////////////////////
// 텍스트 파일 열기
//
// Prameter : (char*) 텍스트 파일 이름
// Return	: bool
////////////////////////////////////////////////////////////
bool	AyaINIParse::LoadFile(char *fileName)
{
	HANDLE	hFile;
	DWORD	read;

	if (0 != _loadSize)
	{
		Initial();
	}

	hFile = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	// 프로젝트 우클릭 - 추가 - 참조 - 구성속성 - 문자집합 - 멀티바이트 문자집합 사용

	// 파일 오픈 체크
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return false;
	}

	// 파일 크기 체크
	_loadSize = GetFileSize(hFile, NULL);

	if (BUFFER_SIZE < _loadSize)
	{
		CloseHandle(hFile);
		Initial();
		return false;
	}

	// 읽기
	ReadFile(hFile, _pBuffer, _loadSize, &read, NULL);

	if (read != (DWORD)_loadSize)
	{
		CloseHandle(hFile);

		Initial();

		return false;
	}

	CloseHandle(hFile);

	return true;
}

////////////////////////////////////////////////////////////
// ProvideArea
//
// Prameter : (char*) 구역 이름
// Return	: bool
////////////////////////////////////////////////////////////
bool	AyaINIParse::ProvideArea(char* areaName)
{
	char*	pBuff		= NULL;
	char	buff[256]	= { 0, };
	int		length		= 0;
	bool	areaFlag	= false;

	_bufferareaStart	= -1;
	_bufferAreaEnd		= -1;
	_bufferFocusPos		= 0;
	_provideAreaMode	= true;

	//====================================
	// :, {, }로 구역을 찾는다
	//====================================
	while (GetNextWord(&pBuff, &length))
	{
		memset(buff, 0, 256);
		memcpy(buff, pBuff, length);

		if (buff[0] == ':')
		{
			if (0 == strcmp(buff + 1, areaName))
			{
				if (GetNextWord(&pBuff, &length))
				{
					memset(buff, 0, 256);
					memcpy(buff, pBuff, length);

					if (pBuff[0] == '{')
					{
						if (!SkipNoneCommand())
						{
							_provideAreaMode = false;
							return false;
						}

						_bufferareaStart = _bufferFocusPos;
						areaFlag = true;
					}
				}
				else // if (GetNextWord(&pBuff, &length))
				{
					_provideAreaMode = false;
					return false;
				}
			}
		}
		else if (areaFlag && buff[0] == '}')
		{
			_bufferAreaEnd = _bufferFocusPos - 1;
			_provideAreaMode = false;
			return true;
		}
	}

	_provideAreaMode = false;

	return false;
}

////////////////////////////////////////////////////////////
// GetValue
//
// Prameter : (char*)INI의 변수 이름. (char*) 저장될 문자열. (int*) 버퍼길이 in / 문자열 길이 out
// Prameter : (char*)INI의 변수 이름. (int*) 저장될 변수
// Prameter : (char*)INI의 변수 이름. (char*) 저장될 변수
// Return	: bool
////////////////////////////////////////////////////////////
bool	AyaINIParse::GetValue(char* name, char *value, int *buffSize)
{
	char* pBuff = NULL;
	char buff[256] = { 0, };

	int length = 0;

	// 현재 구역의 처음부터 검사
	_bufferFocusPos = _bufferareaStart;

	// name과 같은 문자열을 찾는다.

	while (GetNextWord(&pBuff, &length))
	{
		memset(buff, 0, 256);
		memcpy(buff, pBuff, length);

		// name과 같은 문자열을 찾으면
		if (0 == strcmp(name, buff))
		{
			if (GetNextWord(&pBuff, &length))
			{
				memset(buff, 0, 256);
				memcpy(buff, pBuff, length);

				if (0 == strcmp(buff, "="))
				{
					if (GetNextWord(&pBuff, &length))
					{
						if (*buffSize <= length)
						{
							return false;
						}

						memset(value, 0, *buffSize);
						memcpy(value, pBuff, length);

						*buffSize = length;

						return true;
					}
					
					return false;
				}
				return false;
			}
		}
	}
	return false;
}
bool	AyaINIParse::GetValue(char* name, int *value)
{
	char* pBuff = NULL;
	char buff[256] = { 0, };
	int length = 0;

	_bufferFocusPos = _bufferareaStart;

	while (GetNextWord(&pBuff, &length))
	{
		memset(buff, 0, 256);
		memcpy(buff, pBuff, length);

		if (0 == strcmp(name, buff))
		{
			if (GetNextWord(&pBuff, &length))
			{
				memset(buff, 0, 256);
				memcpy(buff, pBuff, length);

				if (0 == strcmp(buff, "="))
				{
					if (GetNextWord(&pBuff, &length))
					{
						memset(buff, 0, 256);
						memcpy(buff, pBuff, length);

						*value = atoi(buff);

						return true;
					}
					return false;
				}
			}
			return false;
		}
	}
	return false;
}
bool	AyaINIParse::GetValue(char* name, float *value)
{
	char* pBuff = NULL;
	char buff[256] = { 0, };
	int length = 0;
	double dValue = 0;

	_bufferFocusPos = _bufferareaStart;

	while (GetNextWord(&pBuff, &length))
	{
		memset(buff, 0, 256);
		memcpy(buff, pBuff, length);

		if (0 == strcmp(name, buff))
		{
			if (GetNextWord(&pBuff, &length))
			{
				memset(buff, 0, 256);
				memcpy(buff, pBuff, length);

				if (0 == strcmp(buff, "="))
				{
					if (GetNextWord(&pBuff, &length))
					{
						memset(buff, 0, 256);
						memcpy(buff, pBuff, length);

						dValue = atof(buff);
						*value = (float)dValue;

						return true;
					}
					return false;
				}
			}
			return false;
		}
	}
	return false;
}


////////////////////////////////////////////////////////////
// 스페이스, 탭, 엔터코드, 주석 처리
// m_buffer에 로드된 정보를 m_bufferFocusPos부터 주석처리하고
// 다음 위치를 찾아낸다
//
// Prameter : 없음
// Return	: bool
////////////////////////////////////////////////////////////
bool	AyaINIParse::SkipNoneCommand(void)
{
	char* pBuffer = NULL;

	pBuffer = _pBuffer + _bufferFocusPos;

	while (1)
	{
		if ((_bufferFocusPos > _loadSize) || (!_provideAreaMode && _bufferFocusPos > _bufferAreaEnd))
		{
			return false;
		}

		// 백스페이스	: 0x08
		// 탭			: 0x09
		// 라인 피드	: 0x0a
		// 캐리지 리턴	: 0x0d
		// 공백			: 0x20

		if (*pBuffer == 0x20 || *pBuffer == 0x0d ||
			*pBuffer == 0x0a || *pBuffer == 0x09 ||
			*pBuffer == 0x08)
		{
			*pBuffer = 0x20;

			++pBuffer;
			++_bufferFocusPos;
		}
		
		// '//' 처리
		else if (*pBuffer == '/' && *(pBuffer + 1) == '/')
		{
			while (*pBuffer != 0x0d)
			{
				*pBuffer = 0x20;
				++_bufferFocusPos;
				++pBuffer;

				if ((_bufferFocusPos > _loadSize) || (!_provideAreaMode && _bufferFocusPos > _bufferAreaEnd))
				{
					return false;
				}
			}
		}

		// '/* */\ 처리

		else if (*pBuffer == '/' && *(pBuffer + 1) == '*')
		{
			while ( !(	(*pBuffer == '*')	&&	(*(pBuffer + 1) == '/') ) )
			{
				*pBuffer = 0x20;
				++_bufferFocusPos;
				++pBuffer;

				if ((_bufferFocusPos > _loadSize) || (!_provideAreaMode && _bufferFocusPos > _bufferAreaEnd))
				{
					return false;
				}
			}
			*pBuffer = 0x20;
			++pBuffer;
			*pBuffer = 0x20;
			++pBuffer;

			_bufferFocusPos += 2;
		}

		else
		{
			break;
		}
	}

	return true;
	
}


////////////////////////////////////////////////////////////
// 다음 단어 얻기
// 현재 버퍼의 m_bufferFocusPos 부터 다음 단어 위치를 얻는다
// m_bufferFocusPos는 얻은 단어의 끝에 위치
// 포인터를 받기 위해 포인터에 포인터로 넣음
//
// Prameter : (char**)다음 단어의 시작 위치(받음), (int*)다음 단어의 길이(받음)
// Return	: bool
////////////////////////////////////////////////////////////
bool	AyaINIParse::GetNextWord(char **ppBuffer, int *pLength)
{
	char* pBufferTemp = NULL;

	if ( !SkipNoneCommand() )
	{
		return false;
	}

	if ((_bufferFocusPos > _loadSize) || (!_provideAreaMode && _bufferFocusPos > _bufferAreaEnd))
	{
		return false;
	}

	*ppBuffer = _pBuffer + _bufferFocusPos;
	pBufferTemp = *ppBuffer;

	*pLength = 0;

	if (**ppBuffer == '"')
	{
		if (GetStringWord(ppBuffer, pLength))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	while (1)
	{
		if (**ppBuffer == 0x20 || **ppBuffer == 0x0d ||
			**ppBuffer == 0x0a || **ppBuffer == 0x09 ||
			**ppBuffer == ','  || **ppBuffer == '"'  ||
			**ppBuffer == 0x08)
		{
			break;
		}

		_bufferFocusPos++;
		(*ppBuffer)++;
		(*pLength)++;

		// 파일의 끝이면 word 끝으로 봄
		if ((_bufferFocusPos > _loadSize) || (!_provideAreaMode && _bufferFocusPos > _bufferAreaEnd))
		{
			break;
		}
	}

	*ppBuffer = pBufferTemp;

	if (*pLength == 0)
	{
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////
// "~" 문자열 읽기
// m_bufferFocusPos는 얻은 대화문장의 끝에 위치
//
// Prameter : (char**) 다음 단어의 시작 위치(받음), (int*) 다음 단어의 길이(받음)
// Return	: bool
////////////////////////////////////////////////////////////
bool	AyaINIParse::GetStringWord(char** ppBuffer, int *pLength)
{
	char* pBufferTemp = NULL;
	*pLength = 0;

	*ppBuffer = _pBuffer + _bufferFocusPos;
	pBufferTemp = *ppBuffer;

	if (**ppBuffer != '"')
	{
		return false;
	}

	_bufferFocusPos++;
	(*ppBuffer)++;
	pBufferTemp++;

	while (1)
	{
		if (**ppBuffer == '"' || **ppBuffer == 0x0a || **ppBuffer == 0x0d)
		{
			_bufferFocusPos++;
			break;
		}

		_bufferFocusPos++;
		(*ppBuffer)++;
		(*pLength)++;

		if ((_bufferFocusPos > _loadSize) || (!_provideAreaMode && _bufferFocusPos > _bufferAreaEnd))
		{
			return false;
		}
	}

	*ppBuffer = pBufferTemp;

	if (pLength == 0)
	{
		return true;
	}

	return true;
}