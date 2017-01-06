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

//< main
void main(void)
{
	char inText[128] = { 0, };
	char outText[256] = { 0, };

	int wordCount = 0;

	printf_s("영어 입력\n");

	gets( inText );

	strlwr( inText );

	Translate( inText, outText, &wordCount );

	printf_s("\n - 총 %d 단어가 번역 되었습니다.", wordCount );
	printf_s(" # %s \n", outText);
}


//< 번역
void Translate( char *in, char *out, int *wordCount )
{
	char	*pEng;
	char	*pKor;
	int		count	= 0;
	int		len		= strlen(in);

	*wordCount = 0;

	pEng = in;

	for( count = 0; count < len; ++count )
	{
		if ( ' ' == in[count] || '\0' == in[count] )
		{
			in[count] = '\0';

			pKor = FindWord(pEng);

			strcat(out, " ");

			if( NULL == pKor )
			{
				strcat(out, pKor);
				
			}
		
			else
			{
				strcat(out, pKor);
				++(*wordCount);
			}

			pEng = &in[count + 1];
		}
	}
}

//< 검색
char* findWord( char *eng )
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
