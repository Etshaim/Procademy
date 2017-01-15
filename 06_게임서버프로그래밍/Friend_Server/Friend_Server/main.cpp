// Friend_Server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.

#include "stdafx.h"
#include "Friend_Server.h"
#include "JSON.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////
// 전역 변수
/////////////////////////////////////////////////////////////////////////

UINT64		g_AccountIncrement			= 1;
UINT64		g_FriendIncrement			= 1;
UINT64		g_FriendRequestIncrement	= 1;
SOCKET		g_ListenSocket				= INVALID_SOCKET;

map<SOCKET, st_CLIENT*>				g_ClientMap;     // key값을 소켓으로 쓰는 이유?
map<UINT64, st_DATA_ACCOUNT*>		g_AccountMap;

// 친구 관계가 성립된 것
multimap<UINT64, st_DATA_FRIEND*>	g_FriendMap;
multimap<UINT64, UINT64>			g_FriendIndex_From;
multimap<UINT64, UINT64>			g_FriendIndex_To;

// 친구 요청
multimap<UINT64, st_DATA_FRIEND_REQUEST*>    g_FriendRequestMap;
multimap<UINT64, UINT64>                     g_FriendRequestIndex_From;
multimap<UINT64, UINT64>                     g_FriendRequestIndex_To;

// JSON
StringBuffer					StringJSON;
Writer<StringBuffer, UTF16<>>	writer(StringJSON);

BOOL g_bShutdown = FALSE;

int wmain( int argc, wchar_t *argv[] )
{
	setlocale(LC_ALL, "");

	if (!NetworkInitial())
	{
		wprintf(L"NetworkInitial()\n");
		return 0;
	}

	// 데이터 로드
	LoadData();

	// 계정 생성
	CreateAccount(L"Account01");
	CreateAccount(L"Account02");
	CreateAccount(L"Account03");
	CreateAccount(L"Account04");
	CreateAccount(L"Account05");
	CreateAccount(L"Account06");
	CreateAccount(L"Account07");
	CreateAccount(L"Account08");
	CreateAccount(L"Account09");

	DWORD dwTick = 0;

	while (!g_bShutdown)
	{
		NetworkProcess();
		//ControlMode();

		if (GetTickCount() - dwTick > 1000)
		{
			dwTick = GetTickCount();
			wprintf(L"Connect : %zd\n", g_ClientMap.size());
		}
	}

	// 서버 마무리 후 종료

	// data 저장
	SaveData();


	return 0;
}

