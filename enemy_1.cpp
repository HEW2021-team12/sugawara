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
#include "patrol.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define ENEMY_SPD			(1.4f)
#define ENEMY_SIZE			(55.0f)
#define SERCH_RANGE			(220.0f)
#define FIND_SPD			(2.0f)
#define NONE_MOVE			(150.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void SetEnemy(int i);
void WatchEnemy(int i);
void NoneWatchEnemy(int i);
bool SerchPlayer(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
unsigned char	g_EnemyTexture = 0;
unsigned char	g_watch_SE = 0;
unsigned char	g_watch_lost_SE = 0;

// �������m�ۗp�A�h���X
ENEMY* g_Enemy;		  // �G�l�~�[�\����
char* g_RouteX;		  // ���񃋁[�g�p
char* g_RouteY;		  // ���񃋁[�g�p

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{
	g_EnemyTexture = LoadTexture("data/TEXTURE/enemy.png");
	g_watch_SE = LoadSound("data/SE/watch1.wav");
	g_watch_lost_SE = LoadSound("data/SE/enemy_worp.wav");

	// �������m��
	g_Enemy = new ENEMY[ENEMY_MAX];

	g_RouteX = new char[ROOT_MAX];
	g_RouteY = new char[ROOT_MAX];
	
	// �G�l�~�[�\���̂̏�����
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// ��̖�
		if (i == 0)
		{
			g_Enemy[i].use = true;
			g_Enemy[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Enemy[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 16;
			g_Enemy[i].routenom = 3;
		}
		// ��̖�
		if (i == 1)
		{
			g_Enemy[i].use = false;
			g_Enemy[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 3;
			g_Enemy[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 9;
			g_Enemy[i].routenom = 0;
		}
		// �O�̖�
		if (i == 2)
		{
			g_Enemy[i].use = false;
			g_Enemy[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 23;
			g_Enemy[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 7;
			g_Enemy[i].routenom = 8;
		}

		// ����
		g_Enemy[i].watch = false;
		g_Enemy[i].w = ENEMY_SIZE;
		g_Enemy[i].h = ENEMY_SIZE;
		g_Enemy[i].vel = D3DXVECTOR2(ENEMY_SPD, ENEMY_SPD);
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

	// ���񃋁[�g�ݒ�
	for (char r = 0; r < ROOT_MAX; r++)
	{
		g_RouteX[r] = SetRouteX(r);
		g_RouteY[r] = SetRouteY(r);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	// ���������
	delete[] g_Enemy;
	delete[] g_RouteX;
	delete[] g_RouteY;
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
					PlaySound(g_watch_lost_SE, 0);

					char work = rand() % ROOT_MAX;
					
					g_Enemy[i].routenom = work;
					g_Enemy[i].pos.x = SetRouteX(work) * CHIP_SIZE + (CHIP_SIZE / 2);
					g_Enemy[i].pos.y = SetRouteY(work) * CHIP_SIZE + (CHIP_SIZE / 2);
				}

				g_Enemy[i].watch = false;

				NoneWatchEnemy(i);
			}
		}
	}
	// �G�l�~�[�ǉ�
	if (GetTimer() == 20)
	{
		SetEnemy(1);
	}
	if (GetTimer() == 10)
	{
		SetEnemy(2);
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
// �G�̔�������
//=============================================================================
void SetEnemy(int i)
{
	// �������g�p�̃f�[�^��T��
	if (g_Enemy[i].use == false)	// ���g�p��Ԃ̓G�f�[�^��������
	{
		g_Enemy[i].use = true;		// �g�p��Ԃ֕ύX����

		// �o�������Ƃ���SE

		return;						// �G���Z�b�g�ł����̂ŏI������
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
				if (g_Enemy[i].notmove.x > NONE_MOVE)
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
				if (g_Enemy[i].notmove.y > NONE_MOVE)
				{
					g_Enemy[i].movecntY = true;
					g_Enemy[i].notmove.y = 0.0f;
				}
			}

		}

		// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
		if (g_Enemy[i].movecntX)
		{
			g_Enemy[i].movecntX = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE, 0);

			char work = rand() % ROOT_MAX;

			g_Enemy[i].routenom = work;
			g_Enemy[i].pos.x = SetRouteX(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy[i].pos.y = SetRouteY(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}
		if (g_Enemy[i].movecntY)
		{
			g_Enemy[i].movecntY = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE, 0);

			char work = rand() % ROOT_MAX;

			g_Enemy[i].routenom = work;
			g_Enemy[i].pos.x = SetRouteX(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy[i].pos.y = SetRouteY(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

	}

}

// �����Ă��Ȃ�������
void NoneWatchEnemy(int i)
{
	{
		//���݈ʒu
		D3DXVECTOR2 position = g_Enemy[i].pos;

		char work = g_Enemy[i].routenom;

		if (g_Enemy[i].pos.x < g_RouteX[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Enemy[i].pos.x > g_RouteX[work] * CHIP_SIZE)
		{
			if (g_Enemy[i].pos.y < g_RouteY[work] * CHIP_SIZE + CHIP_SIZE &&
				g_Enemy[i].pos.y > g_RouteY[work] * CHIP_SIZE)
			{
				if (i % 2 == 0)
				{
					// ���� or 0
					work++;
					g_Enemy[i].routenom++;
					if (work >= ROOT_MAX)
					{
						work = 0;
						g_Enemy[i].routenom = 0;
					}
				}
				else
				{
					// �
					work--;
					g_Enemy[i].routenom--;
					if (work < 0)
					{
						work = ROOT_MAX;
						g_Enemy[i].routenom = ROOT_MAX;
					}
				}

				
			}
		}

		//�ڕW�n�_
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteX[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteY[work] * CHIP_SIZE + (CHIP_SIZE / 2));

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
		g_Enemy[i].pos.x += direction.x * ENEMY_SPD;
		g_Enemy[i].pos.y += direction.y * ENEMY_SPD;
		
	}
}

// �͈͓��Ƀv���C���[�����荞�񂾂�
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


//=============================================================================
// �G�l�~�[�\���̂̐擪�A�h���X���擾
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}