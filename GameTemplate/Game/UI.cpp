#include "stdafx.h"
#include "UI.h"
#include "Player.h"

UI::UI()
{
}

UI::~UI()
{
}

void UI::OnDestroy()
{
	DeleteGO(m_fontCapacity);
	DeleteGO(m_fontStack);
	DeleteGO(m_fontLine);
}

bool UI::Start()
{
	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	//フォント。
	m_fontCapacity = NewGO<FontRender>(GOPrio_Sprite);
	m_fontCapacity->SetPosition({ 500.0f, -290.0f });
	m_fontCapacity->SetScale(1.0f);

	m_fontStack = NewGO<FontRender>(GOPrio_Sprite);
	wchar_t bullet[50];
	swprintf_s(bullet, L"%d", m_player->GetStack());
	m_fontStack->SetText(bullet);
	m_fontStack->SetPosition({ 560.0f, -290.0f });
	m_fontStack->SetScale(0.6f);

	m_fontLine = NewGO<FontRender>(GOPrio_Sprite);
	m_fontLine->SetText(L"|");
	m_fontLine->SetPosition({ 540.0f, -290.0f });
	m_fontLine->SetScale(1.0f);
	//フォントは消しておく。
	CangeActives(false);
	return true;
}

void UI::Update()
{
	wchar_t bullet[50];
	swprintf_s(bullet, L"%d",m_player->GetCapacity());
	m_fontCapacity->SetText(bullet);
	int red = m_player->GetMaxCapacity()*0.3f;
	if (red >= m_player->GetCapacity()) {
		m_fontCapacity->SetColor(CVector4::Red());
	}
	else if (m_player->GetMaxCapacity() == m_player->GetCapacity()) {
		m_fontCapacity->SetColor(CVector4::Green());
	}
	else{
		m_fontCapacity->SetColor(CVector4::White());
	}

	wchar_t allbullet[50];
	_itow_s(m_player->GetStack(), allbullet, 10);
	m_fontStack->SetText(allbullet);
}