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
	//画像。
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/pose.dds", 1280.f, 720.f);
	//画像を消しておく。
	m_sprite->ChangeActive();

	return true;
}

void Pose::Update()
{
	//スタートボタンを押したら。
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		m_sprite->ChangeActive();
	}
}