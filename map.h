/*==============================================================================

   �}�b�v�Ǘ�[map.h]
														 Author :	�^ ���� �Îs
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef MAP_H_
#define MAP_H_

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//���͐�΂ɂ�����Ȃ����ƁI
#define VIEW_SCREEN_WIDTH	(16.0f)		// �\���͈�(��)
#define VIEW_SCREEN_HEIGHT	(9.0f)		// �\���͈�(�c)

#define CHIP_SIZE			(60.0f)		// �}�X�̑傫��

// ���Ń}�b�v
#define MAP_X				(27)		// �}�b�v�̉��̃}�X��
#define MAP_Y				(18)		// �}�b�v�̏c�̃}�X��

// ����ȍ~�̃}�b�v
#define MAP2_X				(25)		// 2�}�b�v��
#define MAP2_Y				(22)		// 2�}�b�v��

#define MAP3_X				()			// 3�}�b�v��
#define MAP3_Y				()			// 3�}�b�v��

#define MAP4_X				()			// 4�}�b�v��
#define MAP4_Y				()			// 4�}�b�v��

#define MAP5_X				()			// 5�}�b�v��
#define MAP5_Y				()			// 5�}�b�v��

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
void InitMap	(void);
void UninitMap	(void);
void UpdateMap	(void);
void DrawMap	(void);
void ChangeUv   (void);

int GetMapEnter(D3DXVECTOR2 pos);

void InitMap2(void);
void UninitMap2(void);
void UpdateMap2(void);
void DrawMap2(void);
void ChangeUv2(void);

int GetMapEnter2(D3DXVECTOR2 pos);


//int GetMap(int x, int y);

#endif //MAP_H_
