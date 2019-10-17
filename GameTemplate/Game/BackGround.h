#pragma once
#include "physics/PhysicsStaticObject.h"

class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	void Update() override;
	void Draw() override;
private:
	SkinModelRender* m_model = nullptr;					//スキンモデル。
	PhysicsStaticObject m_physicsStaticObject;
	CVector3 m_position = CVector3::Zero();				//座標。
	CQuaternion m_rotation = CQuaternion::SpriteRot();	//回転。
	CVector3 m_scale = CVector3::One();					//大きさ。
};

