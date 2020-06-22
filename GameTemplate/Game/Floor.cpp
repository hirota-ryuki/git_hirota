#include "stdafx.h"
#include "Floor.h"
#include "Navimesh.h"

Floor::Floor()
{
}

Floor::~Floor()
{
}

void Floor::OnDestroy()
{
	DeleteGO(m_model);
	DeleteGO(m_navimesh);
}

bool Floor::Start()
{
	//床。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/battlefield/floor.cmo");
	m_model->SetData(m_position, m_rotation);
	m_model->SetShadowReciever(true);
	m_model->SetShadowCaster(false);
	m_model->UpdateWorldMatrix();
	//ナビメッシュの生成。
	m_navimesh = NewGO<Navimesh>(GOPrio_Defalut);
	m_navimesh->Init(this);
	m_physicsStaticObject.CreateMeshObject(m_model->GetModel(), m_position, m_rotation);

	return true;
}

void Floor::Update()
{
	m_model->SetData(m_position, m_rotation);
	m_model->SetShadowCaster(false);
}