#pragma once
#include <windows.h>
#include "PacketDefine.h"
#include "SpriteDib.h"
#include "EnumList.h"

class CBaseObject
{
public:
	CBaseObject();
	~CBaseObject();

	void ActionInput(DWORD dwAction);
	short GetCurX();
	short GetCurY();
	int GetObjectID();
	e_OBJECT_TYPE GetObjectType();
	int GetSprite();
	BOOL IsEndFrame();
	void NextFrame();


	void virtual Render(CSpriteDib* cSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch);
	BOOL virtual Run();

	void SetPosition(short iX, short iY);
	void SetSprite(int iSpriteStart, int iSpriteMax, int iFrameDelay);

protected:
	BOOL m_bEndFrame;
	DWORD m_dwActionInput;
	short m_iOldX;
	short m_iOldY;
	short m_iCurX;
	short m_iCurY;
	int m_iDelayCount;
	int m_iFrameDelay;
	int m_iObjectID;
	e_OBJECT_TYPE m_iObjectType;
	int m_iSpriteEnd;
	int m_iSpriteNow;
	int m_iSpriteStart;
	int m_iSpriteMax;
};