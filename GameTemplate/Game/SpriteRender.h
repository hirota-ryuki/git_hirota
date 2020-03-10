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
	/// <param name="m_alpha">���W�B</param>
	void SetPos(const CVector3 pos)
	{
		m_pos = pos;
	}
	/// <summary>
	/// ��]���Z�b�g�B
	/// </summary>
	/// <param name="m_alpha">��]�B</param>
	void SetRot(const CQuaternion rot)
	{
		m_rot = rot;
	}
	/// <summary>
	/// ��]���擾�B
	/// </summary>
	/// <param name="m_alpha">��]�B</param>
	CQuaternion GetRot()
	{
		return m_rot;
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
	CQuaternion m_rot = CQuaternion::SpriteRot();
	CVector3	m_scale = CVector3::One();
	float		m_alpha = 1.0f;					//�X�v���C�g�̃��l�B
	bool		m_isActive = true;				//�摜���h���[����Ă��邩�ǂ����B
};

