#include "stdafx.h"
#include "LoadLevel.h"
#include "Floor.h"
#include "Player.h"
#include "Zombie.h"

LoadLevel::LoadLevel()
{
	/*
	//�X�e�[�W�̔ԍ��B
	wchar_t stagelevel[50];
	swprintf_s(stagelevel, L"stage0%d", m_game->GetStageNumber());
	//���̔ԍ��B
	wchar_t floor[50];
	swprintf_s(floor, L"floor0%d", m_game->GetFloorNumber());
	//���x���B
	wchar_t levelname[50];
	swprintf_s(levelname, L"level/level0%d.tkl", m_game->GetStageNumber());
	*/

	//�X�e�[�W�̔ԍ��B
	wchar_t stage[50];
	swprintf_s(stage, L"stage");
	//���̔ԍ��B
	wchar_t floor[50];
	swprintf_s(floor, L"floor");
	//���x���B
	wchar_t levelname[50];
	swprintf_s(levelname, L"level/level01.tkl");
	m_level.Init(
		levelname,
		[&](LevelObjectData& objData) {
			//�X�e�[�W�J�ڂ���ꍇ�͔w�i�Ə��̔ԍ��𑗂�悤��
			
			if (objData.name == floor) {
				//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�͏��B
				//�t�b�N���āA�v���C���[�N���X�����B
				m_floor = NewGO<Floor>(GOPrio_Defalut);
				//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
				m_floor->SetPos(objData.position);
				m_floor->SetRot(objData.rotation);
				return true;
			}
			if (objData.name == L"player") {
				//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�̓v���C���[�B
				//�t�b�N���āA�v���C���[�N���X�����B
				m_player = NewGO<Player>(GOPrio_Defalut);
				//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
				m_player->SetPos(objData.position);
				m_player->SetRot(objData.rotation);
				return true;
			}
			if (objData.name == L"zombie") {
				//�z�u���悤�Ƃ��Ă���I�u�W�F�N�g�̓]���r�B
				//�t�b�N���āA�v���C���[�N���X�����B
				m_zombie = NewGO<Zombie>(GOPrio_Defalut);
				//�z�u��񂩂���W�Ɖ�]���X�e�[�W�ɓn���B
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
