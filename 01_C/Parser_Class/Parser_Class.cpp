#include "Parser_Class.h"
#include <Windows.h>

////////////////////////////////////////////////////////////
// ������, �ı���
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
// Prameter : ���� 
// Return	: ����
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
// �ؽ�Ʈ ���� ����
//
// Prameter : (char*) �ؽ�Ʈ ���� �̸�
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
	// ������Ʈ ��Ŭ�� - �߰� - ���� - �����Ӽ� - �������� - ��Ƽ����Ʈ �������� ���

	// ���� ���� üũ
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return false;
	}

	// ���� ũ�� üũ
	_loadSize = GetFileSize(hFile, NULL);

	if (BUFFER_SIZE < _loadSize)
	{
		CloseHandle(hFile);
		Initial();
		return false;
	}

	// �б�
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
// Prameter : (char*) ���� �̸�
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
	// :, {, }�� ������ ã�´�
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
// Prameter : (char*)INI�� ���� �̸�. (char*) ����� ���ڿ�. (int*) ���۱��� in / ���ڿ� ���� out
// Prameter : (char*)INI�� ���� �̸�. (int*) ����� ����
// Prameter : (char*)INI�� ���� �̸�. (char*) ����� ����
// Return	: bool
////////////////////////////////////////////////////////////
bool	AyaINIParse::GetValue(char* name, char *value, int *buffSize)
{
	char* pBuff = NULL;
	char buff[256] = { 0, };

	int length = 0;

	// ���� ������ ó������ �˻�
	_bufferFocusPos = _bufferareaStart;

	// name�� ���� ���ڿ��� ã�´�.

	while (GetNextWord(&pBuff, &length))
	{
		memset(buff, 0, 256);
		memcpy(buff, pBuff, length);

		// name�� ���� ���ڿ��� ã����
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
// �����̽�, ��, �����ڵ�, �ּ� ó��
// m_buffer�� �ε�� ������ m_bufferFocusPos���� �ּ�ó���ϰ�
// ���� ��ġ�� ã�Ƴ���
//
// Prameter : ����
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

		// �齺���̽�	: 0x08
		// ��			: 0x09
		// ���� �ǵ�	: 0x0a
		// ĳ���� ����	: 0x0d
		// ����			: 0x20

		if (*pBuffer == 0x20 || *pBuffer == 0x0d ||
			*pBuffer == 0x0a || *pBuffer == 0x09 ||
			*pBuffer == 0x08)
		{
			*pBuffer = 0x20;

			++pBuffer;
			++_bufferFocusPos;
		}
		
		// '//' ó��
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

		// '/* */\ ó��

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
// ���� �ܾ� ���
// ���� ������ m_bufferFocusPos ���� ���� �ܾ� ��ġ�� ��´�
// m_bufferFocusPos�� ���� �ܾ��� ���� ��ġ
// �����͸� �ޱ� ���� �����Ϳ� �����ͷ� ����
//
// Prameter : (char**)���� �ܾ��� ���� ��ġ(����), (int*)���� �ܾ��� ����(����)
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

		// ������ ���̸� word ������ ��
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
// "~" ���ڿ� �б�
// m_bufferFocusPos�� ���� ��ȭ������ ���� ��ġ
//
// Prameter : (char**) ���� �ܾ��� ���� ��ġ(����), (int*) ���� �ܾ��� ����(����)
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