//////////////////////////////////////////////////

#include "Parser_Class.h"
#include <iostream>

void main(void)
{
	AyaINIParse		ayaIniParse;
	int				iValue	= 0;
	float			fValue	= 0;
	char			text[32];
	int				length = 32;

	ayaIniParse.LoadFile("Test.ini");
	ayaIniParse.ProvideArea("����1");

	ayaIniParse.GetValue("�������", &iValue);

	ayaIniParse.GetValue("������", &fValue);

	ayaIniParse.ProvideArea("����2");
	ayaIniParse.GetValue("�̸�", text, &length);
	system("pause");

}