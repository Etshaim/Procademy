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

// ����
DWORD		g_dwKey_UserNO;					// User ���̵�
DWORD		g_dwKey_RoomNO;					// ä�ù�

map<DWORD, st_CLIENT*>		g_ClientMap;	// ����� ���� ���� map
map<DWORD, st_CHAT_ROOM*>	g_RoomMap;		// ä�ù� ���� ���� map

SOCKET		g_ListenSocket;					// ����� accept�� listen ����

int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
	setlocale(LC_ALL, "");	// �����ڵ� ���Ĵٵ� ��� ȯ�� ����

	if (!NewworkInitial())
	{
		wprintf(L"NewworkInitial()");
		return 0;
	}


	while (1)
	{
		NetworkProcess();

		// Ư�� Ű �� ��Ȳ�� ���� ����ó��
		
	}

	// ���� ������ ���� �۾�
	return 0;
}

// ���� ����� ����
// 1. ���� ���� ����
// 2. ���� �¿� ������ �ִ´�
// 3. select �Լ��� ȣ���Ѵ�
// 4. select �Լ��� �����ϸ������� �����Լ��� ȣ���Ͽ� ó���Ѵ�



