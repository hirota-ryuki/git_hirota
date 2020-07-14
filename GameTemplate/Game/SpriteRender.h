#pragma once

class SpriteRender :public IGameObject
{
public:
	SpriteRender();
	~SpriteRender();
	bool Start() override;
	void Update() override;
	void DrawHUD() override;
	void SetData(const CVector2& pos, const CQuaternion rot, const CVector3 scale)
	{
		m_pos = CVector3{ pos.x,pos.y, 0.0f };
		m_rot = rot;
		m_scale = scale;
	}
	/// <summary>
	/// 座標をセット。
	/// </summary>
	void SetPos(const CVector2& pos)
	{
		m_pos = CVector3{ pos.x,pos.y, 0.0f };
	}
	/// <summary>
	/// 座標を取得。
	/// </summary>
	/// <param name="m_rot">座標。</param>
	CVector2 GetPos()
	{
		CVector2 pos = { m_pos.x,m_pos.y };
		return pos;
	}
	/// <summary>
	/// 回転をセット。
	/// </summary>
	void SetRot(const CQuaternion rot)
	{
		m_rot = rot;
		m_rot.Multiply(CQuaternion::SpriteRot());
		m_isMulti = true;
	}
	/// <summary>
	/// 回転を取得。
	/// </summary>
	/// <param name="m_rot">回転。</param>
	CQuaternion GetRot()
	{
		return m_rot;
	}
	/// <summary>
	/// 画像を反転させる関数。
	/// 元の画像が反転されて描画されるため、
	/// 一回だけ実行させなければならない。
	/// </summary>
	void RotMultiply()
	{
		if (!m_isMulti) {
			m_rot.Multiply(CQuaternion::SpriteRot());
			m_isMulti = true;
		}
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
	/// <summary>
	/// 初期化。
	/// </summary>
	/// <param name="texFilePath">ファイルパス。</param>
	/// <param name="w">横幅。</param>
	/// <param name="h">縦幅。</param>
	void Init(const wchar_t* texFilePath, float w, float h);
	/// <summary>
	/// スプライトをセット。
	/// </summary>
	/// <param name="sprite">スプライト。</param>
	void SetSprite(const Sprite& sprite) {
		m_sprite = sprite;
	}
	/// <summary>
	/// スプライトをゲット。
	/// </summary>
	/// <returns>スプライト。</returns>
	Sprite GetSprite()
	{
		return m_sprite;
	}
private:
	Sprite		m_sprite;
	CVector3	m_pos = CVector3::Zero();
	CQuaternion m_rot = CQuaternion::Identity();
	CVector3	m_scale = CVector3::One();
	float		m_alpha = 1.0f;					//スプライトのα値。
	bool		m_isActive = true;				//画像がドローされているかどうか。
	bool		m_isMulti = false;				//画像を反転させたかどうか。
};

