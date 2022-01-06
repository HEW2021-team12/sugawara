//=============================================================================
//
// �X���C������ [slime.cpp]
// Author : �Îs
//
//=============================================================================
#include "slime.h"
#include "texture.h"
#include "player.h"
#include "sprite.h"
#include "main.h"
#include "map.h"
#include <time.h>
#include "sound.h"
#include "timer.h"
#include "patrol.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define SLIME_SPD			(1.1f)
#define SLIME_SIZEX			(70.0f)
#define SLIME_SIZEY			(50.0f)

#define BABBLE_SIZE			(40.0f)
#define BABBLE_COUNT		(300)

#define SET_SPD				(150)
#define SERCH_RANGE			(220.0f)
#define FIND_SPD			(2.0f)
#define NONE_MOVE			(150.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void SetBabble2(int i, D3DXVECTOR2 pos);
void MoveSlime2(int i);
void NoneWatchSlime2(int i);
bool SerchPlayerSlime2(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
unsigned char	g_SlimeTexture2 = 0;
unsigned char	g_watch_SE22 = 0;
unsigned char	g_watch_lost_SE22 = 0;


// �������m�ۗp�A�h���X
SLIME* g_Slime2;			  // �X���C���\����
BABBLE** g_Babble2;			  // �o�u���\����
char* g_RouteSlimeX2;		  // ���񃋁[�g�p
char* g_RouteSlimeY2;		  // ���񃋁[�g�p

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSlime2(void)
{
	g_SlimeTexture2 = LoadTexture("data/TEXTURE/slime1.png");

	// �������m��
	g_Slime2 = new SLIME[SLIME_MAX2];
	g_Babble2 = new BABBLE * [SLIME_MAX2];

	for (int i = 0; i < SLIME_MAX; i++)
	{
		g_Babble2[i] = new BABBLE[BABBLE_MAX];
	}

	g_RouteSlimeX2 = new char[ROOT_MAX2_1];
	g_RouteSlimeY2 = new char[ROOT_MAX2_1];

	// �X���C���\���̂̏�����
	for (int i = 0; i < SLIME_MAX2; i++)
	{
		// �P�̖�
		if (i == 0)
		{
			g_Slime2[i].use = true;
			g_Slime2[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime2[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime2[i].routenom = 5;
		}

		// ����
		g_Slime2[i].h = SLIME_SIZEY;
		g_Slime2[i].w = SLIME_SIZEX;
		g_Slime2[i].vel = D3DXVECTOR2(SLIME_SPD, SLIME_SPD);

		// �X���C��UV
		g_Slime2[i].u = 0.0f;
		g_Slime2[i].v = 0.0f;
		g_Slime2[i].uh = 5.0f;
		g_Slime2[i].vh = 2.0f;
	}

	// ���񃋁[�g�ݒ�
	for (char r = 0; r < ROOT_MAX2_1; r++)
	{
		g_RouteSlimeX2[r] = SetRouteX2(r);
		g_RouteSlimeY2[r] = SetRouteY2(r);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSlime2(void)
{
	// ���������
	delete[] g_Slime2;
	delete[] g_Babble2;
	delete[] g_RouteSlimeX2;
	delete[] g_RouteSlimeY2;
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateSlime2(void)
{
	for (int i = 0; i < SLIME_MAX2; i++)
	{
		if (g_Slime2[i].use)
		{
			MoveSlime2(i);

			if (g_Slime2[i].setcount > SET_SPD)
			{
				g_Slime2[i].setcount = 0;
				SetBabble2(i, g_Slime2[i].pos);
			}
			else
			{
				g_Slime2[i].setcount++;
			}
		}

		for (int j = 0; j < BABBLE_MAX; j++)
		{
			if (g_Babble2[i][j].use)
			{
				g_Babble2[i][j].count--;

				if (g_Babble2[i][j].count < 0)
				{
					g_Babble2[i][j].use = false;
				}
			}
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawSlime2(void)
{
	// �X���C��
	for (int i = 0; i < SLIME_MAX2; i++)
	{
		// �o�u��
		for (int j = 0; j < BABBLE_MAX; j++)
		{
			if (g_Babble2[i][j].use)
			{
				PLAYER* player = GetPlayer();

				// ��ʓ��ɓ����Ă��邩
				if (fabsf(g_Babble2[i][j].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
					fabsf(g_Babble2[i][j].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
				{
					//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
					float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Babble2[i][j].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
					float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Babble2[i][j].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
					float pw = g_Babble2[i][j].w;		// �G�l�~�[�̕\����
					float ph = g_Babble2[i][j].h;		// �G�l�~�[�̕\������
					D3DXCOLOR col;

					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					DrawSpriteColor(g_SlimeTexture2, px, py, pw, ph,
						g_Babble2[i][j].u, g_Babble2[i][j].v,
						g_Babble2[i][j].uh, g_Babble2[i][j].vh,
						col);
				}
			}
		}

		// �X���C��
		if (g_Slime2[i].use)
		{
			PLAYER* player = GetPlayer();

			// ��ʓ��ɓ����Ă��邩
			if (fabsf(g_Slime2[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Slime2[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Slime2[i].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Slime2[i].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
				float pw = g_Slime2[i].w;		// �G�l�~�[�̕\����
				float ph = g_Slime2[i].h;		// �G�l�~�[�̕\������
				D3DXCOLOR col;

				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				DrawSpriteColor(g_SlimeTexture2, px, py, pw, ph,
					g_Slime2[i].u, g_Slime2[i].v,
					g_Slime2[i].uh, g_Slime2[i].vh,
					col);
			}
		}
	}
}



// �o�u���𔭐�������
void SetBabble2(int i, D3DXVECTOR2 pos)
{
	for (int j = 0; j < BABBLE_MAX; j++)
	{
		// �������g�p�̃f�[�^��T��
		if (g_Babble2[i][j].use == false)	// ���g�p��Ԃ̓G�f�[�^��������
		{
			g_Babble2[i][j].use = true;		// �g�p��Ԃ֕ύX����
			g_Babble2[i][j].pos = pos;
			g_Babble2[i][j].w = BABBLE_SIZE;
			g_Babble2[i][j].h = BABBLE_SIZE - 10.0f;

			g_Babble2[i][j].count = BABBLE_COUNT;

			g_Babble2[i][j].u = 0.5f;
			g_Babble2[i][j].v = 0.0f;
			g_Babble2[i][j].uh = 0.5f;
			g_Babble2[i][j].vh = 0.5f;

			// �o�������Ƃ���SE

			return;						// �G���Z�b�g�ł����̂ŏI������
		}
	}
}

// �X���C���̈ړ�
void MoveSlime2(int i)
{
	//���݈ʒu
	D3DXVECTOR2 position = g_Slime2[i].pos;

	char work = g_Slime2[i].routenom;

	if (g_Slime2[i].pos.x < g_RouteSlimeX2[work] * CHIP_SIZE + CHIP_SIZE &&
		g_Slime2[i].pos.x > g_RouteSlimeX2[work] * CHIP_SIZE)
	{
		if (g_Slime2[i].pos.y < g_RouteSlimeY2[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Slime2[i].pos.y > g_RouteSlimeY2[work] * CHIP_SIZE)
		{
			if (i % 2 == 0)
			{
				// ���� or 0
				work++;
				g_Slime2[i].routenom++;
				if (work >= ROOT_MAX2_1)
				{
					work = 0;
					g_Slime2[i].routenom = 0;
				}
			}
			else
			{
				// �
				work--;
				g_Slime2[i].routenom--;
				if (work < 0)
				{
					work = ROOT_MAX2_1;
					g_Slime2[i].routenom = ROOT_MAX2_1;
				}
			}
		}
	}

	//�ڕW�n�_
	D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteSlimeX2[work] * CHIP_SIZE + (CHIP_SIZE / 2),
		g_RouteSlimeY2[work] * CHIP_SIZE + (CHIP_SIZE / 2));

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
			g_Slime2[i].u = 0.0f;
			g_Slime2[i].v = 0.5f;
			g_Slime2[i].uh = 0.5f;
			g_Slime2[i].vh = 0.5f;
		}
		else // �E�ɍs���Ƃ�
		{
			g_Slime2[i].u = 0.5f;
			g_Slime2[i].v = 0.5f;
			g_Slime2[i].uh = 0.5f;
			g_Slime2[i].vh = 0.5f;
		}
	}
	else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
	{
		// ��ɍs���Ƃ�
		if (direction.y < 0)
		{
			g_Slime2[i].u = 0.5f;
			g_Slime2[i].v = 0.0f;
			g_Slime2[i].uh = 0.5f;
			g_Slime2[i].vh = 0.5f;
		}
		else // ���ɍs���Ƃ�
		{
			g_Slime2[i].u = 0.0f;
			g_Slime2[i].v = 0.0f;
			g_Slime2[i].uh = 0.5f;
			g_Slime2[i].vh = 0.5f;
		}
	}

	//�ړ���̈ʒu���v�Z
	g_Slime2[i].pos.x += direction.x * FIND_SPD;
	if (GetMapEnter(D3DXVECTOR2(g_Slime2[i].pos.x + direction.x * FIND_SPD, g_Slime2[i].pos.y))
		== 1)
	{
		g_Slime2[i].pos.x -= direction.x * FIND_SPD;
		g_Slime2[i].notmove.x += fabsf(direction.x);

		if (!g_Slime2[i].movecntX)
		{
			// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
			if (g_Slime2[i].notmove.x > NONE_MOVE)
			{
				g_Slime2[i].movecntX = true;
				g_Slime2[i].notmove.x = 0.0f;
			}
		}
	}

	g_Slime2[i].pos.y += direction.y * FIND_SPD;
	if (GetMapEnter(D3DXVECTOR2(g_Slime2[i].pos.x, g_Slime2[i].pos.y + direction.y * FIND_SPD))
		== 1)
	{
		g_Slime2[i].pos.y -= direction.y * FIND_SPD;
		g_Slime2[i].notmove.y += fabsf(direction.y);

		if (!g_Slime2[i].movecntY)
		{
			// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
			if (g_Slime2[i].notmove.y > NONE_MOVE)
			{
				g_Slime2[i].movecntY = true;
				g_Slime2[i].notmove.y = 0.0f;
			}
		}

	}

	// ��莞�ԓ����Ȃ��i��Q���Ɉ���������j
	if (g_Slime2[i].movecntX)
	{
		g_Slime2[i].movecntX = false;

		// �����������̏����iSE�j
		PlaySound(g_watch_lost_SE22, 0);

		char work = rand() % ROOT_MAX2_1;

		g_Slime2[i].routenom = work;
		g_Slime2[i].pos.x = SetRouteX2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		g_Slime2[i].pos.y = SetRouteY2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
	}
	if (g_Slime2[i].movecntY)
	{
		g_Slime2[i].movecntY = false;

		// �����������̏����iSE�j
		PlaySound(g_watch_lost_SE22, 0);

		char work = rand() % ROOT_MAX2_1;

		g_Slime2[i].routenom = work;
		g_Slime2[i].pos.x = SetRouteX2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		g_Slime2[i].pos.y = SetRouteY2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
	}

}



// �����Ă��Ȃ�������
void NoneWatchSlime2(int i)
{
	{
		//���݈ʒu
		D3DXVECTOR2 position = g_Slime2[i].pos;

		char work = g_Slime2[i].routenom;

		if (g_Slime2[i].pos.x < g_RouteSlimeX2[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Slime2[i].pos.x > g_RouteSlimeX2[work] * CHIP_SIZE)
		{
			if (g_Slime2[i].pos.y < g_RouteSlimeY2[work] * CHIP_SIZE + CHIP_SIZE &&
				g_Slime2[i].pos.y > g_RouteSlimeY2[work] * CHIP_SIZE)
			{
				if (i % 2 == 0)
				{
					// ���� or 0
					work++;
					g_Slime2[i].routenom++;
					if (work >= ROOT_MAX2_1)
					{
						work = 0;
						g_Slime2[i].routenom = 0;
					}
				}
				else
				{
					// �
					work--;
					g_Slime2[i].routenom--;
					if (work < 0)
					{
						work = ROOT_MAX2_1;
						g_Slime2[i].routenom = ROOT_MAX2_1;
					}
				}


			}
		}

		//�ڕW�n�_
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteSlimeX2[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteSlimeX2[work] * CHIP_SIZE + (CHIP_SIZE / 2));

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
				g_Slime2[i].u = 1.0f;
				g_Slime2[i].v = 0.0f;
				g_Slime2[i].uh = -0.5f;
				g_Slime2[i].vh = 0.5f;
			}
			else // �E�ɍs���Ƃ�
			{
				g_Slime2[i].u = 0.5f;
				g_Slime2[i].v = 0.0f;
				g_Slime2[i].uh = 0.5f;
				g_Slime2[i].vh = 0.5f;
			}
		}
		else // �c�̈ړ��ʂ̕����傫�� or �����Ƃ�
		{
			// ��ɍs���Ƃ�
			if (direction.y < 0)
			{
				g_Slime2[i].u = 0.5f;
				g_Slime2[i].v = 0.5f;
				g_Slime2[i].uh = 0.5f;
				g_Slime2[i].vh = 0.5f;
			}
			else // ���ɍs���Ƃ�
			{
				g_Slime2[i].u = 0.0f;
				g_Slime2[i].v = 0.5f;
				g_Slime2[i].uh = 0.5f;
				g_Slime2[i].vh = 0.5f;
			}
		}

		//�ړ���̈ʒu���v�Z
		g_Slime2[i].pos.x += direction.x * SLIME_SPD;
		g_Slime2[i].pos.y += direction.y * SLIME_SPD;

	}
}

// �͈͓��Ƀv���C���[�����荞�񂾂�
bool SerchPlayerSlime2(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos)
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
// �X���C���\���̂̐擪�A�h���X���擾
//=============================================================================
SLIME* GetSlime2(void)
{
	return g_Slime2;
}

BABBLE** GetBabble2(void)
{
	return g_Babble2;
}