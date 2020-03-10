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
	UpdateWorldMatrix();
	if (m_isShadow) {
		ShadowMap::GetInstance().RegistShadowCaster(&m_model);
	}
}
bool SkinModelRender::Start()
{
	return true;
}
void SkinModelRender::UpdateWorldMatrix()
{
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_pos, m_rot, m_scale);
	//Update���Ă񂾁B
	m_isUpdate = true;
}
void SkinModelRender::Draw()
{
	if (m_isUpdate) {
		if (m_isNotModel) {
			//���f��
			m_model.Draw(
				enRenderMode_Normal,
				g_camera3D.GetViewMatrix(),
				g_camera3D.GetProjectionMatrix()
			);
		}
	}
	//�r���[�s��̋t�s����v�Z�B
	m_viewMatrixInv.Inverse(g_camera3D.GetViewMatrix());
	//���f���̑O�����B
	m_forward.Set(m_viewMatrixInv.m[2][0], m_viewMatrixInv.m[2][1], m_viewMatrixInv.m[2][2]);
	int a = 10;
}

void SkinModelRender::Init(const wchar_t * texFilePath)
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(texFilePath);
}
