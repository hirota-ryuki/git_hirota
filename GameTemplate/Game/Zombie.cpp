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

	//キャラコンの初期化
	m_charaCon.Init(
		20.0f,
		100.0f,
		m_position,
		true
	);
	
	//モデルの初期化。
	InitModel();
	
#ifdef DEBUG_MODE
	//debug cmoファイルの読み込み。
	m_debugModel = NewGO<SkinModelRender>(GOPrio_Defalut, "debug");
	m_debugModel->Init(L"modelData/debug/debugstick.cmo");
	m_debugrotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	CVector3 pos = m_position;
	pos.x += NOT_ASTAR_DISTANCE;
	m_debugModel->SetData(pos, m_debugrotation);
#endif //DEBUG_MODE

	//アニメーションの初期化。
	InitAnimationClips();

	//コンポーネントを初期化。
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
	
	//アニメーションの更新。
	m_animation.Update(1.0f / 60.0f);
	//座標の更新。
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
	//アニメーションクリップのロード。
	m_animationClip[enAnimationClip_idle].Load(L"animData/zombie/idle.tka");
	m_animationClip[enAnimationClip_walk].Load(L"animData/zombie/walk.tka");
	m_animationClip[enAnimationClip_attack].Load(L"animData/zombie/attack.tka");
	m_animationClip[enAnimationClip_bite].Load(L"animData/zombie/bite.tka");
	m_animationClip[enAnimationClip_knockback].Load(L"animData/zombie/knockback.tka");
	m_animationClip[enAnimationClip_death].Load(L"animData/zombie/death.tka");
	//ループフラグを設定する。
	m_animationClip[enAnimationClip_idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk].SetLoopFlag(true);
	m_animationClip[enAnimationClip_attack].SetLoopFlag(false);
	m_animationClip[enAnimationClip_bite].SetLoopFlag(false);
	m_animationClip[enAnimationClip_knockback].SetLoopFlag(false);
	m_animationClip[enAnimationClip_death].SetLoopFlag(false);
	//アニメーション初期化。
	m_animation.Init(m_model->GetModel(), m_animationClip, enAnimationClip_num);
}

void Zombie::InitModel()
{
	//cmoファイルの読み込み。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/zombie/zombie.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	m_model->SetData(m_position, m_rotation);
	m_model->SetShadowReciever(true);
}

void Zombie::InitComponents()
{
	//移動コンポーネントを生成。
	m_component.push_back(new ZombieMove);
	//ステートマシンのコンポーネントを生成。
	m_component.push_back(new ZombieStateMachine);
	//ダメージ処理のコンポーネントを生成。
	m_component.push_back(new ZombieDamage);
	//死んだときの処理のコンポーネントを生成。
	m_component.push_back(new ZombieDeath);

	//コンポーネントとゾンビを関連づける。
	for (auto& component : m_component) {
		component->BindZombie(this);
	}
}

float Zombie::CalcViewingAngleDeg(CVector3 v1, CVector3 v2)
{	
	////内積。
	//float angleDot = v1.Dot(v2);
	////内積からcosθを求める。
	//float cos = angleDot / (v1.Length()*v2.Length());
	////cosθから角度(ラジアン)を求める。
	//float radian = acos(cos);
	////ラジアンから度に変換。
	//float degree = radian * 180.0f / PI;

	//内積。
	v1.Normalize();
	v2.Normalize();
	float angleDot = v1.Dot(v2);
	//cosθから角度(ラジアン)を求める。
	float radian = acos(angleDot);
	//ラジアンから度に変換。
	float degree = radian * 180.0f / PI;
	return degree;
}
