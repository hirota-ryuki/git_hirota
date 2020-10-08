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
	//ドアが開いていなかったら。
	if (!m_isOpenDoor) {
		if (diff.Length() < ACTION_DISTANCE) {
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
				else{
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
			m_physicsStaticObject.CreateMeshObject(PSOmodel->GetModel(), model->GetPos(), rot);
			m_isRotate = false;
		}
	}
}