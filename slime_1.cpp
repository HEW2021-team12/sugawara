//=============================================================================
//
// スライム処理 [slime.cpp]
// Author : 古市
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
// マクロ定義
//*****************************************************************************

#define SLIME_SPD			(1.1f)
#define SLIME_SIZE			(50.0f)

#define BABBLE_SIZE			(40.0f)
#define BABBLE_COUNT		(300)

#define SET_SPD				(150)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void SetBabble(int i, D3DXVECTOR2 pos);
void MoveSlime(int i);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
unsigned char	g_SlimeTexture = 0;
unsigned char	g_BabbleTexture = 0;
//unsigned char	g_Slime_SE = 0;

// メモリ確保用アドレス
SLIME*  g_Slime;			  // スライム構造体
BABBLE** g_Babble;			  // バブル構造体
char* g_RouteSlimeX;		  // 巡回ルート用
char* g_RouteSlimeY;		  // 巡回ルート用

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSlime(void)
{
	g_SlimeTexture = LoadTexture("data/TEXTURE/slime.png");
	g_BabbleTexture = LoadTexture("data/TEXTURE/tako.png");

	// メモリ確保
	g_Slime = new SLIME[SLIME_MAX];
	g_Babble = new BABBLE* [SLIME_MAX];

	for (int i = 0; i < SLIME_MAX; i++)
	{
		g_Babble[i] = new BABBLE[BABBLE_MAX];
	}
	
	g_RouteSlimeX = new char[ROOT_MAX];
	g_RouteSlimeY = new char[ROOT_MAX];

	// スライム構造体の初期化
	for (int i = 0; i < SLIME_MAX; i++)
	{
		// １体目
		if (i == 0)
		{
			g_Slime[i].use = true;
			g_Slime[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime[i].routenom = 5;
		}

		// 共通
		g_Slime[i].h = SLIME_SIZE;
		g_Slime[i].w = SLIME_SIZE;
		g_Slime[i].vel = D3DXVECTOR2(SLIME_SPD,SLIME_SPD);

		// スライムUV
		g_Slime[i].u = 0.0f;
		g_Slime[i].v = 0.0f;
		g_Slime[i].uh = 1.0f;
		g_Slime[i].vh = 1.0f;
	}

	// 巡回ルート設定
	for (char r = 0; r < ROOT_MAX; r++)
	{
		g_RouteSlimeX[r] = SetRouteX(r);
		g_RouteSlimeY[r] = SetRouteY(r);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSlime(void)
{
	// メモリ解放
	delete[] g_Slime;
	delete[] g_Babble;
	delete[] g_RouteSlimeX;
	delete[] g_RouteSlimeY;
}
//=============================================================================
// 更新処理
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
// 描画処理
//=============================================================================
void DrawSlime(void)
{
	// スライム
	for (int i = 0; i < SLIME_MAX; i++)
	{
		// バブル
		for (int j = 0; j < BABBLE_MAX; j++)
		{
			if (g_Babble[i][j].use)
			{
				PLAYER* player = GetPlayer();

				// 画面内に入っているか
				if (fabsf(g_Babble[i][j].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
					fabsf(g_Babble[i][j].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
				{
					//エネミーの位置やテクスチャー座標を反映
					float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Babble[i][j].pos.x - player->pos.x);	// エネミーの表示位置X
					float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Babble[i][j].pos.y - player->pos.y);	// エネミーの表示位置Y
					float pw = g_Babble[i][j].w;		// エネミーの表示幅
					float ph = g_Babble[i][j].h;		// エネミーの表示高さ
					D3DXCOLOR col;

					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					DrawSpriteColor(g_BabbleTexture, px, py, pw, ph,
						g_Babble[i][j].u, g_Babble[i][j].v,
						g_Babble[i][j].uh, g_Babble[i][j].vh,
						col);
				}
			}
		}

		// スライム
		if (g_Slime[i].use)
		{
			PLAYER* player = GetPlayer();

			// 画面内に入っているか
			if (fabsf(g_Slime[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Slime[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//エネミーの位置やテクスチャー座標を反映
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Slime[i].pos.x - player->pos.x);	// エネミーの表示位置X
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Slime[i].pos.y - player->pos.y);	// エネミーの表示位置Y
				float pw = g_Slime[i].w;		// エネミーの表示幅
				float ph = g_Slime[i].h;		// エネミーの表示高さ
				D3DXCOLOR col;

				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				DrawSpriteColor(g_SlimeTexture, px, py, pw, ph,
					g_Slime[i].u, g_Slime[i].v,
					g_Slime[i].uh, g_Slime[i].vh,
					col);
			}
		}
	}
}

//=============================================================================
// スライム構造体の先頭アドレスを取得
//=============================================================================
SLIME* GetSlime(void)
{
	return g_Slime;
}

BABBLE** GetBabble(void)
{
	return g_Babble;
}

// バブルを発生させる
void SetBabble(int i,D3DXVECTOR2 pos)
{
	for (int j = 0; j < BABBLE_MAX; j++)
	{
		// もし未使用のデータを探す
		if (g_Babble[i][j].use == false)	// 未使用状態の敵データを見つける
		{
			g_Babble[i][j].use = true;		// 使用状態へ変更する
			g_Babble[i][j].pos = pos;
			g_Babble[i][j].w = BABBLE_SIZE;
			g_Babble[i][j].h = BABBLE_SIZE;

			g_Babble[i][j].count = BABBLE_COUNT;

			g_Babble[i][j].u = 0.0f;
			g_Babble[i][j].v = 0.0f;
			g_Babble[i][j].uh = 1.0f;
			g_Babble[i][j].vh = 1.0f;

			// 出現したときのSE

			return;						// 敵をセットできたので終了する
		}
	}
}

// スライムの移動
void MoveSlime(int i)
{
	//現在位置
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
				// 偶数 or 0
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
				// 奇数
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

	//目標地点
	D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteSlimeX[work] * CHIP_SIZE + (CHIP_SIZE / 2),
		g_RouteSlimeY[work] * CHIP_SIZE + (CHIP_SIZE / 2));

	//成分
	D3DXVECTOR2 components;

	//大きさ
	float magnitude;

	//方向（単位ベクトル）
	D3DXVECTOR2 direction;

	//成分計算
	components.x = destination.x - position.x;
	components.y = destination.y - position.y;

	//大きさ計算
	magnitude = (float)sqrt(components.x * components.x + components.y * components.y);

	//方向計算（正規化）
	direction.x = components.x / magnitude;
	direction.y = components.y / magnitude;

	//移動後の位置を計算
	g_Slime[i].pos.x += direction.x * g_Slime[i].vel.x;
	g_Slime[i].pos.y += direction.y * g_Slime[i].vel.y;
}