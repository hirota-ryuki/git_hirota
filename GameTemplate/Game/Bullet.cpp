#include "stdafx.h"
#include "Bullet.h"
#include "Player.h"
#include "GameCamera.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

void Bullet::OnDestroy()
{
	DeleteGO(m_model);
}

bool Bullet::Start()
{
	//�e�ۂ𐶐��B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/bullet/bullet2.cmo");
	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	//�v���C���[�̃C���X�^���X���擾�B
	m_player = m_game->GetPlayer();
	//�e�ۂ̍��W�Ƀv���C���[�̍��W��������B
	CVector3 l_pos = m_player->GetPos();
	l_pos.y += 140.0f;
	m_position = l_pos;
	m_model->SetPos(m_position);
	//�J�����̃C���X�^���X���擾�B
	m_gamecamera = m_game->GetCamera();
	m_moveSpeed = m_gamecamera->GetTarget() - m_position;
	m_moveSpeed.Normalize();
	return true;
}

void Bullet::Update()
{
	//�e�ۂ��ړ�������B
	m_position += m_moveSpeed * m_speed;
	//��]�B
	Rotation();
	//�X�L�����f�������_�[�ɍ��W��`����B
	m_model->SetData(m_position, m_rotation);
	//�^�C�}�[�����Z����B
	m_deleteTimer++;
	if (m_deleteTimer == 50) {
		//�^�C�}�[��50�ɂȂ�����C���X�^���X���폜����B
		DeleteGO(this);
	}
}

void Bullet::Rotation()
{
	float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotation(CVector3::AxisY(), angle);
}