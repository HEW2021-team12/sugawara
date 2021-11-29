/*==============================================================================

   ���_�Ǘ� [player.cpp]
	Author :	�^�@���� �Îs
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "input.h"
#include "texture.h"
#include "sprite.h"
#include "enemy.h"
#include "map.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_PlayerTexture = 0;
PLAYER		g_Player;
float		control_timer;
float		gravity_timer;
float		vertigo_timer;
bool		button_bool;
float		button_timer;

float		g_DrawGrav = 0;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	g_PlayerTexture = LoadTexture("data/TEXTURE/player.png");

	g_Player.pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 11;
	g_Player.pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
	g_Player.size.x = PLAYER_SIZE;
	g_Player.size.y = PLAYER_SIZE;
	g_Player.vel.x = 0.0f;
	g_Player.vel.y = 0.0f;

	g_Player.u = 0.0f;
	g_Player.v = 0.0f;
	g_Player.uh = 0.0f;
	g_Player.vh = 0.0f;

	g_Player.rot = 0.0f;
	g_Player.rot_vel = 0.1f;
	control_timer = 0.0f;
	gravity_timer = 0.0f;
	g_Player.vertigo_isUse = false;
	vertigo_timer = 0.0f;
	g_Player.difference.x = 0.0f;
	g_Player.difference.y = 0.0f;
	button_timer = 0.0f;
	button_bool = true;

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
	if (g_Player.rot <= -2.0f || g_Player.rot >= 3.0f)
	{
		g_Player.vertigo_isUse = true;
	}

	if (g_Player.vertigo_isUse == false)
	{
		ZeroGravity();
		ControlPlayer();
		FramePlayer();
	}

	if (g_Player.vertigo_isUse == true)
	{
		vertigo_timer ++;
	}

	if (vertigo_timer >= 100.0f)
	{
		g_Player.vertigo_isUse = false;
		g_Player.rot = 0.0f;
		vertigo_timer = 0.0f;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	DrawSpriteColorRotate(g_PlayerTexture,
		SCREEN_WIDTH / 2 + g_Player.difference.x,
		SCREEN_HEIGHT / 2 + g_Player.difference.y + g_DrawGrav,
		g_Player.size.x, g_Player.size.y,
		g_Player.u, g_Player.v,
		g_Player.uh, g_Player.vh,
		col, g_Player.rot);
}

void ControlPlayer(void)
{
	if (!(GetKeyboardPress(DIK_UP)
		&& !IsButtonPressed(0, BUTTON_UP)) &&
		!(GetKeyboardPress(DIK_DOWN)
			&& !IsButtonPressed(0, BUTTON_DOWN)) &&
		!(GetKeyboardPress(DIK_LEFT)
			&& !IsButtonPressed(0, BUTTON_LEFT)) &&
		!(GetKeyboardPress(DIK_RIGHT)
			&& !IsButtonPressed(0, BUTTON_RIGHT)))
	{
		button_bool = true;
	}

	if (button_bool == true)
	{
		button_timer += 1.0f;
	}

	if (button_bool == false)
	{
		button_timer = 0.0f;
	}

	g_Player.pos.x += g_Player.vel.x;
	g_Player.pos.y += g_Player.vel.y;

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

	//��(�ړ�)
	if (GetKeyboardPress(DIK_LEFT)
		|| IsButtonPressed(0, BUTTON_LEFT))
	{
		button_bool = false;
		g_Player.vel.x -= PLAYER_ACCELE;

		// X���x�̏���l
		if (g_Player.vel.x < -PLAYER_MAX_SPD)
		{
			g_Player.vel.x = -PLAYER_MAX_SPD;
		}

		control_timer++;
		if (control_timer >= 20.0f)
		{
			g_Player.rot -= g_Player.rot_vel;
			control_timer = 0.0f;
		}
	}

	//�E(�ړ�)
	if (GetKeyboardPress(DIK_RIGHT)
		|| IsButtonPressed(0, BUTTON_RIGHT))
	{
		button_bool = false;
		g_Player.vel.x += PLAYER_ACCELE;

		// X���x�̏���l
		if (g_Player.vel.x > PLAYER_MAX_SPD)
		{
			g_Player.vel.x = PLAYER_MAX_SPD;
		}
		control_timer++;
		if (control_timer >= 20.0f)
		{
			g_Player.rot += g_Player.rot_vel;
			control_timer = 0.0f;
		}
	}

	//��(�ړ�)
	if (GetKeyboardPress(DIK_UP)
		|| IsButtonPressed(0, BUTTON_UP))
	{
		button_bool = false;
		g_Player.vel.y -= PLAYER_ACCELE;

		// y���x�̏���l
		if (g_Player.vel.y < -PLAYER_MAX_SPD)
		{
			g_Player.vel.y= -PLAYER_MAX_SPD;
		}

	}
	
	//��(�ړ�)
	if (GetKeyboardPress(DIK_DOWN)
		|| IsButtonPressed(0, BUTTON_DOWN))
	{
		button_bool = false;
		g_Player.vel.y += PLAYER_ACCELE;

		// y���x�̏���l
		if (g_Player.vel.y > PLAYER_MAX_SPD)
		{
			g_Player.vel.y = PLAYER_MAX_SPD;
		}
		
	}

	//����]
	if (GetKeyboardPress(DIK_Q)
		|| IsButtonPressed(0, BUTTON_L2))
	{
		g_Player.rot -= 0.01f;
	}

	//�E��]
	if (GetKeyboardPress(DIK_E)
		|| IsButtonPressed(0, BUTTON_R2))
	{
		g_Player.rot += 0.01f;
	}

	if (button_timer >= SLIDE_TIME)
	{
		g_Player.vel.x = PLAYER_MIN_SPD;
		g_Player.vel.y = PLAYER_MIN_SPD;
		button_timer = 0.0f;
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