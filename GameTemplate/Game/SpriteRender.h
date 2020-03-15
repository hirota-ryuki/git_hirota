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
	/// ���W���Z�b�g�B
	/// </summary>
	void SetPos(const CVector3 pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// ��]���Z�b�g�B
	/// </summary>
	void SetRot(const CQuaternion rot)
	{
		m_rot = rot;
		m_rot.Multiply(CQuaternion::SpriteRot());
		m_isMulti = true;
	}
	/// <summary>
	/// ��]���擾�B
	/// </summary>
	/// <param name="m_rot">��]�B</param>
	CQuaternion GetRot()
	{
		return m_rot;
	}
	/// <summary>
	/// 
	/// </summary>
	void RotMultiply()
	{
		if (!m_isMulti) {
			m_rot.Multiply(CQuaternion::SpriteRot());
			m_isMulti = true;
		}
	}
	/// <summary>
	/// �A���t�@�l���Z�b�g�B
	/// </summary>
	/// <param name="m_alpha">�A���t�@�l�B</param>
	void SetAlpha(const float alpha)
	{
		m_alpha = alpha;
	}
	/// <summary>
	/// �`�悷�邩�ǂ����B
	/// �Ă΂ꂽ��true��false�����ւ���B
	/// </summary>
	void ChangeActive()
	{
		m_isActive = !m_isActive;
	}
	/// <summary>
	/// �`�悷�邩�ǂ����B
	/// true��false�����w�肷��B
	/// </summary>
	void ActiveMode(bool active)
	{
		m_isActive = active;
	}
	void Init(const wchar_t* texFilePath, float w, float h);
private:
	Sprite		m_sprite;
	CVector3	m_pos = CVector3::Zero();
	CQuaternion m_rot = CQuaternion::Identity();
	CVector3	m_scale = CVector3::One();
	float		m_alpha = 1.0f;					//�X�v���C�g�̃��l�B
	bool		m_isActive = true;				//�摜���h���[����Ă��邩�ǂ����B
	bool		m_isMulti = false;				//�摜���h���[����Ă��邩�ǂ����B
};

