#include "TCP_Fight.h"
#include "CPlayerObject.h"


CPlayerObject::CPlayerObject(int id, BYTE dir, char hp, BOOL isPlayer)
{
	m_iObjectID = id;
	m_iObjectType = e_OBJECT_TYPE::eTYPE_PLAYER;
	m_bPlayerCharacter = isPlayer;
	m_ByDirCur = dir;
	m_ByMoveCur = dir;
	m_chHP = hp;
	m_dwActionInput = dfACTION_STAND;
	SetActionStand();
}

CPlayerObject::~CPlayerObject()
{

}


BOOL CPlayerObject::Run()
{
	NextFrame();
	ActionProc();
	return FALSE;
}

void CPlayerObject::ActionProc()
{
	switch (m_dwActionCur)
	{
	case dfACTION_ATTACK1:
	case dfACTION_ATTACK2:
	case dfACTION_ATTACK3:
		if (IsPlayer() == false && m_dwActionCur != m_dwActionInput)
		{
			switch (m_dwActionInput)
			{
			case dfACTION_ATTACK1:
				SetActionAttack1();
				break;
			case dfACTION_ATTACK2:
				SetActionAttack2();
				break;
			case dfACTION_ATTACK3:
				SetActionAttack3();
				break;
			}
		}
		if (IsEndFrame())
		{

			SetActionStand();
			m_dwActionInput = dfACTION_STAND;
		}
		break;
	default:
		InputActionProc();
		break;
	}
}

void CPlayerObject::InputActionProc()
{
	short moveX = 0;
	short moveY = 0;

	switch (m_dwActionInput)
	{
	case dfACTION_MOVE_LL:
		m_ByDirCur = 0;
		moveX -= 3;
		break;

	case dfACTION_MOVE_LU:
		m_ByDirCur = 0;
		moveX -= 3;
		moveY -= 2;
		break;

	case dfACTION_MOVE_UU:
		moveY -= 2;
		break;

	case dfACTION_MOVE_RU:
		m_ByDirCur = 4;
		moveX += 3;
		moveY -= 2;
		break;

	case dfACTION_MOVE_RR:
		m_ByDirCur = 4;
		moveX += 3;
		break;

	case dfACTION_MOVE_RD:
		m_ByDirCur = 4;
		moveX += 3;
		moveY += 2;
		break;

	case dfACTION_MOVE_DD:
		moveY += 2;
		break;

	case dfACTION_MOVE_LD:
		m_ByDirCur = 0;
		moveX -= 3;
		moveY += 2;
		break;
	}

	if (moveX != 0 || moveY != 0)
	{
		m_iCurX = max(dfRANGE_MOVE_LEFT, m_iCurX + moveX);
		m_iCurX = min(dfRANGE_MOVE_RIGHT, m_iCurX);

		m_iCurY = max(dfRANGE_MOVE_TOP, m_iCurY + moveY);
		m_iCurY = min(dfRANGE_MOVE_BOTTOM, m_iCurY);

		if (m_dwActionCur != m_dwActionInput)
		{
			SetActionMove(m_dwActionInput);
		}
	}
	else
	{
		if (m_dwActionCur != m_dwActionInput)
		{
			if (m_dwActionCur < 8)
			{
				if (m_bPlayerCharacter == TRUE)
				{
					CPacket cPacket;
					mpMoveStop(&cPacket, m_ByDirCur, GetCurX(), GetCurY());
					SendPacket(&cPacket);
				}
			}

			if (m_dwActionInput == dfACTION_ATTACK1)
				SetActionAttack1();
			else if (m_dwActionInput == dfACTION_ATTACK2)
				SetActionAttack2();
			else if (m_dwActionInput == dfACTION_ATTACK3)
				SetActionAttack3();
			else
				SetActionStand();
		}
	}
}

void CPlayerObject::SetActionStand()
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_STAND;

	if (m_ByDirCur == 0)
	{
		SetSprite(0, 4, dfDELAY_STAND);
	}
	else
	{
		SetSprite(5, 9, dfDELAY_STAND);
	}
}

void CPlayerObject::SetActionMove(DWORD action)
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = action;
	m_ByMoveCur = m_dwActionCur;

	if (m_ByDirCur == 0)
	{
		SetSprite(38, 48, dfDELAY_MOVE);
	}
	else
	{
		SetSprite(49, 59, dfDELAY_MOVE);
	}

	if (m_bPlayerCharacter == TRUE)
	{
		CPacket cPacket;
		mpMoveStart(&cPacket, GetDircection(), GetCurX(), GetCurY());
		SendPacket(&cPacket);
	}

}

void CPlayerObject::SetActionAttack1()
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_ATTACK1;

	if (m_ByDirCur == 0)
	{
		SetSprite(10, 13, dfDELAY_ATTACK1);
	}
	else
	{
		SetSprite(14, 17, dfDELAY_ATTACK1);
	}

	if (m_bPlayerCharacter == TRUE)
	{
		CPacket cPacket;
		mpAttack1(&cPacket, m_ByDirCur, GetCurX(), GetCurY());
		SendPacket(&cPacket);
	}

}


void CPlayerObject::SetActionAttack2()
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_ATTACK2;

	if (m_ByDirCur == 0)
	{
		SetSprite(18, 21, dfDELAY_ATTACK2);
	}
	else
	{
		SetSprite(22, 25, dfDELAY_ATTACK2);
	}

	if (m_bPlayerCharacter == TRUE)
	{
		CPacket cPacket;
		mpAttack2(&cPacket, m_ByDirCur, GetCurX(), GetCurY());
		SendPacket(&cPacket);
	}

}

void CPlayerObject::SetActionAttack3()
{
	m_dwActionOld = m_dwActionCur;
	m_dwActionCur = dfACTION_ATTACK3;

	if (m_ByDirCur == 0)
	{
		SetSprite(26, 31, dfDELAY_ATTACK3);
	}
	else
	{
		SetSprite(32, 37, dfDELAY_ATTACK3);
	}

	if (m_bPlayerCharacter == TRUE)
	{
		CPacket cPacket;
		mpAttack3(&cPacket, m_ByDirCur, GetCurX(), GetCurY());
		SendPacket(&cPacket);
	}
}


void CPlayerObject::mpMoveStart(CPacket* cpPacket, BYTE byDir, short shX, short shY)
{
	stHEADER stHeader;
	stHeader.byCode = dfNETWORK_PACKET_CODE;
	stHeader.bySize = 5;
	stHeader.byType = dfPACKET_CS_MOVE_START;

	cpPacket->PutData((char*)&stHeader, 3);

	*cpPacket << byDir;
	*cpPacket << shX;
	*cpPacket << shY;
}

void CPlayerObject::mpMoveStop(CPacket* cpPacket, BYTE byDir, short shX, short shY)
{
	stHEADER stHeader;
	stHeader.byCode = dfNETWORK_PACKET_CODE;
	stHeader.bySize = 5;
	stHeader.byType = dfPACKET_CS_MOVE_STOP;

	cpPacket->PutData((char*)&stHeader, 3);

	*cpPacket << byDir;
	*cpPacket << shX;
	*cpPacket << shY;
}

void CPlayerObject::mpAttack1(CPacket* cpPacket, BYTE byDir, short shX, short shY)
{
	stHEADER stHeader;
	stHeader.byCode = dfNETWORK_PACKET_CODE;
	stHeader.bySize = 5;
	stHeader.byType = dfPACKET_CS_ATTACK1;

	cpPacket->PutData((char*)&stHeader, 3);

	*cpPacket << byDir;
	*cpPacket << shX;
	*cpPacket << shY;
}

void CPlayerObject::mpAttack2(CPacket* cpPacket, BYTE byDir, short shX, short shY)
{
	stHEADER stHeader;
	stHeader.byCode = dfNETWORK_PACKET_CODE;
	stHeader.bySize = 5;
	stHeader.byType = dfPACKET_CS_ATTACK2;

	cpPacket->PutData((char*)&stHeader, 3);

	*cpPacket << byDir;
	*cpPacket << shX;
	*cpPacket << shY;
}

void CPlayerObject::mpAttack3(CPacket* cpPacket, BYTE byDir, short shX, short shY)
{
	stHEADER stHeader;
	stHeader.byCode = dfNETWORK_PACKET_CODE;
	stHeader.bySize = 5;
	stHeader.byType = dfPACKET_CS_ATTACK3;

	cpPacket->PutData((char*)&stHeader, 3);

	*cpPacket << byDir;
	*cpPacket << shX;
	*cpPacket << shY;
}

void CPlayerObject::Render(CSpriteDib* cSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	// 그림자 출력
	cSprite->DrawSprite50(64, m_iCurX, m_iCurY, bypDest, iDestWidth, iDestHeight, iDestPitch);
	//// 플레이어 객체 출력

	if(m_bPlayerCharacter == TRUE)
		cSprite->DrawSpriteRed(GetSprite(), m_iCurX, m_iCurY, bypDest, iDestWidth, iDestHeight, iDestPitch);
	else
		cSprite->DrawSprite(GetSprite(), m_iCurX, m_iCurY, bypDest, iDestWidth, iDestHeight, iDestPitch);
	//// HP 게이지 바 출력
	cSprite->DrawSprite(65, m_iCurX - 35, m_iCurY + 9, bypDest, iDestWidth, iDestHeight, iDestPitch, GetHP());
}


void CPlayerObject::SetDirection(BYTE dir)
{
	m_ByDirCur = dir;
}

void CPlayerObject::SetHP(char hp)
{
	m_chHP = hp;
}

BYTE CPlayerObject::GetDircection()
{
	return m_ByMoveCur;
}


char CPlayerObject::GetHP()
{
	return m_chHP;
}

BOOL CPlayerObject::IsPlayer()
{
	return m_bPlayerCharacter;
}

//void CPlayerObject::SendPacket(stHEADER* pHeader, char* pPacket)
//{
//	int  sendval = cSendQ->Enqueue((char*)pHeader, 3);
//	sendval = cSendQ->Enqueue(pPacket, pHeader->bySize);
//	WriteEvent();
//}
//
//void CPlayerObject::SendPacket(CPacket* cpPacket)
//{
//	int  sendval = cSendQ->Enqueue(cpPacket);
//	WriteEvent();
//}
//
//void CPlayerObject::WriteEvent()
//{
//	if ((*bSendFlag) == false)
//		return;
//
//	while (1)
//	{
//		if (cSendQ->GetUseSize() == 0)
//			break;
//
//		int sendretval = send((*Socket), cSendQ->GetFrontBufferPtr(), cSendQ->DirectDequeueSize(), 0);
//		if (sendretval == 0 || sendretval == SOCKET_ERROR)
//		{
//			int errorCode = WSAGetLastError();
//
//			(*bSendFlag) = false;
//			return;
//		}
//
//		cSendQ->MoveFront(sendretval);
//	}
//}