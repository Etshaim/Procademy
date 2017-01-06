#include <stdio.h>
#include <string.h>

#define MAX	10

char wordArr[MAX][2][16] =
{
	{ "i",		"��"			},
	{ "you",	"��"			},
	{ "we",		"�츮"		},
	{ "he",		"��"			},
	{ "she",	"�׳�"		},
	{ "love",	"����ϴ�"	},
	{ "play",	"�����ϴ�"	},
	{ "go",		"����"		},
	{ "piano",	"�ǾƳ�"		},
	{ "game",	"����"		}
};


//< ����
void Translate( char *in, char *out, int *wordCount );

//< �˻�
char* FindWord( char *eng );

//< main
void main(void)
{
	char inText[128] = { 0, };
	char outText[256] = { 0, };

	int wordCount = 0;

	printf_s("���� �Է�\n");

	gets( inText );

	strlwr( inText );

	Translate( inText, outText, &wordCount );

	printf_s("\n - �� %d �ܾ ���� �Ǿ����ϴ�.", wordCount );
	printf_s(" # %s \n", outText);
}


//< ����
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

//< �˻�
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
