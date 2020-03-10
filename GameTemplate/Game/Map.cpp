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
	//�}�b�v�̉摜�B
	m_mapSprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_mapSprite->Init(L"sprite/map.dds", m_mapSpriteSize.x, m_mapSpriteSize.y);
	m_mapSprite->SetPos(CVector3{ m_mapSpritePos.x,m_mapSpritePos.y, 0.0f });
	//�摜�������Ă����B
	m_mapSprite->ActiveMode(false);
	//�v���C���[�̉摜�B
	m_playerSprite = NewGO<SpriteRender>(GOPrio_Sprite);
	m_playerSprite->Init(L"sprite/player_icon.dds", 20.0f, 30.0f);
	//�摜�������Ă����B
	m_playerSprite->ActiveMode(false);
	//�Q�[���̃C���X�^���X���擾�B
	m_game = GetGame();
	//�v���C���[�̃C���X�^���X���擾�B
	m_player = m_game->GetPlayer();
	return true;
}

void Map::Update()
{
	//CQuaternion rot;
	//CVector3 v = m_player->GetSkinModelRender()->GetForward();
	////v.Normalize();
	////float angle = atan2(v.z,v.x);
	//float angle = 2 * acos(m_player->GetRot().w);
	//rot.SetRotation(CVector3::AxisZ(), angle);
	//rot.Multiply(m_playerSprite->GetRot());
	////rot.Multiply(m_player->GetRot(),m_playerSprite->GetRot());
	//m_playerSprite->SetRot(rot);

	//Y�{�^������������B
	if (g_pad[0].IsTrigger(enButtonY))
	{
		m_isMap = !m_isMap;
		//�}�b�v��ʂ��\������Ă�����B
		if (m_isMap) {
			float playerSpritePosX = m_player->GetPos().x;
			float playerSpritePosY = m_player->GetPos().z;
			CVector2 ratio = { m_mapSize.x / m_mapSpriteSize.x,m_mapSize.y / m_mapSpriteSize.y };
			playerSpritePosX /= ratio.x;
			playerSpritePosY /= ratio.y;
			CVector3 playerPos = CVector3::Zero();
			//x���W��3dsMax����̏o�͂̍ۂɔ��΂ɂȂ��Ă邩��-1�������Ȃ��Ă悢�B
			//�}�b�v�̉摜�͒��S������Ă��邽�ߍŌ�ɂ��炷�B
			playerPos.x = m_mapSpritePos.x + playerSpritePosX - 35.0f;
			playerPos.y = m_mapSpritePos.y + playerSpritePosY * -1 - 10.0f;
			m_playerSprite->SetPos(playerPos);		
		}
		m_mapSprite->ChangeActive();
		m_playerSprite->ChangeActive();
	}
}
