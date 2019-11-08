#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"


Player::Player()
{
	//キャラコンの初期化
	m_charaCon.Init(
		50.f,
		//高さは2倍にしている
		//階段とかに行くと足元の下にあたり判定が
		//出ると思うけど頑張って(　´∀｀)bｸﾞｯ!
		80.f * 2.0f,
		m_position
	);
	//cmoファイルの読み込み。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalt);
	m_model->Init(L"modelData/unitychan/unityChan.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	//m_position.y += 200.f;
	m_model->SetData(m_position, m_rotation, m_scale);

}


Player::~Player()
{
	
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

	m_moveSpeed += cameraForward * lStick_y * m_speed;	//奥方向への移動速度を代入。
	m_moveSpeed += cameraRight * lStick_x * m_speed;		//右方向への移動速度を加算。
	//m_position += m_moveSpeed;
	//キャラクターコントローラーを使用して、座標を更新。
	m_position = m_charaCon.Execute(1.f / 60.f, m_moveSpeed);
}

void Player::Update()
{
	//Gameクラスの取得
	m_game->GetGame();
	
	Move();
	Rotation();
	
	//ワールド行列の更新。
	m_model->SetData(m_position, m_rotation, m_scale);
}

void Player::Rotation()
{
	//右スティックの入力量で、加算する回転クォータニオンを作る。
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY(), 4.f * g_pad[0].GetRStickXF());
	m_rotation.Multiply(qAddRot, m_rotation);
}