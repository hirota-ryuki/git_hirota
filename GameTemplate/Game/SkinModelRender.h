#pragma once
class SkinModelRender : public IGameObject
{
public:
	SkinModelRender();
	~SkinModelRender();
	void Update() override;
	void Draw() override;
	void Init(const wchar_t* texFilePath);
	void SetData(CVector3 pos, CQuaternion rot, CVector3 scale)
	{
		m_pos = pos;
		m_rot = rot;
		m_scale = scale;
	}
	/// <summary>
	/// �X�L�����f�����擾�B
	/// </summary>
	/// <returns>�X�L�����f��</returns>
	SkinModel& GetModel()
	{
		return m_model;
	}
private:
	SkinModel m_model;									//�X�L�����f���B
	CVector3 m_pos = CVector3::Zero();
	CQuaternion m_rot = CQuaternion::SpriteRot();
	CVector3 m_scale = CVector3::One();
};

