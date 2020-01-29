#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Floor.h"
#include "Player.h"
#include "Goal.h"
#include "Zombie.h"
#include "Result.h"

//静的メンバ変数を定義する。
Game* Game::m_game = nullptr;

Game::Game()
{
}

Game::~Game()
{	
}

void Game::OnDestroy()
{
	DeleteGOs("enemy");
	DeleteGO(m_goal);
	DeleteGO(m_gamecamera);
	DeleteGO(m_player);
	DeleteGO(m_floor);
}

bool Game::Start()
{
	//インスタンスを一つだけにする。
	if (m_game == nullptr)
	{
		//インスタンスの登録。
		m_game = this;
	}
	else {
		//警告。
		abort();
	}

	//床の番号。
	wchar_t floor[50];
	swprintf_s(floor, L"floor");
	//レベル。
	wchar_t levelname[50];
	swprintf_s(levelname, L"level/level01.tkl");
	LevelObjectData floorObjData;
	LevelObjectData playerObjData;
	LevelObjectData goalObjData;
	std::vector<LevelObjectData> zombieObjDatas;
	m_level.Init(
		levelname,
		[&](LevelObjectData& objData) {
		//ステージ遷移する場合は背景と床の番号を送るように
		if (wcscmp(objData.name, floor) == 0) {
			floorObjData = objData;

			return true;
		}
		if (wcscmp(objData.name, L"player") == 0) {
			playerObjData = objData;

			return true;
		}
		if (wcscmp(objData.name, L"goal") == 0) {
			goalObjData = objData;

			return true;
		}
		if (wcscmp(objData.name, L"zombie") == 0) {
			//配置しようとしているオブジェクトはゾンビ。
			zombieObjDatas.push_back(objData);
			return true;
		}
		return false;
	}
	);

	//床を構築。
	m_floor = NewGO<Floor>(GOPrio_Defalut);
	//配置情報から座標と回転をステージに渡す。
	m_floor->SetPos(floorObjData.position);
	m_floor->SetRot(floorObjData.rotation);

	//カメラを構築。
	m_gamecamera = NewGO<GameCamera>(GOPrio_Defalut);

	//プレイヤーを構築。
	m_player = NewGO<Player>(GOPrio_Defalut);
	//配置情報から座標と回転をステージに渡す。
	m_player->SetPos(playerObjData.position);
	m_player->SetRot(playerObjData.rotation);


	//ゴールを構築。
	m_goal = NewGO<Goal>(GOPrio_Defalut);
	//配置情報から座標と回転をステージに渡す。
	m_goal->SetPos(goalObjData.position);
	m_goal->SetRot(goalObjData.rotation);

	//ゾンビを構築。
	for (auto& objData : zombieObjDatas) {
		m_zombie = NewGO<Zombie>(GOPrio_Defalut, "enemy");
		//配置情報から座標と回転をステージに渡す。
		m_zombie->SetPos(objData.position);
		m_zombie->SetRot(objData.rotation);
	}

	return true;
}

void Game::Update()
{
	if (m_isClear) {
		//Resultクラスを生成
		NewGO<Result>(GOPrio_Defalut);
		//Gameクラスを消去
		DeleteGO(this);
	}
}