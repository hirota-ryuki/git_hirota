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
	DeleteGO(m_spriteMenu);
	DeleteGOs("item");
}

bool Pose::Start()
{
	m_itemNameFont = NewGO<FontRender>(GOPrio_Sprite,"item");
	wchar_t bullet[50];
	swprintf_s(bullet, L"��x");
	m_itemNameFont->SetText(bullet);
	m_itemNameFont->SetPosition({ 0.0f, 0.0f });
	m_itemNameFont->ChangeActive();

	m_itemCountfont = NewGO<FontRender>(GOPrio_Sprite,"item");
	m_itemCountfont->SetPosition({ 100.0f, 0.0f });
	m_itemCountfont->ChangeActive();

#ifdef BAG_MODE
	//�摜�B
	m_spriteMenu = NewGO<SpriteRender>(GOPrio_Sprite);
	m_spriteMenu->Init(L"sprite/item_menu.dds", 640.f, 360.f);
	//�摜�������Ă����B
	m_spriteMenu->ActiveMode(false);
#endif // BAG_MODE

	return true;
}

void Pose::Update()
{
	//�X�^�[�g�{�^������������B
	if (g_pad[0].IsTrigger(enButtonStart))
	{
		m_isStart = !m_isStart;
		wchar_t allbullet[50];
		swprintf_s(allbullet, L"%d", m_itemCount);
		m_itemCountfont->SetText(allbullet);
		m_itemNameFont->ChangeActive();
		m_itemCountfont->ChangeActive();
#ifdef BAG_MODE
		m_spriteMenu->ChangeActive();
#endif // BAG_MODE
	}

#ifdef BAG_MODE
	//���j���[��ʂ��\������Ă�����B
	if (m_isStart) {
		
	}
#endif // BAG_MODE
}