#include "stdafx.h"
#include "Pose.h"

Pose::Pose()
{
}

Pose::~Pose()
{
}

void Pose::OnDestroy()
{
	DeleteGO(m_sprite);
}

bool Pose::Start()
{
	//�摜�B
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/pose.dds", 1280.f, 720.f);
	//�摜�������Ă����B
	m_sprite->ChangeActive();

	return true;
}

void Pose::Update()
{
	//�X�^�[�g�{�^������������B
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		m_sprite->ChangeActive();
	}
}