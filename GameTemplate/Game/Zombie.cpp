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
	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	//床のインスタンスを取得。
	m_floor = m_game->GetFloor();
	//ナビメッシュを取得。
	m_nav = m_floor->GetNavimesh();

	//キャラコンの初期化
	m_charaCon.Init(
		20.0f,
		100.0f,
		m_position,
		true
	);
	//cmoファイルの読み込み。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/zombie/zombie.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	m_model->SetData(m_position, m_rotation);
	m_model->SetShadowReciever(true);
	
#ifdef DEBUG_MODE
	//debug cmoファイルの読み込み。
	m_debugModel = NewGO<SkinModelRender>(GOPrio_Defalut, "debug");
	m_debugModel->Init(L"modelData/debug/debugstick.cmo");
	m_debugrotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	CVector3 pos = m_position;
	pos.x += NOT_ASTAR_DISTANCE;
	m_debugModel->SetData(pos, m_debugrotation);
#endif //DEBUG_MODE

	//アニメーション。
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

	//コライダーの設定。
	m_collider.Create(m_boxSize);

	//コンポーネントを初期化。
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
	
	//アニメーションの更新。
	m_animation.Update(1.0f / 60.0f);
	//座標の更新。
	m_model->SetData(m_position, m_rotation);


	//プレイヤーと敵の角度を求める。
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
	//ゾンビの向きをプレイヤーにあわせる。
	{
		//プレイヤーと敵の角度を求める。
		CVector3 f = m_model->GetForward();
		f.z *= -1;
		CVector3 diff = m_player->GetPos() - m_position;
		//視野角。
		float degree = CalcViewingAngleDeg(f, diff);

		//もし角度が10度以内ではなかったら。
		if (degree > 10.0f) {
			CVector3 cross;
			//斜めに傾かないように0にしておく。
			f.y = 0.0f;
			diff.y = 0.0f;
			cross.Cross(f, diff);
			cross.Normalize();
			CQuaternion qAddRot;
			qAddRot.SetRotationDeg(cross, degree - 10.0f);
			qAddRot.Multiply(m_rotation);
			//いずれプレイヤークラスに処理を移す。
			//プレイヤーと敵の角度を求める。
			CVector3 f2 = m_player->GetSkinModelRender()->GetForward();
			f2.z *= -1;
			CVector3 diff2 = m_position - m_player->GetPos();
			//視野角。
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
	//ゾンビをプレイヤーの位置に移動させる。
	auto pPos = m_player->GetPos();
	pPos.x += 30.0f;
	pPos.y += 30.0f;
	m_position = pPos;
	//アニメーションの再生。
	m_animation.Play(enAnimationClip_bite, 0.1f);
	//アニメーションの再生中じゃなかったら。
	if (!m_animation.IsPlaying()) {
		//待機状態に遷移。
		m_state = enState_idle;
		m_charaCon.ActiveMode(true);
		m_isBite = false;
		m_coolTimer++;
	}
}

struct FindCallBack : public btCollisionWorld::ConvexResultCallback
{
	//障害物があるかないか判定。
	bool isHit = false;
	//衝突したら勝手に呼んでくれる。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Map){
			//当たった。
			isHit = true;
		}
		return 0;
	}
};

struct AstarCallBack : public btCollisionWorld::ConvexResultCallback
{
	//障害物があるかないか判定。
	bool isHit = false;
	//衝突したら勝手に呼んでくれる。
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Map
			|| convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_RigidBody) {
			//当たった。
			isHit = true;
		}
		return 0;
	}
};
template<class TCallback> 
bool Zombie::RaycastToPlayer() const
{
	//コリジョンの移動の始点と終点の設定。
	btTransform start, end;
	{
		//回転の設定。
		start.setIdentity();
		end.setIdentity();
		start.setOrigin(btVector3(m_position.x, m_position.y + 20.f, m_position.z));
		end.setOrigin(btVector3(m_player->GetPos().x, m_position.y + 20.f, m_player->GetPos().z));
	}
	TCallback callback;
	//startからendまでコリジョンを移動させて当たり判定を取る。
	g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
	return callback.isHit;
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
