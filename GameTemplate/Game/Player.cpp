#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"


Player::Player()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"modelData/unitychan/unityChan.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	
	//�X�v���C�g
	m_sprite.Init(L"sprite/moca.dds",1334,750);
	
	
	//�L�����R���̏�����
	/*m_charaCon.Init(
		100.f,
		800.f,
		m_position
	);*/
}


Player::~Player()
{
	
}

void Player::Move()
{
	//���X�e�B�b�N�̓��͗ʂ��󂯎��B
	float lStick_x = g_pad[0].GetLStickXF();
	float lStick_y = g_pad[0].GetLStickYF();

	//XZ�����̈ړ����x���N���A�B
	m_moveSpeed.x = 0.f;
	m_moveSpeed.z = 0.f;
	m_moveSpeed.y = 0.f;// * GameTime().GetFrameDeltaTime();

	m_moveSpeed.z +=  -lStick_y * m_speed;	//�������ւ̈ړ����x�����B
	m_moveSpeed.x +=  -lStick_x * m_speed;		//�E�����ւ̈ړ����x�����Z�B
	m_position += m_moveSpeed;

	//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
	//m_position = m_charaCon.Execute(1.f/60.f,m_moveSpeed);
}

void Player::Update()
{
	m_game->GetGame();
	
	Move();
	Rotation();
	
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

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
	//���f��
	m_model.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);

	
}