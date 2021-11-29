/*==============================================================================

	�K�X�Q�[�W�Ǘ� [gas_gauge.h]
	Author :	�^�@
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef GAS_GAUGE_H_
#define GAS_GAUGE_H_

#include "main.h"
#include "renderer.h"

#endif//GAS_GAUGE_H_

//�K�X�Q�[�W�\����
typedef struct
{
	D3DXVECTOR2 pos;
	D3DXVECTOR2 size;
	float gauge_u;
	float gauge_v;
	float gauge_uh;
	float gauge_vh;
}GAS_GAUGE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitGasGauge(void);
void UninitGasGauge(void);
void UpdateGasGauge(void);
void DrawGasGauge(void);