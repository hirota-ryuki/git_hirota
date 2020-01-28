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

bool Title::Start()
{
	return true;
}

void Title::Update()
{
	//A�{�^������������
	if (g_pad[0].IsTrigger(enButtonA))
	{
		//Game�N���X�𐶐�
		NewGO<Game>(GOPrio_Defalut);
		//�^�C�g���N���X������
		DeleteGO(this);
	}
}