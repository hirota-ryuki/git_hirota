#pragma once
#include "physics/PhysicsStaticObject.h"
class Navimesh;

class Floor final:
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
	/// <param name="pos">���W�B</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ���̉�]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]�B</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// �i�r���b�V���̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>�i�r���b�V���̃|�C���^�B</returns>
	Navimesh* GetNavimesh()
	{
		return m_navimesh;
	}
	/// <summary>
	/// �X�L�����f�������_�[�̃C���X�^���X���擾�B
	/// </summary>
	/// <returns>�X�L�����f�������_�[�̃|�C���^�B</returns>
	SkinModelRender* GetSkinModelRender()
	{
		return m_model;
	}
private:
	SkinModelRender*		m_model = nullptr;						//���̃X�L�����f���B
	PhysicsStaticObject		m_physicsStaticObject;					//���̓����蔻��B
	CVector3				m_position = CVector3::Zero();			//���W�B
	CQuaternion				m_rotation = CQuaternion::Identity();	//��]�B
	Navimesh*				m_navimesh;								//Navimesh�N���X�B
	bool					m_isPhysics = false;					//�����蔻���t�������ǂ����B
};