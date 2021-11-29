/*==============================================================================

   �}�b�v�Ǘ�[map.h]
														 Author :	�^ ���� �Îs
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef MAP2_H_
#define MAP2_H_

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//���͐�΂ɂ�����Ȃ����ƁI
#define VIEW_SCREEN_WIDTH	(16.0f)		// �\���͈�(��)
#define VIEW_SCREEN_HEIGHT	(9.0f)		// �\���͈�(�c)

#define CHIP_SIZE			(60.0f)		// �}�X�̑傫��

// sugawara map
#define MAP_X				(28)		// �}�b�v�̉��̃}�X��
#define MAP_Y				(19)		// �}�b�v�̏c�̃}�X��

// ����ȍ~�̃}�b�v


//�}�b�v�\����
typedef struct
{
	float		u = 0.0f;
	float		v = 0.0f;
	float		uh = 0.0f;
	float		vh = 0.0f;
}MAP;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitMap2	(void);
void UninitMap2	(void);
void UpdateMap2	(void);
void DrawMap2	(void);
void ChangeUv2   (void);

int GetMapEnter(D3DXVECTOR2 pos, D3DXVECTOR2 size);
int GetMap2(int x, int y);
#endif //MAP2_H_
