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
	GettingItem(IItem::IsGetItem(diff));
}

void Ball::GettingItem(bool isGetItem)
{
	if (isGetItem) {
		m_pose->AddItemCount();
		DeleteGO(this);
	}
}
