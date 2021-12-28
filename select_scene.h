//=============================================================================
//
// �Z���N�g��ʊǗ� [select_scene.h]
// Author : �^
//
//=============================================================================
#ifndef SELECT_SCENE_H_
#define SELECT_SCENE_H_

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define			STAGE_1		0
#define			STAGE_2		1
#define			STAGE_3		2
#define			STAGE_4		3
#define			STAGE_5		4
#define			STAGE_NONE	99

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSelect_Scene(void);
void UninitSelect_Scene(void);
void UpdateSelect_Scene(void);
void DrawSelect_Scene(void);
void ControlSelect(void);
void StageSelect(void);

#endif // SELECT_SCENE_H_


