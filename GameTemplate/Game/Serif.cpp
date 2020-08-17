#include "stdafx.h"
#include "Serif.h"
#include "Game.h"
Serif* Serif::m_serif = nullptr;

Serif::Serif()
{
	//インスタンスを一つだけにする。
	if (m_serif == nullptr)
	{
		//インスタンスの登録。
		m_serif = this;
	}
	else {
		//警告。
		abort();
	}
}

Serif::~Serif()
{
	m_serif = nullptr;
}

bool Serif::Start()
{
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/serif.dds", m_spriteSize.x, m_spriteSize.y);
	m_sprite->ActiveMode(false);
	m_font = NewGO<FontRender>(GOPrio_Sprite);
	m_font->SetPosition(FONT_POS);
	m_font->SetPivot(FONT_PIVOT);
	m_font->ActiveMode(false);
	m_game = GetGame();
	return true;
}

void Serif::Update()
{
	if (m_isSerif) {
		m_displayCount++;
		if (m_displayCount > 200) {
			m_displayCount = 0;
			m_isSerif = false;
			m_font->SetText(L"");
			m_font->ActiveMode(false);
		}
	}
}

void Serif::Message(const wchar_t * serif)
{
	m_isSerif = true;
	m_font->SetText(serif);
	m_font->ActiveMode(true);
	m_isSerif = true;


	//if (!m_isSerif) {
	//	//m_sprite->ActiveMode(true);
	//	
	//	m_game->IsPose(true);
	//}
	//else {
	//	if (g_pad[0].IsTrigger(enButtonB)) {
	//		//m_sprite->ActiveMode(false);
	//		m_font->SetText(L"");
	//		m_font->ActiveMode(false);
	//		m_isSerif = false;
	//		m_game->IsPose(false);
	//	}
	//}
}