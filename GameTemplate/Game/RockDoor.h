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
	/// ドアのポジションをセット。
	/// </summary>
	/// <param name="pos">座標。</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ドアの回転をセット。
	/// </summary>
	/// <param name="pos">回転。</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model;								//スキンモデル。
	CVector3			m_position = CVector3::Zero();			//座標。
	CQuaternion			m_rotation = CQuaternion::Identity();	//回転。
	PhysicsStaticObject m_physicsStaticObject;			//床の当たり判定。
	Game*				m_game = nullptr;						//Gameのポインタ。
	Player*				m_player = nullptr;						//プレイヤークラスのポインタ。
	Pose*				m_pose = nullptr;
	float			m_maxRotate = 90.0f;					//回転の最大量。
	bool iskokok = false;
	BoxCollider			m_boxCollider;
	
};