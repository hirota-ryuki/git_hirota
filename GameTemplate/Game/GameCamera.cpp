#include "stdafx.h"
#include "GameCamera.h"
#include "Game.h"
#include "Player.h"


GameCamera::GameCamera()
{
	//スプライト
	m_sprite = NewGO<SpriteRender>();
	m_sprite->Init(L"sprite/moca.dds", 100, 50);
	//m_sprite.Init(L"sprite/moca.dds", 1334, 750);

}


GameCamera::~GameCamera()
{
	DeleteGO(m_sprite);
}

void GameCamera::Update()
{
	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	
	//カメラに現在地をセット。
	CVector3 l_setpos = m_player->GetPos();
	l_setpos.y += 100.0f;
	g_camera3D.SetPosition(l_setpos);

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

	//
	/*CVector3 l_target = m_target;
	l_target.y = 0.f;
	CVector3 l_pos = m_pos;
	l_pos.y = 0.f;
	m_forward = l_target - l_pos;
	m_forward.Normalize();*/

	//注視点と視点を設定する。
	g_camera3D.SetTarget(m_target);
	g_camera3D.SetPosition(m_pos);

	//更新。
	g_camera3D.Update();
	
	m_sprite->SetData(
		CVector3::Zero(),
		CQuaternion::SpriteRot(),
		CVector3::One()
	);
}

void GameCamera::Draw()
{
	
}