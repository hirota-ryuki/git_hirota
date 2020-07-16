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
}

bool BulletStack::Start()
{
	//�e��B
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

	/*m_sprite = IItem::SpriteLoad(L"sprite/item/magazine_message.dds", ITEM_SPRITE_W, ITEM_SPRITE_H);
	IItem::ButtonSpriteLoad();*/

	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	//�v���C���[�̃C���X�^���X���擾�B
	m_player = m_game->GetPlayer();
	return true;
}

void BulletStack::Update()
{
	//��]�B
	CQuaternion qAddRot;
	qAddRot.SetRotationDeg(CVector3::AxisY(), 2.0f);
	m_rotation.Multiply(qAddRot, m_rotation);
	m_model->SetRot(m_rotation);
	m_model->UpdateWorldMatrix();

	/*CVector3 diff = m_player->GetPos() - m_position;
	IItem::SpriteMove(m_sprite, diff);
	GettingItem(IItem::IsGetItem(diff));
	IItem::ButtonSpriteMove(diff, m_position);*/
}

void BulletStack::GettingItem(bool isGetItem)
{
	//�A�C�e�����Q�b�g���Ă�����B
	if (isGetItem) {
		if (!m_isAddBullet) {
			//�e��𑝂₷�B
			m_player->AddStack(ADD_BULLET_STACK);
			m_isAddBullet = true;
		}
		//�摜�������I��������B
		if (IItem::GetIsFinishedMove()) {
			//���̃C���X�^���X�������B
			DeleteGO(this);
		}
	}
}