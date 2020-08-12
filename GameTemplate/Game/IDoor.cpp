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
	//プレイヤーが近くに来たら。
	if (diff.Length() < 100.0f) {
		//Bボタンを押したら。
		if (g_pad[0].IsTrigger(enButtonB)) {
			if (Inv_FindItem(L"ball") >= 3) {
				//ワンショット再生のSE
				CSoundSource* m_se = new CSoundSource;
				m_se->Init(L"sound/story/decision.wav");
				m_se->Play(false);

				////回転。
				//CQuaternion qAddRot;
				//qAddRot.SetRotationDeg(CVector3::AxisY(), 2.0f);
				//m_rotation.Multiply(qAddRot, m_rotation);
				//model->SetRot(m_rotation);

			}
		}
	}
}