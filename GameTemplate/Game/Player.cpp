#include "stdafx.h"
#include "Player.h"
#include "Navimesh.h"
#include "Bullet.h"
#include "Zombie.h"

Player::Player()
{	
}

Player::~Player()
{
}

void Player::OnDestroy()
{
	DeleteGO(m_model);
	DeleteGOs("bullet");
}

bool Player::Start()
{
	//キャラコンの初期化
	m_charaCon.Init(
		50.f,
		80.f,
		m_position
	);

	//アニメーションクリップのロード。
	m_animationClip[enAnimationClip_idle].Load(L"animData/player/idle.tka");
	m_animationClip[enAnimationClip_walk].Load(L"animData/player/walk.tka");
	m_animationClip[enAnimationClip_walk_left].Load(L"animData/player/walk_left.tka");
	m_animationClip[enAnimationClip_walk_right].Load(L"animData/player/walk_right.tka");
	m_animationClip[enAnimationClip_run].Load(L"animData/player/run.tka");
	m_animationClip[enAnimationClip_back].Load(L"animData/player/back.tka");
	m_animationClip[enAnimationClip_aim].Load(L"animData/player/aim.tka");
	m_animationClip[enAnimationClip_shot].Load(L"animData/player/shot.tka");

	//ループフラグを設定する。
	m_animationClip[enAnimationClip_idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk_left].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk_right].SetLoopFlag(true);
	m_animationClip[enAnimationClip_run].SetLoopFlag(true);
	m_animationClip[enAnimationClip_back].SetLoopFlag(true);
	m_animationClip[enAnimationClip_aim].SetLoopFlag(true);
	m_animationClip[enAnimationClip_shot].SetLoopFlag(false);

	//cmoファイルの読み込み。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/player/player.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	m_model->SetData(m_position, m_rotation);

	//アニメーション初期化。
	m_animation.Init(m_model->GetModel(), m_animationClip, enAnimationClip_num);

	//スプライト
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/damage.dds", 1280.f, 720.f);
	m_sprite->SetAlpha(m_alpha);

	//ゲームのインスタンスを取得。
	m_game = GetGame();

	return true;
}

void Player::ChangeState()
{
	//L3を押したら走る。
	if (g_pad[0].IsTrigger(enButtonLB3)) {
		m_isRun = !m_isRun;
	}

	//m_isRunが真だったら走る。
	if (m_isRun) {
		m_state = enState_run;
	}
	else {
		m_state = enState_walk;
	}

	//待機に遷移。
	if (g_pad[0].GetLStickXF() <= 0.1f&&g_pad[0].GetLStickXF() >= -0.1f) {
		if (g_pad[0].GetLStickYF() <= 0.1f&&g_pad[0].GetLStickYF() >= -0.1f) {
			m_state = enState_idle;
		}
	}

	//L2を押したらエイム。
	if (g_pad[0].IsPress(enButtonLB2)) {
		m_state = enState_aim;
	}
}

void Player::Move()
{
	//左スティックの入力量を受け取る。
	float lStick_x = g_pad[0].GetLStickXF();
	float lStick_y = g_pad[0].GetLStickYF();

	//カメラの前方方向と右方向を取得。
	CVector3 cameraForward = g_camera3D.GetForward();
	CVector3 cameraRight = g_camera3D.GetRight();

	//XZ平面での前方方向、右方向に変換する。
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();

	//XZ成分の移動速度をクリア。
	m_moveSpeed.x = 0.f;
	m_moveSpeed.z = 0.f;
	m_moveSpeed.y -= 240.f * 1.f / 60.f;

	//走っているかどうか判定。
	if (m_isRun) {
		m_moveSpeed += cameraForward * lStick_y * m_runSpeed;		//奥方向への移動速度を代入。
		m_moveSpeed += cameraRight * lStick_x * m_runSpeed;			//右方向への移動速度を加算。
	}
	else {
		m_moveSpeed += cameraForward * lStick_y * m_speed;		//奥方向への移動速度を代入。
		m_moveSpeed += cameraRight * lStick_x * m_speed;		//右方向への移動速度を加算。
	}
	
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);
}

void Player::Update()
{	
	m_timer++;
	switch (m_state) {
	case enState_idle:
		//アニメーションの再生。
		m_animation.Play(enAnimationClip_idle, 0.4f);

		Move();
		Rotation();
		ChangeState();
		break;
	case enState_walk:
		//アニメーションの再生。
		//完全に横移動だったら。
		if (g_pad[0].GetLStickYF() == 0.0f) {
			//左。
			if (g_pad[0].GetLStickXF() > 0.0f) {
				m_animation.Play(enAnimationClip_walk_left, 0.4f);
			}
			//右。
			if (g_pad[0].GetLStickXF() < 0.0f) {
				m_animation.Play(enAnimationClip_walk_right, 0.4f);
			}
		}

		//前。
		if (g_pad[0].GetLStickYF() > 0.0f) {
			m_animation.Play(enAnimationClip_walk, 0.4f);
		}
		//バック。
		if (g_pad[0].GetLStickYF() < 0.0f) {
			m_animation.Play(enAnimationClip_back, 0.4f);
		}
		
		Move();
		Rotation();
		ChangeState();
		break;
	case enState_run:
		//アニメーションの再生。
		m_animation.Play(enAnimationClip_run, 0.4f);			
		
		Move();
		Rotation();
		ChangeState();
		break;
	case enState_aim:
		//アニメーションの再生。
		m_animation.Play(enAnimationClip_aim, 0.4f);
		
		Move();
		Rotation();
		ChangeState();
		//R2を押したら撃つ。
		if (g_pad[0].IsTrigger(enButtonRB2) && m_timer > 20) {
			m_timer = 0;
			m_state = enState_shot;
		
		}
		break;
	case enState_shot:
		//アニメーションの再生。
		m_animation.Play(enAnimationClip_shot, 0.4f);
		
		if (!m_isBullet) {
			//弾丸の生成。
			m_bullet = NewGO<Bullet>(GOPrio_Defalut, "bullet");
			m_bullet->SetRot(m_rotation);
			m_isBullet = true;
		}

		Move();
		Rotation();

		//R2を押したら撃つ。
		if (g_pad[0].IsTrigger(enButtonRB2) && m_timer > 20) {
			ChangeState();
			m_isBullet = false;
		}

		//アニメーションのフレーム数経ったら。
		if (!m_animation.IsPlaying()) {
			ChangeState();
			m_isBullet = false;
		}
		break;
	default:
		break;
	}
	//回復。
	Heal();
	//HPがなくなったら死ぬ。
	Death();
	//アニメーションの更新。
	m_animation.Update(1.f / 60.f);
	//ワールド行列の更新。
	m_model->SetData(m_position, m_rotation);
}

void Player::Rotation()
{
	//右スティックの入力量で、加算する回転クォータニオンを作る。
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY(), 4.0f*g_pad[0].GetRStickXF());
	m_rotation.Multiply(qAddRot, m_rotation);
}

void Player::Heal()
{
	//ダメージを受けていて。
	if (m_isDamage) {
		m_healTimer++;
		//一定時間経ったら。
		if (m_healTimer == m_healStartTime) {
			//ダメージを受けていない状態にする。
			//（回復できる状態にする。）
			m_isDamage = false;
			m_healTimer = 0;
		}
	}
	
	//HPが減っていて。
	if (m_hp < 10.0f) {
		//ダメージを受けていなかったら。
		if (!m_isDamage) {
			//徐々に回復させるためにカウント。
			m_heaIntervalTimer++;
			if (m_heaIntervalTimer == m_healIntervalTime) {
				//回復。
				m_hp++;
				//画像を透明にしていく。
				m_alpha = 1.0f - (m_hp / 10.0f);
				m_sprite->SetAlpha(m_alpha);
				m_heaIntervalTimer = 0;
			}
		}
	}
}

void Player::Damage()
{
	//ダメージを受ける。
	m_hp -= 1.0f;
	//画像を不透明にしていく。
	m_alpha = 1.0f - (m_hp / 10.0f);
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