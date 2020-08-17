#pragma once
#include "IGameObject.h"
#include "physics/PhysicsStaticObject.h"
const float ADD_ROTATE = 2.0f;	//1フレームに回転する量。
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
	PhysicsStaticObject		m_physicsStaticObject;		//床の当たり判定。
	float					m_maxRotate = 90.0f;		//回転の最大量。
	bool					m_isRotate = false;
	bool					m_isOpenDoor = false;
	
	std::wstring			m_name;
	Game*					m_game = nullptr;
};