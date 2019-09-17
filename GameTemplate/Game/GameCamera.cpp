#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"
#include "Player.h"


GameCamera::GameCamera()
{
	m_game = &Game::GetInstance();
	m_player = m_game->GetPlayer();
}


GameCamera::~GameCamera()
{
}

void GameCamera::Update()
{
	//カメラに現在地をセット。
	CVector3 l_setpos = m_player->GetPos();
	l_setpos.y += 100.0f;
	m_camera.SetPosition(l_setpos);

	// 限界を決める
	float toCameraPosRotAngleold = toCameraPosRotAngle;
	toCameraPosRotAngleold += (g_pad[0].GetRStickYF()*1.5f);

	float t = 60.0f;

	if (toCameraPosRotAngle > t) {
		toCameraPosRotAngleold = t;
	}
	if (toCameraPosRotAngle < -70.0f) {
		toCameraPosRotAngleold = -70.0f;
	}
	toCameraPosRotAngle = toCameraPosRotAngleold;


	//プレイヤーから注視点までのベクトルを設定。
	//zが前
	CVector3 l_toCameraTarget;
	l_toCameraTarget.Set(0.0f, 0.0f, 1000.0f);
	m_player->GetRot().Multiply(l_toCameraTarget);

	CQuaternion qAddRot2;
	CVector3 axis2;
	CVector3 y2 = { 0.0f,1.0f,0.0f };
	axis2.Cross(l_toCameraTarget, y2);
	axis2.Normalize();
	qAddRot2.SetRotationDeg(axis2, toCameraPosRotAngle);
	qAddRot2.Multiply(l_toCameraTarget);


	//視点から注視点までのベクトルを.設定。

	CVector3 l_toCameraPos;
	l_toCameraPos.Set(60.0f, 0.0f, -1200.0f);
	m_player->GetRot().Multiply(l_toCameraPos);

	CQuaternion qAddRot;
	CVector3 axis;
	CVector3 y = { 0.0f,-1.0f,0.0f };
	axis.Cross(l_toCameraPos, y);
	axis.Normalize();
	qAddRot.SetRotationDeg(axis, toCameraPosRotAngle);
	qAddRot.Multiply(l_toCameraPos);



	CVector3 m_pos = m_player->GetPos();
	m_pos.y += 100.0f;

	//注視点を計算する。
	m_target = m_pos + l_toCameraTarget;
	//視点を計算する。
	m_pos = m_target + l_toCameraPos;

	//バネカメラに注視点と視点を設定する。
	m_camera.SetTarget(m_target);
	m_camera.SetPosition(m_pos);

	//バネカメラの更新。
	m_camera.Update();
}

void GameCamera::Draw()
{
	
}