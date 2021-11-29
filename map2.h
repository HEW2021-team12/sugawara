/*==============================================================================

   マップ管理[map.h]
														 Author :	歐 改造 古市
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#ifndef MAP2_H_
#define MAP2_H_

#include "main.h"
#include "renderer.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//↓は絶対にいじらないこと！
#define VIEW_SCREEN_WIDTH	(16.0f)		// 表示範囲(横)
#define VIEW_SCREEN_HEIGHT	(9.0f)		// 表示範囲(縦)

#define CHIP_SIZE			(60.0f)		// マスの大きさ

// sugawara map
#define MAP_X				(28)		// マップの横のマス数
#define MAP_Y				(19)		// マップの縦のマス数

// それ以降のマップ


//マップ構造体
typedef struct
{
	float		u = 0.0f;
	float		v = 0.0f;
	float		uh = 0.0f;
	float		vh = 0.0f;
}MAP;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMap2	(void);
void UninitMap2	(void);
void UpdateMap2	(void);
void DrawMap2	(void);
void ChangeUv2   (void);

int GetMapEnter(D3DXVECTOR2 pos, D3DXVECTOR2 size);
int GetMap2(int x, int y);
#endif //MAP2_H_
