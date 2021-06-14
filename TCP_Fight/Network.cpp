#include "TCP_Fight.h"

void CreateMyCharactor(CPacket* cpPacket)
{
	int iID;
	BYTE byDir;
	short shX;
	short shY;
	BYTE byHP;

	*cpPacket >> iID >> byDir >> shX >> shY >> byHP;

	g_PlayerObject = new CPlayerObject(iID, byDir, byHP, TRUE);
	g_PlayerObject->SetPosition(shX, shY);
	g_objectList.push_back(g_PlayerObject);
}

void CreateOtherCharactor(CPacket* cpPacket)
{
	int iID;
	BYTE byDir;
	short shX;
	short shY;
	BYTE byHP;

	*cpPacket >> iID >> byDir >> shX >> shY >> byHP;

	CPlayerObject* playerObject = new CPlayerObject(iID, byDir, byHP, FALSE);
	playerObject->SetPosition(shX, shY);

	g_objectList.push_back(playerObject);
}

void DeleteCharacter(CPacket* cpPacket)
{
	int iID;

	*cpPacket >> iID;

	list<CBaseObject*>::iterator iter;
	for (iter = g_objectList.begin(); iter != g_objectList.end();)
	{
		if ((*iter)->GetObjectID() == iID)
		{
			delete (*iter);
			iter = g_objectList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void CharacterMoveStart(CPacket* cpPacket)
{
	int iID;
	BYTE byDirection;
	short shX;
	short shY;

	*cpPacket >> iID >> byDirection >> shX >> shY;

	list<CBaseObject*>::iterator iter;
	for (iter = g_objectList.begin(); iter != g_objectList.end(); ++iter)
	{
		if ((*iter)->GetObjectID() == iID)
		{
			(*iter)->ActionInput(byDirection);
			(*iter)->SetPosition(shX, shY);
			break;
		}
	}
}

void CharacterMoveStop(CPacket* cpPacket)
{
	int iID;
	BYTE byDirection;
	short shX;
	short shY;

	*cpPacket >> iID >> byDirection >> shX >> shY;

	list<CBaseObject*>::iterator iter;
	for (iter = g_objectList.begin(); iter != g_objectList.end(); ++iter)
	{
		if ((*iter)->GetObjectID() == iID)
		{
			(*iter)->ActionInput(dfACTION_STAND);
			(*iter)->SetPosition(shX, shY);
			break;
		}
	}
}

void CharacterAttack1(CPacket* cpPacket)
{
	int iID;
	BYTE byDirection;
	short shX;
	short shY;

	*cpPacket >> iID >> byDirection >> shX >> shY;

	list<CBaseObject*>::iterator iter;
	for (iter = g_objectList.begin(); iter != g_objectList.end(); ++iter)
	{
		if ((*iter)->GetObjectID() == iID)
		{
			(*iter)->ActionInput(dfACTION_ATTACK1);
			(*iter)->SetPosition(shX, shY);
			break;
		}
	}
}

void CharacterAttack2(CPacket* cpPacket)
{
	int iID;
	BYTE byDirection;
	short shX;
	short shY;

	*cpPacket >> iID >> byDirection >> shX >> shY;

	list<CBaseObject*>::iterator iter;
	for (iter = g_objectList.begin(); iter != g_objectList.end(); ++iter)
	{
		if ((*iter)->GetObjectID() == iID)
		{
			(*iter)->ActionInput(dfACTION_ATTACK2);
			(*iter)->SetPosition(shX, shY);
			break;
		}
	}
}

void CharacterAttack3(CPacket* cpPacket)
{
	int iID;
	BYTE byDirection;
	short shX;
	short shY;

	*cpPacket >> iID >> byDirection >> shX >> shY;


	list<CBaseObject*>::iterator iter;
	for (iter = g_objectList.begin(); iter != g_objectList.end(); ++iter)
	{
		if ((*iter)->GetObjectID() == iID)
		{
			(*iter)->ActionInput(dfACTION_ATTACK3);
			(*iter)->SetPosition(shX, shY);
			break;
		}
	}
}



void CharacteDamage(CPacket* cpPacket)
{
	int	iAttackID;
	int	iDamageID;
	BYTE byDamageHP;

	*cpPacket >> iAttackID;
	*cpPacket >> iDamageID;
	*cpPacket >> byDamageHP;


	list<CBaseObject*>::iterator iter;
	for (iter = g_objectList.begin(); iter != g_objectList.end(); ++iter)
	{
		if ((*iter)->GetObjectType()  == e_OBJECT_TYPE ::eTYPE_PLAYER)
		{			
			if ((*iter)->GetObjectID() == iDamageID)
			{
				CPlayerObject* player = (CPlayerObject*)(*iter);
				CBaseObject* pEffectObject = new CEffectObject(0, e_OBJECT_TYPE::eTYPE_EFFECT, player->GetCurX(), player->GetCurY() - 70, dfDELAY_EFFECT, 60, 63);
				g_objectList.push_back(pEffectObject);

				player->SetHP(byDamageHP);
				break;
			}
		}
	}

}

void WriteEvent()
{
	if (g_SendFlag == false)
		return;

	while (1)
	{
		if (g_SendQ.GetUseSize() == 0)
			break;

		int sendretval = send(g_Socket, g_SendQ.GetFrontBufferPtr(), g_SendQ.DirectDequeueSize(), 0);
		if (sendretval == 0 || sendretval == SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			if (errorCode == WSAEWOULDBLOCK)
				g_SendFlag = false;

			break;
		}

		g_SendQ.MoveFront(sendretval);
	}
}

void SendPacket(stHEADER* pHeader, char* pPacket)
{
	int  sendval = g_SendQ.Enqueue((char*)pHeader, 3);
	sendval = g_SendQ.Enqueue(pPacket, pHeader->bySize);
	WriteEvent();
}

void SendPacket(CPacket* cpPacket)
{
	int  sendval = g_SendQ.Enqueue(cpPacket);
	WriteEvent();
}