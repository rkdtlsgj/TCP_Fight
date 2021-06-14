#include "CBaseObject.h"

class CEffectObject : public CBaseObject
{
public:
	CEffectObject(int iObjectID, int iObjectType, int iX, int iY, int iFrameDelay, int iSpriteStart, int iSpriteEnd);
	~CEffectObject();

	BOOL virtual Run();
	void Draw(CSpriteDib* pSprite, BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch);
};
