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
#define ENEMY_MAX		(3)			// �G�l�~�[��Max�l��

// �G�l�~�[�\����
struct ENEMY
{
	bool					use;		 // true:�g���Ă���  false:���g�p
	bool					watch;		 // true:������    false:�����ĂȂ�
	float					w, h;		 // ���ƍ���
	D3DXVECTOR2				pos;		 // �|���S���̍��W
	D3DXVECTOR2				vel;		 // �ړ���
	int						timer;		 // ���������̓���^�C�}�[
	int						move;		 // ���������̓��쐧��
	D3DXVECTOR2				notmove;	 // �����Ă��Ȃ����p
	D3DXVECTOR2				addmove;	 // �����Ă��Ȃ����̒ǉ��ړ�
	bool					movecntX, movecntY;
	float					u;
	float					v;
	float					uh;
	float					vh;
};


// AI�̈ړ��̏��
enum MOVE_STATE
{
	MOVE_NONE = 0 , // �������Ȃ�
	MOVE_UP,		// �������
	MOVE_DOWN,		// ��������
	MOVE_RIGHT,		// �E������
	MOVE_LEFT,		// ��������
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

ENEMY *GetEnemy(void);