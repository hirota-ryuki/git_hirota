#include "stdafx.h"
#include "ObjRigidbody.h"
#include "physics/CollisionAttr.h"

ObjRigidbody::ObjRigidbody()
{
}

ObjRigidbody::~ObjRigidbody()
{
}

void ObjRigidbody::OnDestroy()
{
	DeleteGO(m_model);
	DeleteGO(m_PSOmodel);
}

bool ObjRigidbody::Start()
{
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/object/object.cmo");
	m_model->SetData(m_position, m_rotation);
	m_model->UpdateWorldMatrix();
	m_model->SetShadowCaster(true);
	//m_model->ActiveMode(false);

	m_PSOmodel = NewGO<SkinModelRender>(GOPrio_Defalut);
	//m_PSOmodel->Init(L"modelData/object/obj_rigidbody/obj_rigidbody.cmo");
	m_PSOmodel->Init(L"modelData/mapchip/RB_obj.cmo");
	m_PSOmodel->SetData(m_position, m_rotation);
	m_PSOmodel->ActiveMode(false);
	m_physicsStaticObject.CreateMeshObject(m_PSOmodel->GetModel(), m_position, m_rotation);
	m_physicsStaticObject.SetUserIndex(enCollisionAttr_RigidBody);

	return true;
}

void ObjRigidbody::Update()
{
}
