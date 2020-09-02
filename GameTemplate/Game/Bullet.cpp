#include "stdafx.h"
#include "Bullet.h"
#include "Player.h"
#include "GameCamera.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

void Bullet::OnDestroy()
{
	DeleteGO(m_model);
}

bool Bullet::Start()
{
	//弾丸を生成。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/bullet/bullet.cmo");
	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	//弾丸の座標にプレイヤーの座標を代入する。
	CVector3 l_pos = m_player->GetPos();
	l_pos.y += 140.0f;
	m_position = l_pos;
	m_gamecamera = m_game->GetCamera();
	m_position = g_camera3D.GetPosition();
	m_model->SetPos(m_position);
	//カメラのインスタンスを取得。
	m_gamecamera = m_game->GetCamera();
	m_moveSpeed = m_gamecamera->GetTarget() - m_position;
	m_moveSpeed.Normalize();
	return true;
}

void Bullet::Update()
{
	//一時停止していなかったら。
	if (!m_game->GetIsPose()) {
		//弾丸を移動させる。
		m_position += m_moveSpeed * m_speed;
		//回転。
		Rotation();
		//スキンモデルレンダーに座標を伝える。
		m_model->SetData(m_position, m_rotation);
		//タイマーを加算する。
		m_deleteTimer++;
		if (m_deleteTimer == 50) {
			//タイマーが50になったらインスタンスを削除する。
			DeleteGO(this);
		}
	}
}

void Bullet::Rotation()
{
	if (m_moveSpeed.Length() > 0.1f) {
		float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
		/*CVector3 dir;
		dir.Set(0.0f, 0.0f, -1.0f);
		m_rotation.Multiply(dir);
		dir.Normalize();
		float angle2 = dir.Dot(CVector3::AxisY());

		CVector3 cross;
		cross.Cross(dir, CVector3::AxisY());
		cross.Normalize();

		CQuaternion Rot;
		Rot.SetRotation(cross, acos(angle2));

		CQuaternion Rot2;
		Rot2.SetRotationDeg(CVector3::AxisX(), 90.0f);
		Rot2.Multiply(Rot);*/

		m_model->SetRot(m_rotation);
	}
}