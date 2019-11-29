#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Navimesh.h"


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

	m_gamecamera = NewGO<GameCamera>(GOPrio_Defalut);
	m_player = NewGO<Player>(GOPrio_Defalut);
	m_background = NewGO<BackGround>(GOPrio_Defalut);
	m_navimesh = NewGO<Navimesh>(GOPrio_Defalut);

}

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_gamecamera);
	DeleteGO(m_background);
	DeleteGO(m_navimesh);
}