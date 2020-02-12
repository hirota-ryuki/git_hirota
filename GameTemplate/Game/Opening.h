#pragma once
#include "IGameObject.h"

#define EVENT_SCENE 2;

class Opening :
	public IGameObject
{
public:
	Opening();
	~Opening();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
private:
	SpriteRender*	m_sprite[2];			//�摜�̃|�C���^�B
	Game*			m_game = nullptr;		//Game�̃|�C���^�B
	bool			m_isButton = false;		//�{�^�������������ǂ����B
	int				m_poseTimer = 0;		//�ꎞ��~������܂ł̃^�C�}�[�B
	CSoundSource	m_se;
};