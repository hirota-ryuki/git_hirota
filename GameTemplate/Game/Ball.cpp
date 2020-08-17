#include "stdafx.h"
#include "Ball.h"
#include "Player.h"

Key::Key()
{
}

Key::~Key()
{
}

void Key::ItemDestroy()
{
	DeleteGO(m_model);
}

void Key::ItemStart()
{
	//ボール。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	//ボールの識別番号。
	wchar_t ballNo[60];
	/*swprintf_s(ballNo, L"modelData/ball/ball%d.cmo", m_nomber);*/
	swprintf_s(ballNo, L"modelData/ball/key.cmo");
	m_model->Init(ballNo);
	m_model->SetData(m_position, m_rotation);
	//IItem::SetName(L"Ball");

	m_sprite = IItem::SpriteLoad(L"sprite/item/item_message.dds", ITEM_SPRITE_W, ITEM_SPRITE_H);

	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
}

void Key::ItemUpdate()
{
	IItem::ItemCommonProcessing(m_sprite, m_position, m_model);
}

void Key::OnGet()
{
	Inv_AddItem(NAME, ADD);
}