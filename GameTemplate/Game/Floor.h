#pragma once
#include "physics/PhysicsStaticObject.h"
class Navimesh;

class Floor :
	public IGameObject
{
public:
	Floor();
	~Floor(); 
	void OnDestroy() override;
	bool Start() override;
	void Update() override ;
	/// <summary>
	/// ���̃|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ���̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// �i�r���b�V���̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>�i�r���b�V���̃C���X�^���X�B</returns>
	Navimesh* GetNavimesh()
	{
		return m_navimesh;
	}
	SkinModelRender* GetSkinModelRender()
	{
		return m_model;
	}
private:
	SkinModelRender* m_model = nullptr;					//���̃X�L�����f���B
	PhysicsStaticObject m_physicsStaticObject;			//���̓����蔻��B
	CVector3 m_position = CVector3::Zero();				//���W�B
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	Navimesh* m_navimesh;								//Navimesh�N���X�B
	bool m_isPhysics = false;							//�����蔻���t�������ǂ����B
};