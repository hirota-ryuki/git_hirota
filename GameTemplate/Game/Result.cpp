#include "stdafx.h"
#include "Result.h"
#include "Title.h"

Result::Result()
{
}


Result::~Result()
{
}

void Result::OnDestroy()
{
	DeleteGO(m_sprite);
}

bool Result::Start()
{
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	if (m_isClear) {
		m_sprite->Init(L"sprite/moca.dds", 1280.f, 720.f);
	}
	else {
		m_sprite->Init(L"sprite/moca.dds", 1280.f, 720.f);
	}

	return true;
}

void Result::Update()
{
	////Aボタンを押したら
	//if (g_pad[0].IsTrigger(enButtonA))
	//{
	//	//Titleクラスを生成
	//	NewGO<Title>(GOPrio_Defalut);
	//	//Resultクラスを消去
	//	DeleteGO(this);
	//}
}