#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "GameObjectManager.h"


//静的メンバ変数を定義する。
Game* Game::m_game = nullptr;

Game::Game()
{
	if (m_game == nullptr) 
	{
		m_game = this;
	}
	else{
		abort();
	}
	m_player = NewGO<Player>();
	m_gamecamera = NewGO<GameCamera>();
}

Game::~Game()
{
	g_goMgr.DeleteGO(m_player);
	g_goMgr.DeleteGO(m_gamecamera);
}

void Game::Update()
{
}

void Game::Draw()
{
}
