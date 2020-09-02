#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "Navimesh.h"
#include "Bullet.h"
#include "Zombie.h"
#include "UI.h"
#include "SpotLight.h"

Player::Player()
{	
}

Player::~Player()
{
}

void Player::OnDestroy()
{
	DeleteGO(m_model);
	DeleteGO(m_sprite);
	DeleteGOs("bullet");
}

bool Player::Start()
{
	////�L�����R���̏�����
	//m_charaCon.Init(
	//	50.f,
	//	80.f,
	//	m_positionm
	//);
	
	//���̃L�����R���̏�����
	m_charaCon.Init(
		40.f,
		90.f,
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
	m_animationClip[enAnimationClip_reload].Load(L"animData/player/reload.tka");
	m_animationClip[enAnimationClip_lie].Load(L"animData/player/lie.tka");

	//���[�v�t���O��ݒ肷��B
	m_animationClip[enAnimationClip_idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk_left].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk_right].SetLoopFlag(true);
	m_animationClip[enAnimationClip_run].SetLoopFlag(true);
	m_animationClip[enAnimationClip_back].SetLoopFlag(true);
	m_animationClip[enAnimationClip_aim].SetLoopFlag(true);
	m_animationClip[enAnimationClip_shot].SetLoopFlag(false);
	m_animationClip[enAnimationClip_reload].SetLoopFlag(false);
	m_animationClip[enAnimationClip_lie].SetLoopFlag(false);

	//cmo�t�@�C���̓ǂݍ��݁B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/player/player.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	m_model->SetData(m_position, m_rotation);

	//�A�j���[�V�����������B
	m_animation.Init(m_model->GetModel(), m_animationClip, enAnimationClip_num);

	//�_���[�W�摜�B
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/damage.dds", 1280.f, 720.f);
	m_sprite->SetAlpha(m_alpha);
	//m_sprite->ActiveMode(false);

	//�Ə��摜�֌W�B
	//�X�v���C�g
	m_aimSprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_aimSprite->Init(L"sprite/aim.dds", 60.0f, 60.0f);
	m_aimSprite->ActiveMode(false);

	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	m_sl = m_game->GetSpotLight();

	Inv_AddItem(L"�e��", 24);
	return true;
}

void Player::ChangeState()
{
	//L3���������瑖��B
	if (g_pad[0].IsTrigger(enButtonLB3)) {
		m_isRun = !m_isRun;
	}

	//�����Ă��Ȃ����ɑ����Ԃ���������B
	if (g_pad[0].GetLStickXF() == 0 && g_pad[0].GetLStickYF() == 0)
	{
		m_isRun = false;
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
		m_isRun = false;
	}
	
	//X���������烊���[�h�B
	if (g_pad[0].IsTrigger(enButtonX)) {
		//�e�������Ă�����B
		if (m_capacity < m_maxCapacity) {
			//�莝���̒e����0����Ȃ�������B
			if (m_stack > 0) {
				m_state = enState_reload;
				m_isRun = false;
			}
		}
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

	//�G�C�����Ă���Ƃ��͒x������B
	if (m_state == enState_aim || m_state == enState_reload) {
		m_moveSpeed += cameraForward * lStick_y * m_speed / 3.0f;		//�������ւ̈ړ����x�����B
		m_moveSpeed += cameraRight * lStick_x * m_speed / 3.0f;		//�E�����ւ̈ړ����x�����Z�B

	}
	//�����Ă��邩�ǂ�������B
	else if (m_isRun) {
		//�����Ă���B
		m_moveSpeed += cameraForward * lStick_y * m_runSpeed;		//�������ւ̈ړ����x�����B
		m_moveSpeed += cameraRight * lStick_x * m_runSpeed;			//�E�����ւ̈ړ����x�����Z�B
	}
	//�����Ă���B
	else {
		m_moveSpeed += cameraForward * lStick_y * m_speed;		//�������ւ̈ړ����x�����B
		m_moveSpeed += cameraRight * lStick_x * m_speed;		//�E�����ւ̈ړ����x�����Z�B
		
	}
	
	//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
	m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);
}

void Player::Update()
{	
	//m_state = enState_lie;

	GetGameCameraInst();
	//1�񂾂�UI�̃C���X�^���X���擾�B
	if (m_game->GetUI() != nullptr&&m_ui == nullptr) {
		//UI�̃C���X�^���X���擾�B
		m_ui = m_game->GetUI();
	}
	//�G�C�����Ă�����c�e��\���B
	if (m_state == enState_aim || m_state == enState_shot || m_state == enState_reload) {
		m_ui->CangeActives(true);
		m_aimSprite->ActiveMode(true);
	}
	//���i�͔�\���B
	else {
		m_ui->CangeActives(false);
		m_aimSprite->ActiveMode(false);
	}
	if (m_isBite) {
		m_state = enState_lie;
	}
	//�ꎞ��~���Ă��Ȃ�������B
	if (!m_game->GetIsPose()) {
		m_timer++;
		switch (m_state) {
		case enState_idle:
			En_Idle();
			break;
		case enState_walk:
			En_Walk();
			break;
		case enState_run:
			En_Run();
			break;
		case enState_aim:
			En_Aim();
			break;
		case enState_shot:
			En_Shot();
			break;
		case enState_reload:
			En_Reload();
			break;
		case enState_lie:
			En_Lie();
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
		//���D���C�g�B
		ActressLight();
		//�����d���B
		SetLight();
	}
}

void Player::En_Idle()
{
	//�A�j���[�V�����̍Đ��B
	m_animation.Play(enAnimationClip_idle, 0.4f);

	Move();
	Rotation();
	ChangeState();
}

void Player::En_Walk()
{
	//�A�j���[�V�����̍Đ��B
	//���S�ɉ��ړ���������B
	if (g_pad[0].GetLStickYF() < 0.2f&&g_pad[0].GetLStickYF() > -0.2f)
	{
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
	if (g_pad[0].GetLStickYF() > 0.2f) {
		m_animation.Play(enAnimationClip_walk, 0.4f);
	}
	//�o�b�N�B
	if (g_pad[0].GetLStickYF() < -0.2f) {
		m_animation.Play(enAnimationClip_back, 0.4f);
	}

	Move();
	Rotation();
	ChangeState();
}

void Player::En_Run()
{
	//�A�j���[�V�����̍Đ��B
	m_animation.Play(enAnimationClip_run, 0.4f);

	Move();
	Rotation();
	ChangeState();
}

void Player::En_Aim()
{
	//�A�j���[�V�����̍Đ��B
	m_animation.Play(enAnimationClip_aim, 0.4f);

	Move();
	Rotation();
	ChangeState();
	//R2���������猂�B
	if (g_pad[0].IsTrigger(enButtonRB2) && m_timer > 20) {
		//���X�e�[�g�ɑJ�ځB
		m_timer = 0;
		m_state = enState_shot;
	}
}

void Player::En_Shot()
{
	//�c�e����0����Ȃ�������B
	if (m_capacity > 0) {
		//�A�j���[�V�����̍Đ��B
		m_animation.Play(enAnimationClip_shot, 0.4f);

		if (!m_isBullet) {
			//�e�ۂ̐����B
			m_bullet = NewGO<Bullet>(GOPrio_Defalut, "bullet");
			m_bullet->SetRot(m_rotation);
			m_isBullet = true;
			m_capacity--;
			//�����V���b�g�Đ���SE
			CSoundSource* m_seGunShot = new CSoundSource;
			m_seGunShot->Init(L"sound/gun/gun_shot.wav");
			m_seGunShot->Play(false);
		}
	}
	else {
		if (!m_isEmpty) {
			//��C�B
			//�����V���b�g�Đ���SE�B
			CSoundSource* seGunEmpty = new CSoundSource;
			seGunEmpty->Init(L"sound/gun/gun_empty.wav");
			seGunEmpty->Play(false);
			m_isEmpty = true;
		}
	}

	Move();
	Rotation();

	//R2���������猂�B
	if (g_pad[0].IsTrigger(enButtonRB2) && m_timer > 20) {
		ChangeState();
		m_isBullet = false;
		m_isEmpty = false;
	}

	//�A�j���[�V�����̃t���[�����o������B
	if (!m_animation.IsPlaying()) {
		ChangeState();
		m_isBullet = false;
	}
	//��C����������B
	if (m_isEmpty) {
		ChangeState();
		m_isEmpty = false;
	}
}

void Player::En_Reload()
{
	//�A�j���[�V�����̍Đ��B
	m_animation.Play(enAnimationClip_reload, 0.4f);

	//�}�K�W���ɂ��߂�e�̐��B
	int reloadbullet = m_maxCapacity - m_capacity;
	//�莝���̒e�����}�K�W���ɂ��߂���e����菭�Ȃ�������B
	if (m_stack < reloadbullet) {
		//�莝���̒e��S�����U���A
		m_capacity += m_stack;
		//�莝���̒e����ɂ���B
		m_stack = 0;
	}
	else {
		m_capacity += reloadbullet;
		m_stack -= reloadbullet;
	}

	Move();
	Rotation();
	//�A�j���[�V�����̍Đ�������Ȃ�������B
	if (!m_animation.IsPlaying()) {
		ChangeState();
	}
}

void Player::En_Lie()
{
	//m_charaCon.ActiveMode(false);
	if (!m_isOldCameraInfo) {
		//�o�b�N�A�b�v�B
		m_oldplayer = m_gamecamera->GetTargetFromPlayer();
		m_oldpos = m_gamecamera->GetPosFromTarget();
		m_oldAddY = m_gamecamera->GetAddY();
		m_isOldCameraInfo = true;
	}
	m_gamecamera->SetTargetFromPlayer(50.0f);
	m_gamecamera->SetPosFromTarget(-200.0f);
	m_gamecamera->SetAddY(30.0f);
	//�A�j���[�V�����̍Đ��B
	m_animation.Play(enAnimationClip_lie, 0.4f);
	//�A�j���[�V�����̍Đ�������Ȃ�������B
	if (!m_animation.IsPlaying()) {
		ChangeState();
		m_charaCon.ActiveMode(true);
		m_isBite = false;
		//���[�h�B
		m_gamecamera->SetTargetFromPlayer(m_oldplayer);
		m_gamecamera->SetPosFromTarget(m_oldpos);
		m_gamecamera->SetAddY(m_oldAddY);
		m_isOldCameraInfo = false;
	}
}

void Player::Rotation()
{
	//�E�X�e�B�b�N�̓��͗ʂŁA���Z�����]�N�H�[�^�j�I�������B
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY(), 2.0f*g_pad[0].GetRStickXF());
	m_rotation.Multiply(qAddRot, m_rotation);
}

void Player::Heal()
{
	//�_���[�W���󂯂Ă��āB
	if (m_isDamage) {
		m_healTimer++;
		//��莞�Ԍo������B
		if (m_healTimer >= m_healStartTime) {
			//�_���[�W���󂯂Ă��Ȃ���Ԃɂ���B
			//�i�񕜂ł����Ԃɂ���B�j
			m_isDamage = false;
			m_healTimer = 0;
		}
	}
	
	//HP�������Ă��āB
	if (m_hp < m_maxhp) {
		//�_���[�W���󂯂Ă��Ȃ�������B
		if (!m_isDamage) {
			//���X�ɉ񕜂����邽�߂ɃJ�E���g�B
			m_heaIntervalTimer++;
			if (m_heaIntervalTimer == m_healIntervalTime) {
				//�񕜁B
				m_hp++;
				//�摜�𓧖��ɂ��Ă����B
				m_alpha = 1.0f - (m_hp / m_maxhp);
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
	m_alpha = 1.0f - (m_hp / m_maxhp);
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

void Player::ActressLight()
{
	auto dir = g_camera3D.GetForward();
	dir.y = 0.0f;
	dir.Normalize();
	dir.y -= 0.2f;
	m_model->SetLight(0, dir);
}

void Player::SetLight()
{ 
	CVector3 color;
	CVector3 pos;
	if (g_pad[0].IsTrigger(enButtonA)) {
		m_isLight = !m_isLight;
	}
	if (m_isLight) {
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		pos = m_position;
		pos.y += 120.0f;
	}
	else {
		color = CVector3::Zero();
	}
	for (int i = 0; i < NUM_SPOT_LIG; i++) {
			m_sl->SetLight(pos, color, m_model->GetForward(), 600.0f, i);
	}
}
