#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE	0
#define DECODE	1
#define ID_LEN	3

//< 파일 이름 입력
void InputFileName( char* fName );

//< 버퍼에 복사하기
int CopyBuffer( FILE *pFile, char *pFileName, char** pBuff );

//< 부호화 ( 암호 -> 복호화, 암호x -> 암호화)
void Encryption( char** pBuff );

//< 복호화, 암호화 판별
int IdentifyCode( char* pBuff );

//< 복호화
void Decode( FILE *pFile, char *pFileName, char* pBuff, int nRead );

//< 암호화
void Encode( FILE *pFile, char *pFileName, char* pBuff, int nRead );

void main(void)
{
	FILE *pFile			= NULL;			
	
	char *pBuff			= NULL;
	char fileName[32]	= { '\0', };	// 파일 이름
	//char password[16]	= { '\0', };	

	int nRead			= 0;			// ftell 리턴 값
	int count			= 0;			// iterator
	int bEncode			= 0;			// 암호화, 복호화 판단 bool값

	//< 파일 이름 입력
	InputFileName( fileName );

	//< 버퍼에 복사하기
	nRead = CopyBuffer( pFile, fileName, &pBuff );

	//복호화, 암호화 판별
	bEncode = IdentifyCode( pBuff );

	//=========================================
	
	if ( ENCODE == bEncode )
	{
		// 암호화 
		Encode( pFile, fileName, pBuff, nRead );
	}
	else if( DECODE == bEncode )
	{
		// 복호화
		Decode( pFile, fileName, pBuff, nRead );
	}
	else
	{
		printf_s("error");
	}

	free(pBuff);
}

//< 파일 이름 입력
void InputFileName(char* fName)
{
	printf_s("(test 파일 이름은 a.txt 입니다)\n");
	printf_s("파일 이름 입력 : ");
	gets(fName);
}

//< 버퍼에 복사하기
int CopyBuffer( FILE *pFile, char *pFileName, char** pBuff )
{
	int nRead = 0;

	pFile = fopen( pFileName, "rb");

	if( pFile != NULL )
	{
		fseek(pFile, 0, SEEK_END );
		nRead = ftell(pFile);
		fseek(pFile, 0, SEEK_SET );
	}

	*pBuff = ( char* )malloc( (nRead+1) * sizeof(char) );

	(*pBuff)[nRead] = '\0';

	fread( *pBuff, nRead, 1, pFile);

	fclose(pFile);

	return nRead;
}

//< 복호화 암호화 판별
int IdentifyCode( char* pBuff )
{
	char temp[ID_LEN + 1];

	memcpy( temp, pBuff, ID_LEN );

	temp[ID_LEN] = '\0';


	if( 0 == strcmp(temp, "abc") )
	{
		return DECODE;
	}
	else
	{
		return ENCODE;
	}
}

//< 복호화
void Decode( FILE *pFile, char *pFileName, char* pBuff, int nRead )
{
	char* pTemp = &pBuff[ID_LEN];
	int count = 0;

	while( pTemp[count] != NULL )
	{
		pTemp[count] ^= 0X1818;
		count++;
	}

	pFile = fopen( pFileName, "wb" );

	if( pFile != NULL )
	{
		fwrite( pTemp, nRead - ID_LEN, 1, pFile);
		printf_s("복호화\n");
	}

	fclose(pFile);
}

//< 암호화
void Encode( FILE *pFile, char *pFileName, char* pBuff, int nRead )
{
	int count = 0;

	while( pBuff[count] != NULL )
	{
		pBuff[count] ^= 0X1818;
		count++;
	}
	pFile = fopen( pFileName, "wb" );

	if( pFile != NULL )
	{
		fwrite( "abc", ID_LEN, 1, pFile);
		fwrite( pBuff, nRead, 1, pFile);

		printf_s("암호화\n");
	}

	fclose(pFile);
}