#include "stdafx.h"
#include "IDoor.h"

IDoor::IDoor()
{
}

IDoor::~IDoor()
{
	
}

void IDoor::MoveDoor(const CVector3& diff, SkinModelRender* model, SkinModelRender* PSOmodel, PhysicsStaticObject& pso,  CQuaternion rot)
{
	//�h�A���J���Ă��Ȃ�������B
	if (!m_isOpenDoor) {
		if (diff.Length() < ACTION_DISTANCE) {
			//B�{�^������������B
			if (g_pad[0].IsTrigger(enButtonB)) {
				//���s�v�̃h�A��������B
				if (wcscmp(L"������", m_name.c_str()) == 0) {
					Sound(L"sound/story/decision.wav", false);
					Message(L"�h�A���J�����B");
					//�h�A���J���B
					m_isOpenDoor = true;
					//��]������B
					m_isRotate = true;
				}
				else{
					//���������Ă�����B
					if (Inv_FindItem(m_name.c_str()) > 0) {
						Sound(L"sound/story/decision.wav", false);
						Message(L"�����g�p�����B");
						//���̌������炷�B
						Inv_AddItem(m_name.c_str(), -1);
						//�h�A���J���B
						m_isOpenDoor = true;
						//��]������B
						m_isRotate = true;
					}
					else {
						Message(L"���������Ă��Ȃ��B");
					}
				}
			}
		}		
	}

	if (m_isRotate) {
		//90�x��]����܂ŁB
		if (m_maxRotate > 0.0f) {
			//2�x����]�����Ă����B
			CQuaternion qAddRot;
			qAddRot.SetRotationDeg(CVector3::AxisY(), ADD_ROTATE);
			m_maxRotate += ADD_ROTATE;
			rot.Multiply(qAddRot, rot);
			model->SetRot(rot);
		}
		else {
			//�h�A���J���O�̓����蔻��������B
			pso.ReMove();
			//�h�A���J������̓����蔻����쐬�B
			m_physicsStaticObject.CreateMeshObject(PSOmodel->GetModel(), model->GetPos(), rot);
			m_isRotate = false;
		}
	}
}