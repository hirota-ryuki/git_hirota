#include "stdafx.h"
#include "Map.h"
#include "Player.h"

Map::Map()
{
}

Map::~Map()
{
}

void Map::OnDestroy()
{
	DeleteGO(m_mapSprite);
	DeleteGO(m_playerSprite);
}

bool Map::Start()
{
	//マップの画像。
	m_mapSprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_mapSprite->Init(L"sprite/map.dds", m_mapSpriteSize.x, m_mapSpriteSize.y);
	m_mapSprite->SetPos(m_mapSpritePos);
	//画像を消しておく。
	m_mapSprite->ActiveMode(false);
	//プレイヤーの画像。
	m_playerSprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_playerSprite->Init(L"sprite/player_icon.dds", 20.0f, 30.0f);
	//画像を消しておく。
	m_playerSprite->ActiveMode(false);
	//ゲームのインスタンスを取得。
	m_game = GetGame();
	//プレイヤーのインスタンスを取得。
	m_player = m_game->GetPlayer();
	return true;
}

void Map::Update()
{
	//Yボタンを押したら。
	if (g_pad[0].IsTrigger(enButtonY))
	{
		//プレイヤーの回転を計算。
		CQuaternion rot;
		CVector3 forward = m_player->GetSkinModelRender()->GetForward();
		float angle = atan2(forward.x, -forward.z);
		rot.SetRotation(CVector3::AxisZ(), -angle);
		m_playerSprite->SetRot(rot);

		//プレイヤー現在地を画像のマップ上に合わせる。
		float playerSpritePosX = m_player->GetPos().x;
		float playerSpritePosY = m_player->GetPos().z;
		CVector2 ratio = { m_mapSize.x / m_mapSpriteSize.x,m_mapSize.y / m_mapSpriteSize.y };
		playerSpritePosX /= ratio.x;
		playerSpritePosY /= ratio.y;
		CVector2 playerPos = CVector2::Zero();
		//x座標は3dsMaxからの出力の際に反対になってるから-1をかけなくてよい。
		//マップの画像は中心がずれているため最後にずらす。
		playerPos.x = m_mapSpritePos.x + playerSpritePosX /*- 35.0f*/;
		playerPos.y = m_mapSpritePos.y + playerSpritePosY * -1/* - 10.0f*/;
		m_playerSprite->SetPos(playerPos);

		m_mapSprite->ChangeActive();
		m_playerSprite->ChangeActive();
	}
}
