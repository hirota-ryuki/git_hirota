#pragma once
#include "physics/GhostObject.h"
#include "physics/BoxCollider.h"
class Game;
class Player;
//ステート。
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
	/// ステートの初期化。
	/// この中でSetCreateState()を呼び出してください。
	/// </summary>
	virtual	void InitState() = 0;
	/// <summary>
	/// 初期化。
	/// </summary>
	void Init();
	/// <summary>
	/// 当たった時の共通処理。
	/// </summary>
	void IsHit();
	/// <summary>
	/// 当たった時の個々の処理。
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
	//インスタンス関係。
	Game*				m_game = nullptr;						//Gameのポインタ。
	Player*				m_player = nullptr;
};

