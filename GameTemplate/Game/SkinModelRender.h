#pragma once
class SkinModelRender : public IGameObject
{
public:
	SkinModelRender();
	~SkinModelRender();
	void Update() override;
	bool Start() override;
	void Draw() override;
	/// <summary>
	/// ワールド行列を更新。
	/// </summary>
	void UpdateWorldMatrix();
	/// <summary>
	/// スキンモデルの初期化
	/// </summary>
	/// <param name="texFilePath">モデルのファイルパス</param>
	void Init(const wchar_t* texFilePath);
	/// <summary>
	/// 座標、回転を設定
	/// 拡大率は触れないように
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	void SetData(CVector3 pos, CQuaternion rot)
	{
		m_pos = pos;
		m_rot = rot;
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
	/// <summary>
	/// 描画を行わないモード。
	/// </summary>
	void NotModelMode()
	{
		m_isNotModel = false;
	}
	/// <summary>
	/// スキンモデルを取得。
	/// </summary>
	/// <returns>スキンモデル</returns>
	SkinModel& GetModel()
	{
		return m_model;
	}
	/*!
	* @brief	モデルの前方方向を取得。
	*/
	const CVector3& GetForward() const
	{
		return m_forward;
	}
private:
	SkinModel m_model;									//スキンモデル。
	CVector3 m_pos = CVector3::Zero();
	CQuaternion m_rot = CQuaternion::SpriteRot();
	CVector3 m_scale = CVector3::One();
	bool m_isNotModel = true;
	bool m_isUpdate = false;	//Update関数が呼ばれたかどうか。
	CVector3	m_forward = CVector3::Front();		//カメラの前方。
	CMatrix m_viewMatrixInv = CMatrix::Identity();	//ビューの逆行列。
};