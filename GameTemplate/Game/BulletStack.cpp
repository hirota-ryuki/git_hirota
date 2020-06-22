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
	//弾薬。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/bulletstack/bulletstack.cmo");
	m_model->InitNormalMap(L"modelData/bulletstack/Normal.dds");
	m_model->InitSpecMap(L"modelData/bulletstack/Metalness.dds");
	m_model->SetData(m_position, m_rotation);
	m_model->SetDirectionLightDirection(m_directionLightDirection);
	m_model->SetDirectionLightColor(m_directionLightColor);
	m_model->SetSpec(m_spec);
	m_model->SetAmbientLight(m_ambientLight);
	m_model->UpdateWorldMatrix();


	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	m_pose = m_game->GetPose();
	return true;
}

void BulletStack::Update()
{
	//回転。
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY(), 2.0f);
	m_rotation.Multiply(qAddRot, m_rotation);
	m_model->SetRot(m_rotation);
	m_model->UpdateWorldMatrix();	

	CVector3 diff = m_player->GetPos() - m_position;
	if (diff.Length() < 100.0f) {
		//Bボタンを押したら。
		if (g_pad[0].IsTrigger(enButtonB)) {
			//ワンショット再生のSE
			CSoundSource* m_se = new CSoundSource;
			m_se->Init(L"sound/story/decision.wav");
			m_se->Play(false);
			m_player->AddStack(m_bullet);
			DeleteGO(this);
		}
	}
}
