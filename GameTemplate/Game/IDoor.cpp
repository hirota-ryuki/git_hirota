#include "stdafx.h"
#include "IDoor.h"

IDoor::IDoor()
{
}

IDoor::~IDoor()
{
}

void IDoor::MoveDoor(const CVector3 & diff, SkinModelRender * model)
{
	//�v���C���[���߂��ɗ�����B
	if (diff.Length() < 100.0f) {
		//B�{�^������������B
		if (g_pad[0].IsTrigger(enButtonB)) {
			if (Inv_FindItem(L"ball") >= 3) {
				//�����V���b�g�Đ���SE
				CSoundSource* m_se = new CSoundSource;
				m_se->Init(L"sound/story/decision.wav");
				m_se->Play(false);

				////��]�B
				//CQuaternion qAddRot;
				//qAddRot.SetRotationDeg(CVector3::AxisY(), 2.0f);
				//m_rotation.Multiply(qAddRot, m_rotation);
				//model->SetRot(m_rotation);

			}
		}
	}
}