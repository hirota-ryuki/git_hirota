#include "stdafx.h"
#include "BulletStack.h"
#include "Player.h"
#include "Pose.h"
BulletStack::BulletStack()
{
}

BulletStack::~BulletStack()
{
}

void BulletStack::OnDestroy()
{
	DeleteGO(m_model);
}

bool BulletStack::Start()
{
	//�e��B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/bulletstack/bulletstack.cmo");
	m_model->SetData(m_position, m_rotation);
	m_model->UpdateWorldMatrix();


	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	//�v���C���[�̃C���X�^���X���擾�B
	m_player = m_game->GetPlayer();
	m_pose = m_game->GetPose();
	return true;
}

void BulletStack::Update()
{
	CVector3 diff = m_player->GetPos() - m_position;
	if (diff.Length() < 100.0f) {
		//B�{�^������������B
		if (g_pad[0].IsTrigger(enButtonB)) {
			//�����V���b�g�Đ���SE
			CSoundSource* m_se = new CSoundSource;
			m_se->Init(L"sound/story/decision.wav");
			m_se->Play(false);
			m_player->AddStack(m_bullet);
			DeleteGO(this);
		}
	}
}
