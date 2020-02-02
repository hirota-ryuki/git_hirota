#pragma once

class SpriteRender :public IGameObject
{
public:
	SpriteRender();
	~SpriteRender();
	bool Start() override;
	void Update() override;
	void DrawHUD() override;
	void SetData(const CVector3& pos, CQuaternion rot, CVector3 scale)
	{
		m_pos = pos;
		m_rot = rot;
		m_scale = scale;
	}
	/// <summary>
	/// アルファ値をセット。
	/// </summary>
	/// <param name="m_alpha">アルファ値</param>
	void SetAlpha(const float alpha)
	{
		m_alpha = alpha;
	}
	void Init(const wchar_t* texFilePath, float w, float h);
private:
	Sprite		m_sprite;
	CVector3	m_pos = CVector3::Zero();
	CQuaternion m_rot = CQuaternion::SpriteRot();
	CVector3	m_scale = CVector3::One();
	float		m_alpha = 1.0f;						//スプライトのα値。
};

