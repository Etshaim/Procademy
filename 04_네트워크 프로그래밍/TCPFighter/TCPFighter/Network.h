#ifndef		__NETWORK_H__
#define		__NETWORK_H__

#include "stdafx.h"
//#include "Protocol.h"

struct st_PACKET_HEADER;

// 에러 및 종료 return FALSE
// 정상 처리	return TRUE
BOOL	NetworkProc(HWND hWnd, WPARAM wParam, LPARAM lParam);

BOOL	RecvEvent(void);
void	PacketProc(BYTE byPacketType, char *Packet);

BOOL	SendPacket(st_PACKET_HEADER *pHeader, char *pPacket);
BOOL	SendEvent(void);

// conncect
void Connect(HWND hWnd);

// 소켓 관련 윈도우 메시지 처리
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 소켓 함수 오류 출력
void err_quit(LPWSTR msg);
void err_display(LPWSTR msg);

#endif	// !__NETWORK_H__