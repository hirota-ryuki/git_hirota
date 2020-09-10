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
	/// �V��̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	///�@�V��̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]�B</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model = nullptr;								//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	SkinModelRender* m_modelPos = nullptr;
	SkinModelRender* m_modelTag = nullptr;
};

