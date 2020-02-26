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
		m_isStart = !m_isStart;
	}

	//メニュー画面が表示されていたら。
	if (m_isStart) {
		for (int i = 0; i < m_item; i++) {
			//画像。
			m_itemSprite = NewGO<SpriteRender>(GOPrio_Sprite);
			m_itemSprite->Init(m_itemTexList[i], 1280.f, 720.f);
			m_itemSprite->SetPos(*m_itemPosList[i]);
			m_itemSpriteList.emplace_back(m_itemSprite);
		}
	}
}