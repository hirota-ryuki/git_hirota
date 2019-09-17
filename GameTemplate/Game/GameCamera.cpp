#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"
#include "Player.h"


GameCamera::GameCamera()
{
	m_game = &Game::GetInstance();
	m_player = m_game->GetPlayer();
}


GameCamera::~GameCamera()
{
}

void GameCamera::Update()
{
	//�J�����Ɍ��ݒn���Z�b�g�B
	CVector3 l_setpos = m_player->GetPos();
	l_setpos.y += 100.0f;
	m_camera.SetPosition(l_setpos);

	// ���E�����߂�
	float toCameraPosRotAngleold = toCameraPosRotAngle;
	toCameraPosRotAngleold += (g_pad[0].GetRStickYF()*1.5f);

	float t = 60.0f;

	if (toCameraPosRotAngle > t) {
		toCameraPosRotAngleold = t;
	}
	if (toCameraPosRotAngle < -70.0f) {
		toCameraPosRotAngleold = -70.0f;
	}
	toCameraPosRotAngle = toCameraPosRotAngleold;


	//�v���C���[���璍���_�܂ł̃x�N�g����ݒ�B
	//z���O
	CVector3 l_toCameraTarget;
	l_toCameraTarget.Set(0.0f, 0.0f, 1000.0f);
	m_player->GetRot().Multiply(l_toCameraTarget);

	CQuaternion qAddRot2;
	CVector3 axis2;
	CVector3 y2 = { 0.0f,1.0f,0.0f };
	axis2.Cross(l_toCameraTarget, y2);
	axis2.Normalize();
	qAddRot2.SetRotationDeg(axis2, toCameraPosRotAngle);
	qAddRot2.Multiply(l_toCameraTarget);


	//���_���璍���_�܂ł̃x�N�g����.�ݒ�B

	CVector3 l_toCameraPos;
	l_toCameraPos.Set(60.0f, 0.0f, -1200.0f);
	m_player->GetRot().Multiply(l_toCameraPos);

	CQuaternion qAddRot;
	CVector3 axis;
	CVector3 y = { 0.0f,-1.0f,0.0f };
	axis.Cross(l_toCameraPos, y);
	axis.Normalize();
	qAddRot.SetRotationDeg(axis, toCameraPosRotAngle);
	qAddRot.Multiply(l_toCameraPos);



	CVector3 m_pos = m_player->GetPos();
	m_pos.y += 100.0f;

	//�����_���v�Z����B
	m_target = m_pos + l_toCameraTarget;
	//���_���v�Z����B
	m_pos = m_target + l_toCameraPos;

	//�o�l�J�����ɒ����_�Ǝ��_��ݒ肷��B
	m_camera.SetTarget(m_target);
	m_camera.SetPosition(m_pos);

	//�o�l�J�����̍X�V�B
	m_camera.Update();
}

void GameCamera::Draw()
{
	
}