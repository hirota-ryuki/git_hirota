#include "stdafx.h"
#include "ZombieStateMachine.h"
#include "Zombie.h"

void ZombieStateMachine::Update()
{
	switch (m_zombie->m_state) {
	case enState_idle:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_idle, 0.2f);

		//�U����ƃm�b�N�o�b�N��̃N�[���^�C���B
		if (m_zombie->m_coolTimer > 0) {
			m_zombie->m_coolTimer++;
			if (m_zombie->m_coolTimer > 50) {
				//�^�C�}�[�̃��Z�b�g�B
				m_zombie->m_coolTimer = 0;
			}
		}
		else {
			ChangeState();
		}
		break;
	case enState_walk:
		//�A�j���[�V�����̍Đ��B
		if (m_aStarCount > 100) {
			m_zombie->m_animation.Play(m_zombie->enAnimationClip_idle, 0.2f);
		}
		else {
			m_zombie->m_animation.Play(m_zombie->enAnimationClip_walk, 0.2f);
		}
		Move();
		ChangeState();
		break;
	case enState_attack:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_attack, 0.2f);
		m_atkTimer++;
		if (m_atkTimer >= ATK_INTERVAL && !m_isAttack) {
			//�U���B
			Attack();
			m_isAttack = true;
		}
		//�A�j���[�V�����̍Đ�������Ȃ�������B
		if (!m_zombie->m_animation.IsPlaying()) {
			//�ҋ@��ԂɑJ�ځB
			m_zombie->m_state = m_zombie->enState_idle;
			m_zombie->m_coolTimer++;
			m_atkTimer = 0;
			m_isAttack = false;
		}
		break;
	case enState_bite:
		m_zombie->En_Bite();
		break;
	case enState_knockback:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_knockback, 0.1f);
		//�A�j���[�V�����̍Đ�������Ȃ�������B
		if (!m_zombie->m_animation.IsPlaying()) {
			//�ҋ@��ԂɑJ�ځB
			m_zombie->m_state = m_zombie->enState_idle;
			m_zombie->m_coolTimer++;
		}
		break;
	case enState_death:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(m_zombie->enAnimationClip_death, 0.2f);
		//�A�j���[�V�����̍Đ�������Ȃ�������B
		if (!m_zombie->m_animation.IsPlaying())
		{
			//DeleteGO(this);
			m_zombie->m_charaCon.RemoveRigidBoby();
		}
		break;
	default:
		break;
	}
}
void ZombieStateMachine::ChangeState()
{
	//�ҋ@��ԂɑJ�ځB
	m_state = enState_idle;

	float distSq = m_player->CalcDistanceSQFrom(m_position);

	//���������ǂ����𔻒�B
	//�������肾���A�ǉz���Ɍ����Ȃ��悤�ɂ���B
	if (!m_isFind) {
		if (distSq < FIND_DISTANCE_SQ) {
			//�v���C���[�ɑ΂��ă��C�L���X�g�B
			if (RaycastToPlayer<FindCallBack>() == false) {
				//�v���C���[�ɑ΂��ă��C���΂���
				//������Ȃ������̂ŎՕ����͂Ȃ��̂Ŕ��������B
				m_isFind = true;
			}
		}
	}

	if (m_isFind) {
		if (distSq < ATTACK_DISTANCE_SQ) {
			//�U����ԂɑJ�ځB
			m_state = enState_attack;
		}
		else {
			//���s��ԂɑJ�ځB
			m_state = enState_walk;
		}
	}
}

void ZombieStateMachine::Move()
{
	//�v���C���[�Ƃ̋������߂�������B
	//A*�����Ȃ��B
	float distSq = m_player->CalcDistanceSQFrom(m_position);
	if (distSq < NOT_ASTAR_DISTANCE_SQ) {
		//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�B
		if (RaycastToPlayer<AstarCallBack>() == false) {
			//�v���C���[�ɑ΂��ă��C�L���X�g���s���āA��Q���ɂԂ���Ȃ������B
			CVector3 moveDirection = m_player->CalcDirectionXZFrom(m_position);
			m_moveSpeed = moveDirection * WALK_SPEED;		//�ړ����x�����Z�B

			//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
			m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
			//��]�B
			Rotation();
		}
		else {
			//��Q������������A*������B
			Move_AStar();
		}
	}
	//A*������B
	else {
		Move_AStar();
	}
}

bool ZombieStateMachine::IsEndAStarForce() const
{
	float endDistSQ = m_player->CalcDistanceSQFrom(m_position);
	//�ŏI�n�_���v���C���[������Ă�����A*��蒼���B
	float endDistSQ2 = m_player->CalcDistanceSQFrom(m_endPos);
	if (endDistSQ < END_ASTSR_OF_NEER_PLAYER_SQ
		|| endDistSQ2 > END_ASTSR_OF_LEAVE_FINALPOINT_SQ) {
		//�����I���B
		return true;
	}
	return false;
}

void ZombieStateMachine::Move_AStar()
{
	//A*���s���Ă��Ȃ�������B
	if (!m_isAstar)
	{
		//A*���s���B
		m_moveList.clear();
		Astar();
		m_isAstar = true;
		m_aStarCount++;
	}
	else
	{
		//�z��̍Ō�܂Ńp�X��ǂݍ��ށB
		//�S�[���n�_�ɍs���܂ňړ��𑱂���B
		if (m_itr != m_moveList.end()) {
			//A*�J�E���^�̃��Z�b�g�B
			m_aStarCount = 0;
			//�p�X�Ɍ������܂ł��킶��ƈړ��B
			//�ړ���������B
			CVector3 moveDirection = *m_itr - m_position;
			moveDirection.y = 0.0f;
			moveDirection.Normalize();
			m_moveSpeed = moveDirection * WALK_SPEED;		//�ړ����x�����Z�B

			//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
			m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
			//��]�B
			Rotation();

			CVector3 diff = *m_itr - m_position;
			if (diff.Length() < ARRIVAL_DISTANCE) { //todo �o�O�̌�
				m_isPoint = true;
			}
			//A*�����I������K�v������Ȃ狭���I���B
			if (IsEndAStarForce()) {
				m_isAstar = false;
			}

			//n�Ԗڂ̃p�X�ɒ�������B
			//n = m_moveList�̗v�f�̏ꏊ�i���ړ����悤�Ƃ��Ă���p�X�̏ꏊ�j�B
			if (m_isPoint)
			{
				//���̃p�X���w�������B
				m_itr++;
				//�t���O�����Z�b�g�B
				m_isPoint = false;
			}
		}
		else
		{
			//�ړ��I���B
			m_isAstar = false;
		}
	}
}

void ZombieStateMachine::Astar()
{
	//�쐬�����i�r���b�V�����擾�B
	auto allCell = m_nav->GetCell();
	//enemy�����ԋ߂��Z����player�����ԋ߂��Z�������߂�B
	//enemy�̏��B
	CVector3 enemyDiff = allCell[0]->centerPos - m_position;
	Cell* startCell = allCell[0];

	//player�̏��B
	CVector3 playerDiff = allCell[0]->centerPos - m_player->GetPos();
	Cell* endCell = allCell[0];

	//�Z���̐�������
	for (auto &all : allCell)
	{
		//newEnemyDiff�̍X�V
		CVector3 newEnemyDiff = all->centerPos - m_position;

		//enemy�����ԋ߂��Z�������߂�
		//enemyDiff��苗�����Z���Z������������
		if (enemyDiff.Length() > newEnemyDiff.Length())
		{
			//���ƃZ����o�^
			enemyDiff = newEnemyDiff;
			startCell = all;
		}

		//newPlayerDiff�̍X�V
		CVector3 newPlayerDiff = all->centerPos - m_player->GetPos();

		//player�����ԋ߂��Z�������߂�
		//playerDiff��苗�����Z���Z������������
		if (playerDiff.Length() > newPlayerDiff.Length())
		{
			//���ƃZ����o�^
			playerDiff = newPlayerDiff;
			endCell = all;
		}
	}
	m_position = startCell->centerPos;
	m_model->SetPos(m_position);
	//m_charaCon.SetPosition(m_position);
	m_endPos = endCell->centerPos;
	//A*���{
	m_aStar.AstarSearch(startCell, endCell);

	m_moveList = m_aStar.GetMoveList();
	m_itr = m_moveList.begin();
}

void ZombieStateMachine::Rotation()
{
	//�ړ��x�N�g����0�̎��́A0�x�Ȃ̂�3dsMax�Őݒ肳��Ă���O�������Ɍ����Ă��܂��B
	//������A�ړ��x�N�g����0�̎������O����B
	if (m_moveSpeed.Length() > 0.1f) {
		float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
}

void ZombieStateMachine::Attack()
{
	//�v���C���[�ւ̍U������B
	//�v���C���[�ƓG�̊p�x�����߂�B
	CVector3 f;
	//f.Set(0.0f, 0.0f, 1.0f);
	f.Set(CVector3::AxisZ());
	f.Normalize();
	m_rotation.Multiply(f);
	CVector3 diff = m_player->GetPos() - m_position;
	diff.Normalize();
	float dot = f.Dot(diff);
	float angle = acos(dot);
	float degree = angle * 180.0f / PI;


	//����p�B
	//float degree = CalcViewingAngleDeg(f, diff);
	//������200�ȓ����B
	if (diff.Length() < ATTACK_DISTANCE
		//45�x�Ȃ�B
		//���ςɕ����͖����B
		&& degree < ATTACK_VIEWING_ANGLE) {
		//�_���[�W��^����B
		m_player->Damage();
	}
	/*
	//�w�̍��̓ǂݍ��݁B
	auto& model = m_model->GetModel();
	auto bone = model.FindBone(L"RightHandMiddle2");
	bone->CalcWorldTRS(m_bonePos, m_boneRot, m_boneScale);
	//�����蔻��B
	auto playerPos = m_player->GetPos();
	//�v���C���[�̔���̈ʒu���グ��B
	playerPos.y += 100.0f;
	CVector3 diff = playerPos - m_bonePos;
	//���ƃv���C���[������������B
	//if (diff.Length() < 50.0f) {
		//�X�e�[�g�����݂���Ԃɂ���B
		m_state = enState_bite;
		//�v���C���[�̊��݂��t���O��L���ɂ���B
		m_player->SetIsBite(true);
		//�L�����R���̏Փ˔���������B
		m_charaCon.ActiveMode(false);
		m_player->GetCharaCon()->ActiveMode(false);

		//�]���r�̊��݂��t���O��L���ɂ���B
		m_isBite = true;
	//}*/

	//�^�C�}�[�̃��Z�b�g�B
	m_atkTimer = 0;
}
