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
#define SLIME_SIZE			(50.0f)

#define BABBLE_SIZE			(40.0f)
#define BABBLE_COUNT		(300)

#define SET_SPD				(150)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

void SetBabble(int i, D3DXVECTOR2 pos);
void MoveSlime(int i);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
unsigned char	g_SlimeTexture = 0;
unsigned char	g_BabbleTexture = 0;
//unsigned char	g_Slime_SE = 0;

// �������m�ۗp�A�h���X
SLIME*  g_Slime;			  // �X���C���\����
BABBLE** g_Babble;			  // �o�u���\����
char* g_RouteSlimeX;		  // ���񃋁[�g�p
char* g_RouteSlimeY;		  // ���񃋁[�g�p

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSlime(void)
{
	g_SlimeTexture = LoadTexture("data/TEXTURE/slime.png");
	g_BabbleTexture = LoadTexture("data/TEXTURE/tako.png");

	// �������m��
	g_Slime = new SLIME[SLIME_MAX];
	g_Babble = new BABBLE* [SLIME_MAX];

	for (int i = 0; i < SLIME_MAX; i++)
	{
		g_Babble[i] = new BABBLE[BABBLE_MAX];
	}
	
	g_RouteSlimeX = new char[ROOT_MAX];
	g_RouteSlimeY = new char[ROOT_MAX];

	// �X���C���\���̂̏�����
	for (int i = 0; i < SLIME_MAX; i++)
	{
		// �P�̖�
		if (i == 0)
		{
			g_Slime[i].use = true;
			g_Slime[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime[i].routenom = 5;
		}

		// ����
		g_Slime[i].h = SLIME_SIZE;
		g_Slime[i].w = SLIME_SIZE;
		g_Slime[i].vel = D3DXVECTOR2(SLIME_SPD,SLIME_SPD);

		// �X���C��UV
		g_Slime[i].u = 0.0f;
		g_Slime[i].v = 0.0f;
		g_Slime[i].uh = 1.0f;
		g_Slime[i].vh = 1.0f;
	}

	// ���񃋁[�g�ݒ�
	for (char r = 0; r < ROOT_MAX; r++)
	{
		g_RouteSlimeX[r] = SetRouteX(r);
		g_RouteSlimeY[r] = SetRouteY(r);
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSlime(void)
{
	// ���������
	delete[] g_Slime;
	delete[] g_Babble;
	delete[] g_RouteSlimeX;
	delete[] g_RouteSlimeY;
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateSlime(void)
{
	for (int i = 0; i < SLIME_MAX; i++)
	{
		if (g_Slime[i].use)
		{
			MoveSlime(i);

			if (g_Slime[i].setcount > SET_SPD)
			{
				g_Slime[i].setcount = 0;
				SetBabble(i, g_Slime[i].pos);
			}
			else
			{
				g_Slime[i].setcount++;
			}
		}

		for (int j = 0; j < BABBLE_MAX; j++)
		{
			if (g_Babble[i][j].use)
			{
				g_Babble[i][j].count--;

				if (g_Babble[i][j].count < 0)
				{
					g_Babble[i][j].use = false;
				}
			}
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawSlime(void)
{
	// �X���C��
	for (int i = 0; i < SLIME_MAX; i++)
	{
		// �o�u��
		for (int j = 0; j < BABBLE_MAX; j++)
		{
			if (g_Babble[i][j].use)
			{
				PLAYER* player = GetPlayer();

				// ��ʓ��ɓ����Ă��邩
				if (fabsf(g_Babble[i][j].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
					fabsf(g_Babble[i][j].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
				{
					//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
					float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Babble[i][j].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
					float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Babble[i][j].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
					float pw = g_Babble[i][j].w;		// �G�l�~�[�̕\����
					float ph = g_Babble[i][j].h;		// �G�l�~�[�̕\������
					D3DXCOLOR col;

					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
					DrawSpriteColor(g_BabbleTexture, px, py, pw, ph,
						g_Babble[i][j].u, g_Babble[i][j].v,
						g_Babble[i][j].uh, g_Babble[i][j].vh,
						col);
				}
			}
		}

		// �X���C��
		if (g_Slime[i].use)
		{
			PLAYER* player = GetPlayer();

			// ��ʓ��ɓ����Ă��邩
			if (fabsf(g_Slime[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Slime[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//�G�l�~�[�̈ʒu��e�N�X�`���[���W�𔽉f
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Slime[i].pos.x - player->pos.x);	// �G�l�~�[�̕\���ʒuX
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Slime[i].pos.y - player->pos.y);	// �G�l�~�[�̕\���ʒuY
				float pw = g_Slime[i].w;		// �G�l�~�[�̕\����
				float ph = g_Slime[i].h;		// �G�l�~�[�̕\������
				D3DXCOLOR col;

				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
				DrawSpriteColor(g_SlimeTexture, px, py, pw, ph,
					g_Slime[i].u, g_Slime[i].v,
					g_Slime[i].uh, g_Slime[i].vh,
					col);
			}
		}
	}
}

//=============================================================================
// �X���C���\���̂̐擪�A�h���X���擾
//=============================================================================
SLIME* GetSlime(void)
{
	return g_Slime;
}

BABBLE** GetBabble(void)
{
	return g_Babble;
}

// �o�u���𔭐�������
void SetBabble(int i,D3DXVECTOR2 pos)
{
	for (int j = 0; j < BABBLE_MAX; j++)
	{
		// �������g�p�̃f�[�^��T��
		if (g_Babble[i][j].use == false)	// ���g�p��Ԃ̓G�f�[�^��������
		{
			g_Babble[i][j].use = true;		// �g�p��Ԃ֕ύX����
			g_Babble[i][j].pos = pos;
			g_Babble[i][j].w = BABBLE_SIZE;
			g_Babble[i][j].h = BABBLE_SIZE;

			g_Babble[i][j].count = BABBLE_COUNT;

			g_Babble[i][j].u = 0.0f;
			g_Babble[i][j].v = 0.0f;
			g_Babble[i][j].uh = 1.0f;
			g_Babble[i][j].vh = 1.0f;

			// �o�������Ƃ���SE

			return;						// �G���Z�b�g�ł����̂ŏI������
		}
	}
}

// �X���C���̈ړ�
void MoveSlime(int i)
{
	//���݈ʒu
	D3DXVECTOR2 position = g_Slime[i].pos;

	char work = g_Slime[i].routenom;

	if (g_Slime[i].pos.x < g_RouteSlimeX[work] * CHIP_SIZE + CHIP_SIZE &&
		g_Slime[i].pos.x > g_RouteSlimeX[work] * CHIP_SIZE)
	{
		if (g_Slime[i].pos.y < g_RouteSlimeY[work] * CHIP_SIZE + CHIP_SIZE &&
			g_Slime[i].pos.y > g_RouteSlimeY[work] * CHIP_SIZE)
		{
			if (i % 2 == 0)
			{
				// ���� or 0
				work++;
				g_Slime[i].routenom++;
				if (work >= ROOT_MAX)
				{
					work = 0;
					g_Slime[i].routenom = 0;
				}
			}
			else
			{
				// �
				work--;
				g_Slime[i].routenom--;
				if (work < 0)
				{
					work = ROOT_MAX;
					g_Slime[i].routenom = ROOT_MAX;
				}
			}
		}
	}

	//�ڕW�n�_
	D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteSlimeX[work] * CHIP_SIZE + (CHIP_SIZE / 2),
		g_RouteSlimeY[work] * CHIP_SIZE + (CHIP_SIZE / 2));

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

	//�ړ���̈ʒu���v�Z
	g_Slime[i].pos.x += direction.x * g_Slime[i].vel.x;
	g_Slime[i].pos.y += direction.y * g_Slime[i].vel.y;
}