#include "stdafx.h"
#include "MapChip.h"
#include "Level.h"
#include "physics/CollisionAttr.h"

MapChip::MapChip(const LevelObjectData& objData)
{
	wchar_t filePath[256];
	//swprintf_s(filePath, L"modelData/%s.cmo", objData.name);
	//m_model.Init(filePath);
	//m_model.UpdateWorldMatrix(objData.position, objData.rotation, CVector3::One());
	swprintf_s(filePath, L"modelData/mapChip/%s.cmo", objData.name);
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut,"mapchip");
	m_model->Init(filePath);
	m_model->SetData(objData.position, objData.rotation);
	//m_model->NotModelMode();
	//静的物理オブジェクトをメッシュコライダーから作成する。
	m_physicsStaticObject.CreateMeshObject(m_model->GetModel(), objData.position, objData.rotation);
	m_physicsStaticObject.SetUserIndex(enCollisionAttr_Map);
}