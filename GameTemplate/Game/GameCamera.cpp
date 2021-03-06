#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"

GameCamera::GameCamera()
{
}

GameCamera::~GameCamera()
{
}

void GameCamera::OnDestroy()
{
}

bool GameCamera::Start()
{
	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	//ばねカメラの初期化。
	m_springCamera.Init(
		g_camera3D,		//ばねカメラの処理を行うカメラを指定する。
		1000.0f,		//カメラの移動速度の最大値。
		true,			//カメラと地形とのあたり判定を取るかどうかのフラグ。trueだとあたり判定を行う。
		5.0f			//カメラに設定される球体コリジョンの半径。第３引数がtrueの時に有効になる。
	);
	return true;
}

void GameCamera::Update()
{
}

void GameCamera::Update_NotPause()
{
	if (m_player != nullptr) {
		// 限界を決める。
		float toCameraPosRotAngleold = m_toCameraPosRotAngle;
		toCameraPosRotAngleold += (g_pad[0].GetRStickYF()*1.5f);
		float t = 60.0f;
		if (m_toCameraPosRotAngle > POS_ANGLE_UP_LIMIT) {
			toCameraPosRotAngleold = POS_ANGLE_UP_LIMIT;
		}
		if (m_toCameraPosRotAngle < POS_ANGLE_DOWN_LIMIT) {
			toCameraPosRotAngleold = POS_ANGLE_DOWN_LIMIT;
		}
		m_toCameraPosRotAngle = toCameraPosRotAngleold;

		//プレイヤーから注視点までのベクトルを設定。
		//zが前
		//プレイヤーの前ベクトル。
		CVector3 l_toCameraTarget;
		l_toCameraTarget.Set(0.0f, 0.0f, m_targetFromPlayer);
		m_player->GetRot().Multiply(l_toCameraTarget);

		//注視点を動かすための外積。
		CQuaternion qAddRot2;
		CVector3 axis2;
		CVector3 y2 = { 0.0f,1.0f,0.0f };
		axis2.Cross(l_toCameraTarget, y2);
		axis2.Normalize();
		qAddRot2.SetRotationDeg(axis2, m_toCameraPosRotAngle);
		qAddRot2.Multiply(l_toCameraTarget);

		//注視点から視点までのベクトルを設定。
		CVector3 l_toCameraPos;
		l_toCameraPos.Set(60.0f, 0.0f, m_posFromTarget);
		m_player->GetRot().Multiply(l_toCameraPos);

		//視点を動かすための外積。
		CQuaternion qAddRot;
		CVector3 axis;
		CVector3 y = { 0.0f,-1.0f,0.0f };
		axis.Cross(l_toCameraPos, y);
		axis.Normalize();
		qAddRot.SetRotationDeg(axis, m_toCameraPosRotAngle);
		qAddRot.Multiply(l_toCameraPos);

		CVector3 m_pos = m_player->GetPos();
		m_pos.y += m_playerPosAddY;

		//仮の注視点を計算する。
		m_target = m_pos + l_toCameraTarget;
		//視点を計算する。
		m_pos = m_target + l_toCameraPos;

		//視点を設定する。
		m_springCamera.SetPosition(m_pos);

		//注視点をプレイヤーの右側に移す。
		//   〇 仮の注視点(m_target)
		//　　 \
		//　    \
		//　　P  〇　 //ここが最終的な注視点！！！
		//        \
		//         \
		//         〇視点
		//なぜ横にするかというと、プレイヤーの前方方向だと、前方の壁をすり抜けてしまって
		//カメラの当たり判定で誤動作するため。
		{
			//1.プレイヤーの前方方向と逆向きのベクトル v0 を求める。
			CVector3 v0 = l_toCameraTarget;
			v0.Normalize();
			v0 *= -1.0f;
			//2. 1で求めたv0と仮の注視点から視点に向かうベクトルとの内積を求める。
			//注視点をプレイヤーの右側に移す。
			//   〇 仮の注視点(m_target)
			//　こ|\
			//　の| \
			//　長P  〇　 //ここが最終的な注視点！！！
			//  さ|   \
			//  が|    \
			//  t |ーーー〇視点
			
			t = v0.Dot(l_toCameraPos);
			//3.プレイヤーから仮の注視点までの距離÷tで最終的な注視点までの比率を求める。
			//2. 1で求めたv0と仮の注視点から視点に向かうベクトルとの内積を求める。
			//注視点をプレイヤーの右側に移す。
			//   〇 仮の注視点(m_target)
			//この||\   この
			//長さ|| \　長さ！！！　
			//　　P  〇　 //ここが最終的な注視点！！！
			//  　|   \
			//  　|    \
			//  　|ーーー〇視点
			t = m_targetFromPlayer / t;
			//4.あとは線形ほかーん。
			m_target.Lerp(t, m_target, m_pos);
			m_springCamera.SetTarget(m_target);
		}
		//更新。
		m_springCamera.Update();
	}
}
