#pragma once
#include "IGameObject.h"
#include "physics/PhysicsStaticObject.h"
const float ADD_ROTATE = 2.0f;	//1�t���[���ɉ�]����ʁB
class Player;
class IDoor :
	public IGameObject
{
public:
	IDoor();
	virtual ~IDoor();
	/// <summary>
	/// 
	/// </summary>
	/// <param name="diff"></param>
	/// <param name="model"></param>
	/// <param name="PSOmodel"></param>
	/// <param name="pso"></param>
	/// <param name="rot"></param>
	void MoveDoor(const CVector3& diff, SkinModelRender* model, SkinModelRender* PSOmodel, PhysicsStaticObject& pso, CQuaternion& rot);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="name"></param>
	void SetName(std::wstring name) {
		m_name = name;
	}
	/*std::wstring GetName() {
		return m_name;
	}*/
private:
	PhysicsStaticObject		m_physicsStaticObject;		//���̓����蔻��B
	float					m_maxRotate = 90.0f;		//��]�̍ő�ʁB
	bool					m_isRotate = false;
	bool					m_isOpenDoor = false;
	
	std::wstring			m_name;
	Game*					m_game = nullptr;
};