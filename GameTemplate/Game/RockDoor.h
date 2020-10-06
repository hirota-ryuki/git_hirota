#pragma once

class RockDoor :
	public IDoor
{
public:
	RockDoor(){}
	~RockDoor();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// ドアのポジションをセット。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ドアのセンターポジションをセット。
	/// </summary>
	/// <param name="pos">中心座標。</param>
	void SetCenterPos(const CVector3& pos)
	{
		m_centerPos = pos;
	}
	/// <summary>
	/// ドアの回転をセット。
	/// </summary>
	/// <param name="pos">回転。</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model = nullptr;						//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	SkinModelRender*	m_PSOmodel = nullptr;					//当たり判定のスキンモデル。
	PhysicsStaticObject m_physicsStaticObject;					//床の当たり判定。
	Game*				m_game = nullptr;						//Gameのポインタ。
	Player*				m_player = nullptr;						//プレイヤークラスのポインタ。
	CVector3			m_centerPos = CVector3::Zero();			//ドアの中心座標。
};