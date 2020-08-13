#pragma once
#include "IGameObject.h"
#include "physics/PhysicsStaticObject.h"
#include "Physics/BoxCollider.h"


class Player;
class Pose;
class RockDoor :
	public IGameObject
{
public:
	RockDoor();
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
	/// �h�A�̉�]���Z�b�g�B
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
	PhysicsStaticObject m_physicsStaticObject;			//���̓����蔻��B
	Game*				m_game = nullptr;						//Game�̃|�C���^�B
	Player*				m_player = nullptr;						//�v���C���[�N���X�̃|�C���^�B
	Pose*				m_pose = nullptr;
	float			m_maxRotate = 90.0f;					//��]�̍ő�ʁB
	bool iskokok = false;
	BoxCollider			m_boxCollider;
	
};