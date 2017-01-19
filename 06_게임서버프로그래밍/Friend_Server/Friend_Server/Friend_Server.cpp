#include "stdafx.h"
#include "Friend_Server.h"
#include "ProtocolBuffer.h"

using namespace std;

extern	UINT64		g_AccountIncrement;
extern	UINT64		g_FriendIncrement;
extern	UINT64		g_FriendRequestIncrement;
extern	SOCKET		g_ListenSocket;

extern	map<SOCKET, st_CLIENT*>						 g_ClientMap;     // key���� �������� ���� ����?
extern	map<UINT64, st_DATA_ACCOUNT*>				 g_AccountMap;
													 
// ģ�� ���谡 ������ ��							    
extern	multimap<UINT64, st_DATA_FRIEND*>			 g_FriendMap;
extern	multimap<UINT64, UINT64>					 g_FriendIndex_From;
extern	multimap<UINT64, UINT64>					 g_FriendIndex_To;


// ģ�� ��û
extern	multimap<UINT64, st_DATA_FRIEND_REQUEST*>    g_FriendRequestMap;
extern	multimap<UINT64, UINT64>                     g_FriendRequestIndex_From;
extern	multimap<UINT64, UINT64>                     g_FriendRequestIndex_To;


BOOL NetworkInitial(void)
{
	// ���� �ʱ�ȭ
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return FALSE;
	}

	// socket()
	g_ListenSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (g_ListenSocket == INVALID_SOCKET)
	{
		err_display(L"socket()");
		return FALSE;
	}

	// bind()
	SOCKADDR_IN serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family			= AF_INET;
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serveraddr.sin_port				= htons(dfNETWORK_PORT);

	int iRetVal = bind(g_ListenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (iRetVal == SOCKET_ERROR)
	{
		err_display(L"bind()");
		return FALSE;
	}

	// listen()
	iRetVal = listen(g_ListenSocket, SOMAXCONN);

	if (iRetVal == SOCKET_ERROR)
	{
		err_display(L"listen()");
		return FALSE;
	}

	return TRUE;
}

void NetworkProcess(void)
{
	st_CLIENT *pClient;

	UINT64	userTable_NO[FD_SETSIZE];
	SOCKET	userTable_SOCKET[FD_SETSIZE];

	int		iSocketCount = 0;	// 64�� �Ǹ� ȣ��

	FD_SET	readSet;
	FD_SET	writeSet;

	FD_ZERO( &readSet );
	FD_ZERO( &writeSet );

	memset(userTable_NO, -1, sizeof(DWORD) * FD_SETSIZE);
	memset(userTable_SOCKET, (int)INVALID_SOCKET, sizeof(SOCKET) * FD_SETSIZE);
	 
	// ù ��° �ε������� listen socket ���� ����
	FD_SET(g_ListenSocket, &readSet);
	userTable_NO[iSocketCount] = 0;
	userTable_SOCKET[iSocketCount] = g_ListenSocket;

	iSocketCount++;


	// Client�� ��ȸ�ϸ鼭 ó��
	map< SOCKET, st_CLIENT* >::iterator iter;

	for (iter = g_ClientMap.begin(); iter != g_ClientMap.end(); )
	{
		pClient = (*iter).second;
		
		++iter;

		// pClient�� nullptr�� �ƴϸ�
		if (nullptr != pClient)
		{
			// ���� ������ ���� �� ���� ���� �����Ƿ�
			if (nullptr != pClient->pAccount)
			{
				userTable_NO[iSocketCount] = pClient->pAccount->AccountNo;
			}

			userTable_SOCKET[iSocketCount] = pClient->sock;

			FD_SET(pClient->sock, &readSet);
			FD_SET(pClient->sock, &writeSet);

			iSocketCount++;
		}

		if (iSocketCount >= FD_SETSIZE)
		{
			// FD_ISSET���� recv send ó��
			SelectSocket(userTable_NO, userTable_SOCKET, &readSet, &writeSet);

			FD_ZERO(&readSet);
			FD_ZERO(&writeSet);

			memset(userTable_NO, -1, FD_SETSIZE);
			memset(userTable_SOCKET, (int)INVALID_SOCKET, sizeof(SOCKET) * FD_SETSIZE);

			iSocketCount = 0;
		}
	}

	// �������� iSocketCount�� FD_SETSIZE��ŭ
	// �������� ������ ��쿡�� �ٽ� �ѹ� ó������
	if (0 < iSocketCount)
	{
		SelectSocket(userTable_NO, userTable_SOCKET, &readSet, &writeSet);
	}
}

void SelectSocket(UINT64 *pTableNO, SOCKET * pTableSocket, FD_SET * pReadSet, FD_SET * pWriteSet)
{
	timeval Time;
	int		iRetVal;
	int		iCount;

	//-----------------------------------------------------
	// select �Լ��� ��� �ð�
	//-----------------------------------------------------
	Time.tv_sec		= 0;
	Time.tv_usec	= 0;

	// select ȣ��
	iRetVal = select(0, pReadSet, pWriteSet, nullptr, &Time);

	//-----------------------------------------------------
	// iRetVal ���� 0 �̻��̸� ������ �ִ� ��
	//-----------------------------------------------------
	if ( 0 < iRetVal)
	{
		// ���̺� ��ȸ�ϸ鼭 FD_ISSET ���� rset wset �˻�
		for (iCount = 0; iCount < FD_SETSIZE; ++iCount)
		{
			if (INVALID_SOCKET == pTableSocket[iCount])
			{
				continue;
			}

			// rset ���� ������ 
			if (FD_ISSET(pTableSocket[iCount], pReadSet))
			{
				// listen ������ ���������� accept
				if (0 == pTableNO[iCount])
				{
					netProc_Accept();
				}
				// �ƴϸ� ��Ŷ ���μ��� ó��
				else
				{
					netProc_Recv( pTableSocket[iCount] );
				}
			}

			// wset ���� ������
			if (FD_ISSET(pTableSocket[iCount], pWriteSet))
			{
				netProc_Send(pTableSocket[iCount]);
			}
		}
	}
	else if (SOCKET_ERROR == iRetVal)
	{
		wprintf(L"SelectSocket()");
	}
}

void err_quit(WCHAR * msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(WCHAR * msg)
{
	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	wprintf(L"[%s] %s", msg, (WCHAR *)lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CreateAccount(WCHAR * szNickName)
{
	st_DATA_ACCOUNT *pAccount = new st_DATA_ACCOUNT;

	wcscpy_s(pAccount->szID, szNickName);
	pAccount->AccountNo = g_AccountIncrement;
	g_AccountIncrement++;

	g_AccountMap.insert( pair<UINT64, st_DATA_ACCOUNT*>( pAccount->AccountNo, pAccount ) );
}

BOOL DisconnectClient(SOCKET socket)
{
	st_CLIENT *pClient = g_ClientMap[socket];

	if (nullptr == pClient)
	{
		return FALSE;
	}

	delete g_ClientMap[socket];

	g_ClientMap.erase(socket);

	return TRUE;
}

st_CLIENT * FindClient(SOCKET socket)
{
	st_CLIENT *pClient;

	map<SOCKET, st_CLIENT*>::iterator ClienIter;

	for (ClienIter = g_ClientMap.begin(); ClienIter != g_ClientMap.end(); ClienIter++)
	{
		pClient = ClienIter->second;

		if ( socket == pClient->sock )
		{
			return pClient;
		}
	}

	return nullptr;
}

BOOL PacketProc(st_CLIENT * pClient, WORD wMsgType, CProtocolBuffer * pPacket)
{
	wprintf(L"PacketRecv [UserNO:%lld][Type:%ld]\n", pClient->sock, wMsgType);

	switch (wMsgType)
	{
	case df_REQ_ACCOUNT_ADD:
		return netPacket_ReqAccountAdd(pClient, pPacket);
	
	case df_REQ_LOGIN:
		return netPacket_ReqLogin(pClient, pPacket);

	case df_REQ_ACCOUNT_LIST:
		return netPacket_ReqAccountList(pClient, pPacket);

	case df_REQ_FRIEND_LIST:
		return netPacket_ReqFriendList(pClient, pPacket);

	case df_REQ_FRIEND_REQUEST_LIST:
		return netPacket_ReqFriendList_Request(pClient, pPacket);

	case df_REQ_FRIEND_REPLY_LIST:
		return netPacket_ReqFriendList_Reply(pClient, pPacket);

	case df_REQ_FRIEND_REMOVE:
		return netPacket_ReqFriendRemove(pClient, pPacket);

	case df_REQ_FRIEND_REQUEST:
		return netPacket_ReqFriendRequest(pClient, pPacket);

	case df_REQ_FRIEND_CANCEL:
		return netPacket_ReqFriendCancel(pClient, pPacket);

	case df_REQ_FRIEND_DENY:
		return netPacket_ReqFriendDeny(pClient, pPacket);

	case df_REQ_FRIEND_AGREE: 
		netPacket_ReqFriendAgree(pClient, pPacket);
	}
	return TRUE;
}

int MakeRecvPacket(st_CLIENT * pClient)
{
	// return 1		��Ŷ �̿ϼ�
	// return 0		��Ŷ ó��
	// return -1	����

	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		clPacket;

	int iRecvQSize = pClient->RecvQ.GetUseSize();

	// ��� ������� ������ ó���� �� ���ٰ� �Ǵ�
	if (sizeof(st_PACKET_HEADER) > iRecvQSize)
	{
		// ������ ó��
		return 1;
	}

	// ��Ŷ �ڵ� �˻�
	pClient->RecvQ.Peek((char*)&stHeader, sizeof(st_PACKET_HEADER));

	if (dfPACKET_CODE != stHeader.byCode)
	{
		return -1;
	}

	// ��� + ���̷ε庸�� recvQ ũ�Ⱑ ������ �̿ϼ�
	if (stHeader.wPayloadSize + sizeof(stHeader) > iRecvQSize)
	{
		// ������ ó��
		return 1;
	}

	// ����κ��� �������� ���̷ε常 ������
	pClient->RecvQ.RemoveData( sizeof( stHeader ) );

	if (stHeader.wPayloadSize != pClient->RecvQ.Get(clPacket.GetBufferPtr(), stHeader.wPayloadSize))
	{
		return -1;
	}

	clPacket.MoveWritePos(stHeader.wPayloadSize);

	//------------------------------------------------
	// �������� ��Ŷ ó�� �Լ� ȣ��
	//------------------------------------------------
	if (!PacketProc(pClient, stHeader.wMsgType, &clPacket))
	{
		return -1;
	}

	// ������� �Դٴ� ���� �������� ��Ŷ �ϳ� ó��
	// �̾ ��� ó���� �� �ֵ��� return 0�� ����
	return 0;
}

void netProc_Recv(SOCKET socket)
{
	
	st_CLIENT *pClient  = g_ClientMap[socket];
	int iResult			= 0;
		
	if (nullptr == pClient)
	{
		return;
	}

	iResult = recv(pClient->sock, pClient->RecvQ.GetWriteBufferPtr(),
		pClient->RecvQ.GetFreeSize(), 0);

	if (SOCKET_ERROR == iResult)
	{
		closesocket(pClient->sock);
		DisconnectClient(socket);

		return;
	}

	if (0 < iResult)
	{
		// write�Ѹ�ŭ recvQ �̵�
		pClient->RecvQ.MoveWritePos(iResult);

		// ó���� �� �ִ¸�ŭ �ѹ��� ó��
		while (1)
		{
			iResult = MakeRecvPacket(pClient);

			// ���� ������ ��Ŷ�� ���� ����� �ƴ�
			if (1 == iResult)
			{
				break;
			}
			// ����
			if (-1 == iResult)
			{
				err_display(L"MakeRecvPacket()");
				return;
			}
		}
	}
}

void netProc_Send(SOCKET socket)
{
	st_CLIENT	*pClient;
	int			iResult;
	int			iSendSize;

	pClient = FindClient(socket);

	if ( nullptr == pClient )
	{
		return;
	}

	iSendSize = pClient->SendQ.GetUseSize();

	if (0 >= iSendSize)
	{
		return;
	}

	iResult = send(pClient->sock, pClient->SendQ.GetReadBufferPtr(), iSendSize, 0);

	if ( SOCKET_ERROR == iResult )
	{
		DWORD dwError = WSAGetLastError();

		if (WSAEWOULDBLOCK == dwError)
		{
			wprintf(L"Socket WOULDBLOCK\n");
			return;
		}
		
		wprintf(L"Socket Error\n");

		DisconnectClient(pClient->sock);
		closesocket(pClient->sock);
		
		return;
	}
	else
	{
		if (iSendSize < iResult)
		{
			err_quit(L"Send size Error");
			return;
		}
		else
		{
			pClient->SendQ.RemoveData(iResult);
		}
	}
	return;
}

void netProc_Accept(void)
{
	st_CLIENT *pClient	= new st_CLIENT;
	int iAddrLen		= 0;
	WCHAR szTemp[32]	= { 0, };


	iAddrLen = sizeof(pClient->connectAddr);

	pClient->sock = accept(g_ListenSocket,
		(SOCKADDR*)&pClient->connectAddr, &iAddrLen);

	pClient->pAccount = nullptr;

	if (INVALID_SOCKET == pClient->sock)
	{
		int iErr = WSAGetLastError();

		if (WSAEWOULDBLOCK != iErr)
		{
			err_display(L"accpet()");
		}
	}
	else
	{
		g_ClientMap.insert( pair< SOCKET, st_CLIENT*>( pClient->sock, pClient ) );

		InetNtopW(AF_INET, &pClient->connectAddr.sin_addr, szTemp, 32);
		wprintf(L"Accept - %s:%d [Socket:%lld]\n",
			szTemp, ntohs(pClient->connectAddr.sin_port), pClient->sock);
	}
}

////////////////////////////////////////////////////////////
//
//			Request
//
////////////////////////////////////////////////////////////

BOOL netPacket_ReqAccountAdd(st_CLIENT * pClient, CProtocolBuffer * pPacket)
{
	/*// account�� �ִ� ���¶�� ����
	if (nullptr != pClient->pAccount)
	{
		return FALSE;
	}*/

	st_DATA_ACCOUNT *pAccount = nullptr;

	WCHAR szNickname[dfNICK_MAX_LEN] = { 0, };

	int iSize = pPacket->GetDataSize();
	pPacket->GetData((char*)szNickname, iSize);


	//// �г����� �ߺ��Ǹ� ����
	////szNickname[iSize] = L'\0';

	//// account map ��ȸ�ϸ鼭 �ߺ� ���̵� �ִ��� �˻�
	//map<UINT64, st_DATA_ACCOUNT*>::iterator itAccount;

	//for (itAccount = g_AccountMap.begin(); itAccount != g_AccountMap.end(); itAccount++)
	//{
	//	if ( 0 == wcscmp( szNickname, itAccount->second->szID ) )
	//	{
	//		// �г��� �ߺ�
	//		return FALSE;
	//	}
	//}

	pAccount = new st_DATA_ACCOUNT;

	pClient->pAccount = pAccount;

	// �г��� ����
	wcscpy_s(pClient->pAccount->szID, szNickname);

	// id ����
	pClient->pAccount->AccountNo = g_AccountIncrement;
	
	// map�� �߰�
	g_AccountMap.insert( pair< INT64, st_DATA_ACCOUNT* >( pClient->pAccount->AccountNo, pClient->pAccount ) );

	// ���� �߰� ����� Ŭ�� ����
	Send_ResAccountAdd(pClient);

	return TRUE;
}

BOOL netPacket_ReqLogin(st_CLIENT * pClient, CProtocolBuffer *pPacket)
{
	st_DATA_ACCOUNT *pAccount = nullptr;

	UINT64 AccountNo = 0;
	//WCHAR szNickname[dfNICK_MAX_LEN];

	BYTE byResult = 0;
	
	(*pPacket) >> AccountNo;

	// ã�Ƽ� ������ 
	map<UINT64, st_DATA_ACCOUNT*>::iterator AccountIter;

	for (AccountIter = g_AccountMap.begin(); AccountIter != g_AccountMap.end(); AccountIter++)
	{
		pAccount = AccountIter->second;

		if (AccountNo == pAccount->AccountNo)
		{
			byResult = 1;

			// Ŭ���̾�Ʈ�� ������ ��������
			pClient->pAccount = pAccount;
		}
	}

	Send_ResLogin(pClient);

	return TRUE;
}

BOOL netPacket_ReqAccountList(st_CLIENT * pClient, CProtocolBuffer *pPacket)
{
	Send_ResAccountList(pClient);

	return TRUE;
}

BOOL netPacket_ReqFriendList(st_CLIENT * pClient, CProtocolBuffer *pPacket)
{
	Send_ResFriendList(pClient);

	return TRUE;
}

BOOL netPacket_ReqFriendList_Request(st_CLIENT * pClient, CProtocolBuffer *pPacket)
{
	Send_ResFriendList_Request(pClient);

	return TRUE;
}

BOOL netPacket_ReqFriendList_Reply(st_CLIENT * pclient, CProtocolBuffer *pPacket)
{
	return TRUE;
}

BOOL netPacket_ReqFriendRemove(st_CLIENT * pclient, CProtocolBuffer *pPacket)
{
	return TRUE;
}

BOOL netPacket_ReqFriendRequest(st_CLIENT * pclient, CProtocolBuffer *pPacket)
{
	UINT64	FriendAccountNo = 0;
	BYTE	byResult		= df_RESULT_FRIEND_REQUEST_NOTFOUND;

	(*pPacket) >> FriendAccountNo;

	// �α����� �Ǿ����� Ȯ��
	if (nullptr != pclient->pAccount)
	{
		// �̹� ģ������ Ȯ��
		UINT64 ToAccountNo	= pclient->pAccount->AccountNo;
		UINT64 iKeyCount	= 0;

		iKeyCount = g_FriendIndex_To.count(ToAccountNo) + g_FriendIndex_From.count(ToAccountNo);

		multimap<UINT64, UINT64>::iterator FriendToIter_Lower;
		multimap<UINT64, UINT64>::iterator FriendToIter_Upper;

		FriendToIter_Lower = g_FriendIndex_To.lower_bound(ToAccountNo);
		FriendToIter_Upper = g_FriendIndex_To.upper_bound(ToAccountNo);

		multimap<UINT64, UINT64>::iterator FriendFromIter_Lower;
		multimap<UINT64, UINT64>::iterator FriendFromIter_Upper;

		FriendFromIter_Lower = g_FriendIndex_From.lower_bound(ToAccountNo);
		FriendFromIter_Upper = g_FriendIndex_From.upper_bound(ToAccountNo);

		multimap<UINT64, st_DATA_FRIEND*>::iterator FriendIter_Lower;
		multimap<UINT64, st_DATA_FRIEND*>::iterator FriendIter_Upper;

		multimap<UINT64, UINT64>::iterator IndexIter;
		multimap<UINT64, st_DATA_FRIEND*>::iterator MapIter;

		for (IndexIter = FriendToIter_Lower; IndexIter != FriendToIter_Upper; IndexIter++)
		{
			st_DATA_FRIEND *pFriend;

			UINT64 MapKey = IndexIter->second;

			FriendIter_Lower = g_FriendMap.lower_bound(MapKey);
			FriendIter_Upper = g_FriendMap.upper_bound(MapKey);

			for (MapIter = FriendIter_Lower; MapIter != FriendIter_Upper; MapIter++)
			{
				pFriend = MapIter->second;

				if (pFriend->FromAccountNo == FriendAccountNo)
				{
					byResult = df_RESULT_FRIEND_REQUEST_ALREADY;
				}
			}
		}

		for (IndexIter = FriendFromIter_Lower; IndexIter != FriendFromIter_Upper; IndexIter++)
		{
			st_DATA_FRIEND *pFriend;

			UINT64 MapKey = IndexIter->second;

			FriendIter_Lower = g_FriendMap.lower_bound(MapKey);
			FriendIter_Upper = g_FriendMap.upper_bound(MapKey);

			for (MapIter = FriendIter_Lower; MapIter != FriendIter_Upper; MapIter++)
			{
				pFriend = MapIter->second;

				if (pFriend->ToAccountNo == FriendAccountNo)
				{
					byResult = df_RESULT_FRIEND_REQUEST_ALREADY;
				}
			}
		}

		// ������� �Դٸ� ģ���� �ƴ� ����
		// ģ�� ��û�� ���� �����ϴ� Account No���� ã�´�
		if (df_RESULT_FRIEND_REQUEST_ALREADY != byResult)
		{
			map<UINT64, st_DATA_ACCOUNT*>::iterator AccountIter;

			for (AccountIter = g_AccountMap.begin(); AccountIter != g_AccountMap.end(); AccountIter++)
			{
				if (FriendAccountNo == AccountIter->second->AccountNo)
				{
					if (AddFriendRequest(ToAccountNo, FriendAccountNo))
					{
						byResult = df_RESULT_FRIEND_REQUEST_OK;
					}	
				}
			}
		}
	}

	Send_ResFriendRequest(pclient, FriendAccountNo, byResult);

	return TRUE;
}

BOOL netPacket_ReqFriendCancel(st_CLIENT *pclient, CProtocolBuffer *pPacket)
{
	return TRUE;
}

BOOL netPacket_ReqFriendDeny(st_CLIENT *pclient, CProtocolBuffer *pPacket)
{
	return TRUE;
}

BOOL netPacket_ReqFriendAgree(st_CLIENT *pclient, CProtocolBuffer *pPacket)
{
	return TRUE;
}

////////////////////////////////////////////////////////////
//
//			Response
//
////////////////////////////////////////////////////////////
void Send_ResAccountAdd(st_CLIENT * pClient)
{
	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		Packet;
	
	if (nullptr == pClient->pAccount)
	{
		return;
	}

	makePacket_ResAccountAdd(&stHeader, &Packet, pClient->pAccount->AccountNo);
	SendUnicast(pClient, &stHeader, &Packet);
}

void Send_ResLogin(st_CLIENT * pClient)
{
	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		Packet;

	if (nullptr == pClient->pAccount)
	{
		return;
	}

	makePacket_ResLogin(&stHeader, &Packet, pClient->pAccount);

	SendUnicast(pClient, &stHeader, &Packet);
}

void Send_ResAccountList(st_CLIENT * pClient)
{
	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		Packet;

	makePacket_ResAccountList(&stHeader, &Packet);

	SendUnicast(pClient, &stHeader, &Packet);
}

void Send_ResFriendList(st_CLIENT * pClient)
{
	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		Packet;

	makePacket_ResFriendList(&stHeader, &Packet, pClient->pAccount);

	SendUnicast(pClient, &stHeader, &Packet);
}

void Send_ResFriendList_Request(st_CLIENT * pClient)
{
	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		Packet;

	makePacket_ResFriendList_Request(&stHeader, &Packet, pClient->pAccount);

	SendUnicast(pClient, &stHeader, &Packet);
}

void Send_ResFriendList_Reply(st_CLIENT * pClient)
{
	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		Packet;

	makePacket_ResFriendList_Reply(&stHeader, &Packet, pClient->pAccount);

	SendUnicast(pClient, &stHeader, &Packet);
}

void Send_ResFriendRemove(st_CLIENT * pClient)
{
	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		Packet;

	SendUnicast(pClient, &stHeader, &Packet);
}

void Send_ResFriendRequest(st_CLIENT * pClient, UINT64 AccountNo, BYTE byResult)
{
	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		Packet;

	makePacket_ResFriendRequest(&stHeader, &Packet, AccountNo, byResult);

	SendUnicast(pClient, &stHeader, &Packet);
}

void Send_ResFriendCancel(st_CLIENT * pClient)
{
	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		Packet;

	SendUnicast(pClient, &stHeader, &Packet);
}

void Send_ResFriendDeny(st_CLIENT * pClient)
{
	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		Packet;

	SendUnicast(pClient, &stHeader, &Packet);
}

void Send_ResFriendAgree(st_CLIENT * pClient, BYTE byResult)
{
	st_PACKET_HEADER	stHeader;
	CProtocolBuffer		Packet;

	SendUnicast(pClient, &stHeader, &Packet);
}

////////////////////////////////////////////////////////////
//
//			Response ��Ŷ �����
//
////////////////////////////////////////////////////////////
void makePacket_ResAccountAdd(st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket, UINT64 AccountNo)
{
	pPacket->Clear();

	(*pPacket) << AccountNo;

	pHeader->byCode = dfPACKET_CODE;
	pHeader->wMsgType = df_RES_ACCOUNT_ADD;
	pHeader->wPayloadSize = pPacket->GetDataSize();
}

void makePacket_ResLogin(st_PACKET_HEADER * pHeader, CProtocolBuffer * pPacket, st_DATA_ACCOUNT *pAccount)
{
	pPacket->Clear();

	if (nullptr == pAccount)
	{
		WCHAR	szNickname[dfNICK_MAX_LEN]	= { 0, };
		UINT64	AccountNo					= 0;

		(*pPacket) << AccountNo;

		pPacket->PutData( ( char* )szNickname, dfNICK_MAX_LEN * sizeof(WCHAR));
	}
	else
	{
		(*pPacket) << pAccount->AccountNo;
		pPacket->PutData( ( char* )pAccount->szID, dfNICK_MAX_LEN * sizeof(WCHAR) );
	}

	// ��� �Է�
	pHeader->byCode			= dfPACKET_CODE;
	pHeader->wMsgType		= df_RES_LOGIN;
	pHeader->wPayloadSize	= pPacket->GetDataSize();
}

void makePacket_ResAccountList(st_PACKET_HEADER *pHeader, CProtocolBuffer * pPacket)
{
	st_DATA_ACCOUNT *pAccount;

	UINT64 AccountSize = g_AccountMap.size();

	//WCHAR szNickname[dfNICK_MAX_LEN] = L"TEST";
	//UINT64 AccountNo = 0X01;

	// ��Ŷ ����
	pPacket->Clear();

	// ȸ���� �ְ�
	(*pPacket) << (int)g_AccountMap.size();

	// account map ��ȸ�ϸ鼭 ���� �ϳ��� �������� �ֱ�
	
	map<UINT64, st_DATA_ACCOUNT*>::iterator AccountIter;

	for (AccountIter = g_AccountMap.begin(); AccountIter != g_AccountMap.end(); AccountIter++)
	{
		pAccount = (*AccountIter).second;

		(*pPacket) << pAccount->AccountNo;
		pPacket->PutData((char*)pAccount->szID, dfNICK_MAX_LEN * sizeof(WCHAR));
	}

	// ��� �Է�
	pHeader->byCode			= dfPACKET_CODE;
	pHeader->wMsgType		= df_RES_ACCOUNT_LIST;
	pHeader->wPayloadSize	= pPacket->GetDataSize();
}

void makePacket_ResFriendList(st_PACKET_HEADER *pHeader, CProtocolBuffer * pPacket, st_DATA_ACCOUNT *pAccount)
{
	pPacket->Clear();

	if (nullptr == pAccount)
	{
		(*pPacket) << (int)0;
	}
	else
	{
		UINT64 IndexKey = pAccount->AccountNo;
		UINT64 iKeyCount = 0;

		iKeyCount = g_FriendIndex_To.count(IndexKey) + g_FriendIndex_From.count(IndexKey);

		multimap<UINT64, UINT64>::iterator FriendToIter_Lower;
		multimap<UINT64, UINT64>::iterator FriendToIter_Upper;

		FriendToIter_Lower = g_FriendIndex_To.lower_bound(IndexKey);
		FriendToIter_Upper = g_FriendIndex_To.upper_bound(IndexKey);

		multimap<UINT64, UINT64>::iterator FriendFromIter_Lower;
		multimap<UINT64, UINT64>::iterator FriendFromIter_Upper;

		FriendFromIter_Lower = g_FriendIndex_From.lower_bound(IndexKey);
		FriendFromIter_Upper = g_FriendIndex_From.upper_bound(IndexKey);

		multimap<UINT64, st_DATA_FRIEND*>::iterator FriendIter_Lower;
		multimap<UINT64, st_DATA_FRIEND*>::iterator FriendIter_Upper;

		multimap<UINT64, UINT64>::iterator IndexIter;
		multimap<UINT64, st_DATA_FRIEND*>::iterator MapIter;

		for (IndexIter = FriendToIter_Lower; IndexIter != FriendToIter_Upper; IndexIter++ )
		{
			st_DATA_FRIEND *pFriend;

			UINT64 MapKey = IndexIter->second;

			FriendIter_Lower = g_FriendMap.lower_bound(MapKey);
			FriendIter_Upper = g_FriendMap.upper_bound(MapKey);

			for (MapIter = FriendIter_Lower; MapIter != FriendIter_Upper; MapIter++)
			{
				pFriend = MapIter->second;

				(*pPacket) << pFriend->FromAccountNo;
				pPacket->PutData((char*)g_AccountMap[pFriend->FromAccountNo]->szID, dfNICK_MAX_LEN * sizeof(WCHAR));
			}
		}
	}

	// ��� �Է�
	pHeader->byCode			= dfPACKET_CODE;
	pHeader->wMsgType		= df_RES_FRIEND_LIST;
	pHeader->wPayloadSize	= pPacket->GetDataSize();
}

void makePacket_ResFriendList_Request(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_DATA_ACCOUNT *pAccount)
{
	pPacket->Clear();

	if (nullptr == pAccount)
	{
		(*pPacket) << (UINT)0;
	}
	else
	{
		UINT64 IndexKey = pAccount->AccountNo;
		UINT64 iKeyCount = 0;

		iKeyCount = g_FriendRequestIndex_To.count(IndexKey) + g_FriendRequestIndex_From.count(IndexKey);
		(*pPacket) << (UINT)iKeyCount;

		multimap<UINT64, UINT64>::iterator FriendToIter_Lower;
		multimap<UINT64, UINT64>::iterator FriendToIter_Upper;

		FriendToIter_Lower = g_FriendRequestIndex_To.lower_bound(IndexKey);
		FriendToIter_Upper = g_FriendRequestIndex_To.upper_bound(IndexKey);

		multimap<UINT64, UINT64>::iterator FriendFromIter_Lower;
		multimap<UINT64, UINT64>::iterator FriendFromIter_Upper;

		FriendFromIter_Lower = g_FriendRequestIndex_From.lower_bound(IndexKey);
		FriendFromIter_Upper = g_FriendRequestIndex_From.upper_bound(IndexKey);

		multimap<UINT64, st_DATA_FRIEND_REQUEST*>::iterator FriendIter_Lower;
		multimap<UINT64, st_DATA_FRIEND_REQUEST*>::iterator FriendIter_Upper;

		multimap<UINT64, UINT64>::iterator IndexIter;
		multimap<UINT64, st_DATA_FRIEND_REQUEST*>::iterator MapIter;

		for (IndexIter = FriendToIter_Lower; IndexIter != FriendToIter_Upper; IndexIter++)
		{
			st_DATA_FRIEND_REQUEST *pFriend;

			UINT64 MapKey = IndexIter->second;

			FriendIter_Lower = g_FriendRequestMap.lower_bound(MapKey);
			FriendIter_Upper = g_FriendRequestMap.upper_bound(MapKey);

			for (MapIter = FriendIter_Lower; MapIter != FriendIter_Upper; MapIter++)
			{
				pFriend = MapIter->second;

				(*pPacket) << pFriend->FromAccountNo;
				pPacket->PutData((char*)g_AccountMap[pFriend->FromAccountNo]->szID, dfNICK_MAX_LEN * sizeof(WCHAR));
			}
		}

		for (IndexIter = FriendFromIter_Lower; IndexIter != FriendFromIter_Upper; IndexIter++)
		{
			st_DATA_FRIEND_REQUEST *pFriend;

			UINT64 MapKey = IndexIter->second;

			FriendIter_Lower = g_FriendRequestMap.lower_bound(MapKey);
			FriendIter_Upper = g_FriendRequestMap.upper_bound(MapKey);

			for (MapIter = FriendIter_Lower; MapIter != FriendIter_Upper; MapIter++)
			{
				pFriend = MapIter->second;

				(*pPacket) << pFriend->ToAccountNo;
				pPacket->PutData((char*)g_AccountMap[pFriend->ToAccountNo]->szID, dfNICK_MAX_LEN * sizeof(WCHAR));
			}
		}
	}

	// ��� �Է�
	pHeader->byCode			= dfPACKET_CODE;
	pHeader->wMsgType		= df_RES_FRIEND_REQUEST_LIST;
	pHeader->wPayloadSize	= pPacket->GetDataSize();
}

void makePacket_ResFriendList_Reply(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, st_DATA_ACCOUNT *pAccount)
{
	pPacket->Clear();

	if (nullptr == pAccount)
	{
		(*pPacket) << (UINT)0;
	}
	else
	{
		UINT64 IndexKey = pAccount->AccountNo;
		UINT64 iKeyCount = 0;

		iKeyCount = g_FriendRequestIndex_To.count(IndexKey) + g_FriendRequestIndex_From.count(IndexKey);
		(*pPacket) << (UINT)iKeyCount;

		multimap<UINT64, UINT64>::iterator FriendToIter_Lower;
		multimap<UINT64, UINT64>::iterator FriendToIter_Upper;

		FriendToIter_Lower = g_FriendRequestIndex_To.lower_bound(IndexKey);
		FriendToIter_Upper = g_FriendRequestIndex_To.upper_bound(IndexKey);

		multimap<UINT64, UINT64>::iterator FriendFromIter_Lower;
		multimap<UINT64, UINT64>::iterator FriendFromIter_Upper;

		FriendFromIter_Lower = g_FriendRequestIndex_From.lower_bound(IndexKey);
		FriendFromIter_Upper = g_FriendRequestIndex_From.upper_bound(IndexKey);

		multimap<UINT64, st_DATA_FRIEND_REQUEST*>::iterator FriendIter_Lower;
		multimap<UINT64, st_DATA_FRIEND_REQUEST*>::iterator FriendIter_Upper;

		multimap<UINT64, UINT64>::iterator IndexIter;
		multimap<UINT64, st_DATA_FRIEND_REQUEST*>::iterator MapIter;

		for (IndexIter = FriendToIter_Lower; IndexIter != FriendToIter_Upper; IndexIter++)
		{
			st_DATA_FRIEND_REQUEST *pFriend;

			UINT64 MapKey = IndexIter->second;

			FriendIter_Lower = g_FriendRequestMap.lower_bound(MapKey);
			FriendIter_Upper = g_FriendRequestMap.upper_bound(MapKey);

			for (MapIter = FriendIter_Lower; MapIter != FriendIter_Upper; MapIter++)
			{
				pFriend = MapIter->second;

				(*pPacket) << pFriend->FromAccountNo;
				pPacket->PutData((char*)g_AccountMap[pFriend->FromAccountNo]->szID, dfNICK_MAX_LEN * sizeof(WCHAR));
			}
		}

		for (IndexIter = FriendFromIter_Lower; IndexIter != FriendFromIter_Upper; IndexIter++)
		{
			st_DATA_FRIEND_REQUEST *pFriend;

			UINT64 MapKey = IndexIter->second;

			FriendIter_Lower = g_FriendRequestMap.lower_bound(MapKey);
			FriendIter_Upper = g_FriendRequestMap.upper_bound(MapKey);

			for (MapIter = FriendIter_Lower; MapIter != FriendIter_Upper; MapIter++)
			{
				pFriend = MapIter->second;

				(*pPacket) << pFriend->ToAccountNo;
				pPacket->PutData((char*)g_AccountMap[pFriend->ToAccountNo]->szID, dfNICK_MAX_LEN * sizeof(WCHAR));
			}
		}
	}

	// ��� �Է�
	pHeader->byCode			= dfPACKET_CODE;
	pHeader->wMsgType		= df_RES_FRIEND_REQUEST_LIST;
	pHeader->wPayloadSize	= pPacket->GetDataSize();
}

void makePacket_ResFriendRemove(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket)
{

}

void makePacket_ResFriendRequest(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket, UINT64 FriendAccountNo, BYTE byResult)
{
	pPacket->Clear();

	(*pPacket) << FriendAccountNo;
	(*pPacket) << byResult;

	pHeader->byCode			= dfPACKET_CODE;
	pHeader->wMsgType		= df_RES_FRIEND_REQUEST;
	pHeader->wPayloadSize	= pPacket->GetDataSize();
}

void makePacket_ResFriendCancel(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket)
{

}

void makePacket_ResFriendDeny(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket)
{

}

void makePacket_ResFriendAgree(st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket)
{

}

////////////////////////////////////////////////////////////
//
//			Ư�� Ŭ���̾�Ʈ���� ������
//
////////////////////////////////////////////////////////////
void SendUnicast(st_CLIENT * pClient, st_PACKET_HEADER *pHeader, CProtocolBuffer *pPacket)
{
	if (nullptr == pClient)
	{
		err_display(L"SendUnicast()");
	}

	char* chpTemp = pPacket->GetBufferPtr();

	pClient->SendQ.Put( ( char* )pHeader, sizeof( st_PACKET_HEADER ) );
	pClient->SendQ.Put( ( char* )pPacket->GetBufferPtr(), pPacket->GetDataSize() );
}

BOOL AddFriendRequest(UINT64 FromAccountNo, UINT64 ToAccountNo)
{
	st_DATA_FRIEND_REQUEST *pFriendRequest = new st_DATA_FRIEND_REQUEST;

	pFriendRequest->FromAccountNo	= FromAccountNo;
	pFriendRequest->ToAccountNo		= ToAccountNo;
	
	pFriendRequest->No				= g_FriendRequestIncrement;
	g_FriendRequestIncrement++;

	g_FriendRequestMap.insert(
		pair<UINT64, st_DATA_FRIEND_REQUEST*>(pFriendRequest->No, pFriendRequest)
	);

	g_FriendRequestIndex_From.insert(
		pair<UINT64, UINT64>(FromAccountNo, pFriendRequest->No)
	);

	g_FriendRequestIndex_To.insert(
		pair<UINT64, UINT64>(ToAccountNo, pFriendRequest->No)
	);

	return TRUE;
}
