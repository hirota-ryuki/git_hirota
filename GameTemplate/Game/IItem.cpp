#include "stdafx.h"
#include "IItem.h"

IItem::IItem()
{
}

bool IItem::IsGetItem(CVector3 diff)
{
	if (diff.Length() < 100.0f) {
		//B�{�^������������B
		if (g_pad[0].IsTrigger(enButtonB)) {
			//�����V���b�g�Đ���SE
			CSoundSource* m_se = new CSoundSource;
			m_se->Init(L"sound/story/decision.wav");
			m_se->Play(false);
			m_isGetItem = true;
		}
	}
	return m_isGetItem;
}