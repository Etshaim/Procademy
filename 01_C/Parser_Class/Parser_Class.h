#ifndef __AYA_INI_PARSE__
#define __AYA_INI_PARSE__

class AyaINIParse
{
public :

	enum AYA_INI_PARSE
	{
		BUFFER_SIZE		= 512000	// 구역을 메모리에 읽을 버퍼
	};

	////////////////////////////////////////////////////////////
	// 생성자, 파괴자
	//
	////////////////////////////////////////////////////////////
	AyaINIParse();
	~AyaINIParse();

	////////////////////////////////////////////////////////////
	// Initial
	//
	// Prameter : 없음 
	// Return	: 없음
	////////////////////////////////////////////////////////////
	void	Initial(void);

	////////////////////////////////////////////////////////////
	// 텍스트 파일 열기
	//
	// Prameter : (char*) 텍스트 파일 이름
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	LoadFile(char* fileName);
	
	////////////////////////////////////////////////////////////
	// ProvideArea
	//
	// Prameter : (char*) 구역 이름
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	ProvideArea(char* areaName);

	////////////////////////////////////////////////////////////
	// GetValue
	//
	// Prameter : (char*)INI의 변수 이름. (char*) 저장될 문자열. (int*) 버퍼길이 in / 문자열 길이 out
	// Prameter : (char*)INI의 변수 이름. (int*) 저장될 변수
	// Prameter : (char*)INI의 변수 이름. (char*) 저장될 변수
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	GetValue( char* name, char *value, int *buffSize );
	bool	GetValue( char* name, int *value );
	bool	GetValue( char* name, float *value );
	
protected:

	////////////////////////////////////////////////////////////
	// 스페이스, 탭, 엔터코드, 주석 처리
	// m_buffer에 로드된 정보를 m_bufferFocusPos부터 주석처리하고
	// 다음 위치를 찾아낸다
	//
	// Prameter : 없음
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	SkipNoneCommand( void );

	////////////////////////////////////////////////////////////
	// 다음 단어 얻기
	// 현재 버퍼의 m_bufferFocusPos 부터 다음 단어 위치를 얻는다
	// m_bufferFocusPos는 얻은 단어의 끝에 위치
	// 포인터를 받기 위해 포인터에 포인터로 넣음
	//
	// Prameter : (char**)다음 단어의 시작 위치(받음), (int*)다음 단어의 길이(받음)
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	GetNextWord( char **chppBuffer, int *pLength );

	////////////////////////////////////////////////////////////
	// "~" 문자열 읽기
	// m_bufferFocusPos는 얻은 대화문장의 끝에 위치
	//
	// Prameter : (char**) 다음 단어의 시작 위치(받음), (int*) 다음 단어의 길이(받음)
	// Return	: bool
	////////////////////////////////////////////////////////////
	bool	GetStringWord(char** ppBuffer, int *pLength);

protected:
	// 버퍼	로드시 파일을 죄다 읽음
	char	*_pBuffer;

	// 버퍼에 로드된 INI 크기
	int		_loadSize;

	// 버퍼의 현재 위치
	int		_bufferareaStart;
	int		_bufferAreaEnd;
	int		_bufferFocusPos;

	// 구역 찾는 중
	bool	_provideAreaMode;
};

#endif //__AYA_INI_PARSE__