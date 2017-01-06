#include "Parser_Class.h"
#include <Windows.h>

////////////////////////////////////////////////////////////
// ������, �ı���
////////////////////////////////////////////////////////////
CParser::CParser()
{
	m_chpBuffer = new char[eBUFFER_SIZE];
	Initial();
}

////////////////////////////////////////////////////////////
// �ʱ�ȭ
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
// �ؽ�Ʈ ���� ����
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

	// ���� ���� ����
	if (INVALID_HANDLE_VALUE == hFile)
	{
		return FALSE;
	}

	// ���� ũ�� �˻�
	m_iLoadSize = GetFileSize(hFile, NULL);

	// ���ۺ��� ū�� �˻�
	if (eBUFFER_SIZE < m_iLoadSize)
	{
		CloseHandle(hFile);
		Initial();
		return FALSE;
	}

	// �� �б�
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
//	���� ������
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
// �� ���
//		���ڿ�
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
// �����̽�, ��, �����ڵ�, �ּ� ó��
// m_chBuffer�� �ε�� ������ m_iBufferFocusPos���� ó���ϰ�
// ���� ��ġ�� ã�Ƴ���
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

		// �齺���̽�	0x08
		// ��			0x09
		// ���� �ǵ�	0x0a
		// ĳ���� ����	0x0d
		// ����			0x20

		if (*chpBuffer == 0x20 || *chpBuffer == 0x0d ||
			*chpBuffer == 0x0a || *chpBuffer == 0x09 ||
			*chpBuffer == 0x08)
		{
			*chpBuffer = 0x20;
			++chpBuffer;
			++m_iBufferFocusPos;
		}

		// �ּ� '//' ���� �ڵ尡 ���ö����� ����
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
		// �ּ� '/*' 
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
// ���� �ܾ� ���
// ���� ������ m_iBufferPos���� ���� �ܾ� ��ġ�� ��´�
// m_iBufferFocusPos�� ���� �ܾ��� ���� ��ġ
////////////////////////////////////////////////////////////
BOOL	CParser::GetNextWord(char **chppBuffer, int *ipLength)
{
	char	*chpBufferTemp;

	// ���� ����
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
// ���ڿ� �б�
// m_iBufferFocusPos�� ���� ���ڿ� ���� ��ġ.
////////////////////////////////////////////////////////////
BOOL	CParser::GetStringWord(char **chppBuffer, int *ipLength)
{

}

