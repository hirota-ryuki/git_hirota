#include "stdafx.h"
#include "BackGround.h"


BackGround::BackGround()
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"modelData/battlefield/battlefield.cmo");
	//m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
}


BackGround::~BackGround()
{
}

void BackGround::Update()
{
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale * 50.f);
}

void BackGround::Draw()
{
	//���f��
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
