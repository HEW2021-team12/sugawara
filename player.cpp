/*==============================================================================

   ���_�Ǘ� [player.cpp]
	Author :	�^�@���� �Îs
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "input.h"
#include "keyboard.h"
#include "texture.h"
#include "sprite.h"
#include "enemy.h"
#include "map.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//�v���C���[�̏����ʒu
// �X�e�[�W�P
#define	PLAYER_POS_X		(13)
#define	PLAYER_POS_Y		(16)

// �X�e�[�W�Q
#define	PLAYER_POS_X_2		(7)
#define	PLAYER_POS_Y_2		(4)

//// �X�e�[�W�R
//#define	PLAYER_POS_X_3		(7)
//#define	PLAYER_POS_Y_3		(4)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_PlayerTexture = 0;
PLAYER		g_Player;
float		gravity_timer;

float		g_DrawGrav = 0;

// �v���C���[�𓮓I�ɒu��������͕̂ۗ�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	g_PlayerTexture = LoadTexture("data/TEXTURE/player.png");

	// �X�e�[�W�P
	if (GetScene() == SCENE_GAME1)
	{
		g_Player.pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_X;
		g_Player.pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_Y;
	}
	// �X�e�[�W�Q
	if (GetScene() == SCENE_GAME2)
	{
		g_Player.pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_X_2;
		g_Player.pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_Y_2;
	}
	//// �X�e�[�W�R
	//if (GetScene() == SCENE_GAME3)
	//{
	//	g_Player.pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_X_3;
	//	g_Player.pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_Y_3;
	//}

	g_Player.size.x = PLAYER_SIZE;
	g_Player.size.y = PLAYER_SIZE;
	g_Player.vel.x = 0.0f;
	g_Player.vel.y = 0.0f;

	g_Player.u = 0.0f;
	g_Player.v = 0.0f;
	g_Player.uh = 0.0f;
	g_Player.vh = 0.0f;

	gravity_timer = 0.0f;
	g_Player.difference.x = 0.0f;
	g_Player.difference.y = 0.0f;

	g_Player.penalty = false;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	ZeroGravity();
	ControlPlayer();
	//FramePlayer();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	DrawSpriteColor(g_PlayerTexture,
		SCREEN_WIDTH / 2 + g_Player.difference.x,
		SCREEN_HEIGHT / 2 + g_Player.difference.y + g_DrawGrav,
		g_Player.size.x, g_Player.size.y,
		g_Player.u, g_Player.v,
		g_Player.uh, g_Player.vh,
		col);
}

void ControlPlayer(void)
{
	if (g_Player.penalty)
	{
		g_Player.pos.x += g_Player.vel.x / 2;
		g_Player.pos.y += g_Player.vel.y / 2;

		g_Player.penalty = false;
	}
	else
	{
		g_Player.pos.x += g_Player.vel.x;
		g_Player.pos.y += g_Player.vel.y;
	}

	// ���̈ړ��ʂ̕����傫���Ƃ�
	if (fabsf(g_Player.vel.x) > fabsf(g_Player.vel.y))
	{
		// ���ɍs���Ƃ�
		if (g_Player.vel.x < 0)
		{
			g_Player.u = 0.0f;
			g_Player.v = 0.5f;
			g_Player.uh = 0.5f;
			g_Player.vh = 0.5f;
		}
		else // �E�ɍs���Ƃ�
		{
			g_Player.u = 0.5f;
			g_Player.v = 0.5f;
			g_Player.uh = 0.5f;
			g_Player.vh = 0.5f;
		}
	}
	else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
	{
		// ��ɍs���Ƃ�
		if (g_Player.vel.y < 0)
		{
			g_Player.u = 0.5f;
			g_Player.v = 0.0f;
			g_Player.uh = 0.5f;
			g_Player.vh = 0.5f;
		}
		else // ���ɍs���Ƃ�
		{
			g_Player.u = 0.0f;
			g_Player.v = 0.0f;
			g_Player.uh = 0.5f;
			g_Player.vh = 0.5f;
		}
	}

	// �}�b�v�P�̓����蔻��
	if (GetScene() == SCENE_GAME1)
	{
		// X���̓����蔻��
		if (GetMapEnter(D3DXVECTOR2(g_Player.pos.x + g_Player.vel.x, g_Player.pos.y))
			== 1)
		{
			g_Player.pos.x -= g_Player.vel.x;
		}
		// Y���̓����蔻��
		if (GetMapEnter(D3DXVECTOR2(g_Player.pos.x, g_Player.pos.y + g_Player.vel.y))
			== 1)
		{
			g_Player.pos.y -= g_Player.vel.y;
		}
	}

	// �}�b�v�Q�̓����蔻��
	if (GetScene() == SCENE_GAME2)
	{
		// X���̓����蔻��
		if (GetMapEnter2(D3DXVECTOR2(g_Player.pos.x + g_Player.vel.x, g_Player.pos.y))
			== 1)
		{
			g_Player.pos.x -= g_Player.vel.x;
		}
		// Y���̓����蔻��
		if (GetMapEnter2(D3DXVECTOR2(g_Player.pos.x, g_Player.pos.y + g_Player.vel.y))
			== 1)
		{
			g_Player.pos.y -= g_Player.vel.y;
		}
	}

	//��(�ړ�)
	if (Keyboard_IsKeyDown(KK_LEFT) // �L�[�{�[�h
		|| GetThumbLeftX(0) < -INPUT_ZONE) // �p�b�h
	{
		g_Player.vel.x -= PLAYER_ACCELE;

		// X���x�̏���l
		if (g_Player.vel.x < -PLAYER_MAX_SPD)
		{
			g_Player.vel.x = -PLAYER_MAX_SPD;
		}

	}

	//�E(�ړ�)
	if (Keyboard_IsKeyDown(KK_RIGHT) // �L�[�{�[�h
		|| GetThumbLeftX(0) > INPUT_ZONE) // �p�b�h
	{
		g_Player.vel.x += PLAYER_ACCELE;

		// X���x�̏���l
		if (g_Player.vel.x > PLAYER_MAX_SPD)
		{
			g_Player.vel.x = PLAYER_MAX_SPD;
		}
	}

	//��(�ړ�)
	if (Keyboard_IsKeyDown(KK_UP) // �L�[�{�[�h
		|| GetThumbLeftY(0) > INPUT_ZONE) // �p�b�h
	{
		g_Player.vel.y -= PLAYER_ACCELE;

		// y���x�̏���l
		if (g_Player.vel.y < -PLAYER_MAX_SPD)
		{
			g_Player.vel.y= -PLAYER_MAX_SPD;
		}
	}
	
	//��(�ړ�)
	if (Keyboard_IsKeyDown(KK_DOWN) // �L�[�{�[�h
		|| GetThumbLeftY(0) < -INPUT_ZONE) // �p�b�h
	{
		g_Player.vel.y += PLAYER_ACCELE;

		// y���x�̏���l
		if (g_Player.vel.y > PLAYER_MAX_SPD)
		{
			g_Player.vel.y = PLAYER_MAX_SPD;
		}
	}
}

void ZeroGravity(void)
{
	if (gravity_timer < 20.0f)
	{
		g_DrawGrav -= GRAVITY_VEL;
		gravity_timer++;
	}
	if (gravity_timer >= 20.0f && gravity_timer < 40.0f)
	{
		g_DrawGrav += GRAVITY_VEL;
		gravity_timer++;
	}
	if (gravity_timer >= 40.0f && gravity_timer < 60.0f)
	{
		g_DrawGrav += GRAVITY_VEL;
		gravity_timer++;
	}
	if (gravity_timer >= 60.0f && gravity_timer < 80.0f)
	{
		g_DrawGrav -= GRAVITY_VEL;
		gravity_timer++;
	}

	if (gravity_timer >= 80.0f)
	{
		gravity_timer = 0.0f;
	}
}

void FramePlayer(void)
{
	//��(�g)
	if (g_Player.pos.x <= 0.0f + (g_Player.size.x / 2))
	{
		g_Player.pos.x = 0.0f + (g_Player.size.x / 2);
	}
	//�E(�g)
	if (g_Player.pos.x >= (MAP_X * 60.0f) - (g_Player.size.x / 2))
	{
		g_Player.pos.x = (MAP_X * 60.0f) - (g_Player.size.x / 2);
	}

	//��(�g)
	if (g_Player.pos.y <= 0.0f + (g_Player.size.y / 2))
	{
		g_Player.pos.y = 0.0f + (g_Player.size.y / 2);
	}
	//��(�g)
	if (g_Player.pos.y >= (MAP_Y * 60.0f) - (g_Player.size.y / 2))
	{
		g_Player.pos.y = (MAP_Y * 60.0f) - (g_Player.size.y / 2);
	}
}

PLAYER* GetPlayer(void)
{
	return &g_Player;
}

D3DXVECTOR2 GetPlayerPosition(void)
{
	return g_Player.pos;
}