#pragma once
#include "physics/GhostObject.h"
#include "physics/BoxCollider.h"
class Game;
class Player;
//�X�e�[�g�B
enum CreateState {
	enState_createBox,
	enState_createCapsule,
	enState_createSphere,
};
using namespace hirota;
class IGimmick :
	public IGameObject
{
public:
	IGimmick() {}
	virtual ~IGimmick() {}
	bool Start() override;
	void Update() override;
	void OnDestroy() override {}
	/// <summary>
	/// �X�e�[�g�̏������B
	/// ���̒���SetCreateState()���Ăяo���Ă��������B
	/// </summary>
	virtual	void InitState() = 0;
	/// <summary>
	/// �������B
	/// </summary>
	void Init();
	/// <summary>
	/// �����������̋��ʏ����B
	/// </summary>
	void IsHit();
	/// <summary>
	/// �����������̌X�̏����B
	/// </summary>
	virtual void OnHit() = 0;
	void SetPos(const CVector3& pos) {
		m_position = pos;
	}
	void SetRot(const CQuaternion& rot) {
		m_rotation = rot;
	}
	void SetCreateState(CreateState state) {
		m_state = state;
	}
private:
	CPhysicsGhostObject		m_ghost;
	CVector3				m_position = CVector3::Zero();
	CQuaternion				m_rotation = CQuaternion::Identity();
	CVector3				m_size = CVector3::One();
	float					m_radius = 1.0f;
	float					m_height = 1.0f;
	CreateState				m_state = enState_createBox;
	//�C���X�^���X�֌W�B
	Game*				m_game = nullptr;						//Game�̃|�C���^�B
	Player*				m_player = nullptr;
};

