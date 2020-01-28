#pragma once
#include "IGameObject.h"
class Bullet :
	public IGameObject
{
public:
	Bullet();
	~Bullet();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// ��]����֐��B
	/// </summary>
	void Rotation();
	/// <summary>
	/// �e�e�̃|�W�V�������擾����֐��B
	/// </summary>
	/// <returns>�|�W�V�����B</returns>
	CVector3 GetPos()
	{
		return m_position;
	}
	// <summary>
	/// ���[�u�X�s�[�h�����Z�B
	/// </summary>
	/// <param name="pos">���Z��</param>
	void AddMoveSpeed(CVector3 add) {
		m_moveSpeed += add;
	}
	/// <summary>
	/// ���[�u�X�s�[�h���Z�b�g�B
	/// </summary>
	/// <param name="pos">���W</param>
	void SetMoveSpeed(CVector3 pos) {
		m_moveSpeed = pos;
	}
	/// <summary>
	/// �e�ۂ̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]</param>
	void SetRot(CQuaternion rot) {
		m_rotation = rot;
	}
private:
	SkinModelRender* m_model = nullptr;			//�X�L�����f�������_�[	
	CVector3 m_position = CVector3::Zero();		//���W�B
	CVector3 m_moveSpeed = CVector3::Zero();	//�ړ����x�B
	float m_speed = 20.0f;						//���x���グ��B
	CVector3 m_bulletpos = CVector3::Zero();
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	int m_timer = 0;
	//�C���X�^���X�֌W�B
	Game* m_game = nullptr;							//�Q�[���̃|�C���^�B
	Player*	m_player = nullptr;						//�v���C���[�̃|�C���^�B
	GameCamera* m_gamecamera = nullptr;				//GameCamera�̃|�C���^�B
};