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

void Ball::OnDestroy()
{
	DeleteGO(m_model);
}

bool Ball::Start()
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

	m_sprite = IItem::SpriteLoad(L"sprite/item/magazine_message.dds", ITEM_SPRITE_W, ITEM_SPRITE_H);
	IItem::ButtonSpriteLoad();

	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	m_pose = m_game->GetPose();
	return true;
}

void Ball::Update()
{
	CVector3 diff = m_player->GetPos() - m_position;
	IItem::SpriteMove(m_sprite, diff);
	GettingItem(IItem::IsGetItem(diff));
	IItem::ButtonSpriteMove(diff, m_position);
}

void Ball::OnGet()
{
	m_pose->AddItemCount();
}