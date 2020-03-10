#pragma once

class SpriteRender :public IGameObject
{
public:
	SpriteRender();
	~SpriteRender();
	bool Start() override;
	void Update() override;
	void DrawHUD() override;
	void SetData(const CVector3& pos, const CQuaternion rot, const CVector3 scale)
	{
		m_pos = pos;
		m_rot = rot;
		m_scale = scale;
	}
	/// <summary>
	/// 座標をセット。
	/// </summary>
	/// <param name="m_alpha">座標。</param>
	void SetPos(const CVector3 pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// 回転をセット。
	/// </summary>
	/// <param name="m_alpha">回転。</param>
	void SetRot(const CQuaternion rot)
	{
		m_rot = rot;
	}
	/// <summary>
	/// 回転を取得。
	/// </summary>
	/// <param name="m_alpha">回転。</param>
	CQuaternion GetRot()
	{
		return m_rot;
	}
	
	/// <summary>
	/// アルファ値をセット。
	/// </summary>
	/// <param name="m_alpha">アルファ値。</param>
	void SetAlpha(const float alpha)
	{
		m_alpha = alpha;
	}
	/// <summary>
	/// 描画するかどうか。
	/// 呼ばれたらtrueとfalseを入れ替える。
	/// </summary>
	void ChangeActive()
	{
		m_isActive = !m_isActive;
	}
	/// <summary>
	/// 描画するかどうか。
	/// trueかfalseかを指定する。
	/// </summary>
	void ActiveMode(bool active)
	{
		m_isActive = active;
	}
	void Init(const wchar_t* texFilePath, float w, float h);
private:
	Sprite		m_sprite;
	CVector3	m_pos = CVector3::Zero();
	CQuaternion m_rot = CQuaternion::SpriteRot();
	CVector3	m_scale = CVector3::One();
	float		m_alpha = 1.0f;					//スプライトのα値。
	bool		m_isActive = true;				//画像がドローされているかどうか。
};

