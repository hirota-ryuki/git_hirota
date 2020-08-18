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
	else {
		//�x���B
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
	//���̔ԍ��B
	wchar_t floor[50];
	swprintf_s(floor, L"floor");
	//���x���B
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
		//�X�e�[�W�J�ڂ���ꍇ�͔w�i�Ə��̔ԍ��𑗂�悤��
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
		//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�̓]���r�B
		if (wcscmp(objData.name, L"zombie") == 0) {
			zombieObjDatas.push_back(objData);
			return true;
		}
		//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�͌��B
		if (objData.ForwardMatchName(L"key")) {
			ballObjDatas.push_back(objData);
			keyNames.push_back(objData.name);
			return true;
		}
		//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�̓h�A�B
		if (objData.ForwardMatchName(L"door")) {
			rockdoorObjDatas.push_back(objData);
			doorNames.push_back(objData.name);
			return true;
		}
		//���̃I�u�W�F�N�g�̓h�A�̒��S���W�B
		if (objData.ForwardMatchName(L"centerdoorbox")) {
			doorboxObjDatas.push_back(objData);
			return true;
		}
		//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�̓{�[���B
		if (objData.ForwardMatchName(L"bulletstack")) {
			bulletstackObjDatas.push_back(objData);
			return true;
		}
		return false;
	}
	);

	//�����\�z�B
	m_floor = NewGO<Floor>(GOPrio_Defalut);
	m_floor->SetPos(floorObjData.position);
	m_floor->SetRot(floorObjData.rotation);
	
	//�V����\�z�B
	m_ceiling = NewGO<Ceiling>(GOPrio_Defalut);
	m_ceiling->SetPos(ceilingObjData.position);
	m_ceiling->SetRot(ceilingObjData.rotation);
	   
	//�v���C���[���\�z�B
	m_player = NewGO<Player>(GOPrio_Defalut);
	m_player->SetPos(playerObjData.position);
	m_player->SetRot(playerObjData.rotation);

	//�J�������\�z�B
	m_gamecamera = NewGO<GameCamera>(GOPrio_Defalut);
	
	//�S�[�����\�z�B
	m_goal = NewGO<Goal>(GOPrio_Defalut);
	m_goal->SetPos(goalObjData.position);
	m_goal->SetRot(goalObjData.rotation);

	////�]���r���\�z�B
	//for (auto& objData : zombieObjDatas) {
	//	m_zombie = NewGO<Zombie>(GOPrio_Defalut, "enemy");
	//	m_zombie->SetPos(objData.position);
	//	m_zombie->SetRot(objData.rotation);
	//}
	
	//UI�̍\�z�B
	m_ui = NewGO<UI>(GOPrio_Defalut);
	
	//Map�̍\�z�B
	m_map = NewGO<Map>(GOPrio_Defalut);
	
	//�����\�z�B
	auto knItr = keyNames.begin();
	for (auto& objData : ballObjDatas) {
		std::wstring name;
		name = knItr->substr(4);

		if (name == L"reception") {
			name = L"��t";
		}
		if (name == L"detective") {
			name = L"�Y����";
		}
		if (name == L"document") {
			name = L"������";
		}
		if (name == L"chief") {
			name = L"������";
		}
		name.append(L"�̌�");

		m_key = NewGO<Key>(GOPrio_Defalut, "key");
		m_key->SetPos(objData.position);
		m_key->SetRot(objData.rotation);
		m_key->SetName(name);
		knItr++;
		//m_key->SetNomber(_wtoi(&objData.name[11]));
	}
	
	//�e����\�z�B
	for (auto& objData : bulletstackObjDatas) {
		m_bs = NewGO<BulletStack>(GOPrio_Defalut, "bulletstack");
		m_bs->SetPos(objData.position);
		m_bs->SetRot(objData.rotation);
	}

	//�h�A���\�z�B
	auto dnItr = doorNames.begin();	
	for (auto& objData : rockdoorObjDatas) {
		std::wstring name;
		if (*dnItr == L"door") {
			name = L"������";
		}
		else {
			name = dnItr->substr(5);
		}

		if (name == L"reception") {
			name = L"��t";
		}
		if (name == L"detective") {
			name = L"�Y����";
		}
		if (name == L"document") {
			name = L"������";
		}
		if (name == L"chief") {
			name = L"������";
		}
		if (name != L"������") {
			name.append(L"�̌�");
		}

		m_rockdoor = NewGO<RockDoor>(GOPrio_Defalut, "door");
		m_rockdoor->SetPos(objData.position);
		m_rockdoor->SetRot(objData.rotation);
		m_rockdoor->SetName(name);
		//���̃h�A�Ɉ�ԋ߂����S���W���擾�B
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
	
	//���j���[���\�z�B
	//�A�C�e���f�[�^�ǉ��̗L���𔻒肷�邽�߈�ԍŌ�ɐ����B
	m_menu = NewGO<Menu>(GOPrio_Defalut);
	
#ifdef CREATE_MODE
#else
	//�I�[�v�j���O���\�z�B
	m_op = NewGO<Opening>(GOPrio_Defalut);
#endif // CREATE_MODE	

	//BGM�Đ���SE�B
	CSoundSource* m_bgm = new CSoundSource;
	m_bgm->Init(L"sound/story/bgm.wav");
	m_bgm->Play(true);

	return true;
}

void Game::Update()
{
	//�X�^�[�g�{�^����Y�{�^������������B
	if (g_pad[0].IsTrigger(enButtonStart)
		|| g_pad[0].IsTrigger(enButtonY))
	{
		//�ꎞ��~�B
		ChangePose();
	}

	if (m_isClear || m_isGameOver) {
		//Result�N���X�𐶐��B
		m_result = NewGO<Result>(GOPrio_Defalut);
		if (m_isClear) {
			m_result->SetIsClear();
		}
		//Game�N���X�������B
		DeleteGO(this);
	}
	//A�{�^������������B
	if (g_pad[0].IsTrigger(enButtonA))
	{
	}
}