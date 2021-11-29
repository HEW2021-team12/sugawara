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


typedef struct
{
	D3DXVECTOR2 pos[7];
	D3DXVECTOR2 size;
	D3DXVECTOR2 yajirushi_pos;
	D3DXVECTOR2 yajirushi_size;
}SELECT_SCENE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSelect_Scene(void);
void UninitSelect_Scene(void);
void UpdateSelect_Scene(void);
void DrawSelect_Scene(void);
void ControlSelect(void);

#endif // SELECT_SCENE_H_


