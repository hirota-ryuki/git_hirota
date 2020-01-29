#include "stdafx.h"
#include "Game.h"
#include "GameCamera.h"
#include "Floor.h"
#include "Player.h"
#include "Goal.h"
#include "Zombie.h"
#include "Result.h"

//�ÓI�����o�ϐ����`����B
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

	//���̔ԍ��B
	wchar_t floor[50];
	swprintf_s(floor, L"floor");
	//���x���B
	wchar_t levelname[50];
	swprintf_s(levelname, L"level/level01.tkl");
	LevelObjectData floorObjData;
	LevelObjectData playerObjData;
	LevelObjectData goalObjData;
	std::vector<LevelObjectData> zombieObjDatas;
	m_level.Init(
		levelname,
		[&](LevelObjectData& objData) {
		//�X�e�[�W�J�ڂ���ꍇ�͔w�i�Ə��̔ԍ��𑗂�悤��
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
			//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�̓]���r�B
			zombieObjDatas.push_back(objData);
			return true;
		}
		return false;
	}
	);

	//�����\�z�B
	m_floor = NewGO<Floor>(GOPrio_Defalut);
	//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
	m_floor->SetPos(floorObjData.position);
	m_floor->SetRot(floorObjData.rotation);

	//�J�������\�z�B
	m_gamecamera = NewGO<GameCamera>(GOPrio_Defalut);

	//�v���C���[���\�z�B
	m_player = NewGO<Player>(GOPrio_Defalut);
	//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
	m_player->SetPos(playerObjData.position);
	m_player->SetRot(playerObjData.rotation);


	//�S�[�����\�z�B
	m_goal = NewGO<Goal>(GOPrio_Defalut);
	//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
	m_goal->SetPos(goalObjData.position);
	m_goal->SetRot(goalObjData.rotation);

	//�]���r���\�z�B
	for (auto& objData : zombieObjDatas) {
		m_zombie = NewGO<Zombie>(GOPrio_Defalut, "enemy");
		//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
		m_zombie->SetPos(objData.position);
		m_zombie->SetRot(objData.rotation);
	}

	return true;
}

void Game::Update()
{
	if (m_isClear) {
		//Result�N���X�𐶐�
		NewGO<Result>(GOPrio_Defalut);
		//Game�N���X������
		DeleteGO(this);
	}
}