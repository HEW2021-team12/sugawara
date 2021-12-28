
#include "timer.h"
#include "texture.h"
#include "sprite.h"
#include "input.h"
#include "player.h"
#include "scene.h"
#include "fade.h"

#include <time.h>
//*****************************************************************************
// マクロ定義
//*****************************************************************************

// ステージごとの制限時間
#define GAME_TIME			(30)

#define GAME_TIME_2			(60)

//#define GAME_TIME_3			(90)

#define TIMER_SIZE_X		(40.0f)
#define TIMER_SIZE_Y		(70.0f)

#define TIMER_DIGIT_M		(2)
#define TIMER_DIGIT_S		(2)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
unsigned char	g_TimerTexture = 0;

DWORD g_Cnt;
DWORD g_Nextcnt;
short g_Gametime;

//=============================================================================
// 初期化処理
//=============================================================================
void InitTimer(void)
{
	g_TimerTexture = LoadTexture("data/TEXTURE/timer.png");

	g_Cnt = 0;
	g_Nextcnt = 0;

	// ステージ１
	if (GetScene() == SCENE_GAME1)
	{
		g_Gametime = GAME_TIME;
	}

	// ステージ２
	if (GetScene() == SCENE_GAME2)
	{
		g_Gametime = GAME_TIME_2;
	}
	
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer(void)
{

}

void UpdateTimer(void)
{
	g_Cnt = timeGetTime();				// システム時刻を取得

	if ((g_Cnt - g_Nextcnt) >= 1000)	// 1秒ごとに実行
	{
		g_Nextcnt = g_Cnt;				// FPSを測定した時刻を保存

		g_Gametime--;
	}

	if (GetFadeState() == FADE_NONE && g_Gametime < 0
		//|| GetKeyboardTrigger(DIK_RETURN)
		) {

		//RESULTへ移行する
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

	// 分
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
			tx, 0.0f,	// UV値開始
			0.1f, 1.0f, // UV値終わり
			col);
	}

	// 秒
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
			tx, 0.0f,	// UV値開始
			0.1f, 1.0f, // UV値終わり
			col);
	}

}

int GetTimer(void)
{
	return g_Gametime;
}