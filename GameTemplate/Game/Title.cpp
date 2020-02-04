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
	//�����V���b�g�Đ���SE
	m_se.Init(L"sound/coinGet.wav");
	return true;
}

void Title::Update()
{
	//A�{�^������������B
	if (g_pad[0].IsTrigger(enButtonA))
	{
		//A�{�^���������ꂽ��SE��炷�B
		m_se.Play(false);
		//Game�N���X�𐶐��B
		NewGO<Game>(GOPrio_Defalut);
		//�^�C�g���N���X�������B
		DeleteGO(this);
	}
}