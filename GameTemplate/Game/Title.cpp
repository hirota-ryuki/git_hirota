#include "stdafx.h"
#include "Title.h"


Title::Title()
{
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/title.dds", 1280.f, 720.f);
}

Title::~Title()
{
	DeleteGO(m_sprite);
}

void Title::Update()
{
	if (g_pad[0].IsTrigger(enButtonA))
	{
		NewGO<Game>(GOPrio_Defalut);
		DeleteGO(this);
	}
}