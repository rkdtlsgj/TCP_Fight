#pragma once

class CSpriteDib
{
public:
	typedef struct st_SPRITE
	{
		BYTE* bypImage; // ��������Ʈ �̹��� ������.
		int iWidth; // Widht
		int iHeight; // Height
		int iPitch; // Pitch
		int iCenterPointX; // ���� X
		int iCenterPointY; // ���� Y
	};
	CSpriteDib(int iMaxSprite, DWORD dwColorKey);
	virtual ~CSpriteDib();
	BOOL LoadDibSprite(int iSpriteIndex, const WCHAR* szFileName, int iCenterPointX, int iCenterPointY);
	void ReleaseSprite(int iSpriteIndex);
	void DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY,
		BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);

	void DrawSprite50(int iSpriteIndex, int iDrawX, int iDrawY, BYTE* bypDest,
		int iDestWidth, int iDestHeight, int iDestPitch);

	void DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY,
		BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch);

	void DrawImage(int iSpriteIndex, int iDrawX, int iDrawY,
		BYTE* bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDrawLen = 100);

protected:
	int m_iMaxSprite;
	st_SPRITE* m_stpSprite;
	DWORD m_dwColorKey;
};