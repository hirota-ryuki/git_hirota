#include "stdafx.h"
#include "BulletStack.h"
#include "Player.h"
BulletStack::BulletStack()
{
}

BulletStack::~BulletStack()
{
}

void BulletStack::OnDestroy()
{
	DeleteGO(m_model);
	IItem::Destroy();
}

bool BulletStack::Start()
{
	//弾薬。
	m_model = NewGO<SkinModelRender>(GOPrio_Defalut);
	m_model->Init(L"modelData/bulletstack/bulletstack.cmo");
	m_model->InitNormalMap(L"modelData/bulletstack/Normal.dds");
	m_model->InitSpecMap(L"modelData/bulletstack/Metalness.dds");
	m_model->SetData(m_position, m_rotation);
	m_model->SetDirectionLightDirection(m_directionLightDirection);
	m_model->SetDirectionLightColor(m_directionLightColor);
	m_model->SetSpec(m_spec);
	m_model->SetAmbientLight(m_ambientLight);
	IItem::SetName(L"BulletStack");

	m_sprite = IItem::SpriteLoad(L"sprite/item/magazine_message.dds", ITEM_SPRITE_W, ITEM_SPRITE_H);
	IItem::IItemInit();

	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	return true;
}

void BulletStack::Update()
{
	//回転。
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY(), 2.0f);
	m_rotation.Multiply(qAddRot, m_rotation);
	m_model->SetRot(m_rotation);
	m_model->UpdateWorldMatrix();

	IItem::ItemCommonProcessing(m_sprite, m_position, m_model);
}

void BulletStack::OnGet()
{
	//弾薬を増やす。
	m_player->AddStack(ADD_BULLET_STACK);
	m_isAddBullet = true;
}