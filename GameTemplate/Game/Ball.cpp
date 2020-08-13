#include "stdafx.h"
#include "Ball.h"
#include "Player.h"
#include "Pose.h"

Ball::Ball()
{
}

Ball::~Ball()
{
}

void Ball::ItemDestroy()
{
	DeleteGO(m_model);
}

void Ball::ItemStart()
{
	//ボール。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	//ボールの識別番号。
	wchar_t ballNo[60];
	/*swprintf_s(ballNo, L"modelData/ball/ball%d.cmo", m_nomber);*/
	swprintf_s(ballNo, L"modelData/ball/ball.cmo");
	m_model->Init(ballNo);
	m_model->SetData(m_position, m_rotation);
	IItem::SetName(L"Ball");

	m_sprite = IItem::SpriteLoad(L"sprite/item/item_message.dds", ITEM_SPRITE_W, ITEM_SPRITE_H);

	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	m_pose = m_game->GetPose();
}

void Ball::ItemUpdate()
{
	IItem::ItemCommonProcessing(m_sprite, m_position, m_model);
}

void Ball::OnGet()
{
	m_pose->AddItemCount();
	Inv_AddItem(NAME, ADD);
}