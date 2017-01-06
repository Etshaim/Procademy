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
	ayaIniParse.ProvideArea("备开1");

	ayaIniParse.GetValue("积斥岿老", &iValue);

	ayaIniParse.GetValue("盔林啦", &fValue);

	ayaIniParse.ProvideArea("备开2");
	ayaIniParse.GetValue("捞抚", text, &length);
	system("pause");

}