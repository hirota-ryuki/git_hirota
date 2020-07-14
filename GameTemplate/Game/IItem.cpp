#include "stdafx.h"
#include "IItem.h"

//static�Œ�`�����Ȃ�cpp�Ŏ��̂������K�v������B
std::unordered_map<
	std::wstring,
	std::unique_ptr<SpriteRender, deleter_SpriteRender>
> IItem::m_itemSpriteMap;
Sprite	IItem::m_sprite;
bool	IItem::m_isCreateSprite = false;

IItem::IItem()
{
}

IItem::~IItem()
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

SpriteRender* IItem::SpriteLoad(const wchar_t* filePath, float w, float h)
{
	SpriteRender* sprite = nullptr;
	auto it = m_itemSpriteMap.find(filePath);
	if (it == m_itemSpriteMap.end()) 
	{
	//if (m_itemSpriteMap.count(filePath) == 1) {
		//���o�^�Ȃ̂ŁA�V�K�Ń��[�h����B
		SpriteRender* spriteData = NewGO<SpriteRender>(GOPrio_Sprite);
		spriteData->Init(filePath, w, h);
		spriteData->SetPos(FRAME_OUT_POS);
		sprite = spriteData;
		m_itemSpriteMap.emplace(filePath, spriteData);
	}
	else {
		auto mapData = it->second.get();
		sprite = mapData;
	}
	return sprite;
}

SpriteRender* IItem::ButtonSpriteLoad(SpriteRender * sprite)
{
	sprite = NewGO<SpriteRender>(GOPrio_Sprite);
	if (!m_isCreateSprite) {
		sprite->Init(L"sprite/item/button.dds", 50.0f, 50.0f);
		m_sprite = sprite->GetSprite();
		m_isCreateSprite = true;
	}
	else {
		sprite->SetSprite(m_sprite);
	}
	return sprite;
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
			m_isFinishedMove = false;
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
			m_isFinishedMove = true;
		}
		break;
	default:
		break;
	}
}
