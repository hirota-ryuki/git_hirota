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
	//�{�[���B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	//�{�[���̎��ʔԍ��B
	wchar_t ballNo[10];
	swprintf_s(ballNo, L"modelData/ball/ball%d.cmo", m_nomber);
	m_model->Init(ballNo);
	m_model->SetData(m_position, m_rotation);
	m_model->UpdateWorldMatrix();
	return true;
}

void Ball::Update()
{

	//B�{�^������������B
	if (g_pad[0].IsTrigger(enButtonB))
	{
		//�����V���b�g�Đ���SE
		CSoundSource* m_se = new CSoundSource;
		m_se->Init(L"sound/story/decision.wav");
		m_se->Play(false);
	}
}
