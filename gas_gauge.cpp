/*==============================================================================

    �K�X�Q�[�W�Ǘ� [gas_gauge.cpp]
	Author :	�^�@
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "gas_gauge.h"
#include "player.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GAGE_SIZE2_Y			(180.0f)
#define GAGE_DECREASE		(GAGE_SIZE2_Y / 2) // �������̂���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_GasGaugeTexture1 = 0;
static int	g_GasGaugeTexture2 = 0;
GAS_GAUGE gas_gauge1;
GAS_GAUGE gas_gauge2;

//=============================================================================
// ����������
//=============================================================================
void InitGasGauge(void)
{
	g_GasGaugeTexture1 = LoadTexture("data/TEXTURE/gas1_0.png");
	g_GasGaugeTexture2 = LoadTexture("data/TEXTURE/gas2_0.png");
	//�Q�[�W1
	gas_gauge1.pos.x	= 860.0f;
	gas_gauge1.pos.y	= SCREEN_HEIGHT - 150.0f;
	gas_gauge1.size.x	= 40.0f;
	gas_gauge1.size.y	= 240.0f;
	gas_gauge1.gauge_u	= 0.0f;
	gas_gauge1.gauge_v	= 0.0f;
	gas_gauge1.gauge_uh = 1.0f;
	gas_gauge1.gauge_vh = 1.0f;

	//�Q�[�W2
	gas_gauge2.pos.x	= gas_gauge1.pos.x;
	gas_gauge2.pos.y	= gas_gauge1.pos.y + 20.0f;
	gas_gauge2.size.x	= 25.0f;
	gas_gauge2.size.y	= GAGE_SIZE2_Y;
	gas_gauge2.gauge_u	= 0.0f; 
	gas_gauge2.gauge_v	= 0.0f;
	gas_gauge2.gauge_uh = 1.0f;
	gas_gauge2.gauge_vh = 1.0f;
}
//=============================================================================
// �I������
//=============================================================================
void UninitGasGauge(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGasGauge(void)
{
	if (GetPlayer()->vertigo_isUse == false)
	{
		if (GetKeyboardPress(DIK_Q) || GetKeyboardPress(DIK_E))
		{
			gas_gauge2.gauge_vh -= 0.01f;
		}
		if (gas_gauge2.gauge_vh <= 0.2f)
		{
			gas_gauge2.gauge_vh = 1.0f;
		}
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGasGauge(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	DrawSpriteColorRotate(g_GasGaugeTexture2,
		gas_gauge2.pos.x, gas_gauge2.pos.y + (GAGE_DECREASE * (1.0f - gas_gauge2.gauge_vh)),
		gas_gauge2.size.x, gas_gauge2.size.y * gas_gauge2.gauge_vh,
		gas_gauge2.gauge_u, gas_gauge2.gauge_v,
		gas_gauge2.gauge_uh, gas_gauge2.gauge_vh,
		col, 0.0f);

	DrawSpriteColorRotate(g_GasGaugeTexture1,
		gas_gauge1.pos.x,	gas_gauge1.pos.y,
		gas_gauge1.size.x,	gas_gauge1.size.y,
		gas_gauge1.gauge_u, gas_gauge1.gauge_v,
		gas_gauge1.gauge_uh,gas_gauge1.gauge_vh,
		col, 0.0f);
	
}