#pragma once

#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"

using namespace rapidjson;

extern StringBuffer						StringJSON;
extern Writer<StringBuffer, UTF16<>>	writer;

// UTF-8의 char 문자열을 UTF-16 WCHAR로 변환
BOOL UTF8toUTF16(const char *szText, WCHAR *szBuff, int iBuffLen);

void LoadData(void);

void SaveData(void);