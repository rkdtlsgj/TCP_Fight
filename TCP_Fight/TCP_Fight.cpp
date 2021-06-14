// TCP_Fight.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"winmm.lib")
#pragma warning(disable:4996)

#pragma once
#include "TCP_Fight.h"


CScreenDib g_ScreenDib(680, 480, 32);
CSpriteDib g_Sprite(100, 0x00ffffff);

HINSTANCE hInst;
HWND g_hWnd;
BOOL g_bActive;

list<CBaseObject*> g_objectList;
CPlayerObject* g_PlayerObject;
BOOL g_SendFlag;


CRingBuffer g_RecvQ;
CRingBuffer g_SendQ;

SOCKET g_Socket;
BOOL bIsConnect;


BOOL InitialGame();
BOOL CreateWindows();
BOOL SetWindowSize(int nCmdShow);
void Update();
void Action();
void Draw();
void KeyProcess();
BOOL FrameSkip(int deltaTime);



BOOL CreateSocket();
void SocketClose();
void ReadEvent();
void WriteEvent();
void NetworkProc(WPARAM wParam, LPARAM lParam);
//void SendPacket(st_DRAW_PACKET* packet, stHEADER* header);

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int g_curTime;
int g_oldTime;
int g_deletaTime;
int g_skipTime;
bool g_flag;

int g_TitleTime;

int g_Frame;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)

{

	if (CreateWindows() == FALSE)
		return -1;

	if (SetWindowSize(nCmdShow) == FALSE)
		return -1;

	if (InitialGame() == FALSE)
		return -1;

	if (CreateSocket() == FALSE)
	{
		SocketClose();
	}

	MSG msg;


	timeBeginPeriod(1);

	g_TitleTime = 0;
	g_oldTime = timeGetTime();

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}

	return (int)msg.wParam;
}

void ReadEvent()
{		
	int retval = recv(g_Socket, g_RecvQ.GetRearBufferPtr(), g_RecvQ.DirectEnqueueSize(), 0);
	if (retval == 0 || retval == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		SocketClose();
	}

	g_RecvQ.MoveRear(retval);



	while (1)
	{
		stHEADER header;
		g_RecvQ.Peek((char*)&header, sizeof(stHEADER));

		if (header.bySize + sizeof(stHEADER) > g_RecvQ.GetUseSize())
			break;

		if (header.byCode != dfNETWORK_PACKET_CODE)
			SocketClose();


		g_RecvQ.MoveFront(sizeof(stHEADER));

		CPacket cpPacket;
		g_RecvQ.Dequeue(cpPacket.GetBufferPtr(), header.bySize);
			
		
		switch (header.byType)
		{
		case dfPACKET_SC_CREATE_MY_CHARACTER://자신의 캐릭터 할당
			CreateMyCharactor(&cpPacket);
			break;

		case dfPACKET_SC_CREATE_OTHER_CHARACTER:
			CreateOtherCharactor(&cpPacket);
			break;

		case dfPACKET_SC_DELETE_CHARACTER:
			DeleteCharacter(&cpPacket);
			break;

		case dfPACKET_SC_MOVE_START:
			CharacterMoveStart(&cpPacket);
			break;

		case dfPACKET_SC_MOVE_STOP:	
			CharacterMoveStop(&cpPacket);
			break;

		case dfPACKET_SC_ATTACK1:
			CharacterAttack1(&cpPacket);
			break;

		case dfPACKET_SC_ATTACK2:
			CharacterAttack2(&cpPacket);
			break;

		case dfPACKET_SC_ATTACK3:
			CharacterAttack3(&cpPacket);
			break;

		case dfPACKET_SC_DAMAGE:
			CharacteDamage(&cpPacket);
			break;
		}
	}
}


void Update()
{
	if (g_bActive == TRUE)
		KeyProcess();

	Action();

	g_Frame++;

	g_curTime = timeGetTime();

	if (g_curTime - g_TitleTime > 1000)
	{
		WCHAR temp[100];
		wsprintf(temp, L"LogicFrame:%d", g_Frame);
		SetWindowText(g_hWnd, temp);
		g_Frame = 0;

		g_TitleTime = g_curTime;
	}

	if (g_flag)
	{
		g_oldTime = g_curTime - (g_deletaTime - (20 - (g_curTime - g_skipTime)));
		g_flag = FALSE;
	}

	g_deletaTime = g_curTime - g_oldTime;

	if (FrameSkip(g_deletaTime) == false)
	{
		if (g_deletaTime < 20)
		{
			Sleep(20 - g_deletaTime);
		}

		g_oldTime = g_curTime - (g_deletaTime - 20);
		Draw();
	}
	else
	{
		g_flag = TRUE;
		g_skipTime = g_curTime;
	}

	g_ScreenDib.DrawBuffer(g_hWnd);

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SOCKET:
		NetworkProc(wParam, lParam);
		break;

	case WM_ACTIVATEAPP:
		g_bActive = (BOOL)wParam;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void NetworkProc(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		SocketClose();
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		bIsConnect = TRUE;
		break;

	case FD_CLOSE:
		SocketClose();
		break;

	case FD_WRITE:
		if (bIsConnect == TRUE)
		{
			g_SendFlag = true;
			WriteEvent();
		}
		break;

	case FD_READ:
		if (bIsConnect == TRUE)
		{
			ReadEvent();
		}
		break;

	default:
		break;
	}
}

void KeyProcess()
{
	DWORD action = dfACTION_STAND;

	if (GetAsyncKeyState(VK_UP))
	{
		action = dfACTION_MOVE_UU;
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		action = dfACTION_MOVE_LL;
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		action = dfACTION_MOVE_RR;
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		action = dfACTION_MOVE_DD;
	}

	if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_LEFT))
	{
		action = dfACTION_MOVE_LU;
	}

	if (GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT))
	{
		action = dfACTION_MOVE_RU;
	}

	if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT))
	{
		action = dfACTION_MOVE_LD;
	}

	if (GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_RIGHT))
	{
		action = dfACTION_MOVE_RD;
	}


	if (GetAsyncKeyState(0x41))
	{
		action = dfACTION_ATTACK1;
	}

	if (GetAsyncKeyState(0x53))
	{
		action = dfACTION_ATTACK2;
	}

	if (GetAsyncKeyState(0x44))
	{
		action = dfACTION_ATTACK3;
	}

	if(bIsConnect == TRUE && g_PlayerObject != NULL)
		g_PlayerObject->ActionInput(action);
}

void Action()
{
	list<CBaseObject*>::iterator iter;

	for (iter = g_objectList.begin(); iter != g_objectList.end();)
	{
		if ((*iter)->Run() == TRUE)
		{
			delete (*iter);
			g_objectList.erase(iter++);
		}
		else
		{
			++iter;
		}
	}

	//g_PlayerObject->Run();
}

void Draw()
{
	BYTE* bypDest = g_ScreenDib.GetDibBuffer();
	int iDestWidth = g_ScreenDib.GetWidth();
	int iDestHeight = g_ScreenDib.GetHeight();
	int iDestPitch = g_ScreenDib.GetPitch();

	g_Sprite.DrawImage(66, 0, 0, bypDest, iDestWidth, iDestHeight, iDestPitch);


	g_objectList.sort(YSort());
	g_objectList.sort(PlayerSort());

	list<CBaseObject*>::iterator iter;
	for (iter = g_objectList.begin(); iter != g_objectList.end(); ++iter)
	{
		(*iter)->Render(&g_Sprite, bypDest, iDestWidth, iDestHeight, iDestPitch);
	}	
}

BOOL InitialGame()
{

	if (g_Sprite.LoadDibSprite(0, L"SpriteData\\Stand_L_01.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(1, L"SpriteData\\Stand_L_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(2, L"SpriteData\\Stand_L_03.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(3, L"SpriteData\\Stand_L_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(4, L"SpriteData\\Stand_L_01.bmp", 71, 90) == FALSE)
		return FALSE;

	if (g_Sprite.LoadDibSprite(5, L"SpriteData\\Stand_R_01.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(6, L"SpriteData\\Stand_R_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(7, L"SpriteData\\Stand_R_03.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(8, L"SpriteData\\Stand_R_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(9, L"SpriteData\\Stand_R_01.bmp", 71, 90) == FALSE)
		return FALSE;

	if (g_Sprite.LoadDibSprite(10, L"SpriteData\\Attack1_L_01.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(11, L"SpriteData\\Attack1_L_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(12, L"SpriteData\\Attack1_L_03.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(13, L"SpriteData\\Attack1_L_04.bmp", 71, 90) == FALSE)
		return FALSE;

	if (g_Sprite.LoadDibSprite(14, L"SpriteData\\Attack1_R_01.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(15, L"SpriteData\\Attack1_R_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(16, L"SpriteData\\Attack1_R_03.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(17, L"SpriteData\\Attack1_R_04.bmp", 71, 90) == FALSE)
		return FALSE;


	if (g_Sprite.LoadDibSprite(18, L"SpriteData\\Attack2_L_01.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(19, L"SpriteData\\Attack2_L_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(20, L"SpriteData\\Attack2_L_03.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(21, L"SpriteData\\Attack2_L_04.bmp", 71, 90) == FALSE)
		return FALSE;


	if (g_Sprite.LoadDibSprite(22, L"SpriteData\\Attack2_R_01.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(23, L"SpriteData\\Attack2_R_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(24, L"SpriteData\\Attack2_R_03.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(25, L"SpriteData\\Attack2_R_04.bmp", 71, 90) == FALSE)
		return FALSE;


	if (g_Sprite.LoadDibSprite(26, L"SpriteData\\Attack3_L_01.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(27, L"SpriteData\\Attack3_L_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(28, L"SpriteData\\Attack3_L_03.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(29, L"SpriteData\\Attack3_L_04.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(30, L"SpriteData\\Attack3_L_05.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(31, L"SpriteData\\Attack3_L_06.bmp", 71, 90) == FALSE)
		return FALSE;


	if (g_Sprite.LoadDibSprite(32, L"SpriteData\\Attack3_R_01.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(33, L"SpriteData\\Attack3_R_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(34, L"SpriteData\\Attack3_R_03.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(35, L"SpriteData\\Attack3_R_04.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(36, L"SpriteData\\Attack3_R_05.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(37, L"SpriteData\\Attack3_R_06.bmp", 71, 90) == FALSE)
		return FALSE;

	if (g_Sprite.LoadDibSprite(38, L"SpriteData\\Move_L_01.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(39, L"SpriteData\\Move_L_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(40, L"SpriteData\\Move_L_03.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(41, L"SpriteData\\Move_L_04.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(42, L"SpriteData\\Move_L_05.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(43, L"SpriteData\\Move_L_06.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(44, L"SpriteData\\Move_L_07.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(45, L"SpriteData\\Move_L_08.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(46, L"SpriteData\\Move_L_09.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(47, L"SpriteData\\Move_L_10.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(48, L"SpriteData\\Move_L_12.bmp", 71, 90) == FALSE)
		return FALSE;

	if (g_Sprite.LoadDibSprite(49, L"SpriteData\\Move_R_01.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(50, L"SpriteData\\Move_R_02.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(51, L"SpriteData\\Move_R_03.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(52, L"SpriteData\\Move_R_04.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(53, L"SpriteData\\Move_R_05.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(54, L"SpriteData\\Move_R_06.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(55, L"SpriteData\\Move_R_07.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(56, L"SpriteData\\Move_R_08.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(57, L"SpriteData\\Move_R_09.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(58, L"SpriteData\\Move_R_10.bmp", 71, 90) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(59, L"SpriteData\\Move_R_12.bmp", 71, 90) == FALSE)
		return FALSE;

	if (g_Sprite.LoadDibSprite(60, L"SpriteData\\xSpark_1.bmp", 70, 70) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(61, L"SpriteData\\xSpark_2.bmp", 70, 70) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(62, L"SpriteData\\xSpark_3.bmp", 70, 70) == FALSE)
		return FALSE;
	if (g_Sprite.LoadDibSprite(63, L"SpriteData\\xSpark_4.bmp", 70, 70) == FALSE)
		return FALSE;

	if (g_Sprite.LoadDibSprite(64, L"SpriteData\\Shadow.bmp", 32, 4) == FALSE)
		return FALSE;

	if (g_Sprite.LoadDibSprite(65, L"SpriteData\\HPGuage.bmp", 0, 0) == FALSE)
		return FALSE;

	if (g_Sprite.LoadDibSprite(66, L"SpriteData\\_Map.bmp", 0, 0) == FALSE)
		return FALSE;



	return TRUE;
}

BOOL FrameSkip(int deltaTime)
{
	if (deltaTime >= 40)
		return TRUE;

	return FALSE;
}

BOOL CreateSocket()
{
	bIsConnect = FALSE;

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return FALSE;
	}

	g_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (g_Socket == INVALID_SOCKET)
	{
		return FALSE;
	}

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &addr.sin_addr);
	addr.sin_port = htons(SERVER_PORT);

	WSAAsyncSelect(g_Socket, g_hWnd, WM_SOCKET, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

	int retval = connect(g_Socket, (SOCKADDR*)&addr, sizeof(addr));

	if (retval == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSAEWOULDBLOCK)
		{
			return FALSE;
		}
	}
	else
	{
		bIsConnect = TRUE;
	}

	return TRUE;
}

void SocketClose()
{
	closesocket(g_Socket);
	PostQuitMessage(0);
}

BOOL CreateWindows()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInst;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"TCP";
	wcex.hIconSm = NULL;

	RegisterClassExW(&wcex);

	g_hWnd = CreateWindowW(L"TCP", L"TCP Fight", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInst, nullptr);

	if (!g_hWnd)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL SetWindowSize(int nCmdShow)
{
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);
	SetFocus(g_hWnd);

	RECT WindowRect;
	WindowRect.top = 0;
	WindowRect.left = 0;
	WindowRect.right = 640;
	WindowRect.bottom = 480;
	AdjustWindowRectEx(&WindowRect, GetWindowStyle(g_hWnd), GetMenu(g_hWnd) != NULL,

		GetWindowExStyle(g_hWnd));

	int iX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (640 / 2); //화면의 크기를 얻어서 정 중앙으로
	int iY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (480 / 2);
	MoveWindow(g_hWnd, iX, iY,

		WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, TRUE);

	return TRUE;
}
