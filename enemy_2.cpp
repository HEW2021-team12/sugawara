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
#define ROT_SPD				(0.05f)
#define ENEMY_SPD			(2.0f)
#define ENEMY_SIZE			(55.0f)
#define MOVE_TIMER			(50)
#define SERCH_RANGE			(250.0f)
#define FIND_SPD			(2.0f)
#define NONE_MOVE			(150.0f)

#define SEARCH_TIMER		(CHIP_SIZE / ENEMY_SPD)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void SetEnemy2(int i);
void WatchEnemy2(int i);
void NoneWatchEnemy2(int i);
bool SerchPlayer2(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos); //?

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
unsigned char	g_EnemyTexture2 = 0;
unsigned char	g_watch_SE2 = 0;
unsigned char	g_watch_lost_SE2 = 0;

// �������m�ۗp�A�h���X
ENEMY* g_Enemy2;				  // �G�l�~�[�\����
char* g_RouteX2;		  // ���񃋁[�g�p
char* g_RouteY2;		  // ���񃋁[�g�p

char* g_RouteX2_1;		  // ���񃋁[�g�p
char* g_RouteY2_1;		  // ���񃋁[�g�p

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy2(void)
{
	g_EnemyTexture2 = LoadTexture("data/TEXTURE/enemy.png");
	g_watch_SE2 = LoadSound("data/SE/watch1.wav");
	g_watch_lost_SE2 = LoadSound("data/SE/enemy_worp.wav");

	// �������m��
	g_Enemy2 = new ENEMY[ENEMY_MAX2];

	g_RouteX2 = new char[ROOT_MAX2];
	g_RouteY2 = new char[ROOT_MAX2];


	g_RouteX2_1 = new char[ROOT_MAX2_1];
	g_RouteY2_1 = new char[ROOT_MAX2_1];

	// �G�l�~�[�\���̂̏�����
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// ��̖�
		if (i == 0)
		{
			g_Enemy2[i].use = true;
			g_Enemy2[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Enemy2[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 16;
			g_Enemy2[i].routenom = 3;
		}
		// ��̖�
		if (i == 1)
		{
			g_Enemy2[i].use = false;
			g_Enemy2[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 4;
			g_Enemy2[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 7;
			g_Enemy2[i].routenom = 0;
		}
		// �O�̖�
		if (i == 2)
		{
			g_Enemy2[i].use = false;
			g_Enemy2[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 23;
			g_Enemy2[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 7;
			g_Enemy2[i].routenom = 8;
		}

		// ����
		g_Enemy2[i].watch = false;
		g_Enemy2[i].w = ENEMY_SIZE;
		g_Enemy2[i].h = ENEMY_SIZE;
		g_Enemy2[i].vel = D3DXVECTOR2(ENEMY_SPD, ENEMY_SPD);
		g_Enemy2[i].timer = 0;
		g_Enemy2[i].move = 0;
		g_Enemy2[i].notmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy2[i].addmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy2[i].movecntX = false;
		g_Enemy2[i].movecntY = false;

		// �G�l�~�[UV
		g_Enemy2[i].u = 0.0f;
		g_Enemy2[i].v = 0.0f;
		g_Enemy2[i].uh = 0.5f;
		g_Enemy2[i].vh = 0.5f;
	}


	// ���񃋁[�g�ݒ�
	for (char r = 0; r < ROOT_MAX2; r++)
	{
		g_RouteX2[r] = SetRouteX2(r);
		g_RouteY2[r] = SetRouteY2(r);
	}

	for (char r = 0; r < ROOT_MAX2_1; r++)
	{
		g_RouteX2_1[r] = SetRouteX2_1(r);
		g_RouteY2_1[r] = SetRouteY2_1(r);
	}

	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy2(void)
{
	// ���������
	delete[] g_Enemy2;
	delete[] g_RouteX2;
	delete[] g_RouteY2;

	delete[] g_RouteX2_1;
	delete[] g_RouteY2_1;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy2(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy2[i].use == true)
		{
			// �ǐՔ���
			if (SerchPlayer2(GetPlayerPosition(), g_Enemy2[i].pos))
			{// ������(�͈͓��ɓ�����)

				if (!g_Enemy2[i].watch)
				{
					PlaySound(g_watch_SE2, 0);
					g_Enemy2[i].watch = true;
				}

				WatchEnemy2(i);
				
			}
			// �����Ă��Ȃ�
			else
			{
				if (i == 0)
				{
					if (g_Enemy2[i].watch)
					{
						// �����������̏����iSE�j
						PlaySound(g_watch_lost_SE2, 0);

						char work = rand() % ROOT_MAX2;

						g_Enemy2[i].routenom = work;
						g_Enemy2[i].pos.x = SetRouteX2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
						g_Enemy2[i].pos.y = SetRouteY2(work) * CHIP_SIZE + (CHIP_SIZE / 2);

					}
				}
			 
				

				if (i == 1)
				{
					if (g_Enemy2[i].watch)
					{
						// �����������̏����iSE�j
						PlaySound(g_watch_lost_SE2, 0);

						char work = rand() % ROOT_MAX2_1;

						g_Enemy2[i].routenom = work;
						g_Enemy2[i].pos.x = SetRouteX2_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);
						g_Enemy2[i].pos.y = SetRouteY2_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);

					}
				}

				g_Enemy2[i].watch = false;

				NoneWatchEnemy2(i);

			}
		}
	}
	// �G�l�~�[�ǉ�
	if (GetTimer() == 20)
	{
		SetEnemy2(1);
	}
	if (GetTimer() == 10)
	{
		SetEnemy2(2);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy2(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy2[i].use == true)
		{
			PLAYER* player = GetPlayer();

			// ��ʓ��ɓ����Ă��邩
			if (fabsf(g_Enemy2[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Enemy2[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Enemy2[i].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Enemy2[i].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
				float pw = g_Enemy2[i].w;		// �G�l�~�[�̕\����
				float ph = g_Enemy2[i].h;		// �G�l�~�[�̕\������
				D3DXCOLOR col;

				if (SerchPlayer2(GetPlayerPosition(), g_Enemy2[i].pos))
				{
					col = D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				DrawSpriteColor(g_EnemyTexture2, px, py, pw, ph,
					g_Enemy2[i].u, g_Enemy2[i].v,
					g_Enemy2[i].uh, g_Enemy2[i].vh,
					col);
			}
		}
	}
}


//=============================================================================
// �G�̔�������
//=============================================================================
void SetEnemy2(int i)
{
	// �������g�p�̃f�[�^��T��
	if (g_Enemy2[i].use == false)	// ���g�p��Ԃ̓G�f�[�^��������
	{
		g_Enemy2[i].use = true;		// �g�p��Ԃ֕ύX����

		// �o�������Ƃ���SE

		return;						// �G���Z�b�g�ł����̂ŏI������
	}
}

// ��������
void WatchEnemy2(int i)
{
	// ���ŃT�[�`
	{
		//���݈ʒu
		D3DXVECTOR2 position = g_Enemy2[i].pos;

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
				g_Enemy2[i].u = 1.0f;
				g_Enemy2[i].v = 0.0f;
				g_Enemy2[i].uh = -0.5f;
				g_Enemy2[i].vh = 0.5f;
			}
			else // �E�ɍs���Ƃ�
			{
				g_Enemy2[i].u = 0.5f;
				g_Enemy2[i].v = 0.0f;
				g_Enemy2[i].uh = 0.5f;
				g_Enemy2[i].vh = 0.5f;
			}
		}
		else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
		{
			// ��ɍs���Ƃ�
			if (direction.y < 0)
			{
				g_Enemy2[i].u = 0.5f;
				g_Enemy2[i].v = 0.5f;
				g_Enemy2[i].uh = 0.5f;
				g_Enemy2[i].vh = 0.5f;
			}
			else // ���ɍs���Ƃ�
			{
				g_Enemy2[i].u = 0.0f;
				g_Enemy2[i].v = 0.5f;
				g_Enemy2[i].uh = 0.5f;
				g_Enemy2[i].vh = 0.5f;
			}
		}

		//�ړ���̈ʒu���v�Z
		g_Enemy2[i].pos.x += direction.x * FIND_SPD;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy2[i].pos.x + direction.x * FIND_SPD, g_Enemy2[i].pos.y))
			== 1)
		{
			g_Enemy2[i].pos.x -= direction.x * FIND_SPD;
			g_Enemy2[i].notmove.x += fabsf(direction.x);

			if (!g_Enemy2[i].movecntX)
			{
				// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
				if (g_Enemy2[i].notmove.x > NONE_MOVE)
				{
					g_Enemy2[i].movecntX = true;
					g_Enemy2[i].notmove.x = 0.0f;
				}
			}
		}

		g_Enemy2[i].pos.y += direction.y * FIND_SPD;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy2[i].pos.x, g_Enemy2[i].pos.y + direction.y * FIND_SPD))
			== 1)
		{
			g_Enemy2[i].pos.y -= direction.y * FIND_SPD;
			g_Enemy2[i].notmove.y += fabsf(direction.y);

			if (!g_Enemy2[i].movecntY)
			{
				// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
				if (g_Enemy2[i].notmove.y > NONE_MOVE)
				{
					g_Enemy2[i].movecntY = true;
					g_Enemy2[i].notmove.y = 0.0f;
				}
			}

		}

		// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
		if (g_Enemy2[i].movecntX)
		{
			g_Enemy2[i].movecntX = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE2, 0);

			char work = rand() % ROOT_MAX2;

			g_Enemy2[i].routenom = work;
			g_Enemy2[i].pos.x = SetRouteX2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy2[i].pos.y = SetRouteY2(work) * CHIP_SIZE + (CHIP_SIZE / 2);

		}
		if (g_Enemy2[i].movecntY)
		{
			g_Enemy2[i].movecntY = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE2, 0);

			char work = rand() % ROOT_MAX2;

			g_Enemy2[i].routenom = work;
			g_Enemy2[i].pos.x = SetRouteX2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy2[i].pos.y = SetRouteY2(work) * CHIP_SIZE + (CHIP_SIZE / 2);

		
		}

		if (g_Enemy2[i].movecntX)
		{

			g_Enemy2[i].movecntX = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE2, 0);

			char work = rand() % ROOT_MAX2_1;

			g_Enemy2[i].routenom = work;
			g_Enemy2[i].pos.x = SetRouteX2_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy2[i].pos.y = SetRouteY2_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

		if (g_Enemy2[i].movecntY)
		{
			g_Enemy2[i].movecntY = false;

			// �����������̏����iSE�j
			PlaySound(g_watch_lost_SE2, 0);

			char work = rand() % ROOT_MAX2_1;

			g_Enemy2[i].routenom = work;
			g_Enemy2[i].pos.x = SetRouteX2_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy2[i].pos.y = SetRouteY2_1(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

	}

}

// �����Ă��Ȃ�������
void NoneWatchEnemy2(int i)
{
	{
		//���݈ʒu
		D3DXVECTOR2 position = g_Enemy2[i].pos;

		char work = g_Enemy2[i].routenom;

		if (g_Enemy2[i].pos.x < g_RouteX2[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Enemy2[i].pos.x > g_RouteX2[work] * CHIP_SIZE)
		{
			if (g_Enemy2[i].pos.y < g_RouteY2[work] * CHIP_SIZE + CHIP_SIZE &&
				g_Enemy2[i].pos.y > g_RouteY2[work] * CHIP_SIZE)
			{
				if (i % 2 == 0)
				{
					// ���� or 0
					work++;
					g_Enemy2[i].routenom++;
					if (work >= ROOT_MAX2)
					{
						work = 0;
						g_Enemy2[i].routenom = 0;
					}
				}
				else
				{
					// �
					work--;
					g_Enemy2[i].routenom--;
					if (work < 0)
					{
						work = ROOT_MAX2;
						g_Enemy2[i].routenom = ROOT_MAX2;
					}
				}


			}
		}


		if (g_Enemy2[i].pos.x < g_RouteX2_1[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Enemy2[i].pos.x > g_RouteX2_1[work] * CHIP_SIZE)
		{
			if (g_Enemy2[i].pos.y < g_RouteY2_1[work] * CHIP_SIZE + CHIP_SIZE &&
				g_Enemy2[i].pos.y > g_RouteY2_1[work] * CHIP_SIZE)
			{
				if (i % 2 == 0)
				{
					// ���� or 0
					work++;
					g_Enemy2[i].routenom++;
					if (work >= ROOT_MAX2_1)
					{
						work = 0;
						g_Enemy2[i].routenom = 0;
					}
				}
				else
				{
					// �
					work--;
					g_Enemy2[i].routenom--;
					if (work < 0)
					{
						work = ROOT_MAX2_1;
						g_Enemy2[i].routenom = ROOT_MAX2_1;
					}
				}


			}
		}

		//�ڕW�n�_
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteX2[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteY2[work] * CHIP_SIZE + (CHIP_SIZE / 2));


		
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
				g_Enemy2[i].u = 1.0f;
				g_Enemy2[i].v = 0.0f;
				g_Enemy2[i].uh = -0.5f;
				g_Enemy2[i].vh = 0.5f;
			}
			else // �E�ɍs���Ƃ�
			{
				g_Enemy2[i].u = 0.5f;
				g_Enemy2[i].v = 0.0f;
				g_Enemy2[i].uh = 0.5f;
				g_Enemy2[i].vh = 0.5f;
			}
		}
		else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
		{
			// ��ɍs���Ƃ�
			if (direction.y < 0)
			{
				g_Enemy2[i].u = 0.5f;
				g_Enemy2[i].v = 0.5f;
				g_Enemy2[i].uh = 0.5f;
				g_Enemy2[i].vh = 0.5f;
			}
			else // ���ɍs���Ƃ�
			{
				g_Enemy2[i].u = 0.0f;
				g_Enemy2[i].v = 0.5f;
				g_Enemy2[i].uh = 0.5f;
				g_Enemy2[i].vh = 0.5f;
			}
		}

		//�ړ���̈ʒu���v�Z
		g_Enemy2[i].pos.x += direction.x * ENEMY_SPD;
		g_Enemy2[i].pos.y += direction.y * ENEMY_SPD;

	}
}

// �͈͓��Ƀv���C���[�����荞�񂾂�
bool SerchPlayer2(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos)
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
ENEMY* GetEnemy2(void)
{
	return &g_Enemy2[0];
}