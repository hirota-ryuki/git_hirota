#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "Navimesh.h"

Player::Player()
{
	m_position.y += 1000.f;
	//�L�����R���̏�����
	m_charaCon.Init(
		50.f,
		//������2�{�ɂ��Ă���
		//�K�i�Ƃ��ɍs���Ƒ����̉��ɂ����蔻�肪
		//�o��Ǝv�����Ǌ撣����(�@�L�́M)b�ޯ!
		80.f * 2.0f,
		m_position
	);
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
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
	m_moveSpeed.y -= 240.f * 1.f / 60.f;

	m_moveSpeed += cameraForward * lStick_y * m_speed;	//�������ւ̈ړ����x�����B
	m_moveSpeed += cameraRight * lStick_x * m_speed;		//�E�����ւ̈ړ����x�����Z�B
	//m_position += m_moveSpeed;
	//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
	m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);
}

void Player::Update()
{
	//Game�N���X�̎擾
	//m_game->GetGame();
	
	Move();
	Rotation();
	//if (m_game->GetGame() != nullptr) {
	//	//Game�N���X�̎擾
	//	m_game->GetGame();
	//	if (m_game->GetNavi() != nullptr) {
	//		m_nav = m_game->GetNavi();
	//		auto cell = m_nav->GetCell();
	//		m_position = cell[0]->centerPos;
	//	}
	//}
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