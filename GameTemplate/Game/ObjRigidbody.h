#pragma once
#include "physics/PhysicsStaticObject.h"
class ObjRigidbody :
	public IGameObject
{
public:
	ObjRigidbody();
	~ObjRigidbody();
	void OnDestroy() override;
	bool Start() override;
	void Update() override;
	/// <summary>
	/// �|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	///�@��]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]�B</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model;								//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B
	//�����蔻��B
	SkinModelRender* m_PSOmodel = nullptr;					//���̃X�L�����f���B
	PhysicsStaticObject m_physicsStaticObject;			//���̓����蔻��B
};

