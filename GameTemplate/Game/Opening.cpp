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
	m_sprite[0] = NewGO<SpriteRender>(GOPrio_DrawAfter);
	m_sprite[0]->Init(L"sprite/event1.dds", 1280.f, 720.f);
	m_sprite[1] = NewGO<SpriteRender>(GOPrio_DrawAfter);
	m_sprite[1]->Init(L"sprite/event2.dds", 1280.f, 720.f);
	m_sprite[1]->ChangeActive();

	//ゲームのインスタンスを取得。
	m_game = GetGame();
	return true;
}

void Opening::Update()
{
	m_poseTimer++;
	if (m_poseTimer > 10) {
		m_game->IsPose(true);
	}

	if (m_game->GetIsPose()) {
		if (m_isButton) {
			//Aボタンを押したら。
			if (g_pad[0].IsTrigger(enButtonA))
			{
				//ワンショット再生のSE
				m_se.Init(L"sound/story/decision.wav");
				//Aボタンが押されたらSEを鳴らす。
				m_se.Play(false);
				m_sprite[1]->ChangeActive();
				m_game->IsPose(false);
				DeleteGO(this);
			}
		}
		//Aボタンを押したら。
		if (g_pad[0].IsTrigger(enButtonA))
		{
			m_sprite[0]->ChangeActive();
			m_sprite[1]->ChangeActive();
			//ワンショット再生のSE
			m_se.Init(L"sound/story/decision.wav");
			//Aボタンが押されたらSEを鳴らす。
			m_se.Play(false);
			m_isButton = true;
		}
		
	}
}
