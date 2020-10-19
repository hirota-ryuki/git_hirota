#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "floor.h"
#include "Navimesh.h"
#include "Bullet.h"
#include "physics/CollisionAttr.h"
#include "ZombieMove.h"
#include "ZombieStateMachine.h"
#include "ZombieDamage.h"
#include "ZombieDeath.h"

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
#ifdef DEBUG_MODE
	DeleteGOs("debug");
#endif //DEBUG_MODE
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
		20.0f,
		100.0f,
		m_position,
		true
	);
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/zombie/zombie.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	m_model->SetData(m_position, m_rotation);
	m_model->SetShadowReciever(true);
	
#ifdef DEBUG_MODE
	//debug cmo�t�@�C���̓ǂݍ��݁B
	m_debugModel = NewGO<SkinModelRender>(GOPrio_Defalut, "debug");
	m_debugModel->Init(L"modelData/debug/debugstick.cmo");
	m_debugrotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	CVector3 pos = m_position;
	pos.x += NOT_ASTAR_DISTANCE;
	m_debugModel->SetData(pos, m_debugrotation);
#endif //DEBUG_MODE

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

	//�R���|�[�l���g���������B
	InitComponents();
	return true;
}

void Zombie::Update()
{		
}

void Zombie::Update_NotPause()
{
	for (auto& component : m_component) {
		component->Update();
	}
	
	//�A�j���[�V�����̍X�V�B
	m_animation.Update(1.0f / 60.0f);
	//���W�̍X�V�B
	m_model->SetData(m_position, m_rotation);


	//�v���C���[�ƓG�̊p�x�����߂�B
	/*CVector3 f = m_model->GetForward();
	f.z *= -1;*/
	//f.Normalize();
	//CVector3 dir = CVector3::One();
#ifdef DEBUG_MODE
	CVector3 dir;
	dir.Set(0.0f, 0.0f, -1.0f);
	m_rotation.Multiply(dir);
	dir.Normalize();
	float angle = dir.Dot(CVector3::AxisY());

	CVector3 cross;
	cross.Cross(dir, CVector3::AxisY());
	cross.Normalize();

	CQuaternion Rot;
	Rot.SetRotation(cross, acos(angle));

	//m_debugModel->SetData(m_position, Rot);
	CVector3 pos = m_position;
	pos.x += NOT_ASTAR_DISTANCE;
	m_debugModel->SetPos(pos);
#endif //DEBUG_MODE

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

struct FindCallBack : public btCollisionWorld::ConvexResultCallback
{
	//��Q�������邩�Ȃ�������B
	bool isHit = false;
	//�Փ˂����珟��ɌĂ�ł����B
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Map){
			//���������B
			isHit = true;
		}
		return 0;
	}
};

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
template<class TCallback> 
bool Zombie::RaycastToPlayer() const
{
	//�R���W�����̈ړ��̎n�_�ƏI�_�̐ݒ�B
	btTransform start, end;
	{
		//��]�̐ݒ�B
		start.setIdentity();
		end.setIdentity();
		start.setOrigin(btVector3(m_position.x, m_position.y + 20.f, m_position.z));
		end.setOrigin(btVector3(m_player->GetPos().x, m_position.y + 20.f, m_player->GetPos().z));
	}
	TCallback callback;
	//start����end�܂ŃR���W�������ړ������ē����蔻������B
	g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
	return callback.isHit;
}

void Zombie::InitComponents()
{
	//�ړ��R���|�[�l���g�𐶐��B
	m_component.push_back(new ZombieMove);
	//�X�e�[�g�}�V���̃R���|�[�l���g�𐶐��B
	m_component.push_back(new ZombieStateMachine);
	//�_���[�W�����̃R���|�[�l���g�𐶐��B
	m_component.push_back(new ZombieDamage);
	//���񂾂Ƃ��̏����̃R���|�[�l���g�𐶐��B
	m_component.push_back(new ZombieDeath);

	//�R���|�[�l���g�ƃ]���r���֘A�Â���B
	for (auto& component : m_component) {
		component->BindZombie(this);
	}
}

float Zombie::CalcViewingAngleDeg(CVector3 v1, CVector3 v2)
{	
	////���ρB
	//float angleDot = v1.Dot(v2);
	////���ς���cos�Ƃ����߂�B
	//float cos = angleDot / (v1.Length()*v2.Length());
	////cos�Ƃ���p�x(���W�A��)�����߂�B
	//float radian = acos(cos);
	////���W�A������x�ɕϊ��B
	//float degree = radian * 180.0f / PI;

	//���ρB
	v1.Normalize();
	v2.Normalize();
	float angleDot = v1.Dot(v2);
	//cos�Ƃ���p�x(���W�A��)�����߂�B
	float radian = acos(angleDot);
	//���W�A������x�ɕϊ��B
	float degree = radian * 180.0f / PI;
	return degree;
}
