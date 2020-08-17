#pragma once

class RockDoor :
	public IDoor
{
public:
	RockDoor(){}
	~RockDoor();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �h�A�̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �h�A�̃Z���^�[�|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���S���W�B</param>
	void SetCenterPos(CVector3 pos)
	{
		m_centerPos = pos;
	}
	/// <summary>
	/// �h�A�̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]�B</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model = nullptr;						//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	SkinModelRender*	m_PSOmodel = nullptr;					//�����蔻��̃X�L�����f���B
	PhysicsStaticObject m_physicsStaticObject;					//���̓����蔻��B
	Game*				m_game = nullptr;						//Game�̃|�C���^�B
	Player*				m_player = nullptr;						//�v���C���[�N���X�̃|�C���^�B
	const wchar_t*		m_name;
	CVector3			m_centerPos = CVector3::Zero();
};