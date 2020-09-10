#include "stdafx.h"
#include "IItem.h"
#include "Game.h"
#include "Player.h"

//staticで定義したならcppで実体を書く必要がある。
std::unordered_map<
	std::wstring,
	SpriteRender*
> IItem::m_itemSpriteMap;
bool	IItem::m_isMove = false;

IItem::IItem()
{
}

IItem::~IItem()
{
	
}

bool IItem::Start()
{
	ItemStart();
	IItemInit();
	return true;
}

void IItem::Update()
{
	ItemUpdate();
}

void IItem::OnDestroy()
{
	DeleteGO(m_buttonSprite);
	ItemDestroy();
}


void IItem::ItemCommonProcessing(SpriteRender * sprite, CVector3 pos, SkinModelRender* model)
{
	CVector3 diff = m_player->GetPos() - pos;
	IItem::GettingItem(IItem::IsGetItem(diff), sprite, diff, model);
	IItem::ButtonSpriteMove(diff, pos);
}

bool IItem::IsGetItem(CVector3 diff)
{
	//距離が近かったら。
	if (diff.Length() < ENEMY_AND_PLAYER_DISTANCE_MOVE) {
		//Bボタンを押したら。
		if (g_pad[0].IsTrigger(enButtonB)) {
			//ワンショット再生のSE。
			Sound(L"sound/story/decision.wav", false);
			m_isGetItem = true;
		}
	}
	return m_isGetItem;
}

void IItem::GettingItem(bool isGetItem, SpriteRender* sprite, CVector3 diff, SkinModelRender* model)
{
	//アイテムをゲットしていたら。
	if (isGetItem) {
		if (!m_isOnce) {
			model->ActiveMode(false);
			model->SetShadowCaster(false);
			model->SetShadowReciever(false);
			m_buttonSprite->ActiveMode(false);
			OnGet();
			m_isOnce = true;
		}
		IItem::SpriteMove(sprite, diff);
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
		sprite = it->second;
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

void IItem::ButtonSpriteMove(CVector3 diff, CVector3 pos) 
{
	if (!m_isGetItem) {
		if (diff.Length() < ENEMY_AND_PLAYER_DISTANCE_BUTTON) { 
			//3D座標から2D座標への変換。
			m_model2Dpos = { pos.x, pos.y, pos.z, 1.0f };
			g_camera3D.GetViewMatrix().Mul(m_model2Dpos);
			g_camera3D.GetProjectionMatrix().Mul(m_model2Dpos);
			m_model2Dpos.x /= m_model2Dpos.w;
			m_model2Dpos.y /= m_model2Dpos.w;
			m_buttonSprite->SetPos({ m_model2Dpos.x*FRAME_BUFFER_W / 2 * -1,m_model2Dpos.y*FRAME_BUFFER_H / 2 });
			//オブジェクトがプレイヤーの後ろ側に来たら表示しない。
			if (m_model2Dpos.w >= 0.0f) {
				m_buttonSprite->ActiveMode(true);
			}
			else {
				m_buttonSprite->ActiveMode(false);
			}
		}
		else {
			m_buttonSprite->ActiveMode(false);
		}
	}
}

//staticだがいつも通りの記述でも実体を定義していることになる。
//cppに書かれてあればok
void IItem::Release()
{
	for (auto itr = m_itemSpriteMap.begin(); itr != m_itemSpriteMap.end(); itr++) 
	{
		DeleteGO(itr->second);
	}
	//mapを空にする。
	m_itemSpriteMap.clear();
}

void IItem::SpriteMove(SpriteRender* sprite, CVector3 diff)
{
	switch (m_state) {
	//画像を動かせる状態になるのを待っている状態。
	case enState_waitMove:
		//画像を動かしているインスタンスがなかったら。
		if (!m_isMove) {
			//画像が動いている状態にする。
			m_isMove = true;
			//次のステップに移行。
			m_state = enState_startMove;
		}
		break;
	//画像が動き始めた状態。
	case enState_startMove:
			//画像を左に動かす。
			m_movedPos.x = sprite->GetPos().x + AMOUNT_OF_CHANGE;
			m_movedPos.y = FRAME_IN_POS.y;
			sprite->SetPos(m_movedPos);
			//最終地点に達したら。
			if (sprite->GetPos().x >= FRAME_IN_POS.x) {
				//次のステップに移行。
				m_state = enState_stopMove;
			}
		break;
	//画像が止まっている状態。
	case enState_stopMove:
		m_stopCount++;
		//時間がたったら。
		if (m_stopCount > TIME_TO_STOP) {
			//次のステップに移行。
			m_state = enState_endMove;
		}
		break;
	//画像が戻っていく状態。
	case enState_endMove:
		//画像を右に動かす。
		m_movedPos.x = sprite->GetPos().x - AMOUNT_OF_CHANGE;
		m_movedPos.y = FRAME_OUT_POS.y;
		sprite->SetPos(m_movedPos);
		//最終地点に達したら。
		if (sprite->GetPos().x <= FRAME_OUT_POS.x) {
			//画像の移動は完了。
			m_isFinishedMove = true;
			//画像が動いていない状態にする。
			m_isMove = false;
		}
		break;
	default:
		break;
	}
}
