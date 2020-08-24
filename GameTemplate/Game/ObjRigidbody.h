#pragma once
class ObjRigidbody :
	public IGameObject
{
public:
	ObjRigidbody();
	~ObjRigidbody();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// ポジションをセット。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	///　回転をセット。
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
	//当たり判定。
	SkinModelRender* m_PSOmodel = nullptr;					//床のスキンモデル。
	PhysicsStaticObject m_physicsStaticObject;			//床の当たり判定。
};

