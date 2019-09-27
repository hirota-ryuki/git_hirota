#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"

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
