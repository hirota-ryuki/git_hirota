#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Floor.h"
#include "Ceiling.h"
#include "Player.h"
#include "Goal.h"
#include "Zombie.h"
#include "Key.h"
#include "RockDoor.h"
#include "BulletStack.h"
#include "Result.h"
#include "Menu.h"
#include "Opening.h"
#include "UI.h"
#include "Map.h"
#include <string>
#include <iostream>


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
	DeleteGO(m_menu);
	DeleteGO(m_ui);
	DeleteGO(m_map);
	DeleteGOs("door");
	DeleteGOs("bulletstack");
	DeleteGOs("key");
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
	std::vector<LevelObjectData>	rockdoorObjDatas;
	std::vector<LevelObjectData>	doorboxObjDatas;
	std::vector<LevelObjectData>	zombieObjDatas;
	std::vector<LevelObjectData>	ballObjDatas;
	std::vector<LevelObjectData>	bulletstackObjDatas;
	std::vector<std::wstring>		doorNames;
	std::vector<std::wstring>		keyNames;
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
		//配置しようとしているオブジェクトはゾンビ。
		if (wcscmp(objData.name, L"zombie") == 0) {
			zombieObjDatas.push_back(objData);
			return true;
		}
		//配置しようとしているオブジェクトは鍵。
		if (objData.ForwardMatchName(L"key")) {
			ballObjDatas.push_back(objData);
			keyNames.push_back(objData.name);
			return true;
		}
		//配置しようとしているオブジェクトはドア。
		if (objData.ForwardMatchName(L"door")) {
			rockdoorObjDatas.push_back(objData);
			doorNames.push_back(objData.name);
			return true;
		}
		//このオブジェクトはドアの中心座標。
		if (objData.ForwardMatchName(L"centerdoorbox")) {
			doorboxObjDatas.push_back(objData);
			return true;
		}
		//配置しようとしているオブジェクトはボール。
		if (objData.ForwardMatchName(L"bulletstack")) {
			bulletstackObjDatas.push_back(objData);
			return true;
		}
		return false;
	}
	);

	//床を構築。
	m_floor = NewGO<Floor>(GOPrio_Defalut);
	m_floor->SetPos(floorObjData.position);
	m_floor->SetRot(floorObjData.rotation);
	
	//天井を構築。
	m_ceiling = NewGO<Ceiling>(GOPrio_Defalut);
	m_ceiling->SetPos(ceilingObjData.position);
	m_ceiling->SetRot(ceilingObjData.rotation);
	   
	//プレイヤーを構築。
	m_player = NewGO<Player>(GOPrio_Defalut);
	m_player->SetPos(playerObjData.position);
	m_player->SetRot(playerObjData.rotation);

	//カメラを構築。
	m_gamecamera = NewGO<GameCamera>(GOPrio_Defalut);
	
	//ゴールを構築。
	m_goal = NewGO<Goal>(GOPrio_Defalut);
	m_goal->SetPos(goalObjData.position);
	m_goal->SetRot(goalObjData.rotation);

	////ゾンビを構築。
	//for (auto& objData : zombieObjDatas) {
	//	m_zombie = NewGO<Zombie>(GOPrio_Defalut, "enemy");
	//	m_zombie->SetPos(objData.position);
	//	m_zombie->SetRot(objData.rotation);
	//}
	
	//UIの構築。
	m_ui = NewGO<UI>(GOPrio_Defalut);
	
	//Mapの構築。
	m_map = NewGO<Map>(GOPrio_Defalut);
	
	//鍵を構築。
	auto knItr = keyNames.begin();
	for (auto& objData : ballObjDatas) {
		std::wstring name;
		name = knItr->substr(4);

		if (name == L"reception") {
			name = L"受付";
		}
		if (name == L"detective") {
			name = L"刑事課";
		}
		if (name == L"document") {
			name = L"資料室";
		}
		if (name == L"chief") {
			name = L"署長室";
		}
		name.append(L"の鍵");

		m_key = NewGO<Key>(GOPrio_Defalut, "key");
		m_key->SetPos(objData.position);
		m_key->SetRot(objData.rotation);
		m_key->SetName(name);
		knItr++;
		//m_key->SetNomber(_wtoi(&objData.name[11]));
	}
	
	//弾薬を構築。
	for (auto& objData : bulletstackObjDatas) {
		m_bs = NewGO<BulletStack>(GOPrio_Defalut, "bulletstack");
		m_bs->SetPos(objData.position);
		m_bs->SetRot(objData.rotation);
	}

	//ドアを構築。
	auto dnItr = doorNames.begin();	
	for (auto& objData : rockdoorObjDatas) {
		std::wstring name;
		if (*dnItr == L"door") {
			name = L"鍵無し";
		}
		else {
			name = dnItr->substr(5);
		}

		if (name == L"reception") {
			name = L"受付";
		}
		if (name == L"detective") {
			name = L"刑事課";
		}
		if (name == L"document") {
			name = L"資料室";
		}
		if (name == L"chief") {
			name = L"署長室";
		}
		if (name != L"鍵無し") {
			name.append(L"の鍵");
		}

		m_rockdoor = NewGO<RockDoor>(GOPrio_Defalut, "door");
		m_rockdoor->SetPos(objData.position);
		m_rockdoor->SetRot(objData.rotation);
		m_rockdoor->SetName(name);
		//このドアに一番近い中心座標を取得。
		float min = FLT_MAX;
		LevelObjectData nearObj;
		for (auto& boxdata : doorboxObjDatas) {
			CVector3 diff = boxdata.position - objData.position;
			if (diff.Length() < min) {
				min = diff.Length();
				nearObj = boxdata;
			}
		}
		m_rockdoor->SetCenterPos(nearObj.position);
		dnItr++;
	}
	
	//メニューを構築。
	//アイテムデータ追加の有無を判定するため一番最後に生成。
	m_menu = NewGO<Menu>(GOPrio_Defalut);
	
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