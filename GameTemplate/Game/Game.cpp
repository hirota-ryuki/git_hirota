#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"

//静的メンバ変数を定義する。
Game* Game::m_game = nullptr;

Game::Game()
{
	//インスタンスを一つだけにする。
	if (m_game == nullptr) 
	{
		//インスタンスの登録。
		m_game = this;
	}
	else{
		//警告。
		abort();
	}
	m_gamecamera = NewGO<GameCamera>(GOPrio_Defalt);
	m_player = NewGO<Player>(GOPrio_Defalt);
	m_background = NewGO<BackGround>(GOPrio_Defalt);
}

Game::~Game()
{
	DeleteGO(m_player);
	DeleteGO(m_gamecamera);
	DeleteGO(m_background);
}

void Game::Update()
{
}

void Game::Draw()
{
}
