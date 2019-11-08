#include "stdafx.h"
#include "GameObjectManager.h"

//GameObjectManager�N���X�̃C���X�^���X�B
GameObjectManager g_goMgr;

void GameObjectManager::Update()
{
	//�o�^����Ă���Q�[���I�u�W�F�N�g�̍X�V�������ĂԁB
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto go : m_goList[i]) {
			go->Update();
		}
		for (auto go : m_goList[i]) {
			go->Draw();
		}
	}

	//�S�ẴQ�[���I�u�W�F�N�g��1�t���[�����̏������I����Ă���A�폜����B
	for (int i = 0; i < GOPrio_num; i++) {
		for (auto it = m_goList[i].begin(); it != m_goList[i].end();) {
			if ((*it)->IsRequestDelete()) {
				//�폜���N�G�X�g���󂯂Ă���̂ō폜�B
				delete* it;
				it = m_goList[i].erase(it);
			}
			else {
				//���N�G�X�g���󂯂Ă��Ȃ��̂ŁB
				it++;
			}
		}
	}
}