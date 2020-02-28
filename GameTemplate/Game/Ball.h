#pragma once
#include "IGameObject.h"
class Player;
class Pose;

class Ball :
	public IGameObject
{
public:
	Ball();
	~Ball();
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
	/// <summary>
	/// �{�[���̎��ʔԍ����Z�b�g�B
	/// </summary>
	/// <param name="pos">���ʔԍ��B</param>
	void SetNomber(int no)
	{
		m_nomber = no;
	}
private:
	SkinModelRender*	m_model;								//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	int					m_nomber = 0;							//���ʔԍ��B		
	Game*				m_game = nullptr;						//Game�̃|�C���^�B
	Player*				m_player = nullptr;						//�v���C���[�N���X�̃|�C���^�B
	Pose*				m_pose = nullptr;
};

