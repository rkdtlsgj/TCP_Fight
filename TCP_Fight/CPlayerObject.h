#pragma once
#include <winsock2.h>
#include "CBaseObject.h"
#include "CRingBuffer.h"
#include "CPacket.h"

class CPlayerObject : public CBaseObject
{

public:
	CPlayerObject(int id, BYTE dir, char hp, BOOL isPlayer);
	~CPlayerObject();

	void ActionProc();
	BYTE GetDircection();
	char GetHP();
	void InputActionProc();
	BOOL IsPlayer();

	void virtual Render(CSpriteDib* cSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch);
	BOOL virtual Run();

	void SetDirection(BYTE dir);
	void SetHP(char hp);


private:

	void SetActionAttack1();
	void SetActionAttack2();
	void SetActionAttack3();
	void SetActionMove(DWORD action);
	void SetActionStand();

	BOOL m_bPlayerCharacter;
	char m_chHP;
	DWORD m_dwActionCur;
	DWORD m_dwActionOld;

	BYTE m_ByDirCur;
	BYTE m_ByMoveCur;

	void mpMoveStart(CPacket* cpPacket, BYTE byDir, short shX, short shY);
	void mpMoveStop(CPacket* cpPacket, BYTE byDir, short shX, short shY);
	void mpAttack1(CPacket* cpPacket, BYTE byDir, short shX, short shY);
	void mpAttack2(CPacket* cpPacket, BYTE byDir, short shX, short shY);
	void mpAttack3(CPacket* cpPacket, BYTE byDir, short shX, short shY);

	

};