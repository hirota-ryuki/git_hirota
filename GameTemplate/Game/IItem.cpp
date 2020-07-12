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

SpriteRender* IItem::SpriteLoad(const wchar_t* filePath)
{
	SpriteRender* sprite = nullptr;
	auto it = m_itemSpriteMap.find(filePath);
	if (it == m_itemSpriteMap.end()) {
		//未登録なので、新規でロードする。
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
	//プレイヤーが近くに来た時。
	case enState_nearPlayer:
		//プレイヤーとの距離が近くなったら。
		if (diff.Length() < 100.0f) {
			//フラグを立てる。
			m_isNearPlayer = true;
		}
		//フラグが立っていたら。
		if (m_isNearPlayer) {
			//画像を左に動かす。
			m_movedPos.x = sprite->GetPos().x + AMOUNT_OF_CHANGE;
			m_movedPos.y = FRAME_IN_POS.y;
			sprite->SetPos(m_movedPos);
			//最終地点に達したら。
			if (sprite->GetPos().x >= FRAME_IN_POS.x) {
				//次のステップに移行。
				m_state = enState_stopPlayer;
				m_isNearPlayer = false;
			}
		}
		break;
	//プレイヤーがアイテムの近くにいる時。
	case enState_stopPlayer:
		//プレイヤーがアイテムから離れたら。
		if (diff.Length() >= 100.0f) {
			//次のステップに移行。
			m_state = enState_farPlayer;
		}
		break;
	//プレイヤーがアイテムから離れた時。
	case enState_farPlayer:
		//画像を右に動かす。
		m_movedPos.x = sprite->GetPos().x - AMOUNT_OF_CHANGE;
		m_movedPos.y = FRAME_OUT_POS.y;
		sprite->SetPos(m_movedPos);
		//最終地点に達したら。
		if (sprite->GetPos().x <= FRAME_OUT_POS.x) {
			//最初のステップに戻る。
			m_state = enState_nearPlayer;
		}
		break;
	default:
		break;
	}
}
