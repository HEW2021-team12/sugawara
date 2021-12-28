/*==============================================================================

   頂点管理 [player.cpp]
	Author :	歐　改造 古市
	Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "player.h"
#include "input.h"
#include "keyboard.h"
#include "texture.h"
#include "sprite.h"
#include "enemy.h"
#include "map.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//プレイヤーの初期位置
// ステージ１
#define	PLAYER_POS_X		(13)
#define	PLAYER_POS_Y		(16)

// ステージ２
#define	PLAYER_POS_X_2		(7)
#define	PLAYER_POS_Y_2		(4)

//// ステージ３
//#define	PLAYER_POS_X_3		(7)
//#define	PLAYER_POS_Y_3		(4)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int	g_PlayerTexture = 0;
PLAYER		g_Player;
float		gravity_timer;

float		g_DrawGrav = 0;

// プレイヤーを動的に置き換えるのは保留

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	g_PlayerTexture = LoadTexture("data/TEXTURE/player.png");

	// ステージ１
	if (GetScene() == SCENE_GAME1)
	{
		g_Player.pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_X;
		g_Player.pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_Y;
	}
	// ステージ２
	if (GetScene() == SCENE_GAME2)
	{
		g_Player.pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_X_2;
		g_Player.pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_Y_2;
	}
	//// ステージ３
	//if (GetScene() == SCENE_GAME3)
	//{
	//	g_Player.pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_X_3;
	//	g_Player.pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * PLAYER_POS_Y_3;
	//}

	g_Player.size.x = PLAYER_SIZE;
	g_Player.size.y = PLAYER_SIZE;
	g_Player.vel.x = 0.0f;
	g_Player.vel.y = 0.0f;

	g_Player.u = 0.0f;
	g_Player.v = 0.0f;
	g_Player.uh = 0.0f;
	g_Player.vh = 0.0f;

	gravity_timer = 0.0f;
	g_Player.difference.x = 0.0f;
	g_Player.difference.y = 0.0f;

	g_Player.penalty = false;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	ZeroGravity();
	ControlPlayer();
	//FramePlayer();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	DrawSpriteColor(g_PlayerTexture,
		SCREEN_WIDTH / 2 + g_Player.difference.x,
		SCREEN_HEIGHT / 2 + g_Player.difference.y + g_DrawGrav,
		g_Player.size.x, g_Player.size.y,
		g_Player.u, g_Player.v,
		g_Player.uh, g_Player.vh,
		col);
}

void ControlPlayer(void)
{
	if (g_Player.penalty)
	{
		g_Player.pos.x += g_Player.vel.x / 2;
		g_Player.pos.y += g_Player.vel.y / 2;

		g_Player.penalty = false;
	}
	else
	{
		g_Player.pos.x += g_Player.vel.x;
		g_Player.pos.y += g_Player.vel.y;
	}

	// 横の移動量の方が大きいとき
	if (fabsf(g_Player.vel.x) > fabsf(g_Player.vel.y))
	{
		// 左に行くとき
		if (g_Player.vel.x < 0)
		{
			g_Player.u = 0.0f;
			g_Player.v = 0.5f;
			g_Player.uh = 0.5f;
			g_Player.vh = 0.5f;
		}
		else // 右に行くとき
		{
			g_Player.u = 0.5f;
			g_Player.v = 0.5f;
			g_Player.uh = 0.5f;
			g_Player.vh = 0.5f;
		}
	}
	else // 縦の移動量の方が大きい or 同じとき
	{
		// 上に行くとき
		if (g_Player.vel.y < 0)
		{
			g_Player.u = 0.5f;
			g_Player.v = 0.0f;
			g_Player.uh = 0.5f;
			g_Player.vh = 0.5f;
		}
		else // 下に行くとき
		{
			g_Player.u = 0.0f;
			g_Player.v = 0.0f;
			g_Player.uh = 0.5f;
			g_Player.vh = 0.5f;
		}
	}

	// マップ１の当たり判定
	if (GetScene() == SCENE_GAME1)
	{
		// X軸の当たり判定
		if (GetMapEnter(D3DXVECTOR2(g_Player.pos.x + g_Player.vel.x, g_Player.pos.y))
			== 1)
		{
			g_Player.pos.x -= g_Player.vel.x;
		}
		// Y軸の当たり判定
		if (GetMapEnter(D3DXVECTOR2(g_Player.pos.x, g_Player.pos.y + g_Player.vel.y))
			== 1)
		{
			g_Player.pos.y -= g_Player.vel.y;
		}
	}

	// マップ２の当たり判定
	if (GetScene() == SCENE_GAME2)
	{
		// X軸の当たり判定
		if (GetMapEnter2(D3DXVECTOR2(g_Player.pos.x + g_Player.vel.x, g_Player.pos.y))
			== 1)
		{
			g_Player.pos.x -= g_Player.vel.x;
		}
		// Y軸の当たり判定
		if (GetMapEnter2(D3DXVECTOR2(g_Player.pos.x, g_Player.pos.y + g_Player.vel.y))
			== 1)
		{
			g_Player.pos.y -= g_Player.vel.y;
		}
	}

	//左(移動)
	if (Keyboard_IsKeyDown(KK_LEFT) // キーボード
		|| GetThumbLeftX(0) < -INPUT_ZONE) // パッド
	{
		g_Player.vel.x -= PLAYER_ACCELE;

		// X速度の上限値
		if (g_Player.vel.x < -PLAYER_MAX_SPD)
		{
			g_Player.vel.x = -PLAYER_MAX_SPD;
		}

	}

	//右(移動)
	if (Keyboard_IsKeyDown(KK_RIGHT) // キーボード
		|| GetThumbLeftX(0) > INPUT_ZONE) // パッド
	{
		g_Player.vel.x += PLAYER_ACCELE;

		// X速度の上限値
		if (g_Player.vel.x > PLAYER_MAX_SPD)
		{
			g_Player.vel.x = PLAYER_MAX_SPD;
		}
	}

	//上(移動)
	if (Keyboard_IsKeyDown(KK_UP) // キーボード
		|| GetThumbLeftY(0) > INPUT_ZONE) // パッド
	{
		g_Player.vel.y -= PLAYER_ACCELE;

		// y速度の上限値
		if (g_Player.vel.y < -PLAYER_MAX_SPD)
		{
			g_Player.vel.y= -PLAYER_MAX_SPD;
		}
	}
	
	//下(移動)
	if (Keyboard_IsKeyDown(KK_DOWN) // キーボード
		|| GetThumbLeftY(0) < -INPUT_ZONE) // パッド
	{
		g_Player.vel.y += PLAYER_ACCELE;

		// y速度の上限値
		if (g_Player.vel.y > PLAYER_MAX_SPD)
		{
			g_Player.vel.y = PLAYER_MAX_SPD;
		}
	}
}

void ZeroGravity(void)
{
	if (gravity_timer < 20.0f)
	{
		g_DrawGrav -= GRAVITY_VEL;
		gravity_timer++;
	}
	if (gravity_timer >= 20.0f && gravity_timer < 40.0f)
	{
		g_DrawGrav += GRAVITY_VEL;
		gravity_timer++;
	}
	if (gravity_timer >= 40.0f && gravity_timer < 60.0f)
	{
		g_DrawGrav += GRAVITY_VEL;
		gravity_timer++;
	}
	if (gravity_timer >= 60.0f && gravity_timer < 80.0f)
	{
		g_DrawGrav -= GRAVITY_VEL;
		gravity_timer++;
	}

	if (gravity_timer >= 80.0f)
	{
		gravity_timer = 0.0f;
	}
}

void FramePlayer(void)
{
	//左(枠)
	if (g_Player.pos.x <= 0.0f + (g_Player.size.x / 2))
	{
		g_Player.pos.x = 0.0f + (g_Player.size.x / 2);
	}
	//右(枠)
	if (g_Player.pos.x >= (MAP_X * 60.0f) - (g_Player.size.x / 2))
	{
		g_Player.pos.x = (MAP_X * 60.0f) - (g_Player.size.x / 2);
	}

	//上(枠)
	if (g_Player.pos.y <= 0.0f + (g_Player.size.y / 2))
	{
		g_Player.pos.y = 0.0f + (g_Player.size.y / 2);
	}
	//下(枠)
	if (g_Player.pos.y >= (MAP_Y * 60.0f) - (g_Player.size.y / 2))
	{
		g_Player.pos.y = (MAP_Y * 60.0f) - (g_Player.size.y / 2);
	}
}

PLAYER* GetPlayer(void)
{
	return &g_Player;
}

D3DXVECTOR2 GetPlayerPosition(void)
{
	return g_Player.pos;
}