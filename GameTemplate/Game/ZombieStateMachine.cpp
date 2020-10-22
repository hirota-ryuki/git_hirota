#include "stdafx.h"
#include "ZombieStateMachine.h"
#include "physics/CollisionAttr.h"
#include "Player.h"
#include "floor.h"
#include "Navimesh.h"
#include "Zombie.h"

#define PI 3.14f

void ZombieStateMachine::Start()
{
	//�C���X�^���X�̎擾�B
	InitInstance();

	//�R���C�_�[�̐ݒ�B
	m_collider.Create(m_boxSize);
}

void ZombieStateMachine::InitInstance()
{
	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	//�v���C���[�̃C���X�^���X���擾�B
	m_player = m_game->GetPlayer();
	//���̃C���X�^���X���擾�B
	m_floor = m_game->GetFloor();
	//�i�r���b�V�����擾�B
	m_nav = m_floor->GetNavimesh();
}

void ZombieStateMachine::Update()
{
	switch (m_zombie->m_state) {
	case enState_idle:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(enAnimationClip_idle, m_comprementTime[enAnimationClip_idle]);

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
			m_zombie->m_animation.Play(enAnimationClip_idle, m_comprementTime[enAnimationClip_idle]);
		}
		else {
			m_zombie->m_animation.Play(enAnimationClip_walk, m_comprementTime[enAnimationClip_walk]);
		}
		Move();
		ChangeState();
		break;
	case enState_attack:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(enAnimationClip_attack, m_comprementTime[enAnimationClip_attack]);
		m_atkTimer++;
		if (m_atkTimer >= ATK_INTERVAL && !m_isAttack) {
			//�U���B
			Attack();
			m_isAttack = true;
		}
		//�A�j���[�V�����̍Đ�������Ȃ�������B
		if (!m_zombie->m_animation.IsPlaying()) {
			//�ҋ@��ԂɑJ�ځB
			m_zombie->m_state = enState_idle;
			m_zombie->m_coolTimer++;
			m_atkTimer = 0;
			m_isAttack = false;
		}
		break;
	case enState_bite:
		En_Bite();
		break;
	case enState_knockback:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(enAnimationClip_knockback, m_comprementTime[enAnimationClip_knockback]);
		//�A�j���[�V�����̍Đ�������Ȃ�������B
		if (!m_zombie->m_animation.IsPlaying()) {
			//�ҋ@��ԂɑJ�ځB
			m_zombie->m_state = enState_idle;
			m_zombie->m_coolTimer++;
		}
		break;
	case enState_death:
		//�A�j���[�V�����̍Đ��B
		m_zombie->m_animation.Play(enAnimationClip_death, m_comprementTime[enAnimationClip_death]);
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

struct FindCallBack : public btCollisionWorld::ConvexResultCallback
{
	//��Q�������邩�Ȃ�������B
	bool isHit = false;
	//�Փ˂����珟��ɌĂ�ł����B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Map) {
			//���������B
			isHit = true;
		}
		return 0;
	}
};

void ZombieStateMachine::ChangeState()
{
	//�ҋ@��ԂɑJ�ځB
	m_zombie->m_state = enState_idle;

	float distSq = m_player->CalcDistanceSQFrom(m_zombie->m_position);

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
			m_zombie->m_state = enState_attack;
		}
		else {
			//���s��ԂɑJ�ځB
			m_zombie->m_state = enState_walk;
		}
	}
}

struct AstarCallBack : public btCollisionWorld::ConvexResultCallback
{
	//��Q�������邩�Ȃ�������B
	bool isHit = false;
	//�Փ˂����珟��ɌĂ�ł����B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Map
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_RigidBody) {
			//���������B
			isHit = true;
		}
		return 0;
	}
};

void ZombieStateMachine::Move()
{
	//�v���C���[�Ƃ̋������߂�������B
	//A*�����Ȃ��B
	float distSq = m_player->CalcDistanceSQFrom(m_zombie->m_position);
	if (distSq < NOT_ASTAR_DISTANCE_SQ) {
		//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�B
		if (RaycastToPlayer<AstarCallBack>() == false) {
			//�v���C���[�ɑ΂��ă��C�L���X�g���s���āA��Q���ɂԂ���Ȃ������B
			CVector3 moveDirection = m_player->CalcDirectionXZFrom(m_zombie->m_position);
			m_moveSpeed = moveDirection * WALK_SPEED;		//�ړ����x�����Z�B

			//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
			m_zombie->m_position = m_zombie->m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
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
	float endDistSQ = m_player->CalcDistanceSQFrom(m_zombie->m_position);
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
			CVector3 moveDirection = *m_itr - m_zombie->m_position;
			moveDirection.y = 0.0f;
			moveDirection.Normalize();
			m_moveSpeed = moveDirection * WALK_SPEED;		//�ړ����x�����Z�B

			//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
			m_zombie->m_position = m_zombie->m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
			//��]�B
			Rotation();

			CVector3 diff = *m_itr - m_zombie->m_position;
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
	CVector3 enemyDiff = allCell[0]->centerPos - m_zombie->m_position;
	Cell* startCell = allCell[0];

	//player�̏��B
	CVector3 playerDiff = allCell[0]->centerPos - m_player->GetPos();
	Cell* endCell = allCell[0];

	//�Z���̐�������
	for (auto &all : allCell)
	{
		//newEnemyDiff�̍X�V
		CVector3 newEnemyDiff = all->centerPos - m_zombie->m_position;

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
	m_zombie->m_position = startCell->centerPos;
	m_zombie->m_model->SetPos(m_zombie->m_position);
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
		m_zombie->m_rotation.SetRotation(CVector3::AxisY(), angle);
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
	m_zombie->m_rotation.Multiply(f);
	CVector3 diff = m_player->GetPos() - m_zombie->m_position;
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

void ZombieStateMachine::En_Bite()
{
	//�]���r�̌������v���C���[�ɂ��킹��B
	AngleCorrection();

	//�]���r���v���C���[�̈ʒu�Ɉړ�������B
	auto pPos = m_player->GetPos();
	pPos.x += 30.0f;
	pPos.y += 30.0f;
	m_zombie->m_position = pPos;
	//�A�j���[�V�����̍Đ��B
	m_zombie->m_animation.Play(enAnimationClip_bite, m_comprementTime[enAnimationClip_bite]);
	//�A�j���[�V�����̍Đ�������Ȃ�������B
	if (!m_zombie->m_animation.IsPlaying()) {
		//�ҋ@��ԂɑJ�ځB
		m_zombie->m_state = enState_idle;
		m_zombie->m_charaCon.ActiveMode(true);
		m_isBite = false;
		m_zombie->m_coolTimer++;
	}
}

void ZombieStateMachine::AngleCorrection()
{
	//�v���C���[�ƓG�̊p�x�����߂�B
	CVector3 zombieforword = m_zombie->m_model->GetForward();
	zombieforword.z *= -1;
	CVector3 playerfromzombie = m_player->GetPos() - m_zombie->m_position;
	//����p�B
	float angle = m_zombie->CalcViewingAngleDeg(zombieforword, playerfromzombie);
	//�����p�x��10�x�ȓ��ł͂Ȃ�������B
	if (angle > 10.0f) {
		CVector3 cross;
		//�΂߂ɌX���Ȃ��悤��0�ɂ��Ă����B
		zombieforword.y = 0.0f;
		playerfromzombie.y = 0.0f;
		cross.Cross(zombieforword, playerfromzombie);
		cross.Normalize();
		CQuaternion qAddRot;
		qAddRot.SetRotationDeg(cross, angle - 10.0f);
		qAddRot.Multiply(m_zombie->m_rotation);
		//������v���C���[�N���X�ɏ������ڂ��B
		//�v���C���[�ƓG�̊p�x�����߂�B
		CVector3 f2 = m_player->GetSkinModelRender()->GetForward();
		f2.z *= -1;
		CVector3 diff2 = m_zombie->m_position - m_player->GetPos();
		//����p�B
		float degree2 = m_zombie->CalcViewingAngleDeg(f2, diff2);
		CVector3 cross2;
		f2.y = 0.0f;
		diff2.y = 0.0f;
		cross2.Cross(f2, diff2);
		cross2.Normalize();
		CQuaternion qAddRot2;
		qAddRot2.SetRotationDeg(cross2, degree2 - 10.0f);
		qAddRot2.Multiply(m_player->GetRot());
	}
}

template<class TCallback>
bool ZombieStateMachine::RaycastToPlayer() const
{
	//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�B
	btTransform start, end;
	{
		//��]�̐ݒ�B
		start.setIdentity();
		end.setIdentity();
		start.setOrigin(btVector3(m_zombie->m_position.x, m_zombie->m_position.y + 20.f, m_zombie->m_position.z));
		end.setOrigin(btVector3(m_player->GetPos().x, m_zombie->m_position.y + 20.f, m_player->GetPos().z));
	}
	TCallback callback;
	//start����end�܂ŃR���W�������ړ������ē����蔻������B
	g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
	return callback.isHit;
}
