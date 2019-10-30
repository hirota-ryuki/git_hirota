#include "stdafx.h"
#include "physics/Physics.h"
#include "Physics/RigidBody.h"


PhysicsWorld g_physics;

PhysicsWorld::~PhysicsWorld()
{
	Release();
}
void PhysicsWorld::Release()
{
	delete dynamicWorld;
	delete constraintSolver;
	delete overlappingPairCache;
	delete collisionDispatcher;
	delete collisionConfig;

	dynamicWorld = nullptr;
	constraintSolver = nullptr;
	overlappingPairCache = nullptr;
	collisionDispatcher = nullptr;
	collisionConfig = nullptr;
}
void PhysicsWorld::Init()
{
	Release();

	//デバッグワイヤーフレームの準備
	dw.Prepare();

	//物理エンジンを初期化。
	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	collisionConfig = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	collisionDispatcher = new	btCollisionDispatcher(collisionConfig);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	constraintSolver = new btSequentialImpulseConstraintSolver;

	dynamicWorld = new btDiscreteDynamicsWorld(
		collisionDispatcher,
		overlappingPairCache,
		constraintSolver,
		collisionConfig
		);

	dynamicWorld->setGravity(btVector3(0, -10, 0));

	//デバッグワイヤーフレームをダイナミックワールドに設定
	//ここでdrawLineを勝手に呼んでくれる
	//流石です
	dynamicWorld->setDebugDrawer(&dw);
}
void PhysicsWorld::Update()
{
	dynamicWorld->stepSimulation(1.0f/60.0f);
}
void PhysicsWorld::AddRigidBody(RigidBody& rb)
{
	dynamicWorld->addRigidBody(rb.GetBody());
}
void PhysicsWorld::RemoveRigidBody(RigidBody& rb)
{
	dynamicWorld->removeRigidBody(rb.GetBody());
}

void PhysicsWorld::DebugDraw()
{
	dw.Context();
	dynamicWorld->debugDrawWorld();
}
