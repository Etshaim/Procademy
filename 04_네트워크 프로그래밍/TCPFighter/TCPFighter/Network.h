#ifndef		__NETWORK_H__
#define		__NETWORK_H__

#include "stdafx.h"
//#include "Protocol.h"

struct st_PACKET_HEADER;

// ���� �� ���� return FALSE
// ���� ó��	return TRUE
BOOL	NetworkProc(HWND hWnd, WPARAM wParam, LPARAM lParam);

BOOL	RecvEvent(void);
void	PacketProc(BYTE byPacketType, char *Packet);

BOOL	SendPacket(st_PACKET_HEADER *pHeader, char *pPacket);
BOOL	SendEvent(void);

// conncect
void Connect(HWND hWnd);

// ���� ���� ������ �޽��� ó��
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ���� �Լ� ���� ���
void err_quit(LPWSTR msg);
void err_display(LPWSTR msg);

#endif	// !__NETWORK_H__