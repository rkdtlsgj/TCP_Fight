
#include "CBaseObject.h"

CBaseObject::CBaseObject()
{

}

CBaseObject::~CBaseObject()
{

}

void CBaseObject::ActionInput(DWORD dwAction)
{
	m_dwActionInput = dwAction;
}

void CBaseObject::Render(CSpriteDib* cSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	cSprite->DrawSprite(GetSprite(), m_iCurX, m_iCurY, bypDest, iDestWidth, iDestHeight, iDestPitch);
}

BOOL CBaseObject::Run()
{
	NextFrame();
	return FALSE;
}

void CBaseObject::NextFrame()
{
	if (0 > m_iSpriteStart)
		return;
	m_iDelayCount++;
	if (m_iDelayCount >= m_iFrameDelay)
	{
		m_iDelayCount = 0;
		m_iSpriteNow++;
		if (m_iSpriteNow > m_iSpriteMax)
		{
			m_iSpriteNow = m_iSpriteStart;
			m_bEndFrame = TRUE;
		}
	}
}


void CBaseObject::SetSprite(int iSpriteStart, int iSpriteMax, int iFrameDelay)
{
	m_iSpriteStart = iSpriteStart;
	m_iSpriteMax = iSpriteMax;
	m_iFrameDelay = iFrameDelay;
	m_iSpriteNow = iSpriteStart;
	m_iDelayCount = 0;
	m_bEndFrame = FALSE;
}

int CBaseObject::GetSprite()
{
	return m_iSpriteNow;
}

short CBaseObject::GetCurX()
{
	return m_iCurX;
}

short CBaseObject::GetCurY()
{
	return m_iCurY;
}

int CBaseObject::GetObjectID()
{
	return m_iObjectID;
}

e_OBJECT_TYPE CBaseObject::GetObjectType()
{
	return m_iObjectType;
}

BOOL CBaseObject::IsEndFrame()
{
	return m_bEndFrame;
}

void CBaseObject::SetPosition(short iX, short iY)
{
	m_iOldX = m_iCurX;
	m_iOldY = m_iCurY;

	m_iCurX = iX;
	m_iCurY = iY;
}