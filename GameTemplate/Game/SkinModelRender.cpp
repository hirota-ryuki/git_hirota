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
	
		//ワールド行列の更新。
		m_model.UpdateWorldMatrix(m_pos, m_rot, m_scale);
	
}

void SkinModelRender::Draw()
{
	if (isNotModel) {
		//モデル
		m_model.Draw(
			g_camera3D.GetViewMatrix(),
			g_camera3D.GetProjectionMatrix()
		);
	}
}

void SkinModelRender::Init(const wchar_t * texFilePath)
{
	//cmoファイルの読み込み。
	m_model.Init(texFilePath);
}
