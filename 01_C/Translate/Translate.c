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
		printf_s("%d����\n", nRead);
		fclose(pFile);
	}
	//printf_s("���� �Է�");

	//gets(inText);

	strlwr( inText );	// �ҹ��ڷ�

	Translate( inText, outText, &wordCount );

	printf_s("\n - �� %d �ܾ ���� �Ǿ����ϴ�.\n", wordCount );
	printf_s("%s\n\n", outText);
}

//< ����
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

			// enter�� ���ȴٸ�
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

//< �˻�
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