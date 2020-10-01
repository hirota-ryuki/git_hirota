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
	//�S�[�X�g�̍쐬�B
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
	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	//�v���C���[�̃C���X�^���X���擾�B
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
