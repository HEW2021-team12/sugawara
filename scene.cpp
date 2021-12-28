/*==============================================================================

   �V�[���Ǘ� [scene.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/

#include "scene.h"
#include "title.h"
#include "select_scene.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "sound.h"
#include "game_over.h"

/*------------------------------------------------------------------------------
   �萔��`
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   �\���̐錾
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   �v���g�^�C�v�錾
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
   �O���[�o���ϐ��̒�`
------------------------------------------------------------------------------*/
static SCENE g_SceneIndex = SCENE_NONE;
static SCENE g_SceneNextIndex = g_SceneIndex;


/*------------------------------------------------------------------------------
   �������֐�
------------------------------------------------------------------------------*/
void InitScene(SCENE index)
{
	g_SceneIndex = g_SceneNextIndex = index;

	switch (g_SceneIndex)
	{
	case SCENE_NONE:
		break;

	case SCENE_TITLE:
		InitTitle();
		break;

	case SCENE_SELECT:
		InitSelect_Scene();
		break;

	case SCENE_GAME1:
		InitGame();
		break;

	case SCENE_GAME2:
		InitGame2();
		break;

	case SCENE_RESULT:
		InitResult();
		break;

	case SCENE_LOSE:
		InitGameOver();
		break;
	}
}

/*------------------------------------------------------------------------------
   �I������������֐�
------------------------------------------------------------------------------*/
void UninitScene(void)
{
	switch (g_SceneIndex)
	{
	case SCENE_NONE:
		break;

	case SCENE_TITLE:
		UninitTitle();
		break;

	case SCENE_SELECT:
		UninitSelect_Scene();
		break;

	case SCENE_GAME1:
		UninitGame();
		break;

	case SCENE_GAME2:
		UninitGame2();
		break;

	case SCENE_RESULT:
		UninitResult();
		break;

	case SCENE_LOSE:
		UninitGameOver();
		break;
	}
}

/*------------------------------------------------------------------------------
   �X�V����������֐�
------------------------------------------------------------------------------*/
void UpdateScene(void)
{
	switch( g_SceneIndex ) 
	{
	case SCENE_NONE:
		break;

	case SCENE_TITLE:
		UpdateTitle();
		break;

	case SCENE_SELECT:
		UpdateSelect_Scene();
		break;

	case SCENE_GAME1:
		UpdateGame();
		break;

	case SCENE_GAME2:
		UpdateGame2();
		break;

	case SCENE_RESULT:
		UpdateResult();
		break;

	case SCENE_LOSE:
		UpdateGameOver();
		break;
	}

	UpdateFade();
	
}

/*------------------------------------------------------------------------------
   �`�揈��������֐�
------------------------------------------------------------------------------*/
void DrawScene(void)
{
	switch( g_SceneIndex )
	{
	case SCENE_NONE:
		break;

	case SCENE_TITLE:
		DrawTitle();
		break;

	case SCENE_SELECT:
		DrawSelect_Scene();
		break;

	case SCENE_GAME1:
		DrawGame();
		break;

	case SCENE_GAME2:
		DrawGame2();
		break;

	case SCENE_RESULT:
		DrawResult();
		break;

	case SCENE_LOSE:
		DrawGameOver();
		break;
	}

	DrawFade();
}

/*------------------------------------------------------------------------------
   �V�[���J�ڂ�v������֐�
------------------------------------------------------------------------------*/
void SetScene(SCENE index)
{
	//�J�ڐ�V�[����ݒ肷��
	g_SceneNextIndex = index;
}

/*------------------------------------------------------------------------------
   �J�ڂ�v��������ꍇ�ɃV�[����؂�ւ���֐�
------------------------------------------------------------------------------*/
void CheckScene(void)
{
	//���݂̃V�[���ƑJ�ڐ�V�[��������Ă�����
	if( g_SceneIndex != g_SceneNextIndex )
	{
		//���݂̃V�[�����I��������
		UninitScene();
		
		//�J�ڐ�V�[���̏������������s��
		InitScene(g_SceneNextIndex);
	}
}

SCENE GetScene(void)
{
	return g_SceneIndex;
}