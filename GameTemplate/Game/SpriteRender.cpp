#include "stdafx.h"
#include "SpriteRender.h"

SpriteRender::SpriteRender()
{
}

SpriteRender::~SpriteRender()
{
}

bool SpriteRender::Start()
{
	return true;
}

void SpriteRender::Init(const wchar_t* texFilePath, float w, float h)
{
	m_sprite.Init(texFilePath, w, h);
}

void SpriteRender::Update()
{
	m_sprite.UpdateWorldMatrix(
		m_pos,
		m_rot,
		m_scale
	);
}

void SpriteRender::DrawHUD()
{
	//スプライト
	CMatrix mView;
	CMatrix mProj;
	mView.MakeLookAt(
		{ 0, 0, 1 },
		{ 0, 0, 0 },
		{ 0, 1, 0 }
	);
	mProj.MakeOrthoProjectionMatrix(1280, 720, 0, 100);
	m_sprite.SetAlpha(m_alpha);
	m_sprite.Draw(mView, mProj);
}