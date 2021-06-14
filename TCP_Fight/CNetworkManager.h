#pragma once
#include "CRingBuffer.h"
#include "PacketDefine.h"
#include "EnumList.h"

#include <winsock2.h>
#include <WS2tcpip.h>
#include <windowsx.h>
#include <timeapi.h>

class CNetworkManager
{
public:
	CNetworkManager();
	~CNetworkManager();

	BOOL NetWorkProc(WPARAM wParam, LPARAM lParam);
	BOOL CreateSocket(HWND hWnd);

private:
	SOCKET g_Socket;
	BOOL bIsConnect;
	BOOL g_SendFlag;

	CRingBuffer g_RecvQ;
	CRingBuffer g_SendQ;


	void SocketClose();
	void ReadEvent();
	void WriteEvent();
};