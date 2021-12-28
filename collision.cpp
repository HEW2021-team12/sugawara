//=============================================================================
//
// �����蔻�菈�� [collision.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "collision.h"
#include "player.h"
#include "enemy.h"
#include "fade.h"
#include "scene.h"
#include "slime.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �\���̒�`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool CollisionBB(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2);
bool CollisionBC(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, float size1, float size2);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ENEMY* enemy;
SLIME* slime;
BABBLE** babble;

//=============================================================================
// �����蔻�菈��
//=============================================================================
void UpdateCollision(void)
{
	PLAYER *player = GetPlayer();	// �v���C���[�̃|�C���^�[��������
	int enemy_max = 0;
	int slime_max = 0;
	int babble_max = 0;
	
	// �X�e�[�W�P
	if (GetScene() == SCENE_GAME1)
	{
		enemy = GetEnemy();		// �G�l�~�[�̃|�C���^�[��������
		enemy_max = ENEMY_MAX;

		slime = GetSlime();
		slime_max = SLIME_MAX;

		babble = GetBabble();
		babble_max = BABBLE_MAX;
	}
	// �X�e�[�W�Q
	if (GetScene() == SCENE_GAME2)
	{
		enemy = GetEnemy2();	// �G�l�~�[�̃|�C���^�[��������
		enemy_max = ENEMY_MAX2;
	}
	
	// �G�Ƒ���L����(BB)
	for (int i = 0; i < enemy_max; i++)
	{
		if (enemy[i].use == false)
			continue;

//		if (CollisionBB(player->pos, enemy[i].pos, player->size, D3DXVECTOR2(enemy[i].w, enemy[i].h)))
		if (CollisionBC(player->pos, enemy[i].pos, player->size.y/2, enemy[i].h/2))
		{
			// ����L�����N�^�[�͎���
			SceneTransition(SCENE_LOSE);
		}
	}

	// �v���C���[�ƃX���C��
	for (int i = 0; i < slime_max; i++)
	{
		if (slime[i].use)
		{
			if (CollisionBB(player->pos, slime[i].pos, player->size, D3DXVECTOR2(slime[i].w, slime[i].h)))
			{
				player->penalty = true;
			}
		}

		// �v���C���[�ƃo�u��
		for (int j = 0; j < babble_max; j++)
		{
			if (babble[i][j].use)
			{
				if (CollisionBB(player->pos, babble[i][j].pos, player->size, D3DXVECTOR2(babble[i][j].w, babble[i][j].h)))
				{
					player->penalty = true;
				}
			}
		}
	}
}

//=============================================================================
// BB�ɂ�铖���蔻�菈��
// ��]�͍l�����Ȃ�
// �߂�l�F�������Ă���true
//=============================================================================
bool CollisionBB(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, D3DXVECTOR2 size1, D3DXVECTOR2 size2)
{
	D3DXVECTOR2 min1, max1;
	D3DXVECTOR2 min2, max2;

	min1.x = pos1.x - size1.x / 2;
	min1.y = pos1.y - size1.y / 2;
	max1.x = pos1.x + size1.x / 2;
	max1.y = pos1.y + size1.y / 2;

	min2.x = pos2.x - size2.x / 2;
	min2.y = pos2.y - size2.y / 2;
	max2.x = pos2.x + size2.x / 2;
	max2.y = pos2.y + size2.y / 2;

	//X���̔���
	if (min1.x < max2.x &&
		max1.x > min2.x)
	{
		//Y���̔���
		if (min1.y < max2.y &&
			max1.y > min2.y)
		{
			//�S�Ă̏�����TRUE�Ȃ�q�b�g����
			return true;
		}
	}

	return false;
}

//=============================================================================
// BC�ɂ�铖���蔻�菈��
// �T�C�Y�͔��a
// �߂�l�F�������Ă���true
//=============================================================================
bool CollisionBC(D3DXVECTOR2 pos1, D3DXVECTOR2 pos2, float size1, float size2)
{
	//pos1��pos2���Ȃ��x�N�g�������
	D3DXVECTOR2 vDistance = pos1 - pos2;

	//vDistance�̃x�N�g�����𓾂�
	float length;
//	length = D3DXVec2Length(&vDistance);
	length = D3DXVec2LengthSq(&vDistance);

//	float size = size1 + size2;
	float size = (size1 + size2) * (size1 + size2);

	//1��2�̔��a�𑫂��Ĕ��肷��
	//�����̃x�N�g�����̕������������
	//�q�b�g���Ă���
	if (length < size)
	{
		return true;
	}

	return false;
}