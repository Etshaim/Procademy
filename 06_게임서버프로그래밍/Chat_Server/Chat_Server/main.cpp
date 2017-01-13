#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <locale.h>
#include <stdio.h>
#include <list>
#include <map>
#include "Protocol.h"
#include "ProtocolBuffer.h"
#include "StreamSQ.h"
#include "Chat_Server.h"

using namespace std;

// 전역
DWORD		g_dwKey_UserNO;					// User 아이디
DWORD		g_dwKey_RoomNO;					// 채팅방

map<DWORD, st_CLIENT*>		g_ClientMap;	// 사용자 관리 메인 map
map<DWORD, st_CHAT_ROOM*>	g_RoomMap;		// 채팅방 관리 메인 map

SOCKET		g_ListenSocket;					// 사용자 accept용 listen 소켓

int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	setlocale(LC_ALL, "");	// 유니코드 스탠다드 출력 환경 세팅

	if (!NewworkInitial())
	{
		wprintf(L"NewworkInitial()");
		return 0;
	}


	while (1)
	{
		NetworkProcess();

		// 특정 키 및 상황에 따른 종료처리
		
	}

	// 서버 마무리 종료 작업
	return 0;
}

// 소켓 입출력 절차
// 1. 소켓 셋을 비운다
// 2. 소켓 셋에 소켓을 넣는다
// 3. select 함수를 호출한다
// 4. select 함수가 리턴하면적절한 소켓함수를 호출하여 처리한다



