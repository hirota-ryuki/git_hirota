/*!
 * @brief	静的オブジェクト。
 */


#include "stdafx.h"
#include "physics/GhostObject.h"
#include "physics/PhysicsObjectBase.h"
#include "physics/BoxCollider.h"
#include "physics/CapsuleCollider.h"
#include "physics/SphereCollider.h"

using namespace std;
namespace hrEngine 
{

	void CPhysicsGhostObject::Release()
	{
		if (m_isRegistPhysicsWorld == true) {
			PhysicsWorld().RemoveCollisionObject(m_ghostObject);
			m_isRegistPhysicsWorld = false;
		}
	}
	void CPhysicsGhostObject::CreateCommon(CVector3 pos, CQuaternion rot)
	{
		m_ghostObject.setCollisionShape(m_collider->GetBody());
		btTransform btTrans;
		btTrans.setOrigin({ pos.x, pos.y, pos.z });
		btTrans.setRotation({ rot.x, rot.y, rot.z, rot.w });
		m_ghostObject.setWorldTransform(btTrans);

		//物理エンジンに登録。
		PhysicsWorld().AddCollisionObject(m_ghostObject);
		m_isRegistPhysicsWorld = true;
	}
}
