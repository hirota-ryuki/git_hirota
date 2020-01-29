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
	/// �v���C���[�̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �v���C���[�̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender* m_model;							//�X�L�����f���B
	CVector3 m_position = CVector3::Zero();				//���W�B
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	Game* m_game = nullptr;								//Game�̃|�C���^�B
	Player* m_player = nullptr;							//�v���C���[�N���X�̃|�C���^�B
};

