//=============================================================================
//
// セレクト画面管理 [select_scene.cpp]
// Author : 歐
//
//=============================================================================

#include "select_scene.h"
#include "input.h"
#include "texture.h"
#include "sprite.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define		uv_size_x	(0.077f)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
SELECT_SCENE select_scene;

static int	g_TextureNo = 0;	// テクスチャ情報
static int	g_TextureNo_2 = 0;	
static int	g_TextureNo_3 = 0;
int select_num;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSelect_Scene(void)
{
	g_TextureNo = LoadTexture("data/TEXTURE/back.jpg");
	g_TextureNo_2 = LoadTexture("data/TEXTURE/select_scene.png");
	g_TextureNo_3 = LoadTexture("data/TEXTURE/yajirushi.png");

	select_scene.pos[0].x = (SCREEN_WIDTH / 7);
	select_scene.pos[1].x = select_scene.pos[0].x * 2;
	select_scene.pos[2].x = select_scene.pos[0].x * 3;
	select_scene.pos[3].x = select_scene.pos[0].x * 4;
	select_scene.pos[4].x = select_scene.pos[0].x;
	select_scene.pos[5].x = select_scene.pos[0].x * 2;
	select_scene.pos[6].x = select_scene.pos[0].x * 3;

	select_scene.pos[0].y = SCREEN_HEIGHT / 2 - 40.0f;
	select_scene.pos[1].y = select_scene.pos[0].y;
	select_scene.pos[2].y = select_scene.pos[0].y;
	select_scene.pos[3].y = select_scene.pos[0].y;
	select_scene.pos[4].y = SCREEN_HEIGHT / 2 + 40.0f;
	select_scene.pos[5].y = select_scene.pos[4].y;
	select_scene.pos[6].y = select_scene.pos[4].y;


	select_scene.size.x = 32.0f;
	select_scene.size.y = 64.0f;

	select_num = 0;
	
	select_scene.yajirushi_size.x = 100.0f;
	select_scene.yajirushi_size.y = 50.0f;

	select_scene.yajirushi_pos.x = select_scene.pos[select_num].x - select_scene.size.x / 2 - select_scene.yajirushi_size.x / 2;
	select_scene.yajirushi_pos.y = select_scene.pos[select_num].y;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSelect_Scene(void)
{
	StopSoundAll();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateSelect_Scene(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) && GetFadeState() == FADE_NONE && select_num == 0)
	{
		SceneTransition(SCENE_GAME);
	}

	if (GetKeyboardTrigger(DIK_RETURN) && GetFadeState() == FADE_NONE && select_num == 1)
	{
		SceneTransition(SCENE_GAME2);
	}

	if (GetKeyboardTrigger(DIK_RETURN) && GetFadeState() == FADE_NONE && select_num != 0)
	{
		SceneTransition(SCENE_TITLE);
	}

	ControlSelect();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSelect_Scene(void)
{
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	DrawSpriteLeftTop(g_TextureNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	//1
	DrawSpriteColor(g_TextureNo_2, select_scene.pos[0].x, select_scene.pos[0].y, select_scene.size.x, select_scene.size.y, 0.0f, 0.0f, uv_size_x, 1.0f, col);

	//2
	DrawSpriteColor(g_TextureNo_2, select_scene.pos[1].x, select_scene.pos[1].y, select_scene.size.x, select_scene.size.y, uv_size_x, 0.0f, uv_size_x, 1.0f, col);

	//3
	DrawSpriteColor(g_TextureNo_2, select_scene.pos[2].x, select_scene.pos[2].y, select_scene.size.x, select_scene.size.y, uv_size_x * 2, 0.0f, uv_size_x, 1.0f, col);

	//4
	DrawSpriteColor(g_TextureNo_2, select_scene.pos[3].x, select_scene.pos[3].y, select_scene.size.x, select_scene.size.y, uv_size_x * 3, 0.0f, uv_size_x, 1.0f, col);

	//5
	DrawSpriteColor(g_TextureNo_2, select_scene.pos[4].x, select_scene.pos[4].y, select_scene.size.x, select_scene.size.y, uv_size_x * 4, 0.0f, uv_size_x, 1.0f, col);

	//6
	DrawSpriteColor(g_TextureNo_2, select_scene.pos[5].x, select_scene.pos[5].y, select_scene.size.x, select_scene.size.y, uv_size_x * 5, 0.0f, uv_size_x, 1.0f, col);
																									   							  
	//7																								  							  
	DrawSpriteColor(g_TextureNo_2, select_scene.pos[6].x, select_scene.pos[6].y, select_scene.size.x, select_scene.size.y, uv_size_x * 6, 0.0f, uv_size_x, 1.0f, col);
	
	//矢印
	DrawSpriteColor(g_TextureNo_3, select_scene.yajirushi_pos.x, select_scene.yajirushi_pos.y, select_scene.yajirushi_size.x, select_scene.yajirushi_size.y, 0.0f,0.0f,1.0f, 1.0f, col);
}

void ControlSelect(void)
{
	//左
	if (GetKeyboardTrigger(DIK_LEFT)
		|| IsButtonTriggered(0, BUTTON_LEFT))
	{
		select_num--;
	}

	//右
	if (GetKeyboardTrigger(DIK_RIGHT)
		|| IsButtonTriggered(0, BUTTON_RIGHT))
	{
		select_num++;
	}

	if (select_num != 3)
	{
		if (select_num != 0 && select_num != 1 && select_num != 2)
		{
			//上
			if (GetKeyboardTrigger(DIK_UP)
				|| IsButtonTriggered(0, BUTTON_LEFT))
			{
				select_num -= 4;
			}
		}
		

		if (select_num != 4 && select_num != 5 && select_num != 6)
		{
			//下
			if (GetKeyboardTrigger(DIK_DOWN)
				|| IsButtonTriggered(0, BUTTON_RIGHT))
			{
				select_num += 4;
			}
		}

	}


	if (select_num < 0)
	{
		select_num = 0;
	}

	if (select_num > 6)
	{
		select_num = 6;
	}


	select_scene.yajirushi_pos.x = select_scene.pos[select_num].x - select_scene.size.x / 2 - select_scene.yajirushi_size.x / 2;
	select_scene.yajirushi_pos.y = select_scene.pos[select_num].y;
}