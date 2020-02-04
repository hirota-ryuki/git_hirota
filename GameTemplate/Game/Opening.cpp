#include "stdafx.h"
#include "Opening.h"

Opening::Opening()
{
}

Opening::~Opening()
{
}

void Opening::OnDestroy()
{
	DeleteGO(m_sprite[0]);
	DeleteGO(m_sprite[1]);
}

bool Opening::Start()
{
	//�摜�B
	m_sprite[0] = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite[0]->Init(L"sprite/event1.dds", 1280.f, 720.f);
	m_sprite[1] = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite[1]->Init(L"sprite/event2.dds", 1280.f, 720.f);
	m_sprite[1]->ChangeActive();

	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	m_game->ChangePose();
	return true;
}

void Opening::Update()
{
	//A�{�^������������B
	if (g_pad[0].IsTrigger(enButtonA))
	{
		m_sprite[0]->ChangeActive();
		m_sprite[1]->ChangeActive();
		m_isButton = true;
	}
	if (m_isButton) {
		m_game->ChangePose();
		DeleteGO(this);
	}
}
