#pragma once
#include "IGameObject.h"
const float ADD_ROTATE = 2.0f;	//1�t���[���ɉ�]����ʁB
class IDoor :
	public IGameObject
{
public:
	IDoor();
	virtual ~IDoor();
	void MoveDoor(const CVector3& diff, SkinModelRender* model);
private:
	CQuaternion		m_rotation = CQuaternion::Identity();	//��]�B
	float			m_maxRotate = 90.0f;					//��]�̍ő�ʁB
};