/*==============================================================================

   ���_�Ǘ�[player.h]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#pragma once


#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_SIZE			(50.0f)
#define PLAYER_ACCELE		(0.1f)
#define	PLAYER_MIN_SPD		(0.0f)
#define PLAYER_MAX_SPD		(3.0f)

#define GRAVITY_VEL			(0.5f)


// �v���C���[�\����
struct PLAYER
{
	D3DXVECTOR2 pos;
	D3DXVECTOR2 size;
	D3DXVECTOR2 vel;
	float					u;
	float					v;
	float					uh;
	float					vh;

	// �}�b�v�p
	D3DXVECTOR2 difference;

	// �y�i���e�B�[�p
	bool penalty;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void ControlPlayer(void);
void FramePlayer(void);
void ZeroGravity(void);

PLAYER* GetPlayer(void);
D3DXVECTOR2 GetPlayerPosition(void);