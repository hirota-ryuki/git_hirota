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
	SkinModelRender* m_model = nullptr;					//�X�L�����f���B
	PhysicsStaticObject m_physicsStaticObject;
	CVector3 m_position = CVector3::Zero();				//���W�B
	CQuaternion m_rotation = CQuaternion::SpriteRot();	//��]�B
	CVector3 m_scale = CVector3::One();					//�傫���B
};

