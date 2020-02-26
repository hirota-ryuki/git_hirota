#include "stdafx.h"
#include "Ball.h"

Ball::Ball()
{
}

Ball::~Ball()
{
}

void Ball::OnDestroy()
{
	DeleteGO(m_model);
}

bool Ball::Start()
{
	//ボール。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	//ボールの識別番号。
	wchar_t ballNo[10];
	swprintf_s(ballNo, L"modelData/ball/ball%d.cmo", m_nomber);
	m_model->Init(ballNo);
	m_model->SetData(m_position, m_rotation);
	m_model->UpdateWorldMatrix();
	return true;
}

void Ball::Update()
{

	//Bボタンを押したら。
	if (g_pad[0].IsTrigger(enButtonB))
	{
		//ワンショット再生のSE
		CSoundSource* m_se = new CSoundSource;
		m_se->Init(L"sound/story/decision.wav");
		m_se->Play(false);
	}
}
