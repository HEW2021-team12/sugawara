//=============================================================================
//
// エネミー処理 [enemy.cpp]
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
// マクロ定義
//*****************************************************************************
#define ENEMY_SPD			(1.4f)
#define ENEMY_SIZE			(55.0f)
#define SERCH_RANGE			(220.0f)
#define FIND_SPD			(2.0f)
#define NONE_MOVE			(150.0f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

void SetEnemy(int i);
void WatchEnemy(int i);
void NoneWatchEnemy(int i);
bool SerchPlayer(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
unsigned char	g_EnemyTexture = 0;
unsigned char	g_watch_SE = 0;
unsigned char	g_watch_lost_SE = 0;

// メモリ確保用アドレス
ENEMY* g_Enemy;		  // エネミー構造体
char* g_RouteX;		  // 巡回ルート用
char* g_RouteY;		  // 巡回ルート用

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	g_EnemyTexture = LoadTexture("data/TEXTURE/enemy.png");
	g_watch_SE = LoadSound("data/SE/watch1.wav");
	g_watch_lost_SE = LoadSound("data/SE/enemy_worp.wav");

	// メモリ確保
	g_Enemy = new ENEMY[ENEMY_MAX];

	g_RouteX = new char[ROOT_MAX];
	g_RouteY = new char[ROOT_MAX];
	
	// エネミー構造体の初期化
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		// 一体目
		if (i == 0)
		{
			g_Enemy[i].use = true;
			g_Enemy[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 5;
			g_Enemy[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 16;
			g_Enemy[i].routenom = 3;
		}
		// 二体目
		if (i == 1)
		{
			g_Enemy[i].use = false;
			g_Enemy[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 3;
			g_Enemy[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 9;
			g_Enemy[i].routenom = 0;
		}
		// 三体目
		if (i == 2)
		{
			g_Enemy[i].use = false;
			g_Enemy[i].pos.x = (CHIP_SIZE / 2) + CHIP_SIZE * 23;
			g_Enemy[i].pos.y = (CHIP_SIZE / 2) + CHIP_SIZE * 7;
			g_Enemy[i].routenom = 8;
		}

		// 共通
		g_Enemy[i].watch = false;
		g_Enemy[i].w = ENEMY_SIZE;
		g_Enemy[i].h = ENEMY_SIZE;
		g_Enemy[i].vel = D3DXVECTOR2(ENEMY_SPD, ENEMY_SPD);
		g_Enemy[i].notmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy[i].addmove = D3DXVECTOR2(0.0f, 0.0f);
		g_Enemy[i].movecntX = false;
		g_Enemy[i].movecntY = false;

		// エネミーUV
		g_Enemy[i].u = 0.0f;
		g_Enemy[i].v = 0.0f;
		g_Enemy[i].uh = 0.5f;
		g_Enemy[i].vh = 0.5f;

	}

	// 巡回ルート設定
	for (char r = 0; r < ROOT_MAX; r++)
	{
		g_RouteX[r] = SetRouteX(r);
		g_RouteY[r] = SetRouteY(r);
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	// メモリ解放
	delete[] g_Enemy;
	delete[] g_RouteX;
	delete[] g_RouteY;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == true)
		{
			// 追跡判定
			if (SerchPlayer(GetPlayerPosition(), g_Enemy[i].pos))
			{// 見つけた(範囲内に入った)

				if (!g_Enemy[i].watch)
				{
					PlaySound(g_watch_SE, 0);
					g_Enemy[i].watch = true;
				}

				WatchEnemy(i);
				
			}
			// 見つけていない
			else
			{
				if (g_Enemy[i].watch)
				{
					// 見失った時の処理（SE）
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
	// エネミー追加
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
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (g_Enemy[i].use == true)
		{
			PLAYER* player = GetPlayer();

			// 画面内に入っているか
			if (fabsf(g_Enemy[i].pos.x - player->pos.x) < (VIEW_SCREEN_WIDTH * CHIP_SIZE) &&
				fabsf(g_Enemy[i].pos.y - player->pos.y) < (VIEW_SCREEN_HEIGHT * CHIP_SIZE))
			{
				//エネミーの位置やテクスチャー座標を反映
				float px = SCREEN_WIDTH / 2 + player->difference.x + (g_Enemy[i].pos.x - player->pos.x);	// エネミーの表示位置X
				float py = SCREEN_HEIGHT / 2 + player->difference.y + (g_Enemy[i].pos.y - player->pos.y);	// エネミーの表示位置Y
				float pw = g_Enemy[i].w;		// エネミーの表示幅
				float ph = g_Enemy[i].h;		// エネミーの表示高さ
				D3DXCOLOR col;

				if (SerchPlayer(GetPlayerPosition(), g_Enemy[i].pos))
				{
					col = D3DXCOLOR(0.2f, 1.0f, 1.0f, 1.0f);
				}
				else
				{
					col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				// １枚のポリゴンの頂点とテクスチャ座標を設定
				DrawSpriteColor(g_EnemyTexture, px, py, pw, ph,
					g_Enemy[i].u, g_Enemy[i].v,
					g_Enemy[i].uh, g_Enemy[i].vh,
					col);
			}
		}
	}
}

//=============================================================================
// 敵の発生処理
//=============================================================================
void SetEnemy(int i)
{
	// もし未使用のデータを探す
	if (g_Enemy[i].use == false)	// 未使用状態の敵データを見つける
	{
		g_Enemy[i].use = true;		// 使用状態へ変更する

		// 出現したときのSE

		return;						// 敵をセットできたので終了する
	}
}

// 見つけたら
void WatchEnemy(int i)
{
	// 旧版サーチ
	{
		//現在位置
		D3DXVECTOR2 position = g_Enemy[i].pos;

		//目標地点
		D3DXVECTOR2 destination = GetPlayerPosition();

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
				g_Enemy[i].u = 1.0f;
				g_Enemy[i].v = 0.0f;
				g_Enemy[i].uh = -0.5f;
				g_Enemy[i].vh = 0.5f;
			}
			else // 右に行くとき
			{
				g_Enemy[i].u = 0.5f;
				g_Enemy[i].v = 0.0f;
				g_Enemy[i].uh = 0.5f;
				g_Enemy[i].vh = 0.5f;
			}
		}
		else // 縦の移動量の方が大きい or 同じとき
		{
			// 上に行くとき
			if (direction.y < 0)
			{
				g_Enemy[i].u = 0.5f;
				g_Enemy[i].v = 0.5f;
				g_Enemy[i].uh = 0.5f;
				g_Enemy[i].vh = 0.5f;
			}
			else // 下に行くとき
			{
				g_Enemy[i].u = 0.0f;
				g_Enemy[i].v = 0.5f;
				g_Enemy[i].uh = 0.5f;
				g_Enemy[i].vh = 0.5f;
			}
		}

		//移動後の位置を計算
		g_Enemy[i].pos.x += direction.x * FIND_SPD;
		if (GetMapEnter(D3DXVECTOR2(g_Enemy[i].pos.x + direction.x * FIND_SPD, g_Enemy[i].pos.y))
			== 1)
		{
			g_Enemy[i].pos.x -= direction.x * FIND_SPD;
			g_Enemy[i].notmove.x += fabsf(direction.x);

			if (!g_Enemy[i].movecntX)
			{
				// 一定時間動けない（障害物に引っかかる）
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
				// 一定時間動けない（障害物に引っかかる）
				if (g_Enemy[i].notmove.y > NONE_MOVE)
				{
					g_Enemy[i].movecntY = true;
					g_Enemy[i].notmove.y = 0.0f;
				}
			}

		}

		// 一定時間動けない（障害物に引っかかる）
		if (g_Enemy[i].movecntX)
		{
			g_Enemy[i].movecntX = false;

			// 見失った時の処理（SE）
			PlaySound(g_watch_lost_SE, 0);

			char work = rand() % ROOT_MAX;

			g_Enemy[i].routenom = work;
			g_Enemy[i].pos.x = SetRouteX(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy[i].pos.y = SetRouteY(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}
		if (g_Enemy[i].movecntY)
		{
			g_Enemy[i].movecntY = false;

			// 見失った時の処理（SE）
			PlaySound(g_watch_lost_SE, 0);

			char work = rand() % ROOT_MAX;

			g_Enemy[i].routenom = work;
			g_Enemy[i].pos.x = SetRouteX(work) * CHIP_SIZE + (CHIP_SIZE / 2);
			g_Enemy[i].pos.y = SetRouteY(work) * CHIP_SIZE + (CHIP_SIZE / 2);
		}

	}

}

// 見つけていなかったら
void NoneWatchEnemy(int i)
{
	{
		//現在位置
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
					// 偶数 or 0
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
					// 奇数
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

		//目標地点
		D3DXVECTOR2 destination = D3DXVECTOR2(g_RouteX[work] * CHIP_SIZE + (CHIP_SIZE / 2),
			g_RouteY[work] * CHIP_SIZE + (CHIP_SIZE / 2));

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
				g_Enemy[i].u = 1.0f;
				g_Enemy[i].v = 0.0f;
				g_Enemy[i].uh = -0.5f;
				g_Enemy[i].vh = 0.5f;
			}
			else // 右に行くとき
			{
				g_Enemy[i].u = 0.5f;
				g_Enemy[i].v = 0.0f;
				g_Enemy[i].uh = 0.5f;
				g_Enemy[i].vh = 0.5f;
			}
		}
		else // 縦の移動量の方が大きい or 同じとき
		{
			// 上に行くとき
			if (direction.y < 0)
			{
				g_Enemy[i].u = 0.5f;
				g_Enemy[i].v = 0.5f;
				g_Enemy[i].uh = 0.5f;
				g_Enemy[i].vh = 0.5f;
			}
			else // 下に行くとき
			{
				g_Enemy[i].u = 0.0f;
				g_Enemy[i].v = 0.5f;
				g_Enemy[i].uh = 0.5f;
				g_Enemy[i].vh = 0.5f;
			}
		}

		//移動後の位置を計算
		g_Enemy[i].pos.x += direction.x * ENEMY_SPD;
		g_Enemy[i].pos.y += direction.y * ENEMY_SPD;
		
	}
}

// 範囲内にプレイヤーが入り込んだら
bool SerchPlayer(D3DXVECTOR2 Playerpos, D3DXVECTOR2 Enemypos)
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
// エネミー構造体の先頭アドレスを取得
//=============================================================================
ENEMY* GetEnemy(void)
{
	return &g_Enemy[0];
}