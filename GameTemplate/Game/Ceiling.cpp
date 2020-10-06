#include "stdafx.h"
#include "Ceiling.h"

Ceiling::Ceiling()
{
}

Ceiling::~Ceiling()
{
}

void Ceiling::OnDestroy()
{
	DeleteGO(m_model);
}

bool Ceiling::Start()
{	
	//“VˆäB
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/ceiling/ceiling.cmo");
	m_model->SetData(m_position, m_rotation);
	m_model->UpdateWorldMatrix();
	m_model->SetShadowCaster(false);
	return true;
}

void Ceiling::Update()
{
}
