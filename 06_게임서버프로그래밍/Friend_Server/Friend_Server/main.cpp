// Friend_Server.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.

#include "stdafx.h"
#include "Friend_Server.h"

#include <map>

using namespace std;

/////////////////////////////////////////////////////////////////////////
// ���� ����
/////////////////////////////////////////////////////////////////////////

UINT64		g_AccountIncrement			= 1;
UINT64		g_FriendIncrement			= 1;
UINT64		g_FriendRequestIncrement	= 1;
SOCKET		g_ListenSocket				= INVALID_SOCKET;

map<SOCKET, st_CLIENT*>				g_ClientMap;     // key���� �������� ���� ����?
map<UINT64, st_DATA_ACCOUNT*>		g_AccountMap;

// ģ�� ���谡 ������ ��
multimap<UINT64, st_DATA_FRIEND*>	g_FriendMap;
multimap<UINT64, UINT64>			g_FriendIndex_From;
multimap<UINT64, UINT64>			g_FriendIndex_To;


// ģ�� ��û
multimap<UINT64, st_DATA_FRIEND_REQUEST*>    g_FriendRequestMap;
multimap<UINT64, UINT64>                     g_FriendRequestIndex_From;
multimap<UINT64, UINT64>                     g_FriendRequestIndex_To;


int main()
{
	return 0;
}

