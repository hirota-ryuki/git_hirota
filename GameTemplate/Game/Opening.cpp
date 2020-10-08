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
	DeleteGO(m_commentSprite[eSprite_Comment_1]);
	DeleteGO(m_commentSprite[eSprite_Comment_2]);
}

bool Opening::Start()
{
	//画像。
	m_commentSprite[eSprite_Comment_1] = NewGO<SpriteRender>(GOPrio_DrawAfter);
	m_commentSprite[eSprite_Comment_1]->Init(L"sprite/event1.dds", 1280.f, 720.f);
	m_commentSprite[eSprite_Comment_2] = NewGO<SpriteRender>(GOPrio_DrawAfter);
	m_commentSprite[eSprite_Comment_2]->Init(L"sprite/event2.dds", 1280.f, 720.f);
	m_commentSprite[eSprite_Comment_2]->ActiveMode(false);

	//ゲームのインスタンスを取得。
	m_game = GetGame();
	return true;
}

void Opening::Update()
{
	
	if (m_isButton) {
		//Aボタンを押したら。
		if (g_pad[0].IsTrigger(enButtonA))
		{
			Sound(L"sound/story/decision.wav", false);
			m_commentSprite[eSprite_Comment_2]->ActiveMode(false);
			GameObjectManager::GetInstance().SetPause(false);
			DeleteGO(this);
		}
	}
	//Aボタンを押したら。
	else if (g_pad[0].IsTrigger(enButtonA))
	{
		m_commentSprite[eSprite_Comment_1]->ActiveMode(false);
		m_commentSprite[eSprite_Comment_2]->ActiveMode(true);
		Sound(L"sound/story/decision.wav", false);
		m_isButton = true;
	}
}

void Opening::Update_NotPause()
{
	if (!m_isButton) {
		if (m_poseTimer < POSE_TIME_LIMIT) {
			m_poseTimer++;
		}
		if (m_poseTimer >= POSE_TIME_LIMIT) {
			GameObjectManager::GetInstance().SetPause(true);
		}
	}
}