#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "floor.h"
#include "Navimesh.h"
#include "Bullet.h"

#define PI 3.14

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
		30.0f,
		100.0f,
		m_position
	);
	//cmoファイルの読み込み。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/zombie/zombie.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	m_model->SetData(m_position, m_rotation);

	//アニメーション。
	//アニメーションクリップのロード。
	m_animationClip[enAnimationClip_idle].Load(L"animData/zombie/idle.tka");
	m_animationClip[enAnimationClip_walk].Load(L"animData/zombie/walk.tka");
	m_animationClip[enAnimationClip_attack].Load(L"animData/zombie/attack.tka");
	m_animationClip[enAnimationClip_death].Load(L"animData/zombie/death.tka");
	//ループフラグを設定する。
	m_animationClip[enAnimationClip_idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk].SetLoopFlag(true);
	m_animationClip[enAnimationClip_attack].SetLoopFlag(false);
	m_animationClip[enAnimationClip_death].SetLoopFlag(false);
	//アニメーション初期化。
	m_animation.Init(m_model->GetModel(), m_animationClip, enAnimationClip_num);

	//コライダーの設定。
	m_collider.Create(m_boxSize);
	return true;
}

void Zombie::Update()
{
	switch (m_state) {
	case enState_idle:
		//アニメーションの再生。
		m_animation.Play(enAnimationClip_idle, 0.2f);
		
		//攻撃後のクールタイム。
		if (m_coolTimer > 0) {
			m_coolTimer++;
			if (m_coolTimer > 50) {
				//タイマーのリセット。
				m_coolTimer = 0;
			}
		}
		else {
			ChangeState();
		}
		break;
	case enState_walk:
		//アニメーションの再生。
		m_animation.Play(enAnimationClip_walk, 0.2f);
		Move();
		ChangeState();
		break;
	case enState_attack:
		//アニメーションの再生。
		m_animation.Play(enAnimationClip_attack, 0.2f);
		m_atkTimer++;
		if (m_atkTimer == 50) {
			//攻撃。
			Attack();
		}

		//アニメーションの再生中じゃなかったら。
		if (!m_animation.IsPlaying()) {
			//待機状態に遷移。
			m_state = enState_idle;
			m_coolTimer++;
			m_atkTimer = 0;
		}
		break;
	case enState_death:
		//アニメーションの再生。
		m_animation.Play(enAnimationClip_death, 0.2f);
		//アニメーションの再生中じゃなかったら。
		if (!m_animation.IsPlaying()) 
		{
			DeleteGO(this);
		}
		break;
	default:
		break;
	}
	//ダメージを受ける。
	Damage();
	//死ぬ判定。
	Death();
	//重力。
	m_moveSpeed.x = 0.f;
	m_moveSpeed.z = 0.f;
	m_moveSpeed.y -= 240.f * 1.f / 60.f;
	m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);
	//アニメーションの更新。
	m_animation.Update(1.f / 60.f);
	//座標の更新。
	m_model->SetData(m_position, m_rotation);
	
}

struct CallBack : public btCollisionWorld::ConvexResultCallback
{
	//障害物があるかないか判定
	bool isHit = false;
	//衝突したら勝手に呼んでくれる
	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//当たった
		isHit = true;
		return 0;
	}
};

void Zombie::ChangeState()
{
	//待機状態に遷移。
	m_state = enState_idle;

	CVector3 diff = m_player->GetPos() - m_position;	
	if (diff.Length() < 200.0f) {
		//攻撃状態に遷移。
		m_state = enState_attack;
	}
	else if (diff.Length() < 1000.0f) {
		if (!m_isFind) {
			//コリジョンの移動の始点と終点の設定
			btTransform start, end;
			{
				//回転の設定
				start.setIdentity();
				end.setIdentity();
				start.setOrigin(btVector3(m_position.x, m_position.y + 20.f, m_position.z));
				end.setOrigin(btVector3(m_player->GetPos().x, m_position.y + 20.f, m_player->GetPos().z));
			}
			CallBack callback;
			//startからendまでコリジョンを移動させて当たり判定を取る
			g_physics.ConvexSweepTest((btConvexShape*)m_collider.GetBody(), start, end, callback);
			//コリジョンにヒットしなかったら。
			if (callback.isHit == false) {
				//歩行状態に遷移。
				m_state = enState_walk;
				m_isFind = true;
			}
		}
		else {
			//歩行状態に遷移。
			m_state = enState_walk;
		}
	}
}

void Zombie::Move()
{
	//A*を行っていなかったら。
	if (!m_isAstar)
	{
		//A*を行う。
		Astar();
		m_isAstar = true;
	}
	else
	{
		//配列の最後までパスを読み込む。
		//ゴール地点に行くまで移動を続ける。
		if (m_itr != m_moveList.end()) {
			//パスに向かうまでじわじわと移動。
			//移動する向き。
			CVector3 moveDirection = *m_itr - m_position;
			moveDirection.y = 0.0f;
			moveDirection.Normalize();
			m_moveSpeed = moveDirection * m_speed;		//移動速度を加算。

			//キャラクターコントローラーを使用して、座標を更新。
			m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);

			//回転。
			Rotation();

			CVector3 diff = *m_itr - m_position;
			if (diff.Length() < 100.0f) { //todo バグの元
				m_isPoint = true;
			}
			
			CVector3 endDiff = m_player->GetPos() - m_position;
			if (endDiff.Length() < 200.0f) {
				//プレイヤーの近くに来たらA*強制終了。
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
	m_charaCon.SetPosition(m_position);
	//A*実施
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
	//プレイヤーへの攻撃判定。
	//プレイヤーと敵の角度を求める。
	CVector3 diff = m_player->GetPos() - m_position;
	//内積。
	float angleDot = m_model->GetForward().Dot(diff);
	//内積から角度（ラジアン）を求める。
	float radian = 0.0f;
	radian = atan(angleDot);
	//ラジアンから度に変換。
	float degree = radian * 180.0f / PI;
	//距離が200以内かつ。
	if (diff.Length() < 200.0f
		//45度から、
		&& degree < -45.0f
		//135度の範囲だったら。
		&& degree > -135.0f) {
		//ダメージを与える。
		m_player->Damage();
	}
}

void Zombie::Damage()
{
	QueryGOs<Bullet>("bullet", [&](Bullet * bullet)->bool {
		CVector3 pos = m_position;
		pos.y += 100.0f;
		CVector3 diff = bullet->GetPos() - pos;
		if (diff.Length() < 100.0f) {
			m_hp--;
			DeleteGO(bullet);
		}
		return true;
		});
}

void Zombie::Death()
{
	//HPが0以下になったら
	if (m_hp <= 0) {
		//死ぬ。
		m_state = enState_death;
	}
}