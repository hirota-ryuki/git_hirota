#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"


Player::Player()
{
	//cmoファイルの読み込み。
	m_model.Init(L"modelData/unitychan/unityChan.cmo");
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
	
	//スプライト
	m_sprite.Init(L"sprite/moca.dds",1334,750);
	
	
	//キャラコンの初期化
	/*m_charaCon.Init(
		100.f,
		800.f,
		m_position
	);*/
}


Player::~Player()
{
	
}

void Player::Move()
{
	//左スティックの入力量を受け取る。
	float lStick_x = g_pad[0].GetLStickXF();
	float lStick_y = g_pad[0].GetLStickYF();

	//XZ成分の移動速度をクリア。
	m_moveSpeed.x = 0.f;
	m_moveSpeed.z = 0.f;
	m_moveSpeed.y = 0.f;// * GameTime().GetFrameDeltaTime();

	m_moveSpeed.z +=  -lStick_y * m_speed;	//奥方向への移動速度を代入。
	m_moveSpeed.x +=  -lStick_x * m_speed;		//右方向への移動速度を加算。
	m_position += m_moveSpeed;

	//キャラクターコントローラーを使用して、座標を更新。
	//m_position = m_charaCon.Execute(1.f/60.f,m_moveSpeed);
}

void Player::Update()
{
	m_game->GetGame();
	
	Move();
	Rotation();
	
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

}


void Player::Rotation()
{
	//右スティックの入力量で、加算する回転クォータニオンを作る。
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY(), 4.f * g_pad[0].GetRStickXF());
	m_rotation.Multiply(qAddRot, m_rotation);

}

void Player::Draw()
{
	//モデル
	m_model.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);

	
}