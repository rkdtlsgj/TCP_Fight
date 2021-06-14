#pragma once
#include <Windows.h>

#define SERVER_PORT 5000

#define WM_SOCKET WM_USER+1

#define dfNETWORK_PACKET_CODE 0x89

#define dfACTION_MOVE_LL	0
#define dfACTION_MOVE_LU	1
#define dfACTION_MOVE_UU	2
#define dfACTION_MOVE_RU	3
#define dfACTION_MOVE_RR	4
#define dfACTION_MOVE_RD	5
#define dfACTION_MOVE_DD	6
#define dfACTION_MOVE_LD	7

#define dfACTION_ATTACK1	8
#define dfACTION_ATTACK2	9
#define dfACTION_ATTACK3	10

#define dfACTION_STAND		11

#define dfDELAY_STAND		5
#define dfDELAY_MOVE		4
#define dfDELAY_ATTACK1	3
#define dfDELAY_ATTACK2	4
#define dfDELAY_ATTACK3	4
#define dfDELAY_EFFECT		3

#define dfSCREEN_WIDTH		640
#define dfSCREEN_HEIGHT	480

#define dfSPEED_PLAYER_X	3
#define dfSPEED_PLAYER_Y	2

#define dfRANGE_MOVE_TOP	50
#define dfRANGE_MOVE_LEFT	10
#define dfRANGE_MOVE_RIGHT	6390
#define dfRANGE_MOVE_BOTTOM	6390

enum e_OBJECT_TYPE
{
	eTYPE_PLAYER,
	eTYPE_EFFECT
};

struct  stHEADER
{
	BYTE	byCode;			// ��Ŷ�ڵ� 0x89 ����.
	BYTE	bySize;			// ��Ŷ ������.
	BYTE	byType;			// ��ŶŸ��.
};
