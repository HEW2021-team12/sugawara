#pragma once

// �o�^�V�[���ꗗ
enum SCENE
{
	SCENE_NONE,		//�V�[�������Ȃ�
	SCENE_TITLE,	//�^�C�g��
	SCENE_SELECT,   //�X�e�[�W�Z���N�g
	SCENE_GAME1,	//�Q�[��
	SCENE_GAME2,
	SCENE_GAME3,
	SCENE_GAME4,
	SCENE_GAME5,

	// ���݂�5�X�e�[�W�\��

	SCENE_RESULT,	//���U���g
	SCENE_LOSE,     //GAME OVER

	SCENE_MAX		//�Ōゾ�Ƃ킩��z�������
};

void InitScene(SCENE index);
void UninitScene(void);
void UpdateScene(void);
void DrawScene(void);

void SetScene(SCENE index);
void CheckScene(void);
SCENE GetScene(void);