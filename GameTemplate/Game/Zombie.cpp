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
void Zombie::ChangeState()
{
	//待機状態に遷移。
	m_state = enState_idle;

	float distSq = m_player->CalcDistanceSQFrom(m_position);
	
	//見つけたかどうかを判定。
	//距離判定だが、壁越しに見つけないようにする。
	if (!m_isFind) {
		if (distSq < FIND_DISTANCE_SQ) {
			//プレイヤーに対してレイキャスト。
			if (RaycastToPlayer<FindCallBack>() == false) {
				//プレイヤーに対してレイを飛ばして
				//当たらなかったので遮蔽物はないので発見した。
				m_isFind = true;
			}
		}
	}
	
	if (m_isFind) {
		if (distSq < ATTACK_DISTANCE_SQ) {
			//攻撃状態に遷移。
			m_state = enState_attack;
		}
		else {
			//歩行状態に遷移。
			m_state = enState_walk;
		}
	}
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
void Zombie::Move()
{
	//プレイヤーとの距離が近かったら。
	//A*をしない。
	float distSq = m_player->CalcDistanceSQFrom(m_position);
	if (distSq < NOT_ASTAR_DISTANCE_SQ){
		//コリジョンの移動の始点と終点の設定。
		if (RaycastToPlayer<AstarCallBack>() == false){
			//プレイヤーに対してレイキャストを行って、障害物にぶつからなかった。
			CVector3 moveDirection = m_player->CalcDirectionXZFrom(m_position);
			m_moveSpeed = moveDirection * WALK_SPEED;		//移動速度を加算。

			//キャラクターコントローラーを使用して、座標を更新。
			m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
			//回転。
			Rotation();
		}
		else {
			//障害物があったらA*をする。
			Move_AStar();
		}
	}
	//A*をする。
	else {
		Move_AStar();
	}
}
bool Zombie::IsEndAStarForce() const
{
	float endDistSQ = m_player->CalcDistanceSQFrom(m_position);
	//最終地点よりプレイヤーが離れていたらA*やり直し。
	float endDistSQ2 = m_player->CalcDistanceSQFrom(m_endPos);
	if (endDistSQ < END_ASTSR_OF_NEER_PLAYER_SQ
		|| endDistSQ2 > END_ASTSR_OF_LEAVE_FINALPOINT_SQ) {
		//強制終了。
		return true;
	}
	return false;
}
void Zombie::Move_AStar()
{
	//A*を行っていなかったら。
	if (!m_isAstar)
	{
		//A*を行う。
		m_moveList.clear();
		Astar();
		m_isAstar = true;
		m_aStarCount++;
	}
	else
	{
		//配列の最後までパスを読み込む。
		//ゴール地点に行くまで移動を続ける。
		if (m_itr != m_moveList.end()) {
			//A*カウンタのリセット。
			m_aStarCount = 0;
			//パスに向かうまでじわじわと移動。
			//移動する向き。
			CVector3 moveDirection = *m_itr - m_position;
			moveDirection.y = 0.0f;
			moveDirection.Normalize();
			m_moveSpeed = moveDirection * WALK_SPEED;		//移動速度を加算。

			//キャラクターコントローラーを使用して、座標を更新。
			m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
			//回転。
			Rotation();

			CVector3 diff = *m_itr - m_position;
			if (diff.Length() < ARRIVAL_DISTANCE) { //todo バグの元
				m_isPoint = true;
			}
			//A*強制終了する必要があるなら強制終了。
			if (IsEndAStarForce()) {
				m_isAstar = false;
			}
			
			//n番目のパスに着いたら。
			//n = m_moveListの要素の場所（今移動しようとしているパスの場所）。
			if (m_isPoint)
			{
				//次のパスを指し示す。
				m_itr++;
				//フラグをリセット。
				m_isPoint = false;
			}
		}
		else
		{
			//移動終了。
			m_isAstar = false;
		}
	}
}

void Zombie::Astar()
{
	//作成したナビメッシュを取得。
	auto allCell = m_nav->GetCell();
	//enemyから一番近いセルとplayerから一番近いセルを求める。
	//enemyの情報。
	CVector3 enemyDiff = allCell[0]->centerPos - m_position;
	Cell* startCell = allCell[0];

	//playerの情報。
	CVector3 playerDiff = allCell[0]->centerPos - m_player->GetPos();
	Cell* endCell = allCell[0];

	//セルの数だけ回す
	for (auto &all : allCell)
	{
		//newEnemyDiffの更新
		CVector3 newEnemyDiff = all->centerPos - m_position;

		//enemyから一番近いセルを求める
		//enemyDiffより距離が短いセルがあったら
		if (enemyDiff.Length() > newEnemyDiff.Length())
		{
			//差とセルを登録
			enemyDiff = newEnemyDiff;
			startCell = all;
		}

		//newPlayerDiffの更新
		CVector3 newPlayerDiff = all->centerPos - m_player->GetPos();

		//playerから一番近いセルを求める
		//playerDiffより距離が短いセルがあったら
		if (playerDiff.Length() > newPlayerDiff.Length())
		{
			//差とセルを登録
			playerDiff = newPlayerDiff;
			endCell = all;
		}
	}
	m_position = startCell->centerPos;
	m_model->SetPos(m_position);
	//m_charaCon.SetPosition(m_position);
	m_endPos = endCell->centerPos;
	//A*実施
	m_aStar.AstarSearch(startCell, endCell);

	m_moveList = m_aStar.GetMoveList();
	m_itr = m_moveList.begin();	
}

void Zombie::Rotation()
{
	//移動ベクトルが0の時は、0度なので3dsMaxで設定されている前方方向に向いてしまう。
	//だから、移動ベクトルが0の時を除外する。
	if (m_moveSpeed.Length() > 0.1f) {
		float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
}

void Zombie::Attack()
{
	//プレイヤーへの攻撃判定。
	//プレイヤーと敵の角度を求める。
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


	//視野角。
	//float degree = CalcViewingAngleDeg(f, diff);
	//距離が200以内かつ。
	if (diff.Length() < ATTACK_DISTANCE
	//45度なら。
	//内積に符号は無い。
		&& degree < ATTACK_VIEWING_ANGLE) {
		//ダメージを与える。
		m_player->Damage();
	}
	/*
	//指の骨の読み込み。
	auto& model = m_model->GetModel();
	auto bone = model.FindBone(L"RightHandMiddle2");
	bone->CalcWorldTRS(m_bonePos, m_boneRot, m_boneScale);
	//あたり判定。
	auto playerPos = m_player->GetPos();
	//プレイヤーの判定の位置を上げる。
	playerPos.y += 100.0f;
	CVector3 diff = playerPos - m_bonePos;
	//骨とプレイヤーが当たったら。
	//if (diff.Length() < 50.0f) {
		//ステートを噛みつき状態にする。
		m_state = enState_bite;
		//プレイヤーの噛みつきフラグを有効にする。
		m_player->SetIsBite(true);
		//キャラコンの衝突判定を消す。
		m_charaCon.ActiveMode(false);
		m_player->GetCharaCon()->ActiveMode(false);
		
		//ゾンビの噛みつきフラグを有効にする。
		m_isBite = true;
	//}*/

	//タイマーのリセット。
	m_atkTimer = 0;
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
