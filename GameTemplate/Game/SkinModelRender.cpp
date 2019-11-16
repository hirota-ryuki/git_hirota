#include "stdafx.h"
#include "SkinModelRender.h"


SkinModelRender::SkinModelRender()
{
}


SkinModelRender::~SkinModelRender()
{
}

void SkinModelRender::Update()
{
	
		//���[���h�s��̍X�V�B
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_scale);
	
}

void SkinModelRender::Draw()
{
	if (isNotModel) {
		//���f��
		m_model.Draw(
			g_camera3D.GetViewMatrix(),
			g_camera3D.GetProjectionMatrix()
		);
	}
}

void SkinModelRender::Init(const wchar_t * texFilePath)
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(texFilePath);
}
