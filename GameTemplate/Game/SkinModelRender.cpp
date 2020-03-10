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
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_pos, m_rot, m_scale);
	//Updateを呼んだ。
	m_isUpdate = true;
}
void SkinModelRender::Draw()
{
	if (m_isUpdate) {
		if (m_isNotModel) {
			//モデル
			m_model.Draw(
				enRenderMode_Normal,
				g_camera3D.GetViewMatrix(),
				g_camera3D.GetProjectionMatrix()
			);
		}
	}
	//ビュー行列の逆行列を計算。
	m_viewMatrixInv.Inverse(g_camera3D.GetViewMatrix());
	//モデルの前方向。
	m_forward.Set(m_viewMatrixInv.m[2][0], m_viewMatrixInv.m[2][1], m_viewMatrixInv.m[2][2]);
	int a = 10;
}

void SkinModelRender::Init(const wchar_t * texFilePath)
{
	//cmoファイルの読み込み。
	m_model.Init(texFilePath);
}
