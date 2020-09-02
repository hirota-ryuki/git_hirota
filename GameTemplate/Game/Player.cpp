#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "Navimesh.h"
#include "Bullet.h"
#include "Zombie.h"
#include "UI.h"
#include "SpotLight.h"

Player::Player()
{	
}

Player::~Player()
{
}

void Player::OnDestroy()
{
	DeleteGO(m_model);
	DeleteGO(m_sprite);
	DeleteGOs("bullet");
}

bool Player::Start()
{
	////キャラコンの初期化
	//m_charaCon.Init(
	//	50.f,
	//	80.f,
	//	m_positionm
	//);
	
	//今のキャラコンの初期化
	m_charaCon.Init(
		40.f,
		90.f,
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
	m_animationClip[enAnimationClip_reload].Load(L"animData/player/reload.tka");
	m_animationClip[enAnimationClip_lie].Load(L"animData/player/lie.tka");

	//ループフラグを設定する。
	m_animationClip[enAnimationClip_idle].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk_left].SetLoopFlag(true);
	m_animationClip[enAnimationClip_walk_right].SetLoopFlag(true);
	m_animationClip[enAnimationClip_run].SetLoopFlag(true);
	m_animationClip[enAnimationClip_back].SetLoopFlag(true);
	m_animationClip[enAnimationClip_aim].SetLoopFlag(true);
	m_animationClip[enAnimationClip_shot].SetLoopFlag(false);
	m_animationClip[enAnimationClip_reload].SetLoopFlag(false);
	m_animationClip[enAnimationClip_lie].SetLoopFlag(false);

	//cmoファイルの読み込み。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/player/player.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	m_model->SetData(m_position, m_rotation);

	//アニメーション初期化。
	m_animation.Init(m_model->GetModel(), m_animationClip, enAnimationClip_num);

	//ダメージ画像。
	m_sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_sprite->Init(L"sprite/damage.dds", 1280.f, 720.f);
	m_sprite->SetAlpha(m_alpha);
	//m_sprite->ActiveMode(false);

	//照準画像関係。
	//スプライト
	m_aimSprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_aimSprite->Init(L"sprite/aim.dds", 60.0f, 60.0f);
	m_aimSprite->ActiveMode(false);

	//ゲームのインスタンスを取得。
	m_game = GetGame();
	m_sl = m_game->GetSpotLight();

	Inv_AddItem(L"弾薬", 24);
	return true;
}

void Player::ChangeState()
{
	//L3を押したら走る。
	if (g_pad[0].IsTrigger(enButtonLB3)) {
		m_isRun = !m_isRun;
	}

	//動いていない時に走り状態を解除する。
	if (g_pad[0].GetLStickXF() == 0 && g_pad[0].GetLStickYF() == 0)
	{
		m_isRun = false;
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
		m_isRun = false;
	}
	
	//Xを押したらリロード。
	if (g_pad[0].IsTrigger(enButtonX)) {
		//弾が減っていたら。
		if (m_capacity < m_maxCapacity) {
			//手持ちの弾数が0じゃなかったら。
			if (m_stack > 0) {
				m_state = enState_reload;
				m_isRun = false;
			}
		}
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

	//エイムしているときは遅くする。
	if (m_state == enState_aim || m_state == enState_reload) {
		m_moveSpeed += cameraForward * lStick_y * m_speed / 3.0f;		//奥方向への移動速度を代入。
		m_moveSpeed += cameraRight * lStick_x * m_speed / 3.0f;		//右方向への移動速度を加算。

	}
	//走っているかどうか判定。
	else if (m_isRun) {
		//走っている。
		m_moveSpeed += cameraForward * lStick_y * m_runSpeed;		//奥方向への移動速度を代入。
		m_moveSpeed += cameraRight * lStick_x * m_runSpeed;			//右方向への移動速度を加算。
	}
	//歩いている。
	else {
		m_moveSpeed += cameraForward * lStick_y * m_speed;		//奥方向への移動速度を代入。
		m_moveSpeed += cameraRight * lStick_x * m_speed;		//右方向への移動速度を加算。
		
	}
	
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);
}

void Player::Update()
{	
	//m_state = enState_lie;

	GetGameCameraInst();
	//1回だけUIのインスタンスを取得。
	if (m_game->GetUI() != nullptr&&m_ui == nullptr) {
		//UIのインスタンスを取得。
		m_ui = m_game->GetUI();
	}
	//エイムしていたら残弾を表示。
	if (m_state == enState_aim || m_state == enState_shot || m_state == enState_reload) {
		m_ui->CangeActives(true);
		m_aimSprite->ActiveMode(true);
	}
	//普段は非表示。
	else {
		m_ui->CangeActives(false);
		m_aimSprite->ActiveMode(false);
	}
	if (m_isBite) {
		m_state = enState_lie;
	}
	//一時停止していなかったら。
	if (!m_game->GetIsPose()) {
		m_timer++;
		switch (m_state) {
		case enState_idle:
			En_Idle();
			break;
		case enState_walk:
			En_Walk();
			break;
		case enState_run:
			En_Run();
			break;
		case enState_aim:
			En_Aim();
			break;
		case enState_shot:
			En_Shot();
			break;
		case enState_reload:
			En_Reload();
			break;
		case enState_lie:
			En_Lie();
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
		//女優ライト。
		ActressLight();
		//懐中電灯。
		SetLight();
	}
}

void Player::En_Idle()
{
	//アニメーションの再生。
	m_animation.Play(enAnimationClip_idle, 0.4f);

	Move();
	Rotation();
	ChangeState();
}

void Player::En_Walk()
{
	//アニメーションの再生。
	//完全に横移動だったら。
	if (g_pad[0].GetLStickYF() < 0.2f&&g_pad[0].GetLStickYF() > -0.2f)
	{
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
	if (g_pad[0].GetLStickYF() > 0.2f) {
		m_animation.Play(enAnimationClip_walk, 0.4f);
	}
	//バック。
	if (g_pad[0].GetLStickYF() < -0.2f) {
		m_animation.Play(enAnimationClip_back, 0.4f);
	}

	Move();
	Rotation();
	ChangeState();
}

void Player::En_Run()
{
	//アニメーションの再生。
	m_animation.Play(enAnimationClip_run, 0.4f);

	Move();
	Rotation();
	ChangeState();
}

void Player::En_Aim()
{
	//アニメーションの再生。
	m_animation.Play(enAnimationClip_aim, 0.4f);

	Move();
	Rotation();
	ChangeState();
	//R2を押したら撃つ。
	if (g_pad[0].IsTrigger(enButtonRB2) && m_timer > 20) {
		//撃つステートに遷移。
		m_timer = 0;
		m_state = enState_shot;
	}
}

void Player::En_Shot()
{
	//残弾数が0じゃなかったら。
	if (m_capacity > 0) {
		//アニメーションの再生。
		m_animation.Play(enAnimationClip_shot, 0.4f);

		if (!m_isBullet) {
			//弾丸の生成。
			m_bullet = NewGO<Bullet>(GOPrio_Defalut, "bullet");
			m_bullet->SetRot(m_rotation);
			m_isBullet = true;
			m_capacity--;
			//ワンショット再生のSE
			CSoundSource* m_seGunShot = new CSoundSource;
			m_seGunShot->Init(L"sound/gun/gun_shot.wav");
			m_seGunShot->Play(false);
		}
	}
	else {
		if (!m_isEmpty) {
			//空砲。
			//ワンショット再生のSE。
			CSoundSource* seGunEmpty = new CSoundSource;
			seGunEmpty->Init(L"sound/gun/gun_empty.wav");
			seGunEmpty->Play(false);
			m_isEmpty = true;
		}
	}

	Move();
	Rotation();

	//R2を押したら撃つ。
	if (g_pad[0].IsTrigger(enButtonRB2) && m_timer > 20) {
		ChangeState();
		m_isBullet = false;
		m_isEmpty = false;
	}

	//アニメーションのフレーム数経ったら。
	if (!m_animation.IsPlaying()) {
		ChangeState();
		m_isBullet = false;
	}
	//空砲を撃ったら。
	if (m_isEmpty) {
		ChangeState();
		m_isEmpty = false;
	}
}

void Player::En_Reload()
{
	//アニメーションの再生。
	m_animation.Play(enAnimationClip_reload, 0.4f);

	//マガジンにこめる弾の数。
	int reloadbullet = m_maxCapacity - m_capacity;
	//手持ちの弾数がマガジンにこめられる弾数より少なかったら。
	if (m_stack < reloadbullet) {
		//手持ちの弾を全部装填し、
		m_capacity += m_stack;
		//手持ちの弾を空にする。
		m_stack = 0;
	}
	else {
		m_capacity += reloadbullet;
		m_stack -= reloadbullet;
	}

	Move();
	Rotation();
	//アニメーションの再生中じゃなかったら。
	if (!m_animation.IsPlaying()) {
		ChangeState();
	}
}

void Player::En_Lie()
{
	//m_charaCon.ActiveMode(false);
	if (!m_isOldCameraInfo) {
		//バックアップ。
		m_oldplayer = m_gamecamera->GetTargetFromPlayer();
		m_oldpos = m_gamecamera->GetPosFromTarget();
		m_oldAddY = m_gamecamera->GetAddY();
		m_isOldCameraInfo = true;
	}
	m_gamecamera->SetTargetFromPlayer(50.0f);
	m_gamecamera->SetPosFromTarget(-200.0f);
	m_gamecamera->SetAddY(30.0f);
	//アニメーションの再生。
	m_animation.Play(enAnimationClip_lie, 0.4f);
	//アニメーションの再生中じゃなかったら。
	if (!m_animation.IsPlaying()) {
		ChangeState();
		m_charaCon.ActiveMode(true);
		m_isBite = false;
		//ロード。
		m_gamecamera->SetTargetFromPlayer(m_oldplayer);
		m_gamecamera->SetPosFromTarget(m_oldpos);
		m_gamecamera->SetAddY(m_oldAddY);
		m_isOldCameraInfo = false;
	}
}

void Player::Rotation()
{
	//右スティックの入力量で、加算する回転クォータニオンを作る。
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY(), 2.0f*g_pad[0].GetRStickXF());
	m_rotation.Multiply(qAddRot, m_rotation);
}

void Player::Heal()
{
	//ダメージを受けていて。
	if (m_isDamage) {
		m_healTimer++;
		//一定時間経ったら。
		if (m_healTimer >= m_healStartTime) {
			//ダメージを受けていない状態にする。
			//（回復できる状態にする。）
			m_isDamage = false;
			m_healTimer = 0;
		}
	}
	
	//HPが減っていて。
	if (m_hp < m_maxhp) {
		//ダメージを受けていなかったら。
		if (!m_isDamage) {
			//徐々に回復させるためにカウント。
			m_heaIntervalTimer++;
			if (m_heaIntervalTimer == m_healIntervalTime) {
				//回復。
				m_hp++;
				//画像を透明にしていく。
				m_alpha = 1.0f - (m_hp / m_maxhp);
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
	m_alpha = 1.0f - (m_hp / m_maxhp);
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

void Player::ActressLight()
{
	auto dir = g_camera3D.GetForward();
	dir.y = 0.0f;
	dir.Normalize();
	dir.y -= 0.2f;
	m_model->SetLight(0, dir);
}

void Player::SetLight()
{ 
	CVector3 color;
	CVector3 pos;
	if (g_pad[0].IsTrigger(enButtonA)) {
		m_isLight = !m_isLight;
	}
	if (m_isLight) {
		color.x = 1.0f;
		color.y = 1.0f;
		color.z = 1.0f;
		pos = m_position;
		pos.y += 120.0f;
	}
	else {
		color = CVector3::Zero();
	}
	for (int i = 0; i < NUM_SPOT_LIG; i++) {
			m_sl->SetLight(pos, color, m_model->GetForward(), 600.0f, i);
	}
}
