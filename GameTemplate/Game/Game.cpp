#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "GameObjectManager.h"


//�ÓI�����o�ϐ����`����B
//Game* Game::m_game = nullptr;

Game::Game()
{
	m_player = g_goMgr.NewGO<Player>();
	m_gamecamera = g_goMgr.NewGO<GameCamera>();
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
