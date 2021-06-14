#pragma once


#include <winsock2.h>
#include <WS2tcpip.h>
#include <windowsx.h>
#include <timeapi.h>
#include <list>
#include <memory.h>
#include <Windows.h>

#include "framework.h"
#include "SpriteDib.h"
#include "ScreenDib.h"
#include "CPlayerObject.h"
#include "CEffectObject.h"
#include "CPacket.h"
#include "resource.h"

#include "Network.h"



using namespace std;

extern list<CBaseObject*> g_objectList;
extern CPlayerObject* g_PlayerObject;
extern BOOL g_SendFlag;


extern CRingBuffer g_RecvQ;
extern CRingBuffer g_SendQ;

extern SOCKET g_Socket;
extern BOOL bIsConnect;

struct YSort
{
	bool operator() (CBaseObject* left, CBaseObject* right) const
	{
		return left->GetCurY() < right->GetCurY();
	}
};

struct PlayerSort
{
	bool operator() (CBaseObject* left, CBaseObject* right) const
	{
		return left->GetObjectType() < right->GetObjectType();
	}
};