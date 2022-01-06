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
#define SLIME_SIZEX			(70.0f)
#define SLIME_SIZEY			(50.0f)

#define BABBLE_SIZE			(40.0f)
#define BABBLE_COUNT		(300)

#define SET_SPD				(150)
#define SERCH_RANGE			(220.0f)
#define FIND_SPD			(2.0f)
#define NONE_MOVE			(150.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void SetBabble2(int i, D3DXVECTOR2 pos);
void MoveSlime2(int i);
void NoneWatchSlime2(int i);
bool SerchPlayerSlime2(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
unsigned char	g_SlimeTexture2 = 0;
unsigned char	g_watch_SE22 = 0;
unsigned char	g_watch_lost_SE22 = 0;


// メモリ確保用アドレス
SLIME* g_Slime2;			  // スライム構造体
BABBLE** g_Babble2;			  // バブル構造体
char* g_RouteSlimeX2;		  // 巡回ルート用
char* g_RouteSlimeY2;		  // 巡回ルート用

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSlime2(void)
{
	g_SlimeTexture2 = LoadTexture("data/TEXTURE/slime1.png");

	// メモリ確保
	g_Slime2 = new SLIME[SLIME_MAX2];
	g_Babble2 = new BABBLE * [SLIME_MAX2];

	for (int i = 0; i < SLIME_MAX; i++)
	{
		g_Babble2[i] = new BABBLE[BABBLE_MAX];
	}

	g_RouteSlimeX2 = new char[ROOT_MAX2_1];
	g_RouteSlimeY2 = new char[ROOT_MAX2_1];

	// スライム構造体の初期化
	for (int i = 0; i < SLIME_MAX2; i++)
	{
		// １体目
		if (i == 0)
		{
			g_Slime2[i].use = true;
			g_Slime2[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime2[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Slime2[i].routenom = 5;
		}

		// 共通
		g_Slime2[i].h = SLIME_SIZEY;
		g_Slime2[i].w = SLIME_SIZEX;
		g_Slime2[i].vel = D3DXVECTOR2(SLIME_SPD, SLIME_SPD);

		// スライムUV
		g_Slime2[i].u = 0.0f;
		g_Slime2[i].v = 0.0f;
		g_Slime2[i].uh = 5.0f;
		g_Slime2[i].vh = 2.0f;
	}

	// 巡回ルート設定
	for (char r = 0; r < ROOT_MAX2_1; r++)
	{
		g_RouteSlimeX2[r] = SetRouteX2(r);
		g_RouteSlimeY2[r] = SetRouteY2(r);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSlime2(void)
{
	// メモリ解放
	delete[] g_Slime2;
	delete[] g_Babble2;
	delete[] g_RouteSlimeX2;
	delete[] g_RouteSlimeY2;
}
//=============================================================================
// 更新処理
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
// 描画処理
//=============================================================================
void DrawSlime2(void)
{
	// スライム
	for (int i = 0; i < SLIME_MAX2; i++)
	{
		// バブル
		for (int j = 0; j < BABBLE_MAX; j++)
		{
			if (g_Babble2[i][j].use)
			{
				PLAYER* player = GetPlayer();

				// 画面内に入っているか
				if (fabsf(g_Babble2[i][j].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
					fabsf(g_Babble2[i][j].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
				{
					//エネミーの位置やテクスチャー座標を反映
					float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Babble2[i][j].pos.x - player->pos.x);	// エネミーの表示位置X
					float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Babble2[i][j].pos.y - player->pos.y);	// エネミーの表示位置Y
					float pw = g_Babble2[i][j].w;		// エネミーの表示幅
					float ph = g_Babble2[i][j].h;		// エネミーの表示高さ
					D3DXCOLOR col;

					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// １枚のポリゴンの頂点とテクスチャ座標を設定
					DrawSpriteColor(g_SlimeTexture2, px, py, pw, ph,
						g_Babble2[i][j].u, g_Babble2[i][j].v,
						g_Babble2[i][j].uh, g_Babble2[i][j].vh,
						col);
				}
			}
		}

		// スライム
		if (g_Slime2[i].use)
		{
			PLAYER* player = GetPlayer();

			// 画面内に入っているか
			if (fabsf(g_Slime2[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Slime2[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//エネミーの位置やテクスチャー座標を反映
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Slime2[i].pos.x - player->pos.x);	// エネミーの表示位置X
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Slime2[i].pos.y - player->pos.y);	// エネミーの表示位置Y
				float pw = g_Slime2[i].w;		// エネミーの表示幅
				float ph = g_Slime2[i].h;		// エネミーの表示高さ
				D3DXCOLOR col;

				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// １枚のポリゴンの頂点とテクスチャ座標を設定
				DrawSpriteColor(g_SlimeTexture2, px, py, pw, ph,
					g_Slime2[i].u, g_Slime2[i].v,
					g_Slime2[i].uh, g_Slime2[i].vh,
					col);
			}
		}
	}
}



// バブルを発生させる
void SetBabble2(int i, D3DXVECTOR2 pos)
{
	for (int j = 0; j < BABBLE_MAX; j++)
	{
		// もし未使用のデータを探す
		if (g_Babble2[i][j].use == false)	// 未使用状態の敵データを見つける
		{
			g_Babble2[i][j].use = true;		// 使用状態へ変更する
			g_Babble2[i][j].pos = pos;
			g_Babble2[i][j].w = BABBLE_SIZE;
			g_Babble2[i][j].h = BABBLE_SIZE - 10.0f;

			g_Babble2[i][j].count = BABBLE_COUNT;

			g_Babble2[i][j].u = 0.5f;
			g_Babble2[i][j].v = 0.0f;
			g_Babble2[i][j].uh = 0.5f;
			g_Babble2[i][j].vh = 0.5f;

			// 出現したときのSE

			return;						// 敵をセットできたので終了する
		}
	}
}

// スライムの移動
void MoveSlime2(int i)
{
	//現在位置
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
				// 偶数 or 0
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
				// 奇数
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

	//目標地点
	D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteSlimeX2[work] * CHIP_SIZE + (CHIP_SIZE / 2),
		g_RouteSlimeY2[work] * CHIP_SIZE + (CHIP_SIZE / 2));

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

	// 横の移動量の方が大きいとき
	if (fabsf(direction.x) > fabsf(direction.y))
	{
		// 左に行くとき
		if (direction.x < 0)
		{
			g_Slime2[i].u = 0.0f;
			g_Slime2[i].v = 0.5f;
			g_Slime2[i].uh = 0.5f;
			g_Slime2[i].vh = 0.5f;
		}
		else // 右に行くとき
		{
			g_Slime2[i].u = 0.5f;
			g_Slime2[i].v = 0.5f;
			g_Slime2[i].uh = 0.5f;
			g_Slime2[i].vh = 0.5f;
		}
	}
	else // 縦の移動量の方が大きい or 同じとき
	{
		// 上に行くとき
		if (direction.y < 0)
		{
			g_Slime2[i].u = 0.5f;
			g_Slime2[i].v = 0.0f;
			g_Slime2[i].uh = 0.5f;
			g_Slime2[i].vh = 0.5f;
		}
		else // 下に行くとき
		{
			g_Slime2[i].u = 0.0f;
			g_Slime2[i].v = 0.0f;
			g_Slime2[i].uh = 0.5f;
			g_Slime2[i].vh = 0.5f;
		}
	}

	//移動後の位置を計算
	g_Slime2[i].pos.x += direction.x * FIND_SPD;
	if (GetMapEnter(D3DXVECTOR2(g_Slime2[i].pos.x + direction.x * FIND_SPD, g_Slime2[i].pos.y))
		== 1)
	{
		g_Slime2[i].pos.x -= direction.x * FIND_SPD;
		g_Slime2[i].notmove.x += fabsf(direction.x);

		if (!g_Slime2[i].movecntX)
		{
			// 一定時間動けない（障害物に引っかかる）
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
			// 一定時間動けない（障害物に引っかかる）
			if (g_Slime2[i].notmove.y > NONE_MOVE)
			{
				g_Slime2[i].movecntY = true;
				g_Slime2[i].notmove.y = 0.0f;
			}
		}

	}

	// 一定時間動けない（障害物に引っかかる）
	if (g_Slime2[i].movecntX)
	{
		g_Slime2[i].movecntX = false;

		// 見失った時の処理（SE）
		PlaySound(g_watch_lost_SE22, 0);

		char work = rand() % ROOT_MAX2_1;

		g_Slime2[i].routenom = work;
		g_Slime2[i].pos.x = SetRouteX2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		g_Slime2[i].pos.y = SetRouteY2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
	}
	if (g_Slime2[i].movecntY)
	{
		g_Slime2[i].movecntY = false;

		// 見失った時の処理（SE）
		PlaySound(g_watch_lost_SE22, 0);

		char work = rand() % ROOT_MAX2_1;

		g_Slime2[i].routenom = work;
		g_Slime2[i].pos.x = SetRouteX2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		g_Slime2[i].pos.y = SetRouteY2(work) * CHIP_SIZE + (CHIP_SIZE / 2);
	}

}



// 見つけていなかったら
void NoneWatchSlime2(int i)
{
	{
		//現在位置
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
					// 偶数 or 0
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
					// 奇数
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

		//目標地点
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteSlimeX2[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteSlimeX2[work] * CHIP_SIZE + (CHIP_SIZE / 2));

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

		// 横の移動量の方が大きいとき
		if (fabsf(direction.x) > fabsf(direction.y))
		{
			// 左に行くとき
			if (direction.x < 0)
			{
				g_Slime2[i].u = 1.0f;
				g_Slime2[i].v = 0.0f;
				g_Slime2[i].uh = -0.5f;
				g_Slime2[i].vh = 0.5f;
			}
			else // 右に行くとき
			{
				g_Slime2[i].u = 0.5f;
				g_Slime2[i].v = 0.0f;
				g_Slime2[i].uh = 0.5f;
				g_Slime2[i].vh = 0.5f;
			}
		}
		else // 縦の移動量の方が大きい or 同じとき
		{
			// 上に行くとき
			if (direction.y < 0)
			{
				g_Slime2[i].u = 0.5f;
				g_Slime2[i].v = 0.5f;
				g_Slime2[i].uh = 0.5f;
				g_Slime2[i].vh = 0.5f;
			}
			else // 下に行くとき
			{
				g_Slime2[i].u = 0.0f;
				g_Slime2[i].v = 0.5f;
				g_Slime2[i].uh = 0.5f;
				g_Slime2[i].vh = 0.5f;
			}
		}

		//移動後の位置を計算
		g_Slime2[i].pos.x += direction.x * SLIME_SPD;
		g_Slime2[i].pos.y += direction.y * SLIME_SPD;

	}
}

// 範囲内にプレイヤーが入り込んだら
bool SerchPlayerSlime2(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos)
{
	D3DXVECTOR2 maxRange, minRange;

	minRange.x = Enemypos.x - SERCH_RANGE;
	minRange.y = Enemypos.y - SERCH_RANGE;
	maxRange.x = Enemypos.x + SERCH_RANGE;
	maxRange.y = Enemypos.y + SERCH_RANGE;

	//X軸の判定
	if (minRange.x < Playerpos.x &&
		maxRange.x > Playerpos.x)
	{
		//Y軸の判定
		if (minRange.y < Playerpos.y &&
			maxRange.y > Playerpos.y)
		{
			//全ての条件がTRUEならヒット判定
			return true;
		}
	}

	return false;
}


//=============================================================================
// スライム構造体の先頭アドレスを取得
//=============================================================================
SLIME* GetSlime2(void)
{
	return g_Slime2;
}

BABBLE** GetBabble2(void)
{
	return g_Babble2;
}