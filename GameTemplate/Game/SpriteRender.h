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
	/// アルファ値をセット。
	/// </summary>
	/// <param name="m_alpha">アルファ値。</param>
	void SetAlpha(const float alpha)
	{
		m_alpha = alpha;
	}
	/// <summary>
	/// 描画するかどうか。
	/// </summary>
	void ChangeActive()
	{
		m_isActive = !m_isActive;
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

