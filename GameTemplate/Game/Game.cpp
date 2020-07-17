#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Floor.h"
#include "Ceiling.h"
#include "Player.h"
#include "Goal.h"
#include "Zombie.h"
#include "Ball.h"
#include "RockDoor.h"
#include "BulletStack.h"
#include "Result.h"
#include "Pose.h"
#include "Opening.h"
#include "UI.h"
#include "Map.h"

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
	else {
		//警告。
		abort();
	}
}

Game::~Game()
{	
	m_game = nullptr;
}

void Game::OnDestroy()
{
	DeleteGOs("enemy");
	DeleteGO(m_goal);
	DeleteGO(m_gamecamera);
	DeleteGO(m_player);
	DeleteGO(m_ceiling);
	DeleteGO(m_floor);
	DeleteGOs("mapchip");
	DeleteGO(m_pose);
	DeleteGO(m_ui);
	DeleteGO(m_map);
	DeleteGO(m_rockdoor);
	
	DeleteGOs("bulletstack");
	DeleteGOs("ball");
	IItem::Release();
}

bool Game::Start()
{
	//床の番号。
	wchar_t floor[50];
	swprintf_s(floor, L"floor");
	//レベル。
	wchar_t levelname[50];
	swprintf_s(levelname, L"level/level01.tkl");
	LevelObjectData floorObjData;
	LevelObjectData ceilingObjData;
	LevelObjectData playerObjData;
	LevelObjectData goalObjData;
	LevelObjectData rockdoorObjData;
	std::vector<LevelObjectData> zombieObjDatas;
	std::vector<LevelObjectData> ballObjDatas;
	std::vector<LevelObjectData> bulletstackObjDatas;
	m_level.Init(
		levelname,
		[&](LevelObjectData& objData) {
		//ステージ遷移する場合は背景と床の番号を送るように
		if (wcscmp(objData.name, floor) == 0) {
			floorObjData = objData;

			return true;
		}
		if (wcscmp(objData.name, L"ceiling") == 0) {
			ceilingObjData = objData;

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
		if (objData.ForwardMatchName(L"ball")) {
			//配置しようとしているオブジェクトはボール。
			ballObjDatas.push_back(objData);
			return true;
		}
		if (objData.ForwardMatchName(L"rockdoor")) {
			//配置しようとしているオブジェクトはボール。
			rockdoorObjData = objData;
			return true;
		}
		if (objData.ForwardMatchName(L"bulletstack")) {
			//配置しようとしているオブジェクトはボール。
			bulletstackObjDatas.push_back(objData);
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
	
	//天井を構築。
	m_ceiling = NewGO<Ceiling>(GOPrio_Defalut);
	//配置情報から座標と回転をステージに渡す。
	m_ceiling->SetPos(ceilingObjData.position);
	m_ceiling->SetRot(ceilingObjData.rotation);
	   
	//プレイヤーを構築。
	m_player = NewGO<Player>(GOPrio_Defalut);
	//配置情報から座標と回転をステージに渡す。
	m_player->SetPos(playerObjData.position);
	m_player->SetRot(playerObjData.rotation);

	//カメラを構築。
	m_gamecamera = NewGO<GameCamera>(GOPrio_Defalut);
	
	//ゴールを構築。
	m_goal = NewGO<Goal>(GOPrio_Defalut);
	//配置情報から座標と回転をステージに渡す。
	m_goal->SetPos(goalObjData.position);
	m_goal->SetRot(goalObjData.rotation);

	////ゾンビを構築。
	//for (auto& objData : zombieObjDatas) {
	//	m_zombie = NewGO<Zombie>(GOPrio_Defalut, "enemy");
	//	//配置情報から座標と回転をステージに渡す。
	//	m_zombie->SetPos(objData.position);
	//	m_zombie->SetRot(objData.rotation);
	//}
	//
	//UIの構築。
	m_ui = NewGO<UI>(GOPrio_Defalut);
	
	//Mapの構築。
	m_map = NewGO<Map>(GOPrio_Defalut);
	
	//ボールを構築。
	for (auto& objData : ballObjDatas) {
		m_ball = NewGO<Ball>(GOPrio_Defalut, "ball");
		//配置情報から座標と回転をステージに渡す。
		m_ball->SetPos(objData.position);
		m_ball->SetRot(objData.rotation);
		//m_ball->SetNomber(_wtoi(&objData.name[11]));
	}
	
	//弾薬を構築。
	for (auto& objData : bulletstackObjDatas) {
		m_bs = NewGO<BulletStack>(GOPrio_Defalut, "bulletstack");
		//配置情報から座標と回転をステージに渡す。
		//m_bs->GetModel()->SetPos(objData.position);
		m_bs->SetPos(objData.position);
		m_bs->SetRot(objData.rotation);
		//m_ball->SetNomber(_wtoi(&objData.name[11]));
	}
	
	//ドアを構築。
	m_rockdoor = NewGO<RockDoor>(GOPrio_Defalut);
	//配置情報から座標と回転をステージに渡す。
	m_rockdoor->SetPos(rockdoorObjData.position);
	m_rockdoor->SetRot(rockdoorObjData.rotation);
	
	//ポーズを構築。
	m_pose = NewGO<Pose>(GOPrio_Defalut);
	
#ifdef CREATE_MODE
#else
	//オープニングを構築。
	m_op = NewGO<Opening>(GOPrio_Defalut);
#endif // CREATE_MODE	

	//BGM再生のSE。
	CSoundSource* m_bgm = new CSoundSource;
	m_bgm->Init(L"sound/story/bgm.wav");
	m_bgm->Play(true);

	return true;
}

void Game::Update()
{
	//スタートボタンかYボタンを押したら。
	if (g_pad[0].IsTrigger(enButtonStart)
		|| g_pad[0].IsTrigger(enButtonY))
	{
		//一時停止。
		ChangePose();
	}

	if (m_isClear || m_isGameOver) {
		//Resultクラスを生成。
		m_result = NewGO<Result>(GOPrio_Defalut);
		if (m_isClear) {
			m_result->SetIsClear();
		}
		//Gameクラスを消去。
		DeleteGO(this);
	}
	//Aボタンを押したら。
	if (g_pad[0].IsTrigger(enButtonA))
	{
	}
}