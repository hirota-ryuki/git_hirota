#include "stdafx.h"
#include "CameraCollisionSolver.h"
#include "physics/CollisionAttr.h"

/*!
 *@brief	カメラのコリジョンソルバー。
 */

	struct SConvexSweepCallback : public btCollisionWorld::ClosestConvexResultCallback
	{
	public:
		CVector3 m_rayDir;
		SConvexSweepCallback(CVector3 rayDir) :
			btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 0.0f)),
			m_rayDir(rayDir) {}
		virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
		{
			CVector3 normal;
			normal.Set(convexResult.m_hitNormalLocal);
			//一時的
			if (convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_Character||convexResult.m_hitCollisionObject->getUserIndex() == enCollisionAttr_PassingWallCharacter) {
				return 1.0f;
			}
			return btCollisionWorld::ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
		}
	};
	CameraCollisionSolver::CameraCollisionSolver()
	{
	}
	CameraCollisionSolver::~CameraCollisionSolver()
	{
	}
	void CameraCollisionSolver::Init(float radius)
	{
		m_radius = radius;
		m_collider.Create(radius);
		m_isInited = true;	//初期化済みのフラグを立てる。
	}
	bool CameraCollisionSolver::Execute(CVector3& result, const CVector3& position, const CVector3& target)
	{
		if (m_isInited == false) {
			/*WARNING_MESSAGE_BOX("初期化されていないカメラコリジョンソルバーのExecute関数が呼ばれています。\n"
				"CameraCollisionSolver::Init関数を呼び出してください。\n");
			*/
			static wchar_t log[1024 * 10];
			MessageBoxW(NULL, log, L"Warning!!", MB_OK);

			return false;

		}
		result = position;
		CVector3 vWk;
		vWk.Subtract(target, position);
		if (vWk.LengthSq() < FLT_EPSILON) {
			//視点と注視点がほぼ同じ座標にある。
			return false;
		}
		vWk.Normalize();
		//レイを作成する。
		btTransform btStart, btEnd;
		btStart.setIdentity();
		btEnd.setIdentity();
		btStart.setOrigin(btVector3(target.x, target.y, target.z));
		btEnd.setOrigin(btVector3(position.x, position.y, position.z));
		SConvexSweepCallback callback(vWk);
		//	callback.m_collisionFilterGroup = 
		g_physics.ConvexSweepTest((const btConvexShape*)m_collider.GetBody(), btStart, btEnd, callback);
		if (callback.hasHit()) {
			CVector3 vHitPos;
			vHitPos.Set(callback.m_hitPointWorld);
			CVector3 vOffset;
			vOffset.Set(callback.m_hitNormalWorld);
			vOffset.Scale(m_radius);
			result.Add(vHitPos, vOffset);
			return true;
		}
		return false;
	}

