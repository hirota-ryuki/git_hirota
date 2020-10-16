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
	//ドア。
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

	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
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
	//ドアが開いていなかったら。
	if (!m_isOpenDoor) {
		if (diffsq < ACTION_DISTANCE_SQ) {
			//Bボタンを押したら。
			if (g_pad[0].IsTrigger(enButtonB)) {
				//鍵不要のドアだったら。
				if (wcscmp(L"鍵無し", m_name.c_str()) == 0) {
					Sound(L"sound/story/decision.wav", false);
					Message(L"ドアを開けた。");
					//ドアを開く。
					m_isOpenDoor = true;
					//回転させる。
					m_isRotate = true;
				}
				else {
					//鍵を持っていたら。
					if (Inv_FindItem(m_name.c_str()) > 0) {
						Sound(L"sound/story/decision.wav", false);
						Message(L"鍵を使用した。");
						//鍵の個数を減らす。
						Inv_AddItem(m_name.c_str(), -1);
						//ドアを開く。
						m_isOpenDoor = true;
						//回転させる。
						m_isRotate = true;
					}
					else {
						Message(L"鍵を持っていない。");
					}
				}
			}
		}
	}

	if (m_isRotate) {
		//90度回転するまで。
		if (m_maxRotate > 0.0f) {
			//2度ずつ回転させていく。
			CQuaternion qAddRot;
			qAddRot.SetRotationDeg(CVector3::AxisY(), ADD_ROTATE);
			m_maxRotate += ADD_ROTATE;
			rot.Multiply(qAddRot, rot);
			model->SetRot(rot);
		}
		else {
			//ドアが開く前の当たり判定を消す。
			pso.ReMove();
			//ドアが開いた後の当たり判定を作成。
			m_afterMovingPSO.CreateMeshObject(PSOmodel->GetModel(), model->GetPos(), rot);
			m_isRotate = false;
		}
	}
}