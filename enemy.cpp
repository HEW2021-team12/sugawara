//=============================================================================
//
// �G�l�~�[���� [enemy.cpp]
// Author : 
//
//=============================================================================
#include "enemy.h"
#include "texture.h"
#include "sprite.h"
#include "main.h"
#include "player.h"
#include "map.h"
#include <time.h>
#include "sound.h"
#include "timer.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ROT_SPD				(0.05f)
#define ENEMY_SPD			(2.0f)
#define ENEMY_SIZE			(55.0f)
#define MOVE_TIMER			(50)
#define SERCH_RANGE			(250.0f)
#define FIND_SPD			(2.0f)

#define SEARCH_TIMER		(CHIP_SIZE / ENEMY_SPD)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void SetEnemy(void);
void WatchEnemy(int i);
void NoneWatchEnemy(int i);
bool SerchPlayer(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static int	g_EnemyTexture = 0;
static int g_watch_SE = 0;
static ENEMY g_Enemy[ENEMY_MAX];				  // �G�l�~�[�\����
bool g_SetEnemy;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	g_EnemyTexture = LoadTexture("data/TEXTURE/enemy.png");
	g_watch_SE = LoadSound("data/SE/watch1.wav");
	g_SetEnemy = false;

	// �G�l�~�[�\���̂̏�����
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// ��̖�
		if (i == 0)
		{
			g_Enemy[i].use = true;
			g_Enemy[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 6;
			g_Enemy[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 2;
		}
		// ��̖�
		if (i == 1)
		{
			g_Enemy[i].use = false;
			g_Enemy[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 3;
			g_Enemy[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 8;
		}
		// �O�̖�
		if (i == 2)
		{
			g_Enemy[i].use = false;
			g_Enemy[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 23;
			g_Enemy[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 7;
		}

		// ����
		g_Enemy[i].watch = false;
		g_Enemy[i].w = ENEMY_SIZE;
		g_Enemy[i].h = ENEMY_SIZE;
		g_Enemy[i].vel = D3DXVECTOR2(ENEMY_SPD, ENEMY_SPD);
		g_Enemy[i].timer = 0;
		g_Enemy[i].move = 0;
		g_Enemy[i].notmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy[i].addmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy[i].movecntX = false;
		g_Enemy[i].movecntY = false;

		// �G�l�~�[UV
		g_Enemy[i].u = 0.0f;
		g_Enemy[i].v = 0.0f;
		g_Enemy[i].uh = 0.5f;
		g_Enemy[i].vh = 0.5f;
	}
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == true)
		{
			// �ǐՔ���
			if (SerchPlayer(GetPlayerPosition(), g_Enemy[i].pos))
			{// ������(�͈͓��ɓ�����)

				if (!g_Enemy[i].watch)
				{
					PlaySound(g_watch_SE, 0);
					g_Enemy[i].watch = true;
				}

				WatchEnemy(i);
				
			}
			// �����Ă��Ȃ�
			else
			{
				if (g_Enemy[i].watch)
				{
					// �����������̏����iSE�j

				}

				g_Enemy[i].watch = false;

				NoneWatchEnemy(i);

			}
		}
	}
	// �G�l�~�[�ǉ�
	if (GetTimer() == 20)
	{
		g_Enemy[1].use = true;
	}
	if (GetTimer() == 10)
	{
		g_Enemy[2].use = true;
	}
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == true)
		{
			PLAYER* player = GetPlayer();

			// ��ʓ��ɓ����Ă��邩
			if (fabsf(g_Enemy[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Enemy[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Enemy[i].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Enemy[i].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
				float pw = g_Enemy[i].w;		// �G�l�~�[�̕\����
				float ph = g_Enemy[i].h;		// �G�l�~�[�̕\������
				D3DXCOLOR col;

				if (SerchPlayer(GetPlayerPosition(), g_Enemy[i].pos))
				{
					col = D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				DrawSpriteColor(g_EnemyTexture, px, py, pw, ph,
					g_Enemy[i].u, g_Enemy[i].v,
					g_Enemy[i].uh, g_Enemy[i].vh,
					col);
			}
		}
	}
}

//=============================================================================
// �G�l�~�[�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_Enemy[0];
}


//=============================================================================
// �G�̔�������
//=============================================================================
void SetEnemy(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// �������g�p�̃f�[�^��T��
		if (g_Enemy[i].use == false)	// ���g�p��Ԃ̓G�f�[�^��������
		{
			g_Enemy[i].use = true;		// �g�p��Ԃ֕ύX����
			return;						// �G���Z�b�g�ł����̂ŏI������
		}
	}
}

// ��������
void WatchEnemy(int i)
{
	// ���ŃT�[�`
	{
		//���݈ʒu
		D3DXVECTOR2 position = g_Enemy[i].pos;

		//�ڕW�n�_
		D3DXVECTOR2 destination = GetPlayerPosition();

		//����
		D3DXVECTOR2 components;

		//�傫��
		float magnitude;

		//�����i�P�ʃx�N�g���j
		D3DXVECTOR2 direction;

		//�����v�Z
		components.x = destination.x - position.x;
		components.y = destination.y - position.y;

		//�傫���v�Z
		magnitude = (float)sqrt(components.x * components.x + components.y * components.y);

		//�����v�Z�i���K���j
		direction.x = components.x / magnitude;
		direction.y = components.y / magnitude;

		// ���̈ړ��ʂ̕����傫���Ƃ�
		if (fabsf(direction.x) > fabsf(direction.y))
		{
			// ���ɍs���Ƃ�
			if (direction.x < 0)
			{
				g_Enemy[i].u = 1.0f;
				g_Enemy[i].v = 0.0f;
				g_Enemy[i].uh = -0.5f;
				g_Enemy[i].vh = 0.5f;
			}
			else // �E�ɍs���Ƃ�
			{
				g_Enemy[i].u = 0.5f;
				g_Enemy[i].v = 0.0f;
				g_Enemy[i].uh = 0.5f;
				g_Enemy[i].vh = 0.5f;
			}
		}
		else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
		{
			// ��ɍs���Ƃ�
			if (direction.y < 0)
			{
				g_Enemy[i].u = 0.5f;
				g_Enemy[i].v = 0.5f;
				g_Enemy[i].uh = 0.5f;
				g_Enemy[i].vh = 0.5f;
			}
			else // ���ɍs���Ƃ�
			{
				g_Enemy[i].u = 0.0f;
				g_Enemy[i].v = 0.5f;
				g_Enemy[i].uh = 0.5f;
				g_Enemy[i].vh = 0.5f;
			}
		}

		//�ړ���̈ʒu���v�Z
		g_Enemy[i].pos.x += direction.x * FIND_SPD;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy[i].pos.x + direction.x * FIND_SPD, g_Enemy[i].pos.y))
			== 1)
		{
			g_Enemy[i].pos.x -= direction.x * FIND_SPD;
			g_Enemy[i].notmove.x += fabsf(direction.x);

			if (!g_Enemy[i].movecntX)
			{
				// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
				if (g_Enemy[i].notmove.x > CHIP_SIZE)
				{
					g_Enemy[i].movecntX = true;
					g_Enemy[i].notmove.x = 0.0f;
				}
			}
		}

		g_Enemy[i].pos.y += direction.y * FIND_SPD;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy[i].pos.x, g_Enemy[i].pos.y + direction.y * FIND_SPD))
			== 1)
		{
			g_Enemy[i].pos.y -= direction.y * FIND_SPD;
			g_Enemy[i].notmove.y += fabsf(direction.y);

			if (!g_Enemy[i].movecntY)
			{
				// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
				if (g_Enemy[i].notmove.y > CHIP_SIZE)
				{
					g_Enemy[i].movecntY = true;
					g_Enemy[i].notmove.y = 0.0f;
				}
			}

		}

		// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
		if (g_Enemy[i].movecntX)
		{
			g_Enemy[i].addmove.y += ENEMY_SPD;

			g_Enemy[i].pos.y += ENEMY_SPD;
			if (GetMapEnter(D3DXVECTOR2(g_Enemy[i].pos.x, g_Enemy[i].pos.y + ENEMY_SPD))
				== 1)
			{
				g_Enemy[i].pos.y -= ENEMY_SPD;
			}

			// STOP_CNT���ړ�������
			if (g_Enemy[i].addmove.y > CHIP_SIZE * 3 / 2)
			{
				g_Enemy[i].movecntX = false;
				g_Enemy[i].addmove.y = 0.0f;
			}
		}
		if (g_Enemy[i].movecntY)
		{
			g_Enemy[i].addmove.x += ENEMY_SPD;

			g_Enemy[i].pos.x += ENEMY_SPD;
			if (GetMapEnter(D3DXVECTOR2(g_Enemy[i].pos.x + ENEMY_SPD, g_Enemy[i].pos.y))
				== 1)
			{
				g_Enemy[i].pos.x -= ENEMY_SPD;
			}

			// STOP_CNT���ړ�������
			if (g_Enemy[i].addmove.x > CHIP_SIZE * 3 / 2)
			{
				g_Enemy[i].movecntY = false;
				g_Enemy[i].addmove.x = 0.0f;
			}
		}

	}

}

// �����Ă��Ȃ�������
void NoneWatchEnemy(int i)
{
	g_Enemy[i].timer++;

	if (g_Enemy[i].timer > MOVE_TIMER)
	{
		g_Enemy[i].move = rand() % 5;
		g_Enemy[i].timer = 0;
		srand((unsigned)time(NULL));
	}

	// �G�l�~�[�ړ�
	// 1:��@2:���@3:�E�@4:���@0:�Ȃ�(�ҋ@)
	if (g_Enemy[i].move == 1) // ��
	{
		g_Enemy[i].pos.y -= g_Enemy[i].vel.y;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy[i].pos.x, g_Enemy[i].pos.y - g_Enemy[i].vel.y))
			== 1)
		{
			g_Enemy[i].pos.y += g_Enemy[i].vel.y;
		}
		g_Enemy[i].u = 0.5f;
		g_Enemy[i].v = 0.5f;
		g_Enemy[i].uh = 0.5f;
		g_Enemy[i].vh = 0.5f;
	}

	if (g_Enemy[i].move == 2) // ��
	{
		g_Enemy[i].pos.y += g_Enemy[i].vel.y;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy[i].pos.x, g_Enemy[i].pos.y + g_Enemy[i].vel.y))
			== 1)
		{
			g_Enemy[i].pos.y -= g_Enemy[i].vel.y;
		}
		g_Enemy[i].u = 0.0f;
		g_Enemy[i].v = 0.5f;
		g_Enemy[i].uh = 0.5f;
		g_Enemy[i].vh = 0.5f;
	}

	if (g_Enemy[i].move == 3) // �E
	{
		g_Enemy[i].pos.x += g_Enemy[i].vel.x;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy[i].pos.x + g_Enemy[i].vel.x, g_Enemy[i].pos.y))
			== 1)
		{
			g_Enemy[i].pos.x -= g_Enemy[i].vel.x;
		}
		g_Enemy[i].u = 0.5f;
		g_Enemy[i].v = 0.0f;
		g_Enemy[i].uh = 0.5f;
		g_Enemy[i].vh = 0.5f;
	}

	if (g_Enemy[i].move == 4) // ��
	{
		g_Enemy[i].pos.x -= g_Enemy[i].vel.x;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy[i].pos.x - g_Enemy[i].vel.x, g_Enemy[i].pos.y))
			== 1)
		{
			g_Enemy[i].pos.x += g_Enemy[i].vel.x;
		}
		g_Enemy[i].u = 1.0f;
		g_Enemy[i].v = 0.0f;
		g_Enemy[i].uh = -0.5f;
		g_Enemy[i].vh = 0.5f;
	}

	if (g_Enemy[i].move == 0) // �ҋ@
	{
		g_Enemy[i].u = 0.0f;
		g_Enemy[i].v = 0.0f;
		g_Enemy[i].uh = 0.5f;
		g_Enemy[i].vh = 0.5f;
	}

}

// �͈͓��Ƀv���C���[������荞�񂾂�
bool SerchPlayer(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos)
{
	D3DXVECTOR2 maxRange, minRange;

	minRange.x = Enemypos.x - SERCH_RANGE;
	minRange.y = Enemypos.y - SERCH_RANGE;
	maxRange.x = Enemypos.x + SERCH_RANGE;
	maxRange.y = Enemypos.y + SERCH_RANGE;

	//X���̔���
	if (minRange.x < Playerpos.x &&
		maxRange.x > Playerpos.x)
	{
		//Y���̔���
		if (minRange.y < Playerpos.y &&
			maxRange.y > Playerpos.y)
		{
			//�S�Ă̏�����TRUE�Ȃ�q�b�g����
			return true;
		}
	}

	return false;
}