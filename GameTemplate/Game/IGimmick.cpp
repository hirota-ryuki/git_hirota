#include "stdafx.h"
#include "IGimmick.h"
#include "Game.h"
#include "Player.h"

bool IGimmick::Start()
{
	InitState();
	Init();
	return true;
}

void IGimmick::Init()
{
	//ゴーストの作成。
	switch (m_state)
	{
	case enState_createBox:
		m_ghost.CreateBox(m_position, m_rotation, m_size);
		break;
	case enState_createCapsule:
		m_ghost.CreateCapsule(m_position, m_rotation, m_radius, m_height);
		break;
	case enState_createSphere:
		m_ghost.CreateSphere(m_position, m_rotation, m_height);
		break;
	default:
		break;
	}
	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
}

void IGimmick::Update()
{
	IsHit();
}

void IGimmick::IsHit()
{
	if (m_ghost.IsHit(m_player->GetCharaCon_Ghorst())) {
		OnHit();
	}
}
