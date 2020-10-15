#pragma once
#include "IGameObject.h"
class Player;
class Goal :
	public IGameObject
{
public:
	Goal();
	~Goal();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �S�[���̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �S�[���̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]�B</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model = nullptr;						//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	Game*				m_game = nullptr;						//Game�̃|�C���^�B
	Player*				m_player = nullptr;						//�v���C���[�N���X�̃|�C���^�B
	const float			CLEAR_DISTANCE = 100.0f;				//�߂��ɗ�����N���A�Ƃ���B
};

