#include "stdafx.h"
#include "BackGround.h"


BackGround::BackGround()
{
	//床なし背景
	//cmoファイルの読み込み。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/battlefield/battlefield.cmo");	
	m_model->SetData(m_position, m_rotation, m_scale);
	//当たり判定。
	m_physicsStaticObject.CreateMeshObject(m_model->GetModel(), m_position, m_rotation);

	//床
	//m_floorModel = NewGO<SkinModelRender>(GOPrio_Defalut);
	////m_floorModel->Init(L"modelData/battlefield/floor.cmo");
	//m_floorModel->Init(L"modelData/enemy/enemy.cmo");
	//m_floorModel->SetData(m_position, m_rotation, m_scale);
	////あたり判定。
	//m_floorPhysicsStaticObject.CreateMeshObject(m_floorModel->GetModel(), m_position, m_rotation);

}


BackGround::~BackGround()
{
	DeleteGO(m_model);
	DeleteGO(m_floorModel);
}

void BackGround::Update()
{
	//ワールド行列の更新。
	//m_model->SetData(m_position, m_rotation, m_scale * 50.f);
}