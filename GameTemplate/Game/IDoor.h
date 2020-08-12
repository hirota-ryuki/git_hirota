#pragma once
#include "IGameObject.h"
class IDoor :
	public IGameObject
{
public:
	IDoor();
	virtual ~IDoor();
	void MoveDoor(const CVector3& diff, SkinModelRender* model);
private:
	CQuaternion			m_rotation = CQuaternion::Identity();	//âÒì]ÅB

};