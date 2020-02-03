#include "stdafx.h"
#include "Player.h"
#include "Navimesh.h"
#include "Bullet.h"
#include "Zombie.h"

Player::Player()
{	
}

Player::~Player()
{
}

void Player::OnDestroy()
{
	DeleteGO(m_model);
	DeleteGOs("bullet");
}

bool Player::Start()
{
	//�L�����R���̏�����
	m_charaCon.Init(
		50.f,
		80.f,
		m_position
	);

	//�A�j���[�V�����N���b�v�̃��[�h�B
	m_animationClip[enAnimationClip_idle].Load(L"animData/player/idle.tka");
	m_animationClip[enAnimationClip_walk].Load(L"animData/player/walk.tka");
	m_animationClip[enAnimationClip_walk_left].Load(L"animData/player/walk_left.tka");
	m_animationClip[enAnimationClip_walk_right].Load(L"animData/player/walk_right.tka");
	m_animationClip[enAnimationClip_run].Load(L"animData/player/run.tka");
	m_animationClip[enAnimationClip_back].Load(L"animData/player/back.tka");
	m_animationClip[enAnimationClip_aim].Load(L"animData/player/aim.tka");
	m_animationClip[enAnimationClip_shot].Load(L"animData/player/shot.tka");

	//���[�v�t���O��ݒ肷��B
	m_animationClip[enAnimationClip_idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk_left].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk_right].SetLoopFlag(true);
	m_animationClip[enAnimationClip_run].SetLoopFlag(true);
	m_animationClip[enAnimationClip_back].SetLoopFlag(true);
	m_animationClip[enAnimationClip_aim].SetLoopFlag(true);
	m_animationClip[enAnimationClip_shot].SetLoopFlag(false);

	//cmo�t�@�C���̓ǂݍ��݁B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/player/player.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	m_model->SetData(m_position, m_rotation);

	//�A�j���[�V�����������B
	m_animation.Init(m_model->GetModel(), m_animationClip, enAnimationClip_num);

	//�X�v���C�g
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/damage.dds", 1280.f, 720.f);
	m_sprite->SetAlpha(m_alpha);

	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();

	return true;
}

void Player::ChangeState()
{
	//L3���������瑖��B
	if (g_pad[0].IsTrigger(enButtonLB3)) {
		m_isRun = !m_isRun;
	}

	//m_isRun���^�������瑖��B
	if (m_isRun) {
		m_state = enState_run;
	}
	else {
		m_state = enState_walk;
	}

	//�ҋ@�ɑJ�ځB
	if (g_pad[0].GetLStickXF() <= 0.1f&&g_pad[0].GetLStickXF() >= -0.1f) {
		if (g_pad[0].GetLStickYF() <= 0.1f&&g_pad[0].GetLStickYF() >= -0.1f) {
			m_state = enState_idle;
		}
	}

	//L2����������G�C���B
	if (g_pad[0].IsPress(enButtonLB2)) {
		m_state = enState_aim;
	}
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

	//�����Ă��邩�ǂ�������B
	if (m_isRun) {
		m_moveSpeed += cameraForward * lStick_y * m_runSpeed;		//�������ւ̈ړ����x�����B
		m_moveSpeed += cameraRight * lStick_x * m_runSpeed;			//�E�����ւ̈ړ����x�����Z�B
	}
	else {
		m_moveSpeed += cameraForward * lStick_y * m_speed;		//�������ւ̈ړ����x�����B
		m_moveSpeed += cameraRight * lStick_x * m_speed;		//�E�����ւ̈ړ����x�����Z�B
	}
	
	//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
	m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);
}

void Player::Update()
{	
	m_timer++;
	switch (m_state) {
	case enState_idle:
		//�A�j���[�V�����̍Đ��B
		m_animation.Play(enAnimationClip_idle, 0.4f);

		Move();
		Rotation();
		ChangeState();
		break;
	case enState_walk:
		//�A�j���[�V�����̍Đ��B
		//���S�ɉ��ړ���������B
		if (g_pad[0].GetLStickYF() == 0.0f) {
			//���B
			if (g_pad[0].GetLStickXF() > 0.0f) {
				m_animation.Play(enAnimationClip_walk_left, 0.4f);
			}
			//�E�B
			if (g_pad[0].GetLStickXF() < 0.0f) {
				m_animation.Play(enAnimationClip_walk_right, 0.4f);
			}
		}

		//�O�B
		if (g_pad[0].GetLStickYF() > 0.0f) {
			m_animation.Play(enAnimationClip_walk, 0.4f);
		}
		//�o�b�N�B
		if (g_pad[0].GetLStickYF() < 0.0f) {
			m_animation.Play(enAnimationClip_back, 0.4f);
		}
		
		Move();
		Rotation();
		ChangeState();
		break;
	case enState_run:
		//�A�j���[�V�����̍Đ��B
		m_animation.Play(enAnimationClip_run, 0.4f);			
		
		Move();
		Rotation();
		ChangeState();
		break;
	case enState_aim:
		//�A�j���[�V�����̍Đ��B
		m_animation.Play(enAnimationClip_aim, 0.4f);
		
		Move();
		Rotation();
		ChangeState();
		//R2���������猂�B
		if (g_pad[0].IsTrigger(enButtonRB2) && m_timer > 20) {
			m_timer = 0;
			m_state = enState_shot;
		
		}
		break;
	case enState_shot:
		//�A�j���[�V�����̍Đ��B
		m_animation.Play(enAnimationClip_shot, 0.4f);
		
		if (!m_isBullet) {
			//�e�ۂ̐����B
			m_bullet = NewGO<Bullet>(GOPrio_Defalut, "bullet");
			m_bullet->SetRot(m_rotation);
			m_isBullet = true;
		}

		Move();
		Rotation();

		//R2���������猂�B
		if (g_pad[0].IsTrigger(enButtonRB2) && m_timer > 20) {
			ChangeState();
			m_isBullet = false;
		}

		//�A�j���[�V�����̃t���[�����o������B
		if (!m_animation.IsPlaying()) {
			ChangeState();
			m_isBullet = false;
		}
		break;
	default:
		break;
	}
	//�񕜁B
	Heal();
	//HP���Ȃ��Ȃ����玀�ʁB
	Death();
	//�A�j���[�V�����̍X�V�B
	m_animation.Update(1.f / 60.f);
	//���[���h�s��̍X�V�B
	m_model->SetData(m_position, m_rotation);
}

void Player::Rotation()
{
	//�E�X�e�B�b�N�̓��͗ʂŁA���Z�����]�N�H�[�^�j�I�������B
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY(), 4.0f*g_pad[0].GetRStickXF());
	m_rotation.Multiply(qAddRot, m_rotation);
}

void Player::Heal()
{
	//�_���[�W���󂯂Ă��āB
	if (m_isDamage) {
		m_healTimer++;
		//��莞�Ԍo������B
		if (m_healTimer == m_healStartTime) {
			//�_���[�W���󂯂Ă��Ȃ���Ԃɂ���B
			//�i�񕜂ł����Ԃɂ���B�j
			m_isDamage = false;
			m_healTimer = 0;
		}
	}
	
	//HP�������Ă��āB
	if (m_hp < 10.0f) {
		//�_���[�W���󂯂Ă��Ȃ�������B
		if (!m_isDamage) {
			//���X�ɉ񕜂����邽�߂ɃJ�E���g�B
			m_heaIntervalTimer++;
			if (m_heaIntervalTimer == m_healIntervalTime) {
				//�񕜁B
				m_hp++;
				//�摜�𓧖��ɂ��Ă����B
				m_alpha = 1.0f - (m_hp / 10.0f);
				m_sprite->SetAlpha(m_alpha);
				m_heaIntervalTimer = 0;
			}
		}
	}
}

void Player::Damage()
{
	//�_���[�W���󂯂�B
	m_hp -= 1.0f;
	//�摜��s�����ɂ��Ă����B
	m_alpha = 1.0f - (m_hp / 10.0f);
	m_sprite->SetAlpha(m_alpha);
	m_isDamage = true;
	m_healTimer = 0;
}

void Player::Death()
{
	if (m_hp <= 0) {
		m_state = enState_death;
		m_game->GameOver();
	}
}