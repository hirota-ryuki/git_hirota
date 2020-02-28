#include "stdafx.h"
#include "RockDoor.h"
#include "Player.h"
#include "Pose.h"
RockDoor::RockDoor()
{
}

RockDoor::~RockDoor()
{
}

void RockDoor::OnDestroy()
{
	DeleteGO(m_model);
}

bool RockDoor::Start()
{
	//�h�A�B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/rockdoor/rockdoor.cmo");
	m_model->SetData(m_position, m_rotation);
	m_model->UpdateWorldMatrix();
	m_physicsStaticObject.CreateMeshObject(m_model->GetModel(), m_position, m_rotation);

	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	//�v���C���[�̃C���X�^���X���擾�B
	m_player = m_game->GetPlayer();
	m_pose = m_game->GetPose();
	return true;
}

void RockDoor::Update()
{
	CVector3 diff = m_player->GetPos() - m_position;
	if (diff.Length() < 100.0f) {
		//B�{�^������������B
		if (g_pad[0].IsTrigger(enButtonB)){
			if (m_pose->GetItemCount() == 3) {
				//�����V���b�g�Đ���SE
				CSoundSource* m_se = new CSoundSource;
				m_se->Init(L"sound/story/decision.wav");
				m_se->Play(false);
				DeleteGO(this);
			}
		}
	}
}
