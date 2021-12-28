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
#define ENEMY_MAX		(3)			// �G�l�~�[��Max�l�� �X�e�[�W�P
#define ENEMY_MAX2		(5)			// �G�l�~�[��Max�l�� �X�e�[�W�Q
#define ENEMY_MAX3		(5)			// �G�l�~�[��Max�l�� �X�e�[�W�R
#define ENEMY_MAX4		(5)			// �G�l�~�[��Max�l�� �X�e�[�W�S
#define ENEMY_MAX5		(5)			// �G�l�~�[��Max�l�� �X�e�[�W�T

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
	char					routenom;	// ���񃋁[�g�p
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
// �X�e�[�W�P
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(void);

// �X�e�[�W�Q
HRESULT InitEnemy2(void);
void UninitEnemy2(void);
void UpdateEnemy2(void);
void DrawEnemy2(void);
ENEMY* GetEnemy2(void);