#include "stdafx.h"
#include "BackGround.h"


BackGround::BackGround()
{
	//���Ȃ��w�i
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/battlefield/battlefield.cmo");	
	m_model->SetData(m_position, m_rotation, m_scale);
	//�����蔻��B
	m_physicsStaticObject.CreateMeshObject(m_model->GetModel(), m_position, m_rotation);

	//��
	//m_floorModel = NewGO<SkinModelRender>(GOPrio_Defalut);
	////m_floorModel->Init(L"modelData/battlefield/floor.cmo");
	//m_floorModel->Init(L"modelData/enemy/enemy.cmo");
	//m_floorModel->SetData(m_position, m_rotation, m_scale);
	////�����蔻��B
	//m_floorPhysicsStaticObject.CreateMeshObject(m_floorModel->GetModel(), m_position, m_rotation);

}


BackGround::~BackGround()
{
	DeleteGO(m_model);
	DeleteGO(m_floorModel);
}

void BackGround::Update()
{
	//���[���h�s��̍X�V�B
	//m_model->SetData(m_position, m_rotation, m_scale * 50.f);
}