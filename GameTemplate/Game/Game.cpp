#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "BackGround.h"
#include "Navimesh.h"


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