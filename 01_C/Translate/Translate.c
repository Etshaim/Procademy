#include <stdio.h>
#include <string.h>

#define MAX	10

char wordArr[MAX][2][16] =
{
	{ "i",		"나"			},
	{ "you",	"너"			},
	{ "we",		"우리"		},
	{ "he",		"그"			},
	{ "she",	"그녀"		},
	{ "love",	"사랑하다"	},
	{ "play",	"연주하다"	},
	{ "go",		"가다"		},
	{ "piano",	"피아노"		},
	{ "game",	"게임"		}
};

//< 번역
void Translate( char *in, char *out, int *wordCount );

//< 검색
char* FindWord( char *eng );

void main(void)
{
	FILE *pFile = NULL;

	char inText[512] = {0, };
	char outText[512] = {0, };

	int wordCount = 0;
	int nRead = 0;

	pFile = fopen("eng.txt", "rb");

	if( pFile != NULL)
	{
		while(!feof(pFile))
		{
			nRead = fread(inText, 1, 512, pFile);
		}
		printf_s("%d글자\n", nRead);
		fclose(pFile);
	}
	//printf_s("영어 입력");

	//gets(inText);

	strlwr( inText );	// 소문자로

	Translate( inText, outText, &wordCount );

	printf_s("\n - 총 %d 단어가 번역 되었습니다.\n", wordCount );
	printf_s("%s\n\n", outText);
}

//< 번역
void Translate( char *in, char *out, int *wordCount )
{
	char	*pEng;
	char	*pKor;
	int		count	= 0;
	int		len		= strlen(in);
	int		enter	= 0;

	pEng = in;

	*(wordCount) = 0;
	
	for( count = 0; count < len + 1; ++count )
	{
		if ( ' ' == in[count] || '\0' == in[count] )
		{
			
			in[count] = '\0';

			pKor = FindWord(pEng);

			strcat(out, " ");

			if( '\0' == pKor )
			{
				strcat(out, pEng);
				
			}
			else
			{
				strcat(out, pKor);
				(*wordCount)++;
			}

			// enter가 눌렸다면
			if( enter == 1 )
			{
				strcat(out, "\n");
				enter = 0;
			}

			pEng = &in[count + 1];
		}

		// enter
		if( ( *((short*)&(in[count])) == 0x0A0D ) )
		{
			in[count] = '\0';
			in[count+1] = '\0';

			enter = 1;

			continue;
		}
	}
}

//< 검색
char* FindWord( char *eng )
{
	int count = 0;

	for( count = 0; MAX > count; ++count )
	{
		if ( 0 == strcmp(eng, wordArr[count][0]) )
		{
			return wordArr[count][1];
		}
	}

	return NULL;
}