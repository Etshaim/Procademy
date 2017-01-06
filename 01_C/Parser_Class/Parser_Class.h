#ifndef __AYA_INI_PARSE__
#define __AYA_INI_PARSE__

class AyaINIParse
{
public :

	enum AYA_INI_PARSE
	{
		BUFFER_SIZE		= 512000	// ������ �޸𸮿� ���� ����
	};

	////////////////////////////////////////////////////////////
	// ������, �ı���
	//
	////////////////////////////////////////////////////////////
	AyaINIParse();
	~AyaINIParse();

	////////////////////////////////////////////////////////////
	// Initial
	//
	// Prameter : ���� 
	// Return	: ����
	////////////////////////////////////////////////////////////
	void	Initial(void);

	////////////////////////////////////////////////////////////
	// �ؽ�Ʈ ���� ����
	//
	// Prameter : (char*) �ؽ�Ʈ ���� �̸�
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	LoadFile(char* fileName);
	
	////////////////////////////////////////////////////////////
	// ProvideArea
	//
	// Prameter : (char*) ���� �̸�
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	ProvideArea(char* areaName);

	////////////////////////////////////////////////////////////
	// GetValue
	//
	// Prameter : (char*)INI�� ���� �̸�. (char*) ����� ���ڿ�. (int*) ���۱��� in / ���ڿ� ���� out
	// Prameter : (char*)INI�� ���� �̸�. (int*) ����� ����
	// Prameter : (char*)INI�� ���� �̸�. (char*) ����� ����
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	GetValue( char* name, char *value, int *buffSize );
	bool	GetValue( char* name, int *value );
	bool	GetValue( char* name, float *value );
	
protected:

	////////////////////////////////////////////////////////////
	// �����̽�, ��, �����ڵ�, �ּ� ó��
	// m_buffer�� �ε�� ������ m_bufferFocusPos���� �ּ�ó���ϰ�
	// ���� ��ġ�� ã�Ƴ���
	//
	// Prameter : ����
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	SkipNoneCommand( void );

	////////////////////////////////////////////////////////////
	// ���� �ܾ� ���
	// ���� ������ m_bufferFocusPos ���� ���� �ܾ� ��ġ�� ��´�
	// m_bufferFocusPos�� ���� �ܾ��� ���� ��ġ
	// �����͸� �ޱ� ���� �����Ϳ� �����ͷ� ����
	//
	// Prameter : (char**)���� �ܾ��� ���� ��ġ(����), (int*)���� �ܾ��� ����(����)
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	GetNextWord( char **chppBuffer, int *pLength );

	////////////////////////////////////////////////////////////
	// "~" ���ڿ� �б�
	// m_bufferFocusPos�� ���� ��ȭ������ ���� ��ġ
	//
	// Prameter : (char**) ���� �ܾ��� ���� ��ġ(����), (int*) ���� �ܾ��� ����(����)
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	GetStringWord(char** ppBuffer, int *pLength);

protected:
	// ����	�ε�� ������ �˴� ����
	char	*_pBuffer;

	// ���ۿ� �ε�� INI ũ��
	int		_loadSize;

	// ������ ���� ��ġ
	int		_bufferareaStart;
	int		_bufferAreaEnd;
	int		_bufferFocusPos;

	// ���� ã�� ��
	bool	_provideAreaMode;
};

#endif //__AYA_INI_PARSE__