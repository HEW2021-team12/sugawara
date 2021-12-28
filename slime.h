//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �X�e�[�W�P
#define SLIME_MAX		(1)

// �X�e�[�W�Q
#define SLIME_MAX		(1)

// �X�e�[�W�R
#define SLIME_MAX		(1)

// �X�e�[�W�S
#define SLIME_MAX		(1)

// �X�e�[�W�T
#define SLIME_MAX		(1)


// �X�e�[�W�ʂ��ČŒ�
#define BABBLE_MAX		(3)

// �X���C���\����
struct SLIME
{
	bool					use;		 // true:�g���Ă���  false:���g�p
	float					w, h;		 // ���ƍ���
	D3DXVECTOR2				pos;		 // �|���S���̍��W
	D3DXVECTOR2				vel;		 // �ړ���
	float					u;
	float					v;
	float					uh;
	float					vh;
	char					routenom;	// ���񃋁[�g�p
	unsigned char			setcount;
};

struct BABBLE
{
	bool					use;		 // true:�g���Ă���  false:���g�p
	float					w, h;		 // ���ƍ���
	D3DXVECTOR2				pos;		 // �|���S���̍��W
	float					u;
	float					v;
	float					uh;
	float					vh;
	int						count;
};

// �X�e�[�W�P
HRESULT InitSlime(void);
void UninitSlime(void);
void UpdateSlime(void);
void DrawSlime(void);
SLIME* GetSlime(void);
BABBLE** GetBabble(void);