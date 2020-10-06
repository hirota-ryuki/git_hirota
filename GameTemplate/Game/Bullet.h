#pragma once
#include "IGameObject.h"
class Bullet :
	public IGameObject
{
public:
	Bullet();
	~Bullet();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	void Update_NotPause() override;
	/// <summary>
	/// 回転する関数。
	/// </summary>
	void Rotation();
	/// <summary>
	/// 銃弾のポジションを取得する関数。
	/// </summary>
	/// <returns>ポジション。</returns>
	CVector3 GetPos()
	{
		return m_position;
	}
	/// <summary>
	/// ムーブスピードをセット。
	/// </summary>
	/// <param name="pos">座標</param>
	void SetMoveSpeed(CVector3 pos) {
		m_moveSpeed = pos;
	}
	/// <summary>
	/// 弾丸の回転をセット。
	/// </summary>
	/// <param name="pos">回転</param>
	void SetRot(CQuaternion rot) {
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model = nullptr;						//スキンモデルレンダー	
	CVector3			m_position = CVector3::Zero();			//座標。
	CVector3			m_moveSpeed = CVector3::Zero();			//移動速度。
	float				m_speed = 30.0f;						//速度を上げる。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	int					m_deleteTimer = 0;						//弾丸を消すためのタイマー。
	//インスタンス関係。
	Game*				m_game = nullptr;						//ゲームのポインタ。
	Player*				m_player = nullptr;						//プレイヤーのポインタ。
	GameCamera*			m_gamecamera = nullptr;					//GameCameraのポインタ。
};