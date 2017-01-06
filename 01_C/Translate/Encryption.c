#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENCODE	0
#define DECODE	1
#define ID_LEN	3

//< ���� �̸� �Է�
void InputFileName( char* fName );

//< ���ۿ� �����ϱ�
int CopyBuffer( FILE *pFile, char *pFileName, char** pBuff );

//< ��ȣȭ ( ��ȣ -> ��ȣȭ, ��ȣx -> ��ȣȭ)
void Encryption( char** pBuff );

//< ��ȣȭ, ��ȣȭ �Ǻ�
int IdentifyCode( char* pBuff );

//< ��ȣȭ
void Decode( FILE *pFile, char *pFileName, char* pBuff, int nRead );

//< ��ȣȭ
void Encode( FILE *pFile, char *pFileName, char* pBuff, int nRead );

void main(void)
{
	FILE *pFile			= NULL;			
	
	char *pBuff			= NULL;
	char fileName[32]	= { '\0', };	// ���� �̸�
	//char password[16]	= { '\0', };	

	int nRead			= 0;			// ftell ���� ��
	int count			= 0;			// iterator
	int bEncode			= 0;			// ��ȣȭ, ��ȣȭ �Ǵ� bool��

	//< ���� �̸� �Է�
	InputFileName( fileName );

	//< ���ۿ� �����ϱ�
	nRead = CopyBuffer( pFile, fileName, &pBuff );

	//��ȣȭ, ��ȣȭ �Ǻ�
	bEncode = IdentifyCode( pBuff );

	//=========================================
	
	if ( ENCODE == bEncode )
	{
		// ��ȣȭ 
		Encode( pFile, fileName, pBuff, nRead );
	}
	else if( DECODE == bEncode )
	{
		// ��ȣȭ
		Decode( pFile, fileName, pBuff, nRead );
	}
	else
	{
		printf_s("error");
	}

	free(pBuff);
}

//< ���� �̸� �Է�
void InputFileName(char* fName)
{
	printf_s("(test ���� �̸��� a.txt �Դϴ�)\n");
	printf_s("���� �̸� �Է� : ");
	gets(fName);
}

//< ���ۿ� �����ϱ�
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

//< ��ȣȭ ��ȣȭ �Ǻ�
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

//< ��ȣȭ
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
		printf_s("��ȣȭ\n");
	}

	fclose(pFile);
}

//< ��ȣȭ
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

		printf_s("��ȣȭ\n");
	}

	fclose(pFile);
}