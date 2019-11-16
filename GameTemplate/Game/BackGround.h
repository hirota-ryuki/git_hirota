#pragma once
#include "physics/PhysicsStaticObject.h"

class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	void Update() override;
	void Draw() override {};
private:
	SkinModelRender* m_model = nullptr;					//床なし背景のスキンモデル。
	SkinModelRender* m_floorModel = nullptr;			//床のスキンモデル。
	PhysicsStaticObject m_physicsStaticObject;			//床なし背景の当たり判定
	PhysicsStaticObject m_floorPhysicsStaticObject;		//床の当たり判定
	CVector3 m_position = CVector3::Zero();				//座標。
	CQuaternion m_rotation = CQuaternion::SpriteRot();	//回転。
	CVector3 m_scale = CVector3::One();					//大きさ。
};

class Enemy
{
public:
	Enemy() :
		hp(100)
	{
		hp = 100;
	}
private:
	int hp = 0;
};