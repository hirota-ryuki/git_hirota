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
	//画像。
	m_sprite[0] = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite[0]->Init(L"sprite/event1.dds", 1280.f, 720.f);
	m_sprite[1] = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite[1]->Init(L"sprite/event2.dds", 1280.f, 720.f);
	m_sprite[1]->ChangeActive();

	//ゲームのインスタンスを取得。
	m_game = GetGame();
	return true;
}

void Opening::Update()
{
	m_poseTimer++;
	if (m_poseTimer == 10.0f) {
		m_game->ChangePose();
	}

	if (m_game->GetIsPose()) {
		if (m_isButton) {
			//Aボタンを押したら。
			if (g_pad[0].IsTrigger(enButtonA))
			{
				m_sprite[1]->ChangeActive();
				m_game->ChangePose();
				DeleteGO(this);
			}
		}
		//Aボタンを押したら。
		if (g_pad[0].IsTrigger(enButtonA))
		{
			m_sprite[0]->ChangeActive();
			m_sprite[1]->ChangeActive();
			m_isButton = true;
		}
		
	}
}
