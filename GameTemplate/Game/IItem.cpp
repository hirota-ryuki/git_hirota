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

void IItem::SpriteMove(SpriteRender* sprite, CVector3 diff)
{
	//�v���C���[���߂��ɂ������ǂ����B
	if (!m_isNearPlayer) {
		//�v���C���[�Ƃ̋������߂�������B
		if (diff.Length() < 100.0f) {
			m_isNearPlayer = true;
			//�摜�̍��W�̏������B
			sprite->SetPos(m_initialPos);
		}
	}
	else{
		if (m_addCount >= ADD_COUNT) {
			//�v���C���[�Ƃ̋���������������B
			if (diff.Length() >= 100.0f) {
				m_isNearPlayer = false;
				m_addCount = 0;
			}
		}
		else {
			//�摜�𓮂����B
			sprite->SetPos(sprite->GetPos() + m_addPos);
			m_addCount++;
		}
	}	
}
