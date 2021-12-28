//=============================================================================
//
// �Z���N�g��ʊǗ� [select_scene.cpp]
// Author : �^ ���� �Îs
//
//=============================================================================

#include "select_scene.h"
#include "input.h"
#include "keyboard.h"
#include "texture.h"
#include "sprite.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define		uv_size_x			(1.0f / 13)

#define		STAGE_NOM_SIZE_X	(32.0f)
#define		STAGE_NOM_SIZE_Y	(STAGE_NOM_SIZE_X * 2)

#define		YOKO				(4)
#define		TATE				(2)
#define		TOTAL_STAGE			(YOKO * TATE)

#define		CHANGE_INTERVAL		(0.5f)
#define		CHANGE_LIMIT		(15)

#define		PRESS_INTERVAL		(333)		// 1000�ň�b�Ԋu
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
struct SELECT_SCENE
{
	D3DXVECTOR2 pos[TOTAL_STAGE];
	D3DXVECTOR2 size;
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
SELECT_SCENE g_Select;

unsigned char	g_TextureNo = 0;	// �e�N�X�`�����
unsigned char	g_TextureNo_2 = 0;

unsigned char	selectX,selectY;
float g_ChangeSize;
bool g_ChangeSizeSwitch;

bool g_Interval;
DWORD g_IntervalCount;
DWORD g_IntervalNextCount;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSelect_Scene(void)
{
	g_TextureNo = LoadTexture("data/TEXTURE/back.jpg");
	g_TextureNo_2 = LoadTexture("data/TEXTURE/select_scene.png");

	// ���̔z�u
	g_Select.pos[0].x = (SCREEN_WIDTH / (YOKO + 1));

	for (int x = 1; x < TOTAL_STAGE; x++)
	{
		int work = x;
		work %= YOKO;

		if (work == 0)
		{
			// YOKO�̔{�� or 0
			g_Select.pos[x].x = g_Select.pos[0].x;
		}
		else
		{
			g_Select.pos[x].x = g_Select.pos[work - 1].x + g_Select.pos[0].x;
		}
	}
	// �c�̔z�u
	g_Select.pos[0].y = (SCREEN_WIDTH / (TATE + 4));

	for (int y = 1; y < TOTAL_STAGE; y++)
	{
		int work = y;
		work /= YOKO;

		g_Select.pos[y].y = g_Select.pos[0].y + (g_Select.pos[0].y * work);
	}
	g_Select.size = D3DXVECTOR2(STAGE_NOM_SIZE_X, STAGE_NOM_SIZE_Y);

	selectX = 0;
	selectY = 0;
	g_ChangeSize = 0.0f;
	g_ChangeSizeSwitch = true;

	g_Interval = true;
	g_IntervalCount = 0;
	g_IntervalNextCount = 0;
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSelect_Scene(void)
{
	StopSoundAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSelect_Scene(void)
{
	StageSelect();

	ControlSelect();

	// �I�����̃T�C�Y�ω�
	if (g_ChangeSizeSwitch)
	{
		g_ChangeSize += CHANGE_INTERVAL;
		if (g_ChangeSize > CHANGE_LIMIT)
		{
			g_ChangeSizeSwitch = false;
		}
	}
	else
	{
		g_ChangeSize -= CHANGE_INTERVAL;
		if (g_ChangeSize < 0)
		{
			g_ChangeSizeSwitch = true;
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawSelect_Scene(void)
{
	D3DXCOLOR col;

	// �w�i
	DrawSpriteLeftTop(g_TextureNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	// �X�e�[�W�\��
	for (int i = 0; i < TOTAL_STAGE; i++)
	{
		if (i == selectX + (selectY * YOKO))
		{
			col = D3DXCOLOR(0.5f, 1.0f, 1.0f, 1.0f);

			DrawSpriteColor(g_TextureNo_2,
				g_Select.pos[i].x, g_Select.pos[i].y,
				g_Select.size.x + g_ChangeSize, g_Select.size.y + g_ChangeSize,
				uv_size_x * i, 0.0f,
				uv_size_x, 1.0f,
				col);
		}
		else
		{
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			DrawSpriteColor(g_TextureNo_2,
				g_Select.pos[i].x, g_Select.pos[i].y,
				g_Select.size.x, g_Select.size.y,
				uv_size_x * i, 0.0f,
				uv_size_x, 1.0f,
				col);
		}
	}
}

void ControlSelect(void)
{
	//��
	if (Keyboard_IsKeyDown(KK_LEFT)
	//	|| GetThumbLeftX(0) < -INPUT_ZONE
		&& g_Interval)
	{
		g_Interval = false;

		selectX--;
		if (selectX < 0)
		{
			selectX = YOKO - 1;
			selectY--;
			if (selectY < 0)
			{
				selectY = TATE - 1;
			}
		}
	}
	//�E
	if (Keyboard_IsKeyDown(KK_RIGHT)
		|| GetThumbLeftX(0) > INPUT_ZONE)
	{
		if (g_Interval)
		{
			g_Interval = false;

			selectX++;
			if (selectX > YOKO - 1)
			{
				selectX = 0;
				selectY++;
				if (selectY > TATE - 1)
				{
					selectY = 0;
				}
			}
		}
	}

	//��
	if (Keyboard_IsKeyDown(KK_UP)
		|| GetThumbLeftY(0) >  INPUT_ZONE)
	{
		if (g_Interval)
		{
			g_Interval = false;

			selectY--;
			if (selectY < 0)
			{
				selectY = TATE - 1;
			}
		}
	}
	//��
	if (Keyboard_IsKeyDown(KK_DOWN)
		|| GetThumbLeftY(0) < -INPUT_ZONE)
	{
		if (g_Interval)
		{
			g_Interval = false;

			selectY++;
			if (selectY > TATE - 1)
			{
				selectY = 0;
			}
		}
	}

	// ��������̊Ԋu�̊Ǘ�
	if (!g_Interval)
	{
		g_IntervalCount = timeGetTime();				// �V�X�e���������擾

		if ((g_IntervalCount - g_IntervalNextCount) >= PRESS_INTERVAL)
		{
			g_IntervalNextCount = g_IntervalCount;		// FPS�𑪒肵��������ۑ�

			g_Interval = true;
		}
	}
	else
	{
		// �����ƈ�񉟂������肪����i��
		g_IntervalNextCount = g_IntervalCount = timeGetTime();
	}
}

// �X�e�[�W�Z���N�g
void StageSelect(void)
{
	// �X�e�[�W�P�i���Ń}�b�v�j
	if (Keyboard_IsKeyDown(KK_ENTER)
		|| IsButtonTriggered(0, XINPUT_GAMEPAD_B))
	{
		if (GetFadeState() == FADE_NONE
			&& selectX + (selectY * YOKO) == STAGE_1)
		{
			SceneTransition(SCENE_GAME1);
		}
	}

	// �X�e�[�W�Q
	if (Keyboard_IsKeyDown(KK_ENTER)
		|| IsButtonTriggered(0, XINPUT_GAMEPAD_B))
	{
		if (GetFadeState() == FADE_NONE
			&& selectX + (selectY * YOKO) == STAGE_2)
		{
			SceneTransition(SCENE_GAME2);
		}
	}

	// �^�C�g���J�ځi���͂��Ȃ��悤�ɐݒ�j
	if (Keyboard_IsKeyDown(KK_ENTER)
		|| IsButtonTriggered(0, XINPUT_GAMEPAD_B))
	{
		if (GetFadeState() == FADE_NONE
			&& selectX + (selectY * YOKO) == STAGE_NONE)
		{
			SceneTransition(SCENE_TITLE);
		}
	}
}