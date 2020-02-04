#include "stdafx.h"
#include "Title.h"

Title::Title()
{
}

Title::~Title()
{
}

void Title::OnDestroy()
{
	DeleteGO(m_sprite);
}

bool Title::Start()
{
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/title.dds", 1280.f, 720.f);
	//ワンショット再生のSE
	m_se.Init(L"sound/coinGet.wav");
	return true;
}

void Title::Update()
{
	//Aボタンを押したら。
	if (g_pad[0].IsTrigger(enButtonA))
	{
		//Aボタンが押されたらSEを鳴らす。
		m_se.Play(false);
		//Gameクラスを生成。
		NewGO<Game>(GOPrio_Defalut);
		//タイトルクラスを消去。
		DeleteGO(this);
	}
}