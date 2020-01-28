#include "stdafx.h"
#include "LoadLevel.h"
#include "Floor.h"
#include "Player.h"
#include "Zombie.h"

LoadLevel::LoadLevel()
{
	/*
	//ステージの番号。
	wchar_t stagelevel[50];
	swprintf_s(stagelevel, L"stage0%d", m_game->GetStageNumber());
	//床の番号。
	wchar_t floor[50];
	swprintf_s(floor, L"floor0%d", m_game->GetFloorNumber());
	//レベル。
	wchar_t levelname[50];
	swprintf_s(levelname, L"level/level0%d.tkl", m_game->GetStageNumber());
	*/

	//ステージの番号。
	wchar_t stage[50];
	swprintf_s(stage, L"stage");
	//床の番号。
	wchar_t floor[50];
	swprintf_s(floor, L"floor");
	//レベル。
	wchar_t levelname[50];
	swprintf_s(levelname, L"level/level01.tkl");
	m_level.Init(
		levelname,
		[&](LevelObjectData& objData) {
			//ステージ遷移する場合は背景と床の番号を送るように
			
			if (objData.name == floor) {
				//配置しようとしているオブジェクトは床。
				//フックして、プレイヤークラスを作る。
				m_floor = NewGO<Floor>(GOPrio_Defalut);
				//配置情報から座標と回転をステージに渡す。
				m_floor->SetPos(objData.position);
				m_floor->SetRot(objData.rotation);
				return true;
			}
			if (objData.name == L"player") {
				//配置しようとしているオブジェクトはプレイヤー。
				//フックして、プレイヤークラスを作る。
				m_player = NewGO<Player>(GOPrio_Defalut);
				//配置情報から座標と回転をステージに渡す。
				m_player->SetPos(objData.position);
				m_player->SetRot(objData.rotation);
				return true;
			}
			if (objData.name == L"zombie") {
				//配置しようとしているオブジェクトはゾンビ。
				//フックして、プレイヤークラスを作る。
				m_zombie = NewGO<Zombie>(GOPrio_Defalut);
				//配置情報から座標と回転をステージに渡す。
				m_zombie->SetPos(objData.position);
				m_zombie->SetRot(objData.rotation);
				return true;
			}
		});
}


LoadLevel::~LoadLevel()
{
}

void LoadLevel::Update()
{
}
