#pragma once
class SkinModelRender : public IGameObject
{
public:
	SkinModelRender();
	~SkinModelRender();
	void Update() override;
	void Draw() override;
	void Init(const wchar_t* texFilePath);
	/// <summary>
	/// 座標、回転、拡大率を設定
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	void SetData(CVector3 pos, CQuaternion rot, CVector3 scale)
	{
		m_pos = pos;
		m_rot = rot;
		m_scale = scale;
	}
	
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPos(CVector3 pos)
	{
		m_pos = pos;
	}

	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="pos">回転</param>
	void SetRot(CQuaternion rot)
	{
		m_rot = rot;
	}

	void NotModelMode()
	{
		isNotModel = false;
	}
	/// <summary>
	/// スキンモデルを取得。
	/// </summary>
	/// <returns>スキンモデル</returns>
	SkinModel& GetModel()
	{
		return m_model;
	}
private:
	SkinModel m_model;									//スキンモデル。
	CVector3 m_pos = CVector3::Zero();
	CQuaternion m_rot = CQuaternion::SpriteRot();
	CVector3 m_scale = CVector3::One();
	bool isNotModel = true;
};

