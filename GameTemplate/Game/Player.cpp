#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"


Player::Player()
{
	m_position.y += 500.f;
	//�L�����R���̏�����
	m_charaCon.Init(
		10.f,
		80.f,
		m_position
	);

	//cmo�t�@�C���̓ǂݍ��݁B
	m_model = NewGO<SkinModelRender>();
	m_model->Init(L"modelData/unitychan/unityChan.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	//m_position.y += 200.f;
	m_model->SetData(m_position, m_rotation, m_scale);

}


Player::~Player()
{
	
}

void Player::Move()
{
	//���X�e�B�b�N�̓��͗ʂ��󂯎��B
	float lStick_x = g_pad[0].GetLStickXF();
	float lStick_y = g_pad[0].GetLStickYF();

	//�J�����̑O�������ƉE�������擾�B
	CVector3 cameraForward = g_camera3D.GetForward();
	CVector3 cameraRight = g_camera3D.GetRight();

	//XZ���ʂł̑O�������A�E�����ɕϊ�����B
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();

	//XZ�����̈ړ����x���N���A�B
	m_moveSpeed.x = 0.f;
	m_moveSpeed.z = 0.f;
	m_moveSpeed.y -= 60.f * 1.f / 60.f;

	m_moveSpeed += cameraForward * lStick_y * m_speed;	//�������ւ̈ړ����x�����B
	m_moveSpeed += cameraRight * lStick_x * m_speed;		//�E�����ւ̈ړ����x�����Z�B
	//m_position += m_moveSpeed;

	//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
	m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);
}

void Player::Update()
{
	//Game�N���X�̎擾
	m_game->GetGame();
	
	Move();
	Rotation();
	
	//���[���h�s��̍X�V�B
	m_model->SetData(m_position, m_rotation, m_scale);

}


void Player::Rotation()
{
	//�E�X�e�B�b�N�̓��͗ʂŁA���Z�����]�N�H�[�^�j�I�������B
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY(), 4.f * g_pad[0].GetRStickXF());
	m_rotation.Multiply(qAddRot, m_rotation);
}

void Player::Draw()
{
}