#pragma once
#include "IGameObject.h"
class Player;
class Pose;
class BulletStack :
	public IGameObject
{
public:
	BulletStack();
	~BulletStack();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// ボールのポジションをセット。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ボールの回転をセット。
	/// </summary>
	/// <param name="pos">回転。</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model;								//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	int					m_bullet = 10;							//識別番号。		
	Game*				m_game = nullptr;						//Gameのポインタ。
	Player*				m_player = nullptr;						//プレイヤークラスのポインタ。
	Pose*				m_pose = nullptr;
	//ライト関係。
	CVector4			m_directionLightDirection = { 1.0f, -1.0f, 0.0f, 0.0f };
	CVector4			m_directionLightColor = { 0.5f, 0.5f, 0.5f, 10.2f };
	float				m_spec = 10.0f;
	CVector3			m_ambientLight = CVector3::Zero();
};

