#include "stdafx.h"
#include "Zombie.h"
#include "Bullet.h"
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

	//�L�����R���̏�����
	m_charaCon.Init(
		20.0f,
		100.0f,
		m_position,
		true
	);
	
	//���f���̏������B
	InitModel();
	
#ifdef DEBUG_MODE
	//debug cmo�t�@�C���̓ǂݍ��݁B
	m_debugModel = NewGO<SkinModelRender>(GOPrio_Defalut, "debug");
	m_debugModel->Init(L"modelData/debug/debugstick.cmo");
	m_debugrotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	CVector3 pos = m_position;
	pos.x += NOT_ASTAR_DISTANCE;
	m_debugModel->SetData(pos, m_debugrotation);
#endif //DEBUG_MODE

	//�A�j���[�V�����̏������B
	InitAnimationClips();

	//�R���|�[�l���g���������B
	InitComponents();

	for (auto& component : m_component) {
		component->Start();
	}
	return true;
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

void Zombie::InitAnimationClips()
{
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
}

void Zombie::InitModel()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/zombie/zombie.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	m_model->SetData(m_position, m_rotation);
	m_model->SetShadowReciever(true);
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
