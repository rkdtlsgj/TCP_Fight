#include "CNetworkManager.h"


BOOL CNetworkManager::NetWorkProc(WPARAM wParam, LPARAM lParam)
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

	return TRUE;
}

void CNetworkManager::WriteEvent()
{
	if (g_SendFlag == false)
		return;

	char buffsize[5000];

	while (1)
	{
		if (g_SendQ.GetUseSize() == 0)
			break;

		int size = g_SendQ.Peek(buffsize, 5000);

		int sendretval = send(g_Socket, buffsize, size, 0);
		if (sendretval == 0 || sendretval == SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();

			g_SendFlag = false;
			return;
		}

		g_SendQ.MoveFront(sendretval);
	}
}

void CNetworkManager::ReadEvent()
{
	char buffer[5000];
	int size = g_RecvQ.GetFreeSize();

	int retval = recv(g_Socket, buffer, size, 0);
	if (retval == 0 || retval == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();

		SocketClose();
	}

	g_RecvQ.Enqueue(buffer, retval);

	while (1)
	{
		stHEADER header;
		g_RecvQ.Peek((char*)&header, sizeof(stHEADER));

		if (header.bySize + sizeof(stHEADER) > g_RecvQ.GetUseSize())
			break;

		if (header.byCode != 0x89)
			SocketClose();

		g_RecvQ.MoveFront(sizeof(stHEADER));

		switch (header.byType)
		{
		case dfPACKET_SC_CREATE_MY_CHARACTER://자신의 캐릭터 할당
		{
			st_SC_MY_CHARACTOR_CREATE packet;
			g_RecvQ.Dequeue((char*)&packet, header.bySize);
			CreateMyCharactor(packet);
			break;
		}

		case dfPACKET_SC_CREATE_OTHER_CHARACTER:
		{
			st_SC_OTHER_CHARACTOR_CREATE packet;
			g_RecvQ.Dequeue((char*)&packet, header.bySize);
			CreateOtherCharactor(packet);
			break;
		}
		case dfPACKET_SC_DELETE_CHARACTER:
		{
			st_SC_CHARACTOR_DELETE packet;
			g_RecvQ.Dequeue((char*)&packet, header.bySize);
			DeleteCharacter(packet);
			break;
		}
		}
	}
}


BOOL CNetworkManager::CreateSocket(HWND hWnd)
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

	WSAAsyncSelect(g_Socket, hWnd, WM_SOCKET, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);

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


void CNetworkManager::SocketClose()
{
	closesocket(g_Socket);
	PostQuitMessage(0);
}
