
#include "timer.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "fade.h"

#include <time.h>
//*****************************************************************************
// �}�N����`
//*****************************************************************************

// �X�e�[�W���Ƃ̐�������
#define GAME_TIME			(30)

#define GAME_TIME_2			(60)

//#define GAME_TIME_3			(90)

#define TIMER_SIZE_X		(40.0f)
#define TIMER_SIZE_Y		(70.0f)

#define TIMER_DIGIT_M		(2)
#define TIMER_DIGIT_S		(2)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
unsigned char	g_TimerTexture = 0;

DWORD g_Cnt;
DWORD g_Nextcnt;
short g_Gametime;

//=============================================================================
// ����������
//=============================================================================
void InitTimer(void)
{
	g_TimerTexture = LoadTexture("data/TEXTURE/timer.png");

	g_Cnt = 0;
	g_Nextcnt = 0;

	// �X�e�[�W�P
	if (GetScene() == SCENE_GAME1)
	{
		g_Gametime = GAME_TIME;
	}

	// �X�e�[�W�Q
	if (GetScene() == SCENE_GAME2)
	{
		g_Gametime = GAME_TIME_2;
	}
	
}

//=============================================================================
// �I������
//=============================================================================
void UninitTimer(void)
{

}

void UpdateTimer(void)
{
	g_Cnt = timeGetTime();				// �V�X�e���������擾

	if ((g_Cnt - g_Nextcnt) >= 1000)	// 1�b���ƂɎ��s
	{
		g_Nextcnt = g_Cnt;				// FPS�𑪒肵��������ۑ�

		g_Gametime--;
	}

	if (GetFadeState() == FADE_NONE && g_Gametime < 0
		//|| GetKeyboardTrigger(DIK_RETURN)
		) {

		//RESULT�ֈڍs����
		SceneTransition(SCENE_RESULT);
	}

}
void DrawTimer(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	unsigned char m, s;
	short time = g_Gametime;

	if (time < 0)
	{
		time = 0;
		m = 0;
		s = 0;
	}
	else
	{
		m = time / 60;
		s = time % 60;
	}

	// ��
	for (int i = 0; i < TIMER_DIGIT_M; i++)
	{
		unsigned char n;
		float tx;

		n = m % 10;
		tx = n % 10 * 0.1f;
		m /= 10;

		float sx, sy;
		sx = SCREEN_WIDTH - 190.0f + TIMER_SIZE_X * (TIMER_DIGIT_M - i);
		sy = 50.0f;

		DrawSpriteColor(g_TimerTexture,
			sx, sy,
			TIMER_SIZE_X, TIMER_SIZE_Y,
			tx, 0.0f,	// UV�l�J�n
			0.1f, 1.0f, // UV�l�I���
			col);
	}

	// �b
	for (int i = 0; i < TIMER_DIGIT_S; i++)
	{
		unsigned char n;
		float tx;

		n = s % 10;
		tx = n % 10 * 0.1f;
		s /= 10;

		float sx, sy;
		sx = SCREEN_WIDTH - 100.0f + TIMER_SIZE_X * (TIMER_DIGIT_S - i);
		sy = 50.0f;

		DrawSpriteColor(g_TimerTexture,
			sx, sy,
			TIMER_SIZE_X, TIMER_SIZE_Y,
			tx, 0.0f,	// UV�l�J�n
			0.1f, 1.0f, // UV�l�I���
			col);
	}

}

int GetTimer(void)
{
	return g_Gametime;
}