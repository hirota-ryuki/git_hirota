#include "stdafx.h"
#include "BackGround.h"


BackGround::BackGround()
{
	//cmoファイルの読み込み。
	m_model.Init(L"modelData/battlefield/battlefield.cmo");
	//m_rotation.SetRotationDeg(CVector3::AxisY(), 180.f);
}


BackGround::~BackGround()
{
}

void BackGround::Update()
{
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale * 50.f);
}

void BackGround::Draw()
{
	//モデル
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
