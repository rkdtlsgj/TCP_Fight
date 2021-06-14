#include "CEffectObject.h"

CEffectObject::CEffectObject(int iObjectID, int iObjectType, int iX, int iY,int iFrameDelay, int iSpriteStart, int iSpriteEnd)
{
	m_iObjectID = iObjectID;
	m_iObjectType = e_OBJECT_TYPE::eTYPE_EFFECT;
	SetPosition(iX, iY);
	SetSprite(iSpriteStart, iSpriteEnd, iFrameDelay);
}

CEffectObject::~CEffectObject()
{

}

BOOL CEffectObject::Run()
{	
	NextFrame();

	if (IsEndFrame())
		return TRUE;

	return FALSE;
}

void CEffectObject::Draw(CSpriteDib* pSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch)
{
	int iDrawX = GetCurX();
	int	iDrawY = GetCurY();

	pSprite->DrawSprite(GetSprite(), iDrawX, iDrawY, bypDest, iDestWidth, iDestHeight, iDestPitch);
}