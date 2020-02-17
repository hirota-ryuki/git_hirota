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
	//A�{�^������������B
	if (g_pad[0].IsTrigger(enButtonA))
	{
		//�����V���b�g�Đ���SE
		m_se.Init(L"sound/story/decision.wav");
		//A�{�^���������ꂽ��SE��炷�B
		m_se.Play(false);
		//Game�N���X�𐶐��B
		NewGO<Game>(GOPrio_Defalut);
		//�^�C�g���N���X�������B
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