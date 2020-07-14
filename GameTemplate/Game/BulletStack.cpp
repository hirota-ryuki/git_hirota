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
	m_model->InitNormalMap(L"modelData/bulletstackd/Normal.dds");
	m_model->InitSpecMap(L"modelData/bulletstack/Metalness.dds");
	m_model->SetData(m_position, m_rotation);
	m_model->SetDirectionLightDirection(m_directionLightDirection);
	m_model->SetDirectionLightColor(m_directionLightColor);
	m_model->SetSpec(m_spec);
	m_model->SetAmbientLight(m_ambientLight);
	IItem::SetName(L"BulletStack");

	m_sprite = IItem::SpriteLoad(L"sprite/item/item_message.dds", ITEM_SPRITE_W, ITEM_SPRITE_H);
	m_spriteA = IItem::SpriteLoad(L"sprite/item/button.dds", 50.0f, 50.0f);

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

	CVector3 diff = m_player->GetPos() - m_position;
	/*IItem::SpriteMove(m_sprite, diff);
	GettingItem(IItem::IsGetItem(diff));*/
	
}

void BulletStack::GettingItem(bool isGetItem)
{
	//�A�C�e�����Q�b�g���Ă�����B
	if (isGetItem) {
		//�e��𑝂₷�B
		m_player->AddStack(ADD_BULLET_STACK);
		//�摜�������I��������B
		if (IItem::GetIsFinishedMove()) {
			//���̃C���X�^���X�������B
			DeleteGO(this);
		}
	}
}

void BulletStack::sp()
{
	m_spriteA->ActiveMode(false);

	QueryGOs<BulletStack>("bulletstack", [&](BulletStack * bulletstack)->bool {
		CVector3 diff = m_player->GetPos() - bulletstack->GetPos();
		if (diff.Length() < 500.f) { //������1000�ȉ��ɂȂ�����B
			m_model2Dpos = { m_position.x, m_position.y, m_position.z, 1.0f };
			g_camera3D.GetViewMatrix().Mul(m_model2Dpos);
			g_camera3D.GetProjectionMatrix().Mul(m_model2Dpos);
			m_model2Dpos.x /= m_model2Dpos.w;
			m_model2Dpos.y /= m_model2Dpos.w;
			m_spriteA->SetPos({ m_model2Dpos.x*FRAME_BUFFER_W / 2 * -1,m_model2Dpos.y*FRAME_BUFFER_H / 2 });
			m_spriteA->ActiveMode(true);
		}
		return true;
	});
}
