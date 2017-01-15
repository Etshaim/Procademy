#include "stdafx.h"
#include "JSON.h"

BOOL UTF8toUTF16(const char * szText, WCHAR * szBuff, int iBuffLen)
{
	int iRetVal = MultiByteToWideChar(CP_UTF8, 0, szText, strlen(szText), szBuff, iBuffLen);

	if (iRetVal < iBuffLen)
	{
		szBuff[iRetVal] = L'\0';
	}

	return true;
}

void LoadData(void)
{
	const char *pJson = StringJSON.GetString();

	Document Doc;
	Doc.Parse(pJson);

	UINT64	AccountNo;
	WCHAR	szNickname[256];

	Value	&AccountArray = Doc["Account"];

	for (SizeType i = 0; i < AccountArray.Size(); i++)
	{
		Value &AccountObject = AccountArray[i];
		AccountNo = AccountObject["AccountNo"].GetUint64();
		UTF8toUTF16(AccountObject["Nickname"].GetString(), szNickname, 256);
	}

	UINT64 FriendNo;
	UINT64 FromAccountNo;
	UINT64 ToAccountNo;

	Value &FriendArray = Doc["Friend"];

	for (SizeType i = 0; i < FriendArray.Size(); i++)
	{
		Value &FriendObject = FriendArray[i];
		FriendNo = FriendObject["FriendNo"].GetUint64();
		FromAccountNo = FriendObject["FromAccountNo"].GetUint64();
		ToAccountNo = FriendObject["ToAccountNo"].GetUint64();
	}
}

void SaveData(void)
{
	writer.StartObject();
	writer.String(L"Account");
	writer.StartArray();
	{
		// Account 정보 저장
		writer.StartObject();
		writer.String(L"AccountNo");
		writer.Uint64(1);
		writer.String(L"Nickname");
		writer.String(L"etshaim");
		writer.EndObject();
	}
	writer.EndArray();

	writer.String(L"Friend");
	writer.StartArray();
	{
		// 친구정보 저장
		writer.StartObject();
		writer.String(L"FriendNo");
		writer.Uint64(1);
		writer.String(L"FromAccountNo");
		writer.Uint64(2);
		writer.String(L"ToAccountNo");
		writer.Uint64(3);
		writer.EndObject();
	}
	writer.EndArray();

	writer.EndObject();
}