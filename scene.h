#pragma once

// 登録シーン一覧
enum SCENE
{
	SCENE_NONE,		//シーン処理なし
	SCENE_TITLE,	//タイトル
	SCENE_SELECT,   //ステージセレクト
	SCENE_GAME1,	//ゲーム
	SCENE_GAME2,
	SCENE_GAME3,
	SCENE_GAME4,
	SCENE_GAME5,

	// 現在は5ステージ予定

	SCENE_RESULT,	//リザルト
	SCENE_LOSE,     //GAME OVER

	SCENE_MAX		//最後だとわかる奴をいれる
};

void InitScene(SCENE index);
void UninitScene(void);
void UpdateScene(void);
void DrawScene(void);

void SetScene(SCENE index);
void CheckScene(void);
SCENE GetScene(void);