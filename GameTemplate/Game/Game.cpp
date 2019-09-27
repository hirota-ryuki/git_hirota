#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"

//�ÓI�����o�ϐ����`����B
Game* Game::m_game = nullptr;

Game::Game()
{
	//�C���X�^���X��������ɂ���B
	if (m_game == nullptr) 
	{
		//�C���X�^���X�̓o�^�B
		m_game = this;
	}
	else{
		//�x���B
		abort();
	}
	m_player = NewGO<Player>();
	m_gamecamera = NewGO<GameCamera>();
}

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_gamecamera);
}

void Game::Update()
{
}

void Game::Draw()
{
}
