#pragma once
#include "IGameObject.h"
class IItem :
	public IGameObject
{
public:
	IItem();
	~IItem();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="texFilePath"></param>
	void InitModel(const wchar_t* texFilePath);
	/// <summary>
	/// �|�W�V�������Z�b�g�B
	/// </summary>
	/// <param name="pos">���W�B</param>
	void SetPos(CVector3 pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ��]���Z�b�g�B
	/// </summary>
	/// <param name="pos">��]�B</param>
	void SetRot(CQuaternion rot)
	{
		m_rotation = rot;
	}
private:
	SkinModelRender*	m_model;								//�X�L�����f���B
	CVector3			m_position = CVector3::Zero();			//���W�B
	CQuaternion			m_rotation = CQuaternion::Identity();	//��]�B

};

