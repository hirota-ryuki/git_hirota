#include "stdafx.h"
#include "RockDoor.h"
#include "Player.h"
RockDoor::~RockDoor()
{
}

void RockDoor::OnDestroy()
{
	DeleteGO(m_model);
}

bool RockDoor::Start()
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
	

	m_physicsStaticObject.CreateMeshObject(m_PSOmodel->GetModel(), m_position, m_rotation);
	
	/*std::wstring m_name;
	m_name = IDoor::GetName();
	if (m_name.c_str() == L"鍵無し") {

		SkinModelRender* model;
		model = NewGO<SkinModelRender>(GOPrio_Defalut);
		model->Init(L"modelData/debug/debugbox.cmo");
		model->SetData(m_centerPos, m_rotation);
	}*/

	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	return true;
}

void RockDoor::Update()
{
	CVector3 diff = m_player->GetPos() - m_centerPos;
	IDoor::MoveDoor(diff, m_model, m_PSOmodel, m_physicsStaticObject, m_rotation);
}
