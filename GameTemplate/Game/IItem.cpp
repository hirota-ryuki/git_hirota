#include "stdafx.h"
#include "IItem.h"
#include "Game.h"
#include "Player.h"

//staticで定義したならcppで実体を書く必要がある。
std::unordered_map<
	std::wstring,
	std::unique_ptr<SpriteRender>
> IItem::m_itemSpriteMap;

IItem::IItem()
{
}

IItem::~IItem()
{
	DeleteGO(m_buttonSprite);
}

void IItem::ItemCommonProcessing(SpriteRender * sprite, CVector3 pos)
{
	CVector3 diff = m_player->GetPos() - pos;
	IItem::SpriteMove(sprite, diff);
	IItem::GettingItem(IItem::IsGetItem(diff));
	IItem::ButtonSpriteMove(diff, pos);
}

bool IItem::IsGetItem(CVector3 diff)
{
	//距離が近かったら。
	if (diff.Length() < ENEMY_AND_PLAYER_DISTANCE_MOVE) {
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

void IItem::GettingItem(bool isGetItem)
{
	//アイテムをゲットしていたら。
	if (isGetItem) {
		if (!m_isOnce) {
			OnGet();
			m_isOnce = true;
		}
		
		//画像の動きが終わったか。
		if (m_isFinishedMove) {
			//このインスタンスを消す。
			DeleteGO(this);
		}
	}
}

SpriteRender* IItem::SpriteLoad(const wchar_t* filePath, float w, float h)
{
	SpriteRender* sprite = nullptr;
	auto it = m_itemSpriteMap.find(filePath);
	if (it == m_itemSpriteMap.end()) 
	{
	//if (m_itemSpriteMap.count(filePath) == 1) {
		//未登録なので、新規でロードする。
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

void IItem::IItemInit()
{
	ButtonSpriteLoad();
	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
}

void IItem::ButtonSpriteLoad()
{
	m_buttonSprite = NewGO<SpriteRender>(GOPrio_Sprite,"buttonsprite");
	m_buttonSprite->Init(L"sprite/item/button.dds", B_BUTTON_SIZE, B_BUTTON_SIZE);
}

void IItem::ButtonSpriteMove(CVector3 diff, CVector3 position) 
{
	if (diff.Length() < ENEMY_AND_PLAYER_DISTANCE_BUTTON) { //距離が500以下になったら。
		//3D座標から2D座標への変換。
		m_model2Dpos = { position.x, position.y, position.z, 1.0f };
		g_camera3D.GetViewMatrix().Mul(m_model2Dpos);
		g_camera3D.GetProjectionMatrix().Mul(m_model2Dpos);
		m_model2Dpos.x /= m_model2Dpos.w;
		m_model2Dpos.y /= m_model2Dpos.w;
		m_buttonSprite->SetPos({ m_model2Dpos.x*FRAME_BUFFER_W / 2 * -1,m_model2Dpos.y*FRAME_BUFFER_H / 2 });
		m_buttonSprite->ActiveMode(true);
	}
	else {
		m_buttonSprite->ActiveMode(false);
	}
}

//staticだがいつも通りの記述でも実体を定義していることになる。
//cppに書かれてあればok
void IItem::Release()
{
	//mapを空にする。
	m_itemSpriteMap.clear();
}

void IItem::SpriteMove(SpriteRender* sprite, CVector3 diff)
{
	switch (m_state) {
	//プレイヤーが近くに来た時。
	case enState_nearPlayer:
		//プレイヤーとの距離が近くなったら。
		if (diff.Length() < ENEMY_AND_PLAYER_DISTANCE_MOVE) {
			//フラグを立てる。
			m_isNearPlayer = true;
			m_isFinishedMove = false;
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
		if (diff.Length() >= ENEMY_AND_PLAYER_DISTANCE_MOVE || m_isGetItem) {
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
			m_isFinishedMove = true;
		}
		break;
	default:
		break;
	}
}
