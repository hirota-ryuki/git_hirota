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
	LevelObjectData rockdoorObjData;
	std::vector<LevelObjectData> zombieObjDatas;
	std::vector<LevelObjectData> ballObjDatas;
	std::vector<LevelObjectData> bulletstackObjDatas;
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
		if (wcscmp(objData.name, L"zombie") == 0) {
			//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�̓]���r�B
			zombieObjDatas.push_back(objData);
			return true;
		}
		if (objData.ForwardMatchName(L"ball")) {
			//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�̓{�[���B
			ballObjDatas.push_back(objData);
			return true;
		}
		if (objData.ForwardMatchName(L"rockdoor")) {
			//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�̓{�[���B
			rockdoorObjData = objData;
			return true;
		}
		if (objData.ForwardMatchName(L"bulletstack")) {
			//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�̓{�[���B
			bulletstackObjDatas.push_back(objData);
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
	
	//�V����\�z�B
	m_ceiling = NewGO<Ceiling>(GOPrio_Defalut);
	//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
	m_ceiling->SetPos(ceilingObjData.position);
	m_ceiling->SetRot(ceilingObjData.rotation);
	   
	//�v���C���[���\�z�B
	m_player = NewGO<Player>(GOPrio_Defalut);
	//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
	m_player->SetPos(playerObjData.position);
	m_player->SetRot(playerObjData.rotation);

	//�J�������\�z�B
	m_gamecamera = NewGO<GameCamera>(GOPrio_Defalut);
	
	//�S�[�����\�z�B
	m_goal = NewGO<Goal>(GOPrio_Defalut);
	//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
	m_goal->SetPos(goalObjData.position);
	m_goal->SetRot(goalObjData.rotation);

	////�]���r���\�z�B
	//for (auto& objData : zombieObjDatas) {
	//	m_zombie = NewGO<Zombie>(GOPrio_Defalut, "enemy");
	//	//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
	//	m_zombie->SetPos(objData.position);
	//	m_zombie->SetRot(objData.rotation);
	//}
	//
	//UI�̍\�z�B
	m_ui = NewGO<UI>(GOPrio_Defalut);
	
	//Map�̍\�z�B
	m_map = NewGO<Map>(GOPrio_Defalut);
	
	//�{�[�����\�z�B
	for (auto& objData : ballObjDatas) {
		m_ball = NewGO<Ball>(GOPrio_Defalut, "ball");
		//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
		m_ball->SetPos(objData.position);
		m_ball->SetRot(objData.rotation);
		//m_ball->SetNomber(_wtoi(&objData.name[11]));
	}
	
	//�e����\�z�B
	for (auto& objData : bulletstackObjDatas) {
		m_bs = NewGO<BulletStack>(GOPrio_Defalut, "bulletstack");
		//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
		//m_bs->GetModel()->SetPos(objData.position);
		m_bs->SetPos(objData.position);
		m_bs->SetRot(objData.rotation);
		//m_ball->SetNomber(_wtoi(&objData.name[11]));
	}
	
	//�h�A���\�z�B
	m_rockdoor = NewGO<RockDoor>(GOPrio_Defalut);
	//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
	m_rockdoor->SetPos(rockdoorObjData.position);
	m_rockdoor->SetRot(rockdoorObjData.rotation);
	
	//�|�[�Y���\�z�B
	m_pose = NewGO<Pose>(GOPrio_Defalut);
	
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