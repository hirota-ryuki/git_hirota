#include "stdafx.h"
#include "Goal.h"
#include "Player.h"

Goal::Goal()
{
}

Goal::~Goal()
{
	DeleteGO(m_model);
}

bool Goal::Start()
{
	//マンホール。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/goal/goal.cmo");
	m_model->SetData(m_position, m_rotation);
	m_model->UpdateWorldMatrix();

	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	return true;
}

void Goal::Update()
{
	m_model->SetData(m_position, m_rotation);
	CVector3 diff = m_player->GetPos() - m_position;
	if (diff.Length() < 100.0f) {
		m_game->Clear();
	}
}
