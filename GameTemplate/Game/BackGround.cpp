#include "stdafx.h"

#include "BackGround.h"


BackGround::BackGround()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model = NewGO<SkinModelRender>();
	m_model->Init(L"modelData/battlefield/battlefield.cmo");	
	m_model->SetData(m_position, m_rotation, m_scale * 50.f);
	//�����蔻��B
	m_physicsStaticObject.CreateMeshObject(m_model->GetModel(), m_position, m_rotation);
}


BackGround::~BackGround()
{
	DeleteGO(m_model);
}

void BackGround::Update()
{
	//���[���h�s��̍X�V�B
	//m_model->SetData(m_position, m_rotation, m_scale * 50.f);
}

void BackGround::Draw()
{
}
