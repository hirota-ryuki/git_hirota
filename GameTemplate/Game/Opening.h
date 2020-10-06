#pragma once
#include "IGameObject.h"

class Opening :
	public IGameObject
{
public:
	Opening();
	~Opening();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void Update_NotPause() override;
private:
	enum ESprite {
		eSprite_Comment_1,		//�ꌾ�ڂ̃Z���t�p�̃X�v���C�g
		eSprite_Comment_2,		//�񌾖ڂ̃Z���t�p�̃X�v���C�g
		eSprite_Comment_Num,	//�Z���t�p�̃X�v���C�g�̖���
	};

	SpriteRender*	m_commentSprite[eSprite_Comment_Num];	//�Z���t�p�̃X�v���C�g�̃|�C���^�B
	Game*			m_game = nullptr;		//Game�̃|�C���^�B
	bool			m_isButton = false;		//�{�^�������������ǂ����B
	int				m_poseTimer = 0;		//�ꎞ��~������܂ł̃^�C�}�[�B
	const int		POSE_TIME_LIMIT = 10;	//10�b�o������ꎞ��~������B
	CSoundSource	m_se;
};