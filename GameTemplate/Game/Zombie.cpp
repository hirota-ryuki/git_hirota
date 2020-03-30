#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "floor.h"
#include "Navimesh.h"
#include "Bullet.h"

#define PI 3.14f

Zombie::Zombie()
{
}

Zombie::~Zombie()
{
}

void Zombie::OnDestroy()
{
	DeleteGO(m_model);
}

bool Zombie::Start()
{
	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	//�v���C���[�̃C���X�^���X���擾�B
	m_player = m_game->GetPlayer();
	//���̃C���X�^���X���擾�B
	m_floor = m_game->GetFloor();
	//�i�r���b�V�����擾�B
	m_nav = m_floor->GetNavimesh();

	//�L�����R���̏�����
	m_charaCon.Init(
		30.0f,
		100.0f,
		m_position
	);
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/zombie/zombie.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	m_model->SetData(m_position, m_rotation);

	//�A�j���[�V�����B
	//�A�j���[�V�����N���b�v�̃��[�h�B
	m_animationClip[enAnimationClip_idle].Load(L"animData/zombie/idle.tka");
	m_animationClip[enAnimationClip_walk].Load(L"animData/zombie/walk.tka");
	m_animationClip[enAnimationClip_attack].Load(L"animData/zombie/attack.tka");
	m_animationClip[enAnimationClip_bite].Load(L"animData/zombie/bite.tka");
	m_animationClip[enAnimationClip_knockback].Load(L"animData/zombie/knockback.tka");
	m_animationClip[enAnimationClip_death].Load(L"animData/zombie/death.tka");
	//���[�v�t���O��ݒ肷��B
	m_animationClip[enAnimationClip_idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk].SetLoopFlag(true);
	m_animationClip[enAnimationClip_attack].SetLoopFlag(false);
	m_animationClip[enAnimationClip_bite].SetLoopFlag(false);
	m_animationClip[enAnimationClip_knockback].SetLoopFlag(false);
	m_animationClip[enAnimationClip_death].SetLoopFlag(false);
	//�A�j���[�V�����������B
	m_animation.Init(m_model->GetModel(), m_animationClip, enAnimationClip_num);

	//�R���C�_�[�̐ݒ�B
	m_collider.Create(m_boxSize);
	return true;
}

void Zombie::Update()
{
	//�ꎞ��~���Ă��Ȃ�������B
	if (!m_game->GetIsPose()) {
		switch (m_state) {
		case enState_idle:
			//�A�j���[�V�����̍Đ��B
			m_animation.Play(enAnimationClip_idle, 0.2f);

			//�U����ƃm�b�N�o�b�N��̃N�[���^�C���B
			if (m_coolTimer > 0) {
				m_coolTimer++;
				if (m_coolTimer > 50) {
					//�^�C�}�[�̃��Z�b�g�B
					m_coolTimer = 0;
				}
			}
			else {
				ChangeState();
			}
			break;
		case enState_walk:
			//�A�j���[�V�����̍Đ��B
			m_animation.Play(enAnimationClip_walk, 0.2f);
			Move();
			ChangeState();
			break;
		case enState_attack:
			//�A�j���[�V�����̍Đ��B
			m_animation.Play(enAnimationClip_attack, 0.2f);
			m_atkTimer++;
			if (m_atkTimer >= 60 && !m_isAttack) {
				//�U���B
				Attack();
				m_isAttack = true;
			}
			//�A�j���[�V�����̍Đ�������Ȃ�������B
			if (!m_animation.IsPlaying()) {
				//�ҋ@��ԂɑJ�ځB
				m_state = enState_idle;
				m_coolTimer++;
				m_atkTimer = 0;
				m_isAttack = false;
			}
			break; 
		case enState_bite:
			En_Bite();
			break;
		case enState_knockback:
			//�A�j���[�V�����̍Đ��B
			m_animation.Play(enAnimationClip_knockback, 0.1f);
			//�A�j���[�V�����̍Đ�������Ȃ�������B
			if (!m_animation.IsPlaying()) {
				//�ҋ@��ԂɑJ�ځB
				m_state = enState_idle;
				m_coolTimer++;
			}
			break;
		case enState_death:
			//�A�j���[�V�����̍Đ��B
			m_animation.Play(enAnimationClip_death, 0.2f);
			//�A�j���[�V�����̍Đ�������Ȃ�������B
			if (!m_animation.IsPlaying())
			{
				//DeleteGO(this);
				m_charaCon.RemoveRigidBoby();
			}
			break;
		default:
			break;
		}
		//�_���[�W���󂯂�B
		Damage();
		//���ʔ���B
		Death();
		//�d�́B
		if (!m_isBite) {
			m_moveSpeed.x = 0.f;
			m_moveSpeed.z = 0.f;
			m_moveSpeed.y -= 240.f * 1.f / 60.f;
			m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);
		}
		//�A�j���[�V�����̍X�V�B
		m_animation.Update(1.f / 60.f);
		//���W�̍X�V�B
		m_model->SetData(m_position, m_rotation);
	}
}

void Zombie::En_Bite()
{
	//�]���r�̌������v���C���[�ɂ��킹��B
	{
		//�v���C���[�ƓG�̊p�x�����߂�B
		CVector3 f = m_model->GetForward();
		f.z *= -1;
		CVector3 diff = m_player->GetPos() - m_position;
		//����p�B
		float degree = CalcViewingAngleDeg(f, diff);

		//�����p�x��10�x�ȓ��ł͂Ȃ�������B
		if (degree > 10.0f) {
			CVector3 cross;
			//�΂߂ɌX���Ȃ��悤��0�ɂ��Ă����B
			f.y = 0.0f;
			diff.y = 0.0f;
			cross.Cross(f, diff);
			cross.Normalize();
			CQuaternion qAddRot;
			qAddRot.SetRotationDeg(cross, degree - 10.0f);
			qAddRot.Multiply(m_rotation);
			//������v���C���[�N���X�ɏ������ڂ��B
			//�v���C���[�ƓG�̊p�x�����߂�B
			CVector3 f2 = m_player->GetSkinModelRender()->GetForward();
			f2.z *= -1;
			CVector3 diff2 = m_position - m_player->GetPos();
			//����p�B
			float degree2 = CalcViewingAngleDeg(f2, diff2);
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
	//�]���r���v���C���[�̈ʒu�Ɉړ�������B
	auto pPos = m_player->GetPos();
	pPos.x += 30.0f;
	pPos.y += 30.0f;
	m_position = pPos;
	//�A�j���[�V�����̍Đ��B
	m_animation.Play(enAnimationClip_bite, 0.1f);
	//�A�j���[�V�����̍Đ�������Ȃ�������B
	if (!m_animation.IsPlaying()) {
		//�ҋ@��ԂɑJ�ځB
		m_state = enState_idle;
		m_charaCon.ActiveMode(true);
		m_isBite = false;
		m_coolTimer++;
	}
}

struct CallBack : public btCollisionWorld::ConvexResultCallback
{
	//��Q�������邩�Ȃ�������
	bool isHit = false;
	//�Փ˂����珟��ɌĂ�ł����
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//��������
		isHit = true;
		return 0;
	}
};

void Zombie::ChangeState()
{
	//�ҋ@��ԂɑJ�ځB
	m_state = enState_idle;

	CVector3 diff = m_player->GetPos() - m_position;	
	if (diff.Length() < 200.0f) {
		//�U����ԂɑJ�ځB
		m_state = enState_attack;
	}
	else if (diff.Length() < 1000.0f) {
		if (!m_isFind) {
			//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�
			btTransform start, end;
			{
				//��]�̐ݒ�
				start.setIdentity();
				end.setIdentity();
				start.setOrigin(btVector3(m_position.x, m_position.y + 20.f, m_position.z));
				end.setOrigin(btVector3(m_player->GetPos().x, m_position.y + 20.f, m_player->GetPos().z));
			}
			CallBack callback;
			//start����end�܂ŃR���W�������ړ������ē����蔻������
			g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
			//�R���W�����Ƀq�b�g���Ȃ�������B
			if (callback.isHit == false) {
				//���s��ԂɑJ�ځB
				m_state = enState_walk;
				m_isFind = true;
			}
		}
		else {
			//���s��ԂɑJ�ځB
			m_state = enState_walk;
		}
	}
	//else {
	//	if (m_isFind) {
	//		//���s��ԂɑJ�ځB
	//		m_state = enState_walk;
	//	}
	//}
	//else if(diff.Length() < 3000.0f){
	//	if (m_isFind) {
	//		//���s��ԂɑJ�ځB
	//		m_state = enState_walk;
	//	}
	//}
}

void Zombie::Move()
{
	//�v���C���[�Ƃ̋������߂�������B
	CVector3 diff = m_player->GetPos() - m_position;
	//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�
	btTransform start, end;
	{
		//��]�̐ݒ�
		start.setIdentity();
		end.setIdentity();
		start.setOrigin(btVector3(m_position.x, m_position.y + 20.f, m_position.z));
		end.setOrigin(btVector3(m_player->GetPos().x, m_position.y + 20.f, m_player->GetPos().z));
	}
	CallBack callback;
	//start����end�܂ŃR���W�������ړ������ē����蔻������
	g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
	//�R���W�����Ƀq�b�g���Ȃ�������B
	//A*�����Ȃ��B
	if (diff.Length() < 400.f&&callback.isHit == false) {
		CVector3 moveDirection = m_player->GetPos() - m_position;
		moveDirection.y = 0.0f;
		moveDirection.Normalize();
		m_moveSpeed = moveDirection * m_speed;		//�ړ����x�����Z�B

		//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
		m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);

		//��]�B
		Rotation();
		//m_moveList.clear();
	}
	//A*������B
	else {
		//A*���s���Ă��Ȃ�������B
		if (!m_isAstar)
		{
			//A*���s���B
			Astar();
			m_isAstar = true;

		}
		else
		{
			//�z��̍Ō�܂Ńp�X��ǂݍ��ށB
			//�S�[���n�_�ɍs���܂ňړ��𑱂���B
			if (m_itr != m_moveList.end()) {
				//�p�X�Ɍ������܂ł��킶��ƈړ��B
				//�ړ���������B
				CVector3 moveDirection = *m_itr - m_position;
				moveDirection.y = 0.0f;
				moveDirection.Normalize();
				m_moveSpeed = moveDirection * m_speed;		//�ړ����x�����Z�B

				//�L�����N�^�[�R���g���[���[���g�p���āA���W���X�V�B
				m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);

				//��]�B
				Rotation();

				CVector3 diff = *m_itr - m_position;
				if (diff.Length() < 100.0f) { //todo �o�O�̌�
					m_isPoint = true;
				}
				//�v���C���[�̋߂��ɗ�����A*�����I���B
				CVector3 endDiff = m_player->GetPos() - m_position;
				//�ŏI�n�_���v���C���[������Ă�����A*��蒼���B
				CVector3 endDiff2 = m_player->GetPos() - m_endPos;
				if (endDiff.Length() < 200.0f || endDiff2.Length() > 300.0f) {
					m_isAstar = false;
					m_moveList.clear();
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
}

void Zombie::Astar()
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
	m_charaCon.SetPosition(m_position);
	m_endPos = endCell->centerPos;
	//A*���{
	m_aStar.AstarSearch(startCell, endCell);

	m_moveList = m_aStar.GetMoveList();
	m_itr = m_moveList.begin();	
}

void Zombie::Rotation()
{
	float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotation(CVector3::AxisY(), angle);
}

void Zombie::Attack()
{
	//�v���C���[�ւ̍U������B
	//�v���C���[�ƓG�̊p�x�����߂�B
	CVector3 f = m_model->GetForward();
	f.z *= -1;
	CVector3 diff = m_player->GetPos() - m_position;
	//����p�B
	float degree = CalcViewingAngleDeg(f, diff);
	//������200�ȓ����B
	if (diff.Length() < 200.0f
	//45�x�Ȃ�B
	//���ςɕ����͖����B
		&& degree < 45.0f) {
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
	m_atkTimer = 0;
}

void Zombie::Damage()
{
	//���Ƃ̏Փ˔�����s���B
	QueryGOs<Bullet>("bullet", [&](Bullet * bullet)->bool {
		//���̍��̓ǂݍ��݁B
		auto& model = m_model->GetModel();
		auto bone = model.FindBone(L"Head");
		bone->CalcWorldTRS(m_bonePos, m_boneRot, m_boneScale);
		//���̔���B
		CVector3 diff = bullet->GetPos() - m_bonePos;
		if (diff.Length() < 20.0f) {
			m_hp = m_hp - 5;
			m_state = enState_knockback;
			DeleteGO(bullet);
		}
		//�̂̔���B
		else {
			CVector3 pos = m_position;
			pos.y += 70.0f;
			CVector3 diff2 = bullet->GetPos() - pos;
			if (diff2.Length() < 60.0f) {
				m_hp--;
				DeleteGO(bullet);
			}
		}
		return true;
		});
}

void Zombie::Death()
{
	//HP��0�ȉ��ɂȂ�����
	if (m_hp <= 0) {
		//���ʁB
		m_state = enState_death;
	}
}

float Zombie::CalcViewingAngleDeg(CVector3 v1, CVector3 v2)
{	
	//���ρB
	float angleDot = v1.Dot(v2);
	//���ς���cos�Ƃ����߂�B
	float cos = angleDot / (v1.Length()*v2.Length());
	//cos�Ƃ���p�x(���W�A��)�����߂�B
	float radian = acos(cos);
	//���W�A������x�ɕϊ��B
	float degree = radian * 180.0f / PI;
	return degree;
}
