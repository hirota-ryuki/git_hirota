#include "stdafx.h"
#include "Door.h"
#include "Player.h"
Door::~Door()
{
}

void Door::OnDestroy()
{
	DeleteGO(m_model);
}

bool Door::Start()
{
	//�h�A�B
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/rockdoor/blood_door/door.cmo");
	m_model->SetShadowCaster(true);
	m_model->SetShadowReciever(true);
	m_model->SetData(m_position, m_rotation);
	m_model->UpdateWorldMatrix();

	m_PSOmodel = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_PSOmodel->Init(L"modelData/rockdoor/rockdoor.cmo");
	m_PSOmodel->ActiveMode(false);	

	m_beforeMovingPSO.CreateMeshObject(m_PSOmodel->GetModel(), m_position, m_rotation);

	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	//�v���C���[�̃C���X�^���X���擾�B
	m_player = m_game->GetPlayer();
	return true;
}

void Door::Update()
{
	float diffsq = m_player->CalcDistanceSQFrom(m_centerPos);
	MoveDoor(diffsq, m_model, m_PSOmodel, m_beforeMovingPSO, m_rotation);
}

void Door::MoveDoor(const float diffsq, SkinModelRender* model, SkinModelRender* PSOmodel, PhysicsStaticObject& pso, CQuaternion& rot)
{
	//�h�A���J���Ă��Ȃ�������B
	if (!m_isOpenDoor) {
		if (diffsq < ACTION_DISTANCE_SQ) {
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
				else {
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
			m_afterMovingPSO.CreateMeshObject(PSOmodel->GetModel(), model->GetPos(), rot);
			m_isRotate = false;
		}
	}
}