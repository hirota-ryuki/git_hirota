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
	return true;
}

void Title::Update()
{
	//Aボタンを押したら。
	if (g_pad[0].IsTrigger(enButtonA))
	{
		//ワンショット再生のSE
		m_se.Init(L"sound/story/decision.wav");
		//Aボタンが押されたらSEを鳴らす。
		m_se.Play(false);
		//Gameクラスを生成。
		NewGO<Game>(GOPrio_Defalut);
		//タイトルクラスを消去。
		DeleteGO(this);
	}
	m_font = std::make_unique<Font>();

	m_font->Begin();
	wchar_t fps[256];
	swprintf_s(fps, L"FPS = %f", m_fps);
	float w = (float)GraphicsEngine().Get2DSpaceScreenWidth();
	float h = (float)GraphicsEngine().Get2DSpaceScreenHeight();
	m_font->Draw(
		fps,
		{
			w * -0.5f,
			h * 0.5f
		},
		CVector4::White(),
		0.0f,
		1.0f,
		{ 0.0f, 1.0f }
	);
	m_font->End();
}