#include "stdafx.h"
#include "IItem.h"

IItem::IItem()
{
}

bool IItem::IsGetItem(CVector3 diff)
{
	//距離が近かったら。
	if (diff.Length() < 100.0f) {
		//Bボタンを押したら。
		if (g_pad[0].IsTrigger(enButtonB)) {
			//ワンショット再生のSE。
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
	//プレイヤーが近くにいたかどうか。
	if (!m_isNearPlayer) {
		//プレイヤーとの距離が近かったら。
		if (diff.Length() < 100.0f) {
			m_isNearPlayer = true;
			//画像の座標の初期化。
			sprite->SetPos(m_initialPos);
		}
	}
	else{
		if (m_addCount >= ADD_COUNT) {
			//プレイヤーとの距離が遠かったら。
			if (diff.Length() >= 100.0f) {
				m_isNearPlayer = false;
				m_addCount = 0;
			}
		}
		else {
			//画像を動かす。
			sprite->SetPos(sprite->GetPos() + m_addPos);
			m_addCount++;
		}
	}	
}
