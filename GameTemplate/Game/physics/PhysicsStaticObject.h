/*!
 * @brief	静的オブジェクト。
 */

#pragma once

#include "physics/MeshCollider.h"
#include "physics/RigidBody.h"


/*!
* @brief	静的物理オブジェクト
*/
class PhysicsStaticObject{
public:
	/*!
		* @brief	コンストラクタ。
		*/
	PhysicsStaticObject();
	/*!
		* @brief	デストラクタ。
		*/
	~PhysicsStaticObject();
	/*!
		* @brief	メッシュの静的オブジェクトを作成。
		*@param[in]	skinModel	スキンモデル。
		*@param[in]	pos			座標。
		*@param[in]	rot			回転。
		*/
	void CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot);
	/// <summary>
	/// ユーザーインデックスを設定。
	/// </summary>
	/// <param name="index">ユーザーインデックス</param>
	void SetUserIndex(int index){
		m_rigidBody.GetBody()->setUserIndex(index);
	}
	void SetRotation(const CQuaternion& rot) {
		btRigidBody* btBody = m_rigidBody.GetBody();
		//剛体を動かす。
		btBody->setActivationState(DISABLE_DEACTIVATION);
		btTransform& trans = btBody->getWorldTransform();
		trans.setRotation(btQuaternion(rot.x, rot.y, rot.z));
	}
	void ReMove() {
		g_physics.RemoveRigidBody(m_rigidBody);
		m_isReMove = true;
	}
private:
	MeshCollider	m_meshCollider;			//!<メッシュコライダー。
	RigidBody		m_rigidBody;			//!<剛体。
	bool			m_isReMove = false;		//リムーブしたかどうか。
};
