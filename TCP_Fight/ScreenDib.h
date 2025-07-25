#pragma once


class CScreenDib
{
public:
	////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	////////////////////////////////////////////////////////////////////
	CScreenDib(int iWidth, int iHeight, int iColorBit);
	virtual ~CScreenDib();
protected:
	void CreateDibBuffer(int iWidth, int iHeight, int iColorBit);
	void ReleaseDibBuffer(void);
public:	
	BYTE* GetDibBuffer(void);
	int GetWidth(void);
	int GetHeight(void);
	int GetPitch(void);
	void DrawBuffer(HWND hWnd, int iX = 0, int iY = 0);

protected:
	BITMAPINFO m_stDibInfo;
	BYTE* m_bypBuffer;
	int m_iWidth;
	int m_iHeight;
	int m_iPitch; // 4바이트 정렬된 한 줄의 바이트 수
	int m_iColorBit;
	int m_iBufferSize;
};
