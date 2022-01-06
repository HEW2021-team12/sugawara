//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 
//
//=============================================================================
#pragma once

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

// ステージ１
#define SLIME_MAX		(1)

// ステージ２
#define SLIME_MAX2		(1)

// ステージ３
#define SLIME_MAX		(1)

// ステージ４
#define SLIME_MAX		(1)

// ステージ５
#define SLIME_MAX		(1)


// ステージ通して固定
#define BABBLE_MAX		(3)

// スライム構造体
struct SLIME
{
	bool					use;		 // true:使っている  false:未使用
	float					w, h;		 // 幅と高さ
	D3DXVECTOR2				pos;		 // ポリゴンの座標
	D3DXVECTOR2				vel;		 // 移動量
	D3DXVECTOR2				notmove;	 // 動けていない時用
	D3DXVECTOR2				addmove;	 // 動けていない時の追加移動
	bool					movecntX, movecntY;
	float					u;
	float					v;
	float					uh;
	float					vh;
	char					routenom;	// 巡回ルート用
	unsigned char			setcount;
};

struct BABBLE
{
	bool					use;		 // true:使っている  false:未使用
	float					w, h;		 // 幅と高さ
	D3DXVECTOR2				pos;		 // ポリゴンの座標
	float					u;
	float					v;
	float					uh;
	float					vh;
	int						count;
};

// ステージ１
HRESULT InitSlime(void);
void UninitSlime(void);
void UpdateSlime(void);
void DrawSlime(void);
SLIME* GetSlime(void);
BABBLE** GetBabble(void);

// ステージ2
HRESULT InitSlime2(void);
void UninitSlime2(void);
void UpdateSlime2(void);
void DrawSlime2(void);
SLIME* GetSlime2(void);
BABBLE** GetBabble2(void);
