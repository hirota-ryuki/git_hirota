#pragma once
class Ceiling :
	public IGameObject
{
public:
	Ceiling();
	~Ceiling();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// 天井のポジションをセット。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	///　天井の回転をセット。
	/// </summary>
	/// <param name="pos">回転。</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model = nullptr;								//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	SkinModelRender* m_modelPos = nullptr;
	SkinModelRender* m_modelTag = nullptr;
};

