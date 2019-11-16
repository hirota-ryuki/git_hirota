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
	SkinModelRender* m_model = nullptr;					//���Ȃ��w�i�̃X�L�����f���B
	SkinModelRender* m_floorModel = nullptr;			//���̃X�L�����f���B
	PhysicsStaticObject m_physicsStaticObject;			//���Ȃ��w�i�̓����蔻��
	PhysicsStaticObject m_floorPhysicsStaticObject;		//���̓����蔻��
	CVector3 m_position = CVector3::Zero();				//���W�B
	CQuaternion m_rotation = CQuaternion::SpriteRot();	//��]�B
	CVector3 m_scale = CVector3::One();					//�傫���B
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