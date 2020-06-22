#pragma once
#include "IGameObject.h"
class Player;
class Pose;
class BulletStack :
	public IGameObject
{
public:
	BulletStack();
	~BulletStack();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �{�[���̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �{�[���̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]�B</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model;								//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	int					m_bullet = 10;							//���ʔԍ��B		
	Game*				m_game = nullptr;						//Game�̃|�C���^�B
	Player*				m_player = nullptr;						//�v���C���[�N���X�̃|�C���^�B
	Pose*				m_pose = nullptr;
	//���C�g�֌W�B
	CVector4			m_directionLightDirection = { 1.0f, -1.0f, 0.0f, 0.0f };
	CVector4			m_directionLightColor = { 0.5f, 0.5f, 0.5f, 10.2f };
	float				m_spec = 10.0f;
	CVector3			m_ambientLight = CVector3::Zero();
};

