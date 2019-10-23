#include "stdafx.h"

#include "BackGround.h"


BackGround::BackGround()
{
	//cmoファイルの読み込み。
	m_model = NewGO<SkinModelRender>();
	m_model->Init(L"modelData/battlefield/battlefield.cmo");	
	m_model->SetData(m_position, m_rotation, m_scale * 50.f);
	//当たり判定。
	m_physicsStaticObject.CreateMeshObject(m_model->GetModel(), m_position, m_rotation);
}


BackGround::~BackGround()
{
	DeleteGO(m_model);
}

void BackGround::Update()
{
	//ワールド行列の更新。
	//m_model->SetData(m_position, m_rotation, m_scale * 50.f);
}

void BackGround::Draw()
{
}
