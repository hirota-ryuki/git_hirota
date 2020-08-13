#pragma once
#include "IGameObject.h"
const float ADD_ROTATE = 2.0f;	//1フレームに回転する量。
class IDoor :
	public IGameObject
{
public:
	IDoor();
	virtual ~IDoor();
	void MoveDoor(const CVector3& diff, SkinModelRender* model);
private:
	CQuaternion		m_rotation = CQuaternion::Identity();	//回転。
	float			m_maxRotate = 90.0f;					//回転の最大量。
};