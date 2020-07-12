#include "stdafx.h"
#include "IItem.h"

IItem::IItem()
{
}

bool IItem::IsGetItem(CVector3 diff)
{
	//�������߂�������B
	if (diff.Length() < 100.0f) {
		//B�{�^������������B
		if (g_pad[0].IsTrigger(enButtonB)) {
			//�����V���b�g�Đ���SE�B
			CSoundSource* m_se = new CSoundSource;
			m_se->Init(L"sound/story/decision.wav");
			m_se->Play(false);
			m_isGetItem = true;
		}
	}
	return m_isGetItem;
}

SpriteRender* IItem::SpriteLoad(const wchar_t* filePath)
{
	SpriteRender* sprite = nullptr;
	auto it = m_itemSpriteMap.find(filePath);
	if (it == m_itemSpriteMap.end()) {
		//���o�^�Ȃ̂ŁA�V�K�Ń��[�h����B
		SpriteRender* spriteData = NewGO<SpriteRender>(GOPrio_Sprite);
		spriteData->Init(filePath, ITEM_SPRITE_W, ITEM_SPRITE_H);
		spriteData->SetPos(FRAME_OUT_POS);
		sprite = spriteData;
		m_itemSpriteMap.emplace(filePath, std::move(spriteData));
	}
	else {
		auto mapData = it->second.get();
		sprite = *mapData;
	}
	return nullptr;
}

void IItem::SpriteMove(SpriteRender* sprite, CVector3 diff)
{
	switch (m_state) {
	//�v���C���[���߂��ɗ������B
	case enState_nearPlayer:
		//�v���C���[�Ƃ̋������߂��Ȃ�����B
		if (diff.Length() < 100.0f) {
			//�t���O�𗧂Ă�B
			m_isNearPlayer = true;
		}
		//�t���O�������Ă�����B
		if (m_isNearPlayer) {
			//�摜�����ɓ������B
			m_movedPos.x = sprite->GetPos().x + AMOUNT_OF_CHANGE;
			m_movedPos.y = FRAME_IN_POS.y;
			sprite->SetPos(m_movedPos);
			//�ŏI�n�_�ɒB������B
			if (sprite->GetPos().x >= FRAME_IN_POS.x) {
				//���̃X�e�b�v�Ɉڍs�B
				m_state = enState_stopPlayer;
				m_isNearPlayer = false;
			}
		}
		break;
	//�v���C���[���A�C�e���̋߂��ɂ��鎞�B
	case enState_stopPlayer:
		//�v���C���[���A�C�e�����痣�ꂽ��B
		if (diff.Length() >= 100.0f) {
			//���̃X�e�b�v�Ɉڍs�B
			m_state = enState_farPlayer;
		}
		break;
	//�v���C���[���A�C�e�����痣�ꂽ���B
	case enState_farPlayer:
		//�摜���E�ɓ������B
		m_movedPos.x = sprite->GetPos().x - AMOUNT_OF_CHANGE;
		m_movedPos.y = FRAME_OUT_POS.y;
		sprite->SetPos(m_movedPos);
		//�ŏI�n�_�ɒB������B
		if (sprite->GetPos().x <= FRAME_OUT_POS.x) {
			//�ŏ��̃X�e�b�v�ɖ߂�B
			m_state = enState_nearPlayer;
		}
		break;
	default:
		break;
	}
}
